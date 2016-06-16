#!/usr/bin/env python3

import sys,subprocess

from wand.image import Image
from asset_common import writeType

#cmd="convert -format dds -define dds:mipmaps=5 -define dds:compression=dxt1"
cmd="convert -format dds -define dds:compression=dxt1"

def convertImage(infile_path, outfile_fp):
    p = subprocess.Popen(cmd+" "+infile_path+" dds:-", shell=True, stdout=subprocess.PIPE)
    img_blob = p.stdout.read() #[4:]
    dataToWrite = img_blob
    writeType(outfile_fp, [len(dataToWrite)])
    amountWritten = outfile_fp.write(dataToWrite)
    print("\t"+str(amountWritten), "bytes written")
    outfile_fp.flush()

