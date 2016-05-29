SRCDIR=src
BINDIR=bin
DEPDIR=depend
CXX=g++ -fPIC -g
CXXFLAGS=-Wall -ansi -DUSE_GLEW -std=c++11 -Isrc -I/usr/include/freetype2
LDFLAGS=-std=c++11 -lglfw -lGL -lGLU -lGLEW -lfreetype -lpng -lboost_system -lboost_iostreams -lSOIL -pthread
BIN=Three

SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))
DEPENDS = $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $(SOURCES))

# Shaders

SHADER_HOOK = hooks/shaders_convert.py
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
ASSETS_META_HOOK = hooks/asset_makemeta.py
ASSETS_META_FILE = src/util/AssetsMeta.h
ASSETS_CONVERT_HOOK = hooks/asset_convert.py
ASSETS_IMAGE_SCRIPT = hooks/asset_convert_image.py

# MTL Files
ASSETS_MTL = $(shell find $(SRCDIR) -type f -name '*.mtl')
# OBJ Files
ASSETS_OBJ = $(shell find $(SRCDIR) -type f -name '*.obj')
# MD5 Meshes
ASSETS_MD5MESH = $(shell find $(SRCDIR) -type f -name '*.md5mesh')
# MD5 Animations
ASSETS_MD5ANIM = $(shell find $(SRCDIR) -type f -name '*.md5anim')
# Textures
ASSETS_TEXTURES = $(shell find $(SRCDIR) -type f -name '*.tga') $(shell find $(SRCDIR) -type f -name '*.png') $(shell find $(SRCDIR) -type f -name '*.jpg')

ASSETS = $(ASSETS_MTL) $(ASSETS_OBJ) $(ASSETS_MD5MESH) $(ASSETS_MD5ANIM) $(ASSETS_TEXTURES) 
ASSETS_O = $(patsubst src/%,bin/%.o.gz,$(ASSETS))

### Main

all: $(ASSETS_META_FILE) $(ASSETS_FILE) $(BIN)

$(ASSETS_FILE): $(ASSETS_META_FILE) $(ASSETS_O)
	cat $(ASSETS_O) > "$@"

$(ASSETS_META_FILE): $(ASSETS) $(ASSETS_META_HOOK)
	$(ASSETS_META_HOOK) --source $(ASSETS) --meta "$@"


$(BIN):  $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(BIN) $(OBJECTS)

clean:
	@rm -vf $(BIN) $(ASSETS_FILE)
	@rm -vrf $(BINDIR)

-include $(DEPENDS)

### Shaders
$(SHADER_VARIABLES_OUT) $(SHADER_HEADER) $(SHADER_LOADER): $(SHADER_VARIABLES_IN) $(SHADER_DESC) $(SHADER_PROGRAMS) $(SHADER_HOOK)
	$(SHADER_HOOK) --variables-in $(SHADER_VARIABLES_IN) --variables-out $(SHADER_VARIABLES_OUT) --shaders-folder $(SHADER_FOLDER) --shaders-desc $(SHADER_DESC) --shaders-header $(SHADER_HEADER) --shaders-loader $(SHADER_LOADER)

### Files

# C++ Source => Objects
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -c -o "$@" "$<"

# C++ Source => Depend
$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -DSKIP_DEPEND_TREE -MM "$<" -MF "$@" -MT "$(patsubst src/%.cpp,bin/%.o, $(<)) $@"


# Textures
$(BINDIR)/%.o.gz: $(SRCDIR)/% $(ASSETS_CONVERT_HOOK) $(ASSETS_IMAGE_SCRIPT) $(ASSETS_META_FILE)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) --meta "$(ASSETS_META_FILE)" "$<" "$@"
