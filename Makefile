SRCDIR=src
BINDIR=bin
DEPDIR=depend
CXX=g++ -fPIC -g
CXXFLAGS=-Wall -ansi -DUSE_GLEW -std=c++11 -Isrc -I/usr/include/freetype2
LDFLAGS=-std=c++11 -lglfw -lGL -lGLU -lGLEW -lfreetype -lpng -lboost_system -lboost_iostreams -pthread
BIN=Three

SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))
DEPENDS = $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $(SOURCES))

# Shaders
SHADERS = $(shell find $(SRCDIR)/render/shaders/glsl -type f -name '*.glsl.c')

### Assets
ASSETS_FILE = assets.gz
ASSETS_META_HOOK = hooks/asset_makemeta.py
ASSETS_META_FILE = src/util/AssetsMeta.h
ASSETS_CONVERT_HOOK = hooks/asset_convert.py

# MTL Files
ASSETS_MTL = $(shell find $(SRCDIR) -type f -name '*.mtl')
ASSETS_MTL_O = $(patsubst $(SRCDIR)/%.mtl, $(BINDIR)/%.mtl.o.gz, $(ASSETS_MTL))

# OBJ Files
ASSETS_OBJ = $(shell find $(SRCDIR) -type f -name '*.obj')
ASSETS_OBJ_O = $(patsubst $(SRCDIR)/%.obj, $(BINDIR)/%.obj.o.gz, $(ASSETS_OBJ))

# MD5 Meshes
ASSETS_MD5MESH = $(shell find $(SRCDIR) -type f -name '*.md5mesh')
ASSETS_MD5MESH_O = $(patsubst $(SRCDIR)/%.md5mesh, $(BINDIR)/%.md5mesh.o.gz, $(ASSETS_MD5MESH))

# MD5 Animations
ASSETS_MD5ANIM = $(shell find $(SRCDIR) -type f -name '*.md5anim')
ASSETS_MD5ANIM_O = $(patsubst $(SRCDIR)/%.md5anim, $(BINDIR)/%.md5anim.o.gz, $(ASSETS_MD5ANIM))

ASSETS = $(ASSETS_MTL) $(ASSETS_OBJ) $(ASSETS_MD5MESH) $(ASSETS_MD5ANIM)
ASSETS_O = $(ASSETS_MTL_O) $(ASSETS_OBJ_O) $(ASSETS_MD5MESH_O) $(ASSETS_MD5ANIM_O)

### Main

all: $(ASSETS_META_FILE) $(ASSETS_FILE) $(DEPENDS) $(BIN)

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

### Files

# C++ Source => Objects
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -c -o "$@" "$<"

# C++ Source => Depend
$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -DSKIP_DEPEND_TREE -MM "$<" -MF "$@" -MT "$(patsubst src/%.cpp,bin/%.o, $(<)) $@"


# Assets->MTL
$(BINDIR)/%.mtl.o.gz: $(SRCDIR)/%.mtl $(ASSETS_CONVERT_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) --meta "$(ASSETS_META_FILE)" "$<" "$@"

# Assets->OBJ
$(BINDIR)/%.obj.o.gz: $(SRCDIR)/%.obj $(ASSETS_CONVERT_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) --meta "$(ASSETS_META_FILE)" "$<" "$@"

# Assets->MD5Mesh
$(BINDIR)/%.md5mesh.o.gz: $(SRCDIR)/%.md5mesh $(ASSETS_CONVERT_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) --meta "$(ASSETS_META_FILE)" "$<" "$@"

# Assets->MD5Anim
$(BINDIR)/%.md5anim.o.gz: $(SRCDIR)/%.md5anim $(ASSETS_CONVERT_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) --meta "$(ASSETS_META_FILE)" "$<" "$@"

# Shaders
$(SRCDIR)/render/shaders/ShaderUtils.hpp $(SRCDIR)/render/shaders/ShaderUtils.cpp: $(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c
$(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c: $(SHADERS)
	src/render/shaders/build.sh
