#!/usr/bin/env python3

import sys,subprocess

from asset_parsers import *
from asset_common import *


if __name__=="__main__":
    if len(sys.argv)>=3:
        metaFile = sys.argv[2]
        fileToConvert = sys.argv[3]
        fileToPlace = sys.argv[4]
        ext = getFileExtension(fileToConvert)
        verbose = 1

        if ext=="png" or ext=="tga":
            proc = subprocess.Popen("convert -format dds -define dds:mipmaps=5 -define dds:compression=dxt1 "+fileToConvert+" dds:-", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            print(proc.stdout.read())
            print(proc.stderr.read())
        else:
            source_fp = open_sourcefile(fileToConvert)
            object_fp = open_objectfile(fileToPlace)


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

            source_fp.close()
            object_fp.close()
