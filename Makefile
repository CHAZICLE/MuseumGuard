SRCDIR=src
BINDIR=bin
DEPDIR=depend
CXX=g++ -fPIC -g
CXXFLAGS=-Wall -ansi -DUSE_GLEW -std=c++11 -Isrc -I/usr/include/freetype2
LDFLAGS=-std=c++11 -lglfw -lGL -lGLU -lGLEW -lfreetype -lpng -lboost_system -lboost_iostreams
BIN=Three
all: $(MD5OS) $(BIN) 

SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))
DEPENDS = $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $(SOURCES))

# MD5s
MD5S = $(shell find $(SRCDIR) -type f -name '*.md5mesh')
MD5OS = $(patsubst $(SRCDIR)/%.md5mesh, $(BINDIR)/%.md5mesh.o.gz, $(MD5S))

# Shaders
SHADERS = $(shell find $(SRCDIR)/render/shaders/glsl -type f -name '*.glsl.c')
$(SRCDIR)/render/shaders/ShaderUtils.hpp $(SRCDIR)/render/shaders/ShaderUtils.cpp: $(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c
$(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c: $(SHADERS)
	src/render/shaders/build.sh

# Rigid Models
SHADERS = $(shell find $(SRCDIR)/render/rigid-models -type f -name '*.glsl.c')
$(SRCDIR)/render/RigidModels.hpp: $(SRCDIR)/render/export/rigid-models.h.txt
$(SRCDIR)/render/RigidModels.cpp: $(SRCDIR)/render/export/rigid-models.c.txt

# General

clean:
	rm -r $(BINDIR) $(BIN) assets.gz

$(BIN):  $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(BIN) $(OBJECTS)

-include $(DEPENDS)

assets.gz: $(MD5OS)
	cat $(MD5OS) > "$@"

$(BINDIR)/%.md5mesh.o.gz: $(SRCDIR)/%.md5mesh
	@mkdir -p "$(@D)"
	hooks/convert-md5mesh.py "$<" "$@"

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -DSKIP_DEPEND_TREE -MM "$<" -MF "$@" -MT "$(patsubst src/%.cpp,bin/%.o, $(<)) $@"

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -c -o "$@" "$<"
