#!/usr/bin/env python3

import gzip,re,struct

REGEX_INT = "-?\d+"
REGEX_FLOAT = "-?\d*\.{0,1}\d+"
REGEX_INT_CLASS = "("+REGEX_INT+")"
REGEX_FLOAT_CLASS = "("+REGEX_FLOAT+")"

def open_sourcefile(source_filename):
    return open(source_filename, "r")

def close_sourcefile(source_fp):
    source_fp.close()

def open_objectfile(source_filename):
    return gzip.open(source_filename, "wb")

def close_objectfile(object_fp):
    object_fp.close()

def getFileExtension(filename):
    return filename[filename.rfind(".")+1:]

def getFileName(filename):
    return filename[filename.rfind("/")+1:filename.rfind(".")]


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

def parse1i(current, line, key):
    m = re.search("^"+key+"\s(-?\d+)$", line);
    if not m:
        return current
    return int(m.group(1))

def parse1s(current, line, key):
    m = re.search("^"+key+"\s(.+?)$", line);
    if not m:
        return current
    return m.group(1)

def parse1f(current, line, key):
    m = re.search("^"+key+"\s+"+REGEX_FLOAT_CLASS+".*$", line);
    if not m:
        return current
    return float(m.group(1))

def parse3f(current, line, key):
    m = re.search("^"+key+"\s(-?\d+\.{0,1}\d+)\s(-?\d+\.{0,1}\d+)\s(-?\d+\.{0,1}\d+)$", line);
    if not m:
        return current
    return [float(m.group(1)), float(m.group(1)), float(m.group(1))]

if __name__=="__main__":
    pass


