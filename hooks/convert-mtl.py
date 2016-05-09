#!/usr/bin/env python3

import sys,gzip,re,struct,os

from common import writeType

def parse1i(current, line, key):
    m = re.search("^"+key+"\s(-?\d+)\s$", line);
    if not m:
        return current
    return float(m.group(1))

def parse1s(current, line, key):
    m = re.search("^"+key+"\s(.+?)\s$", line);
    if not m:
        return current
    return float(m.group(1))

def parse1f(current, line, key):
    m = re.search("^"+key+"\s(-?\d+\.{0,1}\d+)\s$", line);
    if not m:
        return current
    return float(m.group(1))

def parse3f(current, line, key):
    m = re.search("^"+key+"\s(-?\d+\.{0,1}\d+)\s(-?\d+\.{0,1}\d+)\s(-?\d+\.{0,1}\d+)\s$", line);
    if not m:
        return current
    return [float(m.group(1)), float(m.group(1)), float(m.group(1))]

def flattenArray(material_name, data):
    final_data = [material_name, 0]
    for i in range(len(data)):
        if data[i]!=None:
            final_data.append(data[i])
            final_data[1] = final_data[1]|(1<<i)
    return final_data

def loadMTLFile(mtl_filepath, mtl_file):
    materials = []
    current_material_name = None
    # Color/Illumination
    Ka = None
    Kd = None
    Ks = None
    Tf = None
    illum = None
    d = None
    Ns = None
    sharpness = None
    Ni = None
    # Texture map
    map_Ka = None
    map_Kd = None
    map_Ks = None
    map_Ns = None
    map_d = None
    disp = None
    decal = None
    bump = None
    for line in mtl_file:
        line = line[:-1]
        temp = parse1s(None, line, "newmtl")
        if temp!=None:
            if current_material_name!=None:
                materials.append(flattenArray(current_material_name, [Ka, Kd, Ks, Tf, illum, d, Ns, sharpness, Ni, map_Ka, map_Kd, map_Ks, map_Ns, map_d, disp, decal, bump]))
            current_material_name = temp

        Ka = parse3f(Ka, line, "Ka")
        Kd = parse3f(Kd, line, "Kd")
        Ks = parse3f(Ks, line, "Ks")
        Tf = parse3f(Tf, line, "Tf")
        illum = parse1i(illum, line, "illum")
        d = parse1f(d, line, "d")
        Ns = parse1f(Ns line, "Ns")
        sharpness = parse1i(sharpness, line, "sharpness")
        Ni = parse1f(Ni, line, "Ni")

        map_Ka = parse1s(map_Ka, line, "map_Ka")
        map_Kd = parse1s(map_Kd, line, "map_Kd")
        map_Ks = parse1s(map_Ks, line, "map_Ks")
        map_Ns = parse1s(map_Ns, line, "map_Ns")
        map_d = parse1s(map_d, line, "map_d")
        disp = parse1s(disp, line, "disp")
        decal = parse1s(decal, line, "decal")
        bump = parse1s(bump, line, "bump")
    materials.append(flattenArray(current_material_name, [Ka, Kd, Ks, Tf, illum, d, Ns, sharpness, Ni, map_Ka, map_Kd, map_Ks, map_Ns, map_d, disp, decal, bump]))
    return materials

if __name__ == "__main__":
    mtl_filepath = sys.argv[1]
    mtl_outpath = sys.argv[2] if len(sys.argv)>=3 else mtl_filepath.replace("src/", "bin/").replace(".mtl", ".mtlo.gz")
    print(mtl_filepath+" => "+mtl_outpath)
    mtl_file = open(mtl_filepath, "r")
    fp = gzip.open(mtl_outpath, "wb")
    mtl = loadMTLFile(mtl_filepath, mtl_file)
    print(mtl)
    fp.write(bytes([0]))
    writeType(fp, mtl_filepath[mtl_filepath.rfind("/")+1:]);
    writeType(fp, mtl)
    fp.close()
