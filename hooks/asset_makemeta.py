#!/usr/bin/env python3

import sys,re

from asset_common import *

def printMaterials(source_path, fp, assetId, assetPath):
    path = getFilePath(source_path)
    with open(source_path, "r") as f:
        material_count = -1
        for line in f:
            splits = line[:-1].split(" ")
            if splits[0]=="newmtl":
                material_count+=1
                print("// addMTLMaterial("+str(assetId)+","+str(material_count)+","+assetPath+","+splits[1]+")", file=fp)

if __name__=="__main__":
    sources = sys.argv[2:-2]
    metafile = sys.argv[-1]

    #print("Sources:", sources)
    #print("Objects:", objects)
    #print("Meta:", metafile)

    meta_fp = open(metafile, "w")

    mtl_count = 0
    obj_count = 0
    md5mesh_count = 0
    md5anim_count = 0

    for i in range(len(sources)):
        fn = getFileName(sources[i])
        ext = getFileExtension(sources[i])
        print("\n// addAsset("+str(i)+","+sources[i]+")", file=meta_fp)
        print("#define ASSET_"+fn.upper()+"_"+ext.upper()+" "+str(i), file=meta_fp)
        if ext=="mtl":
            mtl_count += 1
            printMaterials(sources[i], meta_fp, str(i), sources[i])
        elif ext=="obj":
            obj_count += 1
        elif ext=="md5mesh":
            md5mesh_count += 1
        elif ext=="md5anim":
            md5anim_count += 1
        elif ext=="tga" or ext=="png" or ext=="jpg":
            print("// addTexture("+str(i)+","+sources[i]+")", file=meta_fp)

    print("\n#define ASSETS_COUNT "+str(len(sources))+"\n#define ASSETS_MTL_COUNT "+str(mtl_count)+"\n#define ASSETS_OBJ_COUNT "+str(obj_count)+"\n#define ASSETS_MD5MESH_COUNT "+str(md5mesh_count)+"\n#define ASSETS_MD5ANIM_COUNT "+str(md5anim_count), file=meta_fp)
    meta_fp.close()
