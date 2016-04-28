#!/usr/bin/env python3

import struct

def writeType(fp, thetype):
    tt = type(thetype)
    if tt==int:
#        print("Write Int", thetype)
        fp.write(struct.pack("i", thetype))
    elif tt==float:
#        print("Write float", thetype)
        fp.write(struct.pack("f", thetype))
    elif tt==list or tt==tuple:
#        print("Write list", thetype)
        for e in thetype:
            writeType(fp, e)
    elif tt==str:
        writeType(fp, len(thetype))
#        print("Write string", thetype)
        fp.write(bytes(thetype, 'UTF-8'))
    else:
        print("Unknown type", tt)
