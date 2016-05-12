#!/usr/bin/env python3

import sys

from asset_parsers import *
from asset_common import *


if __name__=="__main__":
    if len(sys.argv)>=3:
        fileToConvert = sys.argv[1]
        fileToPlace = sys.argv[2]
        ext = getFileExtension(fileToConvert)

        source_fp = open_sourcefile(fileToConvert)
        object_fp = open_objectfile(fileToPlace)

        verbose = 1

        if ext=="mtl":
            mtl = parseMTL(source_fp, verbose)
            object_fp.write(bytes([0]))
            writeType(object_fp, getFileName(fileToConvert))
            writeType(object_fp, mtl)
        elif ext=="obj":
            mtl = parseOBJ(source_fp, verbose)
            object_fp.write(bytes([1]))
            writeType(object_fp, getFileName(fileToConvert))
            writeType(object_fp, mtl)
        elif ext=="md5mesh":
            mtl = parseMD5Mesh(source_fp, verbose)
            object_fp.write(bytes([2]))
            writeType(object_fp, getFileName(fileToConvert))
            writeType(object_fp, mtl)
        elif ext=="md5anim":
            mtl = parseMD5Anim(source_fp, verbose)
            object_fp.write(bytes([3]))
            writeType(object_fp, getFileName(fileToConvert))
            writeType(object_fp, mtl)
        #print(mtl)

        source_fp.close()
        object_fp.close()
