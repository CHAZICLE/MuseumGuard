#!/usr/bin/env python3

import gzip,re,struct,sys,os

REGEX_INT = "-?\d+"
REGEX_FLOAT = "-?\d*\.{0,1}\d+"
REGEX_INT_CLASS = "("+REGEX_INT+")"
REGEX_FLOAT_CLASS = "("+REGEX_FLOAT+")"

def exitFailure(msg):
    print(msg, file=stderr)
    sys.exit(1)

def open_sourcefile(source_filename):
    return open(source_filename, "r")

def close_sourcefile(source_fp):
    source_fp.close()

def open_objectfile(source_filename):
    return gzip.open(source_filename, "wb")

def close_objectfile(object_fp):
    object_fp.close()

def getFilePath(filepath):
    return filepath[0:filepath.rfind("/")]

def getFileExtension(filename):
    return filename[filename.find(".")+1:]

def getFileName(filename):
    return filename[filename.rfind("/")+1:filename.find(".")]

def getMetadata(metafile):
    assetId = 0
    materials = {}
    textures = {}
    with open(metafile, "r") as fp:
        for line in fp:
            line = line[:-1]

            if line.find("// addMTLMaterial")>=0:
                m = re.search("// addMTLMaterial\("+REGEX_INT_CLASS+","+REGEX_INT_CLASS+",(.+),(.+)\)", line)
                if m:
                    mtlAssetID = int(m.group(1))
                    materialId = int(m.group(2))
                    mtlPath = m.group(3)
                    materialName = m.group(4)
                    materials[mtlPath+":"+materialName] = (mtlAssetID, materialId)
                else:
                    exitFailure("addMTLMaterial loading failed!\n"+line)

            if line.find("// addTexture")>=0:
                m = re.search("// addTexture\("+REGEX_INT_CLASS+",(.+)\)", line)
                if m:
                    textureAssetId = int(m.group(1))
                    texturePath = m.group(2)
                    textures[texturePath] = textureAssetId
                else:
                    exitFailure("addTexture loading failed!\n"+line)

    a = {'materials':materials, 'textures':textures}
    return a

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
            if not writeType(fp, e):
                return False
    elif tt==str:
        writeType(fp, len(thetype))
        fp.write(bytes(thetype, 'UTF-8'))
    else:
        print("Unknown type", tt)
        return False
    return True

def parse1i(current, line, key):
    m = re.search("^"+key+"\s"+REGEX_INT_CLASS+"$", line);
    if not m:
        return current
    return int(m.group(1))

def parse2i(current, line, key):
    m = re.search("^"+key+"\s"+REGEX_INT_CLASS+"\s"+REGEX_INT_CLASS+"$", line);
    if not m:
        return current
    return (int(m.group(1)), int(m.group(2)))

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

def parse2f(current, line, key):
    m = re.search("^"+key+"\s"+REGEX_FLOAT_CLASS+"\s"+REGEX_FLOAT_CLASS+"$", line);
    if not m:
        return current
    return (float(m.group(1)), float(m.group(2)))

def parse3f(current, line, key):
    m = re.search("^"+key+"\s"+REGEX_FLOAT_CLASS+"\s"+REGEX_FLOAT_CLASS+"\s"+REGEX_FLOAT_CLASS+"$", line);
    if not m:
        return current
    return (float(m.group(1)), float(m.group(2)), float(m.group(3)))

if __name__=="__main__":
    pass


