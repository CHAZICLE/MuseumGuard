#!/usr/bin/env python3

import sys

from asset_common import *

if __name__=="__main__":
    current_type = 0
    sources = []
    objects = []
    metafile = None
    for i in range(len(sys.argv[1:])):
        a = sys.argv[i]
        if a=="--source":
            current_type = 1
            continue
        if a=="--object":
            current_type = 2
            continue
        if a=="--meta":
            metafile = sys.argv[i+1]
            continue
        if current_type==1:
            sources.append(a)
        elif current_type==2:
            objects.append(a)

    if len(sources)!=len(objects):
        print("ERROR: Sources length and objects length don't match")

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
        print("#define ASSET_"+fn.upper()+"_"+ext.upper()+" "+str(i), file=meta_fp)
        if ext=="mtl":
            mtl_count += 1
        elif ext=="obj":
            obj_count += 1
        elif ext=="md5mesh":
            md5mesh_count += 1
        elif ext=="md5anim":
            md5anim_count += 1

    print("\n#define ASSETS_COUNT "+str(len(sources))+"\n#define ASSETS_MTL_COUNT "+str(mtl_count)+"\n#define ASSETS_OBJ_COUNT "+str(obj_count)+"\n#define ASSETS_MD5MESH_COUNT "+str(md5mesh_count)+"\n#define ASSETS_MD5ANIM_COUNT "+str(md5anim_count), file=meta_fp)
    meta_fp.close()
