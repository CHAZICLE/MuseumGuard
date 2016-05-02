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

# Wavefronts
WAVEFRONTS = $(shell find $(SRCDIR) -type f -name '*.obj')
WAVEFRONTOS = $(patsubst $(SRCDIR)/%.obj, $(BINDIR)/%.obj.o.gz, $(WAVEFRONTS))

# MD5s
MD5S = $(shell find $(SRCDIR) -type f -name '*.md5mesh')
MD5OS = $(patsubst $(SRCDIR)/%.md5mesh, $(BINDIR)/%.md5mesh.o.gz, $(MD5S))

# Shaders
SHADERS = $(shell find $(SRCDIR)/render/shaders/glsl -type f -name '*.glsl.c')

all: assets.gz $(BIN) 



clean:
	rm -vf $(BIN) assets.gz
	rm -rf $(BINDIR)

$(BIN):  $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(BIN) $(OBJECTS)

-include $(DEPENDS)

### Files

assets.gz: $(WAVEFRONTOS) $(MD5OS)
#	cat $(WAVEFRONTOS) $(MD5OS) > "$@"
	cat $(MD5OS) > "$@"

# C++ Source => Objects
$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -c -o "$@" "$<"

# C++ Source => Depend
$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -DSKIP_DEPEND_TREE -MM "$<" -MF "$@" -MT "$(patsubst src/%.cpp,bin/%.o, $(<)) $@"

# Wavefronts
$(BINDIR)/%.obj.o.gz: $(SRCDIR)/%.obj
	@mkdir -p "$(@D)"
	hooks/convert-wavefront.py "$<" "$@"

# MD5s
$(BINDIR)/%.md5mesh.o.gz: $(SRCDIR)/%.md5mesh
	@mkdir -p "$(@D)"
	hooks/convert-md5mesh.py "$<" "$@"

# Shaders
$(SRCDIR)/render/shaders/ShaderUtils.hpp $(SRCDIR)/render/shaders/ShaderUtils.cpp: $(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c
$(SRCDIR)/render/shaders/shadercode.h $(SRCDIR)/render/shaders/shadercode.c: $(SHADERS)
	src/render/shaders/build.sh
