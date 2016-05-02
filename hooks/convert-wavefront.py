#!/usr/bin/env python3

import sys,gzip,re,struct,os

from common import writeType

def parseFace(line, tt):
    return None

def parseVertexData(line, tt):
    if line.find(tt+" ")==0:
        # Parse vertex
        m = re.search("^"+tt+"\s(\-?[0-9]*(\.[0-9]+)?)\s(\-?[0-9]*(\.[0-9]+)?)\s(\-?[0-9]*(\.[0-9]+)?).*$",line)
        if m:
            try:
                return (float(m.group(1)), float(m.group(1)), float(m.group(1)))
            except Exception as e:
                print("Error loading "+tt, e)
    return None

def loadWavefrontFile(wavefront_filepath, wavefront_file):
    enableTextures = None
    enableNormals = None
    v = []
    vt = []
    vn = []
    object_name = None
    usemtl = ""
    s = False
    f = []
    objects = []
    for line in wavefront_file:
        line = line[:-1]
        tv = parseVertexData(line, "v")
        if tv!=None:
            v.append(tv)
        tvt = parseVertexData(line, "vt")
        if tvt!=None:
            vt.append(tvt)
        tvn = parseVertexData(line, "vn")
        if tvn!=None:
            vn.append(tvn)
        if line.find("o ")>=0:
            if object_name!=None:
                objects.append((object_name, usemtl, s, [len(f)]+f))
            object_name = line.split(" ")[1]
            usemtl = ""
            s = None
            f = []
        if line.find("usemtl ")>=0:
            usemtl = line.split(" ")[1]
        if line.find("s ")>=0:
            s = line.split(" ")[1]=="on"
        if line.find("f ")==0:
            verticies = line[2:].split(" ")
            if len(verticies)>3:
                print("Wavefront file is not triangular!")
                os.exit(1)
            for vertex in verticies:
                fin = vertex.split("/")
                faceFinal = [int(fin[0])]
                if len(fin)>1:
                    if fin[1]=="":
                        if enableTextures==True:
                            print("Textures changed mid file!")
                            os.exit(1)
                        enableTextures = False
                    else:
                        if enableTextures==False:
                            print("Textures changed mid file!")
                            os.exit(1)
                        enableTextures = True
                        faceFinal.append(int(fin[1]))
                if len(fin)>2:
                    if fin[2]=="":
                        if enableNormals==True:
                            print("Textures changed mid file!")
                            os.exit(1)
                        enableNormals = False
                    else:
                        if enableNormals==False:
                            print("Textures changed mid file!")
                            os.exit(1)
                        enableNormals = True
                        faceFinal.append(int(fin[2]))
                f.append(faceFinal)
    objects.append((object_name, usemtl, s, f))
    return (len(v), v, len(vt), vt, len(vn), vn, len(objects), objects)

if __name__ == "__main__":
    wavefront_filepath = sys.argv[1]
    wavefront_outpath = sys.argv[2] if len(sys.argv)>=3 else wavefront_filepath.replace("src/", "bin/").replace(".obj", ".objo.gz")
    print(wavefront_filepath+" => "+wavefront_outpath)
    wavefront_file = open(wavefront_filepath, "r")
    fp = gzip.open(wavefront_outpath, "wb")
    wavefront = loadWavefrontFile(wavefront_filepath, wavefront_file)
    print(wavefront)
    fp.write(bytes([0]))
    writeType(fp, wavefront_filepath[wavefront_filepath.rfind("/")+1:]);
    writeType(fp, wavefront)
    fp.close()
