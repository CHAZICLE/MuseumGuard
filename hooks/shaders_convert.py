#!/usr/bin/env python3

import os,re

shader_folder = "src/render/shaders/glsl/"
skip_line = re.compile("^\s*$")

def processShaderLine(shader_meta, line):
    shader_meta.last_index += 1
    if line.find("import")>=0:
        line = line.replace("import", "layout(location = "+shader_meta.last_index+")")
    return line

def parseShaders():
    shaders = []
    for f in os.listdir("src/render/shaders/glsl"):
        shader_fp = open(shader_folder+f, "r")
        vertex_shader = ""
        fragment_shader = ""
        shader_type = 0
        code = ""
        shader_meta = {}
        for line in shader_fp:
            line = line[:-1]
            if re.match(skip_line, line)!=None:
                continue
            if line.find("//vertex")==0:
                shader_type = 1
                code = ""
            elif line.find("//fragment")==0:
                shader_type = 2
                code = ""
            if shader_type==1:#vertex
                vertex_shader += processShaderLine(shader_meta, line)+" "
            elif shader_type==2:#fragment
                fragment_shader += processShaderLine(shader_meta, line)+" "
            print(line)
        shaders.append((vertex_shader, fragment_shader))

parseShaders()
