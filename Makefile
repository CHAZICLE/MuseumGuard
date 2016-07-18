SRCDIR=src
BINDIR=bin
HOOKSDIR=hooks
BININCDIR=$(BINDIR)/include

all: assets.gz .SHADERS Three

### Assets

RESBIN=assets.gz
RESSRCDIR=$(SRCDIR)/res
RESBINDIR=$(BINDIR)/object/res
RESDEPDIR=$(BINDIR)/depend/res
RESINCDIR=$(BININCDIR)/res

ASSETS_META_HOOK = $(HOOKSDIR)/asset_makemeta.py
ASSETS_META_FILE = $(BININCDIR)/AssetsMeta.h

ASSETS_HOOK_CONVERT = $(HOOKSDIR)/asset_convert.py
ASSETS_HOOK_CONVERT_IMAGE = $(HOOKSDIR)/asset_convert_image.py
ASSETS_HOOK_COMMON = $(HOOKSDIR)/asset_common.py
ASSETS_HOOK_PARSER = $(HOOKSDIR)/asset_parsers.py
ASSETS_CONVERT_DEPS = $(ASSETS_HOOK_CONVERT) $(ASSETS_HOOK_CONVERT_IMAGE) $(ASSETS_HOOK_COMMON) $(ASSETS_HOOK_PARSER)
ASSETS_CONVERT_FLAGS=--global-meta "$(ASSETS_META_FILE)" --verbose 2

# MTL Files
ASSETS_MTL = $(shell find $(RESSRCDIR) -type f -name '*.mtl')
# OBJ Files
ASSETS_OBJ = $(shell find $(RESSRCDIR) -type f -name '*.obj')
# MD5 Meshes
ASSETS_MD5MESH = $(shell find $(RESSRCDIR) -type f -name '*.md5mesh')
# MD5 Animations
ASSETS_MD5ANIM = $(shell find $(RESSRCDIR) -type f -name '*.md5anim')
# Textures
ASSETS_TEXTURES = $(shell find $(RESSRCDIR) -type f -name '*.tga') $(shell find $(RESSRCDIR) -type f -name '*.png') $(shell find $(RESSRCDIR) -type f -name '*.jpg')

RESSRCS = $(ASSETS_MTL) $(ASSETS_OBJ) $(ASSETS_MD5MESH) $(ASSETS_MD5ANIM) $(ASSETS_TEXTURES) 
RESINCS = $(patsubst $(RESSRCDIR)/%,$(RESBINDIR)/%.h,$(RESSRCS))
RESBINS = $(patsubst $(RESSRCDIR)/%,$(RESBINDIR)/%.o,$(RESSRCS))
RESDEPS = $(patsubst $(RESSRCDIR)/%,$(RESBINDIR)/%.d,$(RESSRCS))

-include $(RESDEPS)

$(RESBIN): $(ASSETS_META_FILE) $(RESBINS)
	cat $(RESBINS) > "$@"

$(ASSETS_META_FILE): $(RESSRCS) $(ASSETS_META_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_META_HOOK) --source $(RESSRCS) --meta "$@"

$(RESBINDIR)/%.o $(RESINCDIR)/%.h: $(RESSRCDIR)/%  $(ASSETS_CONVERT_DEPS)
	@mkdir -p "$(@D)"
	@mkdir -p "$(patsubst $(RESBINDIR)%,$(RESINCDIR)%, $(@D))"
	@mkdir -p "$(patsubst $(RESBINDIR)%,$(RESDEPDIR)%, $(@D))"
	$(ASSETS_HOOK_CONVERT) $(ASSETS_CONVERT_FLAGS) --meta "$(patsubst $(RESSRCDIR)%,$(RESINCDIR)%, $(<)).h" --depend "$(patsubst $(RESSRCDIR)%,$(RESDEPDIR)%, $(<)).d" --src "$<" --out "$@"

### CPP Vars

CPPSRCDIR=$(SRCDIR)/cpp
CPPBINDIR=$(BINDIR)/object/cpp
CPPDEPDIR=$(BINDIR)/depend/cpp

CXX=g++
CXXFLAGS=-g -fPIE -Wall -ansi -std=c++11 -I$(CPPSRCDIR) -I$(BININCDIR) -I/usr/include/freetype2 -include util/SuperDebug.h
LDFLAGS=-std=c++11 -lglfw -lGL -lGLU -lGLEW -lfreetype -lpng -lboost_system -lboost_iostreams -lSOIL -pthread
CPPBIN=Three
SUPERGLOBAL=$(CPPSRCDIR)/util/SuperDebug.h

CPPSRCS = $(shell find $(CPPSRCDIR) -type f -name '*.cpp')
CPPBINS = $(patsubst $(CPPSRCDIR)/%.cpp, $(CPPBINDIR)/%.o, $(CPPSRCS))
CPPDEPS = $(patsubst $(CPPSRCDIR)/%.cpp, $(CPPDEPDIR)/%.d, $(CPPSRCS))

-include $(CPPDEPS)

$(CPPBIN):  $(CPPBINS)
	$(CXX) $(LDFLAGS) -o $(CPPBIN) $(CPPBINS)

# C++ Source => Objects
$(CPPBINDIR)/%.o: $(CPPSRCDIR)/%.cpp $(SUPERGLOBAL)
	@mkdir -p "$(@D)"
	@mkdir -p "$(patsubst $(CPPBINDIR)%,$(CPPDEPDIR)%, $(@D))"
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(CXX) $(CXXFLAGS) -DDEP -MM $< -MT $@ -MF $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@))
	@cp -f $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)) $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)).tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@))
	@rm -f $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)).tmp

### Shaders

SHADER_HOOK = $(HOOKSDIR)/shaders_convert.py
SHADER_VARIABLES_IN = $(CPPSRCDIR)/render/shaders/shader_variables.txt
SHADER_FOLDER = $(CPPSRCDIR)/render/shaders/code/
SHADER_DESC = $(CPPSRCDIR)/render/shaders/shaders.txt

SHADER_HEADER = $(BININCDIR)/render/shaders/ShaderPrograms.h
SHADER_VARIABLES_OUT = $(BININCDIR)/render/shaders/ShaderVariables.h
SHADER_LOADER = $(BININCDIR)/render/shaders/ShaderLoader.c

SHADER_PROGRAMS = $(shell find $(SHADER_FOLDER) -type f -name '*.c')

.SHADERS: $(SHADER_VARIABLES_OUT) $(SHADER_HEADER) $(SHADER_LOADER)

$(SHADER_VARIABLES_OUT) $(SHADER_HEADER) $(SHADER_LOADER): $(SHADER_VARIABLES_IN) $(SHADER_DESC) $(SHADER_PROGRAMS) $(SHADER_HOOK)
	@mkdir -p "$(@D)"
	$(SHADER_HOOK) --variables-in $(SHADER_VARIABLES_IN) --variables-out $(SHADER_VARIABLES_OUT) --shaders-folder $(SHADER_FOLDER) --shaders-desc $(SHADER_DESC) --shaders-header $(SHADER_HEADER) --shaders-loader $(SHADER_LOADER)

### Main

clean:
	@rm -vrf $(BINDIR)
	@rm -vf $(CPPBIN) $(RESBIN)
