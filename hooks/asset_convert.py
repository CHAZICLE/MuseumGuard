#!/usr/bin/env python3

import sys

from asset_parsers import *
from asset_common import *
from asset_convert_image import convertImage


if __name__=="__main__":
    global_meta_file = ""
    verbose = 0
    metaFile = ""
    srcFile = ""
    objFile = ""
    depFile = ""

    for i in range(len(sys.argv)):
        arg = sys.argv[i]
        if arg=="--global-meta":
            global_meta_file = sys.argv[i+1]
        elif arg=="--meta":
            metaFile = sys.argv[i+1]
        elif arg=="--src":
            srcFile = sys.argv[i+1]
        elif arg=="--depend":
            depFile = sys.argv[i+1]
        elif arg=="--out":
            objFile = sys.argv[i+1]
        elif arg=="--verbose" or arg=="-v":
            verbose = int(sys.argv[i+1])

    srcFileExt = getFileExtension(srcFile)
    meta = getMetadata(global_meta_file)
    srcFilePath = getFilePath(srcFile)
    srcFileName = srcFile[srcFile.rfind("/")+1:]

    objFileP = open_objectfile(objFile)

    result = True

    try:

        if srcFileExt=="png" or srcFileExt=="tga" or srcFileExt=="jpg":
            objFileP.write(bytes([4]))
            result = writeType(objFileP, getFileName(srcFile))
            convertImage(srcFile, objFileP)
        else:
            if srcFileExt=="mtl":
                parseFunc = parseMTL
            if srcFileExt=="obj":
                parseFunc = parseOBJ
            if srcFileExt=="md5mesh":
                parseFunc = parseMD5Mesh
            if srcFileExt=="md5anim":
                parseFunc = parseMD5Anim
            if srcFileExt=="nav.obj":
                parseFunc = parseNAVOBJ

            srcFileP = open_sourcefile(srcFile)
            fileId,fileMeta,fileData,fileDepend = parseFunc(srcFilePath, srcFileName, srcFileP, meta, verbose)
            
            if fileDepend!=None and len(fileDepend)>0:
                depFileP = open(depFile, "w")
                depFileP.write(srcFile+":")
                for f in fileDepend:
                    #print(f)
                    depFileP.write(" "+f)
                depFileP.write("\n")
                for f in fileDepend:
                    depFileP.write(f+":\n")
                depFileP.close()
            if fileMeta!=None and len(fileMeta)>0:
                with open(metaFile, "w") as fp:
                    for key in fileMeta["macros"]:
                        print("#define "+key+" "+str(fileMeta["macros"][key]), file=fp)

            objFileP.write(bytes([fileId]))
            writeType(objFileP, getFileName(srcFile))
            result = writeType(objFileP, fileData)


            #if ext=="mtl": parseFunc = parseMTL
            #    mtl = parseMTL
            #    object_fp.write(bytes([0]))
            #    result = writeType(object_fp, mtl)
            #elif ext=="obj":
            #    mtl = parseOBJ(filepath, filename, source_fp, meta, verbose)
            #    object_fp.write(bytes([1]))
            #    writeType(object_fp, getFileName(fileToConvert))
            #    result = writeType(object_fp, mtl)
            #elif ext=="md5mesh":
            #    mtl = parseMD5Mesh(filepath, filename, source_fp, meta, verbose)
            #    object_fp.write(bytes([2]))
            #    writeType(object_fp, getFileName(fileToConvert))
            #    result = writeType(object_fp, mtl)
            #elif ext=="md5anim":
            #    mtl = parseMD5Anim(filepath, filename, source_fp, meta, verbose)
            #    object_fp.write(bytes([3]))
            #    writeType(object_fp, getFileName(fileToConvert))
            #    result = writeType(object_fp, mtl)
            ## 4: Image
            #elif ext=="nav.obj":
            #    mtl = parseNAVOBJ(filepath, filename, source_fp, meta, verbose)
            #    object_fp.write(bytes([5]))
            #    writeType(object_fp, getFileName(fileToConvert))
            #    result = writeType(object_fp, mtl)


            srcFileP.close()
        objFileP.close()
    except Exception as e:
        raise e
        print("Exception:",e)
        result = False
    if result==False:
        print("Critical error!")
        os.remove(fileToPlace)
        sys.exit(1)
