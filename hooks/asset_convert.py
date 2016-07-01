#!/usr/bin/env python3

import sys

from asset_parsers import *
from asset_common import *
from asset_convert_image import convertImage


if __name__=="__main__":
    if len(sys.argv)>=3:
        metaFile = sys.argv[2]
        fileToConvert = sys.argv[3]
        fileToPlace = sys.argv[4]
        ext = getFileExtension(fileToConvert)
        verbose = 1
        meta = getMetadata(metaFile)
        filepath = getFilePath(fileToConvert)
        filename = fileToConvert[fileToConvert.rfind("/")+1:]

        object_fp = open_objectfile(fileToPlace)

        result = True

        try:

            if ext=="png" or ext=="tga" or ext=="jpg":
                object_fp.write(bytes([4]))
                result = writeType(object_fp, getFileName(fileToConvert))
                convertImage(fileToConvert, object_fp)
            else:
                source_fp = open_sourcefile(fileToConvert)
                if ext=="mtl":
                    mtl = parseMTL(filepath, filename, source_fp, meta, verbose)
                    object_fp.write(bytes([0]))
                    writeType(object_fp, getFileName(fileToConvert))
                    result = writeType(object_fp, mtl)
                elif ext=="obj":
                    mtl = parseOBJ(filepath, filename, source_fp, meta, verbose)
                    object_fp.write(bytes([1]))
                    writeType(object_fp, getFileName(fileToConvert))
                    result = writeType(object_fp, mtl)
                elif ext=="md5mesh":
                    mtl = parseMD5Mesh(filepath, filename, source_fp, meta, verbose)
                    object_fp.write(bytes([2]))
                    writeType(object_fp, getFileName(fileToConvert))
                    result = writeType(object_fp, mtl)
                elif ext=="md5anim":
                    mtl = parseMD5Anim(filepath, filename, source_fp, meta, verbose)
                    object_fp.write(bytes([3]))
                    writeType(object_fp, getFileName(fileToConvert))
                    result = writeType(object_fp, mtl)
                # 4: Image
                elif ext=="nav.obj":
                    mtl = parseNAVOBJ(filepath, filename, source_fp, meta, verbose)
                    object_fp.write(bytes([5]))
                    writeType(object_fp, getFileName(fileToConvert))
                    result = writeType(object_fp, mtl)

                source_fp.close()
            object_fp.close()
        except Exception as e:
            raise e
            print("Exception:",e)
            result = False
        if result==False:
            print("Critical error!")
            os.remove(fileToPlace)
            sys.exit(1)
