SRCDIR=src
BINDIR=bin
RESDIR=res
HOOKSDIR=hooks

CPPSRCDIR=$(SRCDIR)
CPPBINDIR=$(BINDIR)/cpp
CPPDEPDIR=$(BINDIR)/depend/cpp

RESSRCDIR=$(RESDIR)
RESBINDIR=$(BINDIR)/res

CXX=g++
CXXFLAGS=-g -fPIE -Wall -ansi -std=c++11 -Isrc -I/usr/include/freetype2 -include util/SuperDebug.h
LDFLAGS=-std=c++11 -lglfw -lGL -lGLU -lGLEW -lfreetype -lpng -lboost_system -lboost_iostreams -lSOIL -pthread
BIN=Three
SUPERGLOBAL=$(SRCDIR)/util/SuperDebug.h

CPPSRCS = $(shell find $(CPPSRCDIR) -type f -name '*.cpp')
CPPBINS = $(patsubst $(CPPSRCDIR)/%.cpp, $(CPPBINDIR)/%.o, $(CPPSRCS))
CPPDEPS = $(patsubst $(CPPSRCDIR)/%.cpp, $(CPPDEPDIR)/%.d, $(CPPSRCS))

# Shaders
SHADER_HOOK = $(HOOKSDIR)/shaders_convert.py
SHADER_VARIABLES_IN = $(SRCDIR)/render/shaders/shader_variables.txt
SHADER_VARIABLES_OUT = $(SRCDIR)/render/shaders/ShaderVariables.h
SHADER_FOLDER = $(SRCDIR)/render/shaders/code/
SHADER_DESC = $(SRCDIR)/render/shaders/shaders.txt
SHADER_HEADER = $(SRCDIR)/render/shaders/ShaderPrograms.h
SHADER_LOADER = $(SRCDIR)/render/shaders/ShaderLoader.c

SHADER_PROGRAMS = $(shell find $(SHADER_FOLDER) -type f -name '*.c')

#parseShaders(sys.argv[2], sys.argv[4], sys.argv[6], sys.argv[8], sys.argv[10], sys.argv[12])

### Assets
ASSETS_FILE = assets.gz
ASSETS_META_HOOK = $(HOOKSDIR)/asset_makemeta.py
ASSETS_META_FILE = $(SRCDIR)/util/AssetsMeta.h
ASSETS_CONVERT_HOOK = $(HOOKSDIR)/asset_convert.py
ASSETS_IMAGE_SCRIPT = $(HOOKSDIR)/asset_convert_image.py
ASSETS_COMMON_HOOK = $(HOOKSDIR)/asset_common.py
ASSETS_PARSER_HOOK = $(HOOKSDIR)/asset_parsers.py

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

ASSETS = $(ASSETS_MTL) $(ASSETS_OBJ) $(ASSETS_MD5MESH) $(ASSETS_MD5ANIM) $(ASSETS_TEXTURES) 
ASSETS_O = $(patsubst $(RESSRCDIR)/%,$(RESBINDIR)/%.o.gz,$(ASSETS))

### Main

all: $(ASSETS_FILE) $(BIN)

$(ASSETS_FILE): $(ASSETS_META_FILE) $(ASSETS_O)
	cat $(ASSETS_O) > "$@"

$(ASSETS_META_FILE): $(ASSETS) $(ASSETS_META_HOOK)
	$(ASSETS_META_HOOK) --source $(ASSETS) --meta "$@"


$(BIN):  $(CPPBINS)
	$(CXX) $(LDFLAGS) -o $(BIN) $(CPPBINS)

clean:
	@rm -vf $(BIN) $(ASSETS_FILE)
	@rm -vrf $(BINDIR)

-include $(CPPDEPS)

### Shaders
$(SHADER_VARIABLES_OUT) $(SHADER_HEADER) $(SHADER_LOADER): $(SHADER_VARIABLES_IN) $(SHADER_DESC) $(SHADER_PROGRAMS) $(SHADER_HOOK)
	$(SHADER_HOOK) --variables-in $(SHADER_VARIABLES_IN) --variables-out $(SHADER_VARIABLES_OUT) --shaders-folder $(SHADER_FOLDER) --shaders-desc $(SHADER_DESC) --shaders-header $(SHADER_HEADER) --shaders-loader $(SHADER_LOADER)

### Files

# C++ Source => Objects
$(CPPBINDIR)/%.o: $(CPPSRCDIR)/%.cpp $(SUPERGLOBAL)
	@mkdir -p "$(@D)"
	@mkdir -p "$(patsubst $(CPPBINDIR)%,$(CPPDEPDIR)%, $(@D))"
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@$(CXX) $(CXXFLAGS) -DDEP -MM $< -MT $@ -MF $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@))
	@cp -f $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)) $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)).tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@))
	@rm -f $(patsubst $(CPPBINDIR)/%.o,$(CPPDEPDIR)/%.d, $(@)).tmp

# Assets
$(RESBINDIR)/%.o.gz: $(RESSRCDIR)/% $(ASSETS_CONVERT_HOOK) $(ASSETS_COMMON_HOOK) $(ASSETS_META_FILE) $(ASSETS_PARSER_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) --meta "$(ASSETS_META_FILE)" "$<" "$@"
