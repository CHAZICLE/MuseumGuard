SRCDIR=src
BINDIR=bin
DEPDIR=depend
CXX=g++ -fPIC -g
CXXFLAGS=-Wall -ansi -DUSE_GLEW -std=c++11 -Isrc -I/usr/include/freetype2
LDFLAGS=-std=c++11 -lglfw -lGL -lGLU -lGLEW -lfreetype -lpng
BIN=Three
all: $(BIN)

SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(SOURCES))
DEPENDS = $(patsubst $(SRCDIR)/%.cpp, $(DEPDIR)/%.d, $(SOURCES))

# Shaders
SHADERS = $(shell find $(SRCDIR)/shaders/glsl -type f -name '*.glsl.c')
$(SRCDIR)/shaders/ShaderUtils.hpp $(SRCDIR)/shaders/ShaderUtils.cpp: $(SRCDIR)/shaders/shadercode.h $(SRCDIR)/shaders/shadercode.c
$(SRCDIR)/shaders/shadercode.h $(SRCDIR)/shaders/shadercode.c:
	src/shaders/build.sh

# General

clean:
	rm -r $(BINDIR) $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(BIN) $(OBJECTS)

-include $(DEPENDS)

$(DEPDIR)/%.d: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -DSKIP_DEPEND_TREE -MM "$<" -MF "$@" -MT "$(patsubst src/%.cpp,bin/%.o, $(<)) $@"

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) $(CXXFLAGS) -c -o "$@" "$<"
