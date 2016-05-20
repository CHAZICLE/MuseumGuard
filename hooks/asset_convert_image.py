#!/usr/bin/env python3

import sys

from wand.image import Image

#convert -format dds -define dds:mipmaps=5 -define dds:compression=dxt1

def convertImage(infile_path, outfile_fp):
    with Image(filename=infile_path) as img:
        img.format = "DDS"
        img.save(file=outfile_fp)
