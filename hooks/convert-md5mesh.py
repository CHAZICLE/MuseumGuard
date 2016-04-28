#!/usr/bin/env python3

md5_folder = "src/render/md5/"


import sys,gzip,re,struct

from common import writeType


def loadMD5MeshFile(md5mesh_file):
    # Data to load and store
    numJoints = 0
    numMeshes = 0
    joints = []
    meshes = []
    # Load from the .md5mesh file
    for line in md5mesh_file:
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
            for line2 in md5mesh_file:
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
            shader_name = "Unnamed"
            numverts = 0
            verts = []
            numtris = 0
            tris = []
            numweights = 0
            weights = []
            for line2 in md5mesh_file:
                if line2.find("shader ")>=0:
                    shader_name = line2.split(" ")[1][1:-2]
                    #print("Shader name", shader_name)

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
            meshes.append((shader_name, numverts, verts, numtris, tris, numweights, weights))
    md5mesh_file.close()
    return (numJoints, numMeshes, joints, meshes)

if __name__ == "__main__":
    md5mesh_file_path = sys.argv[1]
    md5mesh_out_path = sys.argv[2] if len(sys.argv)>=3 else md5mesh_file_path.replace("src/", "bin/").replace(".md5mesh", ".mdo.gz")
    print(md5mesh_file_path+" => "+md5mesh_out_path)
    md5mesh_file = open(md5mesh_file_path, "r")
    fp = gzip.open(md5mesh_out_path, "wb")
    md5mesh = loadMD5MeshFile(md5mesh_file)
    fp.write(bytes([2]))
    writeType(fp, md5mesh_file_path[md5mesh_file_path.rfind("/")+1:]);
    writeType(fp, md5mesh)
    fp.close()
