#!/usr/bin/env python5

import re

from asset_common import *

def parseMTL(filepath, filename, source_fp, meta, verbose=0):
    names = ["Ka", "Kd", "Ks", "Tf", "d", "Ns", "Ni", "illum", "sharpness", "map_Ka", "map_Kd", "map_Ks", "map_Ns", "map_d", "disp", "decal", "bump"]
    def flattenArray(material_name, data):
        final_data = [material_name, 0]
        if verbose>=1:
            ap = ""
        for i in range(len(data)):
            if data[i]!=None:
                if type(data[i])==str:
                    textureAssetId = meta['textures'][filepath+"/"+data[i].strip()]
                    if verbose==1:
                        ap += ", "+names[i]+"=["+str(textureAssetId)+"]"+str(data[i])
                    final_data.append(textureAssetId)
                else:
                    if verbose==1:
                        ap += ", "+names[i]+"="+str(data[i])
                    final_data.append(data[i])
                final_data[1] = final_data[1]|(1<<i)
        if verbose==1:
            print("\t"+material_name+": Flags="+str(final_data[1])+ap)
        return final_data

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
    for line in source_fp:
        line = line[:-1]
        temp = parse1s(None, line, "newmtl")
        if temp!=None:
            if current_material_name!=None:
                materials.append(flattenArray(current_material_name, [Ka, Kd, Ks, Tf, d, Ns, Ni, illum, sharpness, map_Ka, map_Kd, map_Ks, map_Ns, map_d, disp, decal, bump]))
            current_material_name = temp

        #float[3]
        Ka = parse3f(Ka, line, "Ka")
        Kd = parse3f(Kd, line, "Kd")
        Ks = parse3f(Ks, line, "Ks")
        Tf = parse3f(Tf, line, "Tf")
        #float
        d = parse1f(d, line, "d")
        Ns = parse1f(Ns, line, "Ns")
        Ni = parse1f(Ni, line, "Ni")
        #int
        illum = parse1i(illum, line, "illum")
        sharpness = parse1i(sharpness, line, "sharpness")
        #str
        map_Ka = parse1s(map_Ka, line, "map_Ka")
        map_Kd = parse1s(map_Kd, line, "map_Kd")
        map_Ks = parse1s(map_Ks, line, "map_Ks")
        map_Ns = parse1s(map_Ns, line, "map_Ns")
        map_d = parse1s(map_d, line, "map_d")
        disp = parse1s(disp, line, "disp")
        decal = parse1s(decal, line, "decal")
        bump = parse1s(bump, line, "bump")
    #Ka, Kd, Ks, Tf, d, Ns, Ni, illum, sharpness, map_Ka, map_Kd, map_Ks, map_Ns, map_d, disp, decal, bump

    materials.append(flattenArray(current_material_name, [Ka, Kd, Ks, Tf, d, Ns, Ni, illum, sharpness, map_Ka, map_Kd, map_Ks, map_Ns, map_d, disp, decal, bump]))
    return [len(materials)]+materials

def parseOBJ(filepath, filename, source_fp, meta, verbose=0):
    enableTextures = None
    enableNormals = None
    numPrimitives = 0
    mtllib = None
    v = []
    vt = []
    vn = []
    object_name = None
    usemtl = ""
    s = False
    f = []
    objects = []
    for line in source_fp:
        line = line[:-1]
        tv = parse3f(None, line, "v")
        if tv!=None:
            v.append(tv)
        tvt = parse2f(None, line, "vt")
        if tvt!=None:
            vt.append(tvt)
        tvn = parse3f(None, line, "vn")
        if tvn!=None:
            vn.append(tvn)
        if line.find("g ")>=0:
            if object_name!=None:
                objects.append((object_name, meta['materials'][filepath+"/"+mtllib+":"+usemtl], s, numPrimitives, f))
            object_name = line.split(" ")[1]
            #print(object_name)
            usemtl = ""
            s = False
            numPrimitives = 0
            f = []
        if line.find("mtllib ")>=0:
            mtllib = line.split(" ")[1]
        if line.find("usemtl ")>=0:
            usemtl = line.split(" ")[1]
        if line.find("s ")>=0:
            s = line.split(" ")[1]=="on"
        if line.find("f ")==0:
            verticies = line[2:].split(" ")
            numPrimitives = numPrimitives+1
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
    #print(meta['materials'][filepath+"/"+mtllib+":"+usemtl])
    objects.append((object_name, meta['materials'][filepath+"/"+mtllib+":"+usemtl], s, numPrimitives, f))
    #print((object_name, meta['materials'][filepath+"/"+mtllib+":"+usemtl], s, numPrimitives, f))
    if verbose>=1:
        print("	"+str(len(v))+" vertecies, "+str(len(vt))+" texture coordinates, "+str(len(vn))+" normal coordinates, "+str(len(objects))+" objects")
    return (len(v), v, len(vt), vt, len(vn), vn, 0, [], len(objects), objects)

def parseNAVOBJ(filepath, filename, source_fp, meta, verbose=0):
    v = []
    l = []
    for line in source_fp:
        line = line[:-1]
        tv = parse3f(None, line, "v")
        if tv!=None:
            v.append(tv)
        tl = parse2i(None, line, "l")
        if tl!=None:
            l.append(tl)
    return (len(verticies), verticies, len(lines), lines)

def parseMD5Mesh(filepath, filename, source_fp, meta, verbose=0):
    # Data to load and store
    numJoints = 0
    numMeshes = 0
    joints = []
    meshes = []
    # Load from the .md5mesh file
    for line in source_fp:
        if line.find("MD5Version")>=0:
            if line.find("MD5Version 10")!=0:
                print("Unknown MD5Version!")
                return False
        if line.find("numJoints")>=0:
            numJoints = int(line.split(" ")[1])
            #print("numJoints=", numJoints)
        if line.find("numMeshes")>=0:
            numMeshes = int(line.split(" ")[1])
            #print("numMeshes=", numMeshes)
        if line.find("joints {")>=0:
            #print("Joints")
            for line2 in source_fp:
                m = re.search("^\s\"(.*)\"\s(-?\d+)\s\(\s(-?\d*\.{0,1}\d+)\s(-?\d*\.{0,1}\d+)\s(-?\d*\.{0,1}\d+)\s\)\s\(\s(-?\d*\.{0,1}\d+)\s(-?\d*\.{0,1}\d+)\s(-?\d*\.{0,1}\d+)\s\).*$",line2)
                if m:
                    try:
                        name = m.group(1)
                        parent = int(m.group(2))
                        pos = [0, 0, 0]
                        pos[0] = float(m.group(3))
                        pos[1] = float(m.group(4))
                        pos[2] = float(m.group(5))
                        ori = [0, 0, 0]
                        ori[0] = float(m.group(6))
                        ori[1] = float(m.group(7))
                        ori[2] = float(m.group(8))
                        joints.append((name, parent, pos, ori))
                    except Exception as e:
                        print("Error loading joint", e)
                if line2.find("}")>=0:
                    #print("Loaded", len(joints), "joints")
                    break
        if line.find("mesh {")>=0:
            #print("Mesh")
            shader_id = None
            numverts = 0
            verts = []
            numtris = 0
            tris = []
            numweights = 0
            weights = []
            for line2 in source_fp:
                if line2.find("shader ")>=0:
                    shader_name = line2.split(" ")[1][1:-2]
                    try:
                        shader_id = meta["materials"][filepath+"/"+filename[:filename.rfind(".")]+".mtl:"+shader_name]
                    except KeyError:
                        shader_id = (-1, -1)

                # Mesh->Vert
                if line2.find("numverts ")>=0:
                    numverts = int(line2.split(" ")[1])
                    #print("numVerts ", numverts)
                if line2.find("vert ")>=0:#vert vertIndex ( s t ) startWeight countWeight
                    m = re.search("^\svert\s(\d+)\s\(\s(-?\d*\.{0,1}\d+)\s(-?\d*\.{0,1}\d+)\s\)\s([0-9]+)\s([0-9]+).*$", line2)
                    if m:
                        try:
                            vertIndex = int(m.group(1))
                            tex = [0, 0]
                            tex[0] = float(m.group(2))
                            tex[1] = float(m.group(3))
                            startWeight = int(m.group(4))
                            countWeight = int(m.group(5))
                            verts.append((vertIndex, tex, startWeight, countWeight))
                        except Exception as e:
                            print("Error loading vert", e)
                    else:
                        print("Vert match failed", line2)

                # Mesh->Tri
                if line2.find("numtris ")>=0:
                    numtris = int(line2.split(" ")[1])
                    #print("numTris", numtris)
                if line2.find("tri ")>=0:#tri triIndex vertIndex[0] vertIndex[1] vertIndex[2]
                    m = re.search("^\stri\s([0-9]+)\s([0-9]+)\s([0-9]+)\s([0-9]+).*$", line2)
                    if m:
                        try:
                            triIndex = int(m.group(1))
                            vertIndex0 = int(m.group(2))
                            vertIndex1 = int(m.group(3))
                            vertIndex2 = int(m.group(4))
                            tris.append((triIndex, vertIndex0, vertIndex1, vertIndex2))
                        except Exception as e:
                            print("Error loading tri", e)
                    else:
                        print("Tri match failed", line2)

                # Mesh->weight
                if line2.find("numweights ")>=0:
                    numweights = int(line2.split(" ")[1])
                    #print("numWeights", numweights)
                if line2.find("weight ")>=0:#weight weightIndex joint bias ( pos.x pos.y pos.z )
                    #weight 17 15 1.000000 ( -0.288115 1.239427 0.166542 )
                    m = re.search("^\sweight\s(-?\d+)\s(-?\d+)\s(-?\d*\.{0,1}\d+)\s\(\s(-?\d*\.{0,1}\d+)\s(-?\d*\.{0,1}\d+)\s(-?\d*\.{0,1}\d+)\s\).*$", line2)
                    if m:
                        try:
                            weightIndex = int(m.group(1))
                            joint = int(m.group(2))
                            bias = float(m.group(3))
                            pos = [0, 0, 0]
                            pos[0] = float(m.group(4))
                            pos[1] = float(m.group(5))
                            pos[2] = float(m.group(6))
                            weights.append((weightIndex, joint, bias, pos))
                        except Exception as e:
                            print("Error loading tri", e)
                    else:
                        print("Tri match failed", line2)
                if line2.find("}")>=0:
                    break
            if len(verts)!=numverts:
                print("Expected to load "+str(numverts)+" verts, but instead loaded "+str(len(verts)))
            if len(tris)!=numtris:
                print("Expected to load "+str(numtris)+" tris, but instead loaded "+str(len(tris)))
            if len(weights)!=numweights:
                print("Expected to load "+str(numweights)+" weights, but instead loaded "+str(len(weights)))
            meshes.append((shader_id, numverts, verts, numtris, tris, numweights, weights))
    if verbose>=1:
        print("\t"+str(numJoints)+" joints, "+str(numMeshes)+" meshes")
    return (numJoints, joints, numMeshes, meshes)

def parseMD5Anim(filepath, filename, source_fp, meta, verbose=0):
        # args
    #hierarchy
    #bounds
    #baseframe
    #frame 0..n
    numFrames = None
    numJoints = None
    frameRate = None
    numAnimatedComponents = None
    hierarchy = []
    bounds = []
    baseframe = []
    frames = []
    for line in source_fp:
        #print(line)
        line = line[:-1]#Remove \n

        temp_numFrames = parse1i(None, line, "numFrames")
        if temp_numFrames!=None:
            numFrames = temp_numFrames
            frames = [0]*temp_numFrames
        numJoints = parse1i(numJoints, line, "numJoints")
        frameRate = parse1i(frameRate, line, "frameRate")
        numAnimatedComponents = parse1i(numAnimatedComponents, line, "numAnimatedComponents")

        if line.find("hierarchy {")>=0:
            for line2 in source_fp:
                m = re.search("^\s+\"(.*)\"\s*"+REGEX_INT_CLASS+"\s+"+REGEX_INT_CLASS+"\s+"+REGEX_INT_CLASS+".*$", line2)

                if m:
                    name = m.group(1)
                    parent = int(m.group(2))
                    flags = int(m.group(3))
                    startIndex = int(m.group(4))
                    hierarchy.append([name, parent, flags, startIndex])

                # Break line2
                if line2.find("}")>=0:
                    break


        if line.find("bounds {")>=0:
            for line2 in source_fp:

                #( -1.634066 -1.634066 -1.634066 ) ( -1.634066 6.444685 5.410537 )
                m = re.search("^\s*\(\s*"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s*\)\s*\(\s*"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s*\).*$", line2)
                if m:
                    mins = [float(m.group(1)), float(m.group(2)), float(m.group(3))]
                    maxs = [float(m.group(4)), float(m.group(5)), float(m.group(6))]
                    bounds.append([mins, maxs])

                # Break line2
                if line2.find("}")>=0:
                    break

        if line.find("baseframe {")>=0:
            for line2 in source_fp:

                m = re.search("^\s*\(\s*"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s*\)\s*\(\s*"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s+"+REGEX_FLOAT_CLASS+"\s*\).*$", line2)
                if m:
                    pos = [float(m.group(1)), float(m.group(2)), float(m.group(3))]
                    ori = [float(m.group(4)), float(m.group(5)), float(m.group(6))]
                    bounds.append([pos, ori])
                
                # Break line2
                if line2.find("}")>=0:
                    break

        m = re.match("frame\s*(\d+)\s*{", line)
        if m:
            frameNum = int(m.group(1))
            frame = []
            for line2 in source_fp:

                # Break line2
                if line2.find("}")>=0:
                    break

                m = re.search("^\s+(.*?)\s*$", line2)
                if m:
                    floats_line = m.group(1)
                    floats_split = floats_line.split(" ")
                    for f in floats_split:
                        frame.append(float(f))
                

            frames[frameNum] = frame

    if(verbose==1):
        print("\t"+str(numFrames)+" frames, "+str(numJoints)+" joints, "+str(numAnimatedComponents)+" Animted Components")
    return [numFrames, numJoints, frameRate, numAnimatedComponents, hierarchy, bounds, baseframe, frames]
