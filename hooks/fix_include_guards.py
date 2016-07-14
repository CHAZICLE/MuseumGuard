#!/usr/bin/env python3

import os,re

def collapseFolders(folders):
    ap = ""
    for f in folders:
        ap += "/"+f
    return ap

def collapseFoldersMacro(folders, e):
    ap = ""
    for f in folders:
        ap += "_"+f
    return ap+"_"+e.replace(".", "_")+"_"

def mkdirs(rootDir, folders):
    os.makedirs(rootDir+collapseFolders(folders), exist_ok=True)

def processHeaderFile(rootDir, folders, e):
    with open(rootDir+collapseFolders(folders)+"/"+e, "r") as fp:
        a = fp.readlines();
        if re.search("^#ifndef .*$", a[0]) and re.search("^#define .*$", a[1]):
            print("Fixing", rootDir+collapseFolders(folders)+"/"+e)
            macro = ("3YP"+collapseFoldersMacro(folders,e)).upper()
            a[0] = "#ifndef "+macro+"\n"
            a[1] = "#define "+macro+"\n"
            #mkdirs("./test", folders)
            with open(rootDir+collapseFolders(folders)+"/"+e, "w") as fp2:
                fp2.writelines(a)

def processDir(rootDir, folders):
    for e in os.listdir(rootDir+collapseFolders(folders)):
        if os.path.isdir(rootDir+collapseFolders(folders)+"/"+e):
            processDir(rootDir, folders+[e])
        if re.search("^.*\.hpp$", e):
            processHeaderFile(rootDir, folders, e)

if __name__=="__main__":
    processDir("./src", []);
