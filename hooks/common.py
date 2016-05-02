#!/usr/bin/env python3

import struct

def writeType(fp, thetype):
    tt = type(thetype)
    if tt==int:
        fp.write(struct.pack("i", thetype))
    elif tt==float:
        fp.write(struct.pack("f", thetype))
    elif tt==bool:
        fp.write(struct.pack("?", thetype))
    elif tt==list or tt==tuple:
        for e in thetype:
            writeType(fp, e)
    elif tt==str:
        writeType(fp, len(thetype))
        fp.write(bytes(thetype, 'UTF-8'))
    else:
        print("Unknown type", tt)
