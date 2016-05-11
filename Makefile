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

all: assets.gz $(DEPENDS) $(BIN)

clean:
	@rm -vf $(BIN) assets.gz
	@rm -vrf $(BINDIR)

assets.gz: $(ASSETS_O)
	cat $(ASSETS_O) > "$@"

$(BIN):  $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(BIN) $(OBJECTS)

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
	$(ASSETS_CONVERT_HOOK) "$<" "$@"

# Assets->OBJ
$(BINDIR)/%.obj.o.gz: $(SRCDIR)/%.obj $(ASSETS_CONVERT_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) "$<" "$@" $(ASSET_MTL_O)

# Assets->MD5Mesh
$(BINDIR)/%.md5mesh.o.gz: $(SRCDIR)/%.md5mesh $(ASSETS_CONVERT_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) "$<" "$@"

# Assets->MD5Anim
$(BINDIR)/%.md5anim.o.gz: $(SRCDIR)/%.md5anim $(ASSETS_CONVERT_HOOK)
	@mkdir -p "$(@D)"
	$(ASSETS_CONVERT_HOOK) "$<" "$@"

# Shaders
$(SRCDIR)/render/shaders/ShaderUtils.hpp $(SRCDIR)/render/shaders/ShaderUtils.cpp: $(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c
$(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c: $(SHADERS)
	src/render/shaders/build.sh
