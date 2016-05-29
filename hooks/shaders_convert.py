#!/usr/bin/env python3

import os,re,sys

def loadShaderPrograms(shader_programsfile):
    shaders = []
    with open(shader_programsfile, "r") as fp:
        for line in fp:
            line = line[:-1]
            colonpos = line.find(":")
            shaderprogram_name = line[0:colonpos]
            shaders.append({'name':shaderprogram_name, 'shaders':line[colonpos+1:].split(",")})
    return shaders

def loadVariableNames(variables_file, variables_outfile):
    variables = []
    with open(variables_file, "r") as fp:
        for line in fp:
            line = line[:-1]
            if line.find("//")!=0:
                variables.append(line)
    shaderNames = ""
    with open(variables_outfile, "w") as fp:
        for i in range(len(variables)):
            print("#define SHADERVAR_"+variables[i]+" "+str(i), file=fp)
            shaderNames += ", \""+variables[i]+"\""
        print("#define SHADERVAR_COUNT "+str(len(variables)), file=fp)
        print("#define SHADERVAR_NAMES "+shaderNames[2:], file=fp)
    return variables

def loadShader(shader_folder, shader_filename, is_include=False, macros={}):
    shader_type = -1
    shader_code = ""
    imports = []
    linecount = 0
    with open(shader_folder+shader_filename, "r") as fp:
        for line in fp:
            line = line[:-1]
            linecount += 1
            # Check the type
            if not is_include:
                if line.find("//vertex")==0:
                    shader_type = 0
                    shader_code = "#version 330\\n"
                    continue
                if line.find("//fragment")==0:
                    shader_type = 1
                    shader_code = "#version 330\\n"
                    continue
            # Remove comments
            line = re.sub("//.*$", "", line)
            # define
            m = re.search("#define ([^\s]+)( (.+))?", line)
            if m:
                if len(m.groups())==3:
                    macros[m.group(1)] = m.group(3)
                else:
                    macros[m.group(1)] = True
                line = ""
            # import
            m = re.search("#include \"(.+)\"", line)
            if m:
                includefile = m.group(1)
                shader = loadShader(shader_folder, includefile, is_include=True, macros=macros)
                line = line.replace("#include \""+includefile+"\"", shader['code'])
                macros = {**macros, **shader["macros"]}
                imports += shader["imports"]
            # Trim
            line = line.strip()
            # Remove blank lines
            if re.search("^\s*$", line):
                continue
            # Macros->ifdef/ifndef
            m = re.search("^\s*#(ifdef|ifndef) (.*)\s*$", line)
            if m:
                sym = m.group(1)
                macro = m.group(2)

                if sym=="ifdef" or sym=="ifndef":
                    if ( sym=="ifdef" and macros.get(macro)==None ) or ( sym=="ifndef" and macros.get(macro)!=None ):#If expression invalid
                        for line2 in fp:#Skip to endif or else
                            linecount += 1
                            if re.search("^\s*#endif\s*$", line2):
                                break
                            if re.search("^\s*#else\s*$", line2):
                                break
                    continue
            # Macros->endif
            if re.search("^\s*#endif\s*$", line):
                continue
            # Macros->Replace
            for macro in macros:
                if type(macros[macro])==str:
                    line = line.replace(macro, macros[macro])
            # Imports
            m = re.search("^.*\s*(import(\((.*)\))?\s+).*?([a-zA-Z0-9_]+);.*$", line)
            if m:
                line = line.replace(m.group(1), "")
                if m.group(3)==None:
                    imports.append({'name':m.group(4), 'identifier':m.group(4), 'uniform':line.find("uniform")>=0, 'file':shader_filename, 'line':linecount})
                else:
                    imports.append({'name':m.group(3), 'identifier':m.group(4), 'uniform':line.find("uniform")>=0, 'file':shader_filename, 'line':linecount})
            # Done
            shader_code += line+"\\n"
    if is_include==False and shader_type==-1:
        print("ERROR: "+shader_filename+" is not typed")
        sys.exit(1)
    return {"type":shader_type, "code":shader_code, "macros":macros, "imports":imports}

def generateShaderMask(shader_variables, imports):
    mask = 0
    for i in range(len(shader_variables)):
        varname = shader_variables[i]
        for sv in imports:
            if sv["name"]==varname:
                mask |= 1<<i
    return mask

def loadShaderfiles(shadervars, shader_folder="src/render/shaders/code/"):
    shaderfiles = []
    shaderfiles_metadata = {}
    for f in os.listdir(shader_folder):
        if f[-2:]==".c":
            shaderfiles.append(f)
            meta = loadShader(shader_folder, f)
            meta['id'] = -1

            shadervar_mask = 0
            shadervar_overridesMask = 0
            shadervar_overrides = [None]*len(shadervars)
            shadervar_custom = []
            for theimport in meta["imports"]:
                override = theimport["name"]!=theimport["identifier"]
                try:#Builtin
                    varindex = shadervars.index(theimport["name"])
                    if override:
                        print(theimport['file']+":"+str(theimport['line'])+"\t\tOverride SHADERVAR "+theimport["name"]+" with "+theimport['identifier'], file=sys.stderr)
                        shadervar_overridesMask |= (1<<varindex)
                        shadervar_overrides[varindex] = theimport["identifier"]
                    else:
                        shadervar_mask |= (1<<varindex)
                except ValueError:#Custom
                    print(theimport['file']+":"+str(theimport['line'])+"\t\tCustom SHADERVAR "+theimport["name"], file=sys.stderr)
                    shadervar_custom.append(theimport["identifier"])

            meta['shadervar_mask'] = shadervar_mask
            meta["shadervar_overridesMask"] = shadervar_overridesMask
            meta['shadervar_overrides'] = shadervar_overrides
            meta['shadervar_custom'] = shadervar_custom

            shaderfiles_metadata[f] = meta
    return shaderfiles, shaderfiles_metadata

def combineOverrides(a, b):
    for i in range(len(a)):
        if b[i]!=None:
            if a[i]!=None:
                print("Override varaiables "+a[i]+" exist twice!")
                sys.exit(1)
            a[i] = b[i]
    return a


def parseShaders(variables_file, variables_outfile, shaders_folder, shaders_file, shaders_outfile, shader_programs_out):
    shader_variables = loadVariableNames(variables_file, variables_outfile)
    shader_programs = loadShaderPrograms(shaders_file)
    shaders, shaderfiles_metadata = loadShaderfiles(shader_variables, shaders_folder)
    shaders_ordered = []
    shader_ids = {}
    shader_lastid = 0
    shader_header_buffer = ""
    shader_files_buffer = ""
    shader_programs_buffer = ""
    with open(shader_programs_out, "w") as fp:
        for shaderprogram_index in range(len(shader_programs)):
            shaderprogram = shader_programs[shaderprogram_index]
            print("#define SHADER_"+shaderprogram["name"]+" "+str(shaderprogram_index), file=fp)

            # ID the shader files
            shaderprogram_shadersline = ""
            shaderprogram_mask = 0
            shaderprogram_overrideMask = 0
            shaderprogram_overrides = [None]*len(shader_variables)
            shaderprogram_customsline = ""
            shaderprogram_customcount = 0

            # Collect the shaders into the program
            for shaderfile_filename in shaderprogram["shaders"]:
                shaderfile_meta = shaderfiles_metadata[shaderfile_filename]
                if shaderfile_meta["id"]==-1:#If new shaderfile
                    shaderfile_meta["id"] = shader_lastid
                    shaders_ordered.append(shaderfile_filename)
                    shader_files_buffer += "shader_files["+str(shader_lastid)+"] = ShaderProgram::compileShader(\""+shaderfile_filename+"\", "+str(shaderfile_meta['type'])+", \""+shaderfile_meta["code"]+"\");\n"
                    shader_lastid += 1
                shaderprogram_shadersline += ", shader_files["+str(shaderfile_meta["id"])+"]"
                shaderprogram_mask |= shaderfile_meta["shadervar_mask"]
                shaderprogram_overrideMask |= shaderfile_meta["shadervar_overridesMask"]
                shaderprogram_overrides = combineOverrides(shaderprogram_overrides, shaderfile_meta["shadervar_overrides"])
                for cv in shaderfile_meta["shadervar_custom"]:
                    shaderprogram_customsline += ", \""+cv+"\""
                    print("#define SHADER_"+shaderprogram["name"]+"_"+cv+" "+str(len(shader_variables)+shaderprogram_customcount), file=fp)
                    shaderprogram_customcount += 1

            if len(shaderprogram_shadersline)>2:
                shaderprogram_shadersline = shaderprogram_shadersline[2:]
            if len(shaderprogram_customsline)>2:
                shaderprogram_customsline = shaderprogram_customsline[2:]
            shaderprogram_overridesline = ""
            for ovr in shaderprogram_overrides:
                if ovr!=None:
                    shaderprogram_overridesline += ", \""+ovr+"\""
            if len(shaderprogram_overridesline)>2:
                shaderprogram_overridesline = shaderprogram_overridesline[2:]

            # Write the shader program
            shader_programs_buffer += "shader_program["+str(shaderprogram_index)+"] = new ShaderProgram("+str(shaderprogram_index)+", \""+shaderprogram["name"]+"\", {"+shaderprogram_shadersline+"}, "+str(shaderprogram_mask)+", "+str(shaderprogram_overrideMask)+", {"+shaderprogram_overridesline+"}, {"+shaderprogram_customsline+"});\n"

    # Write the header line
    shader_header_buffer = "shader_files = new GLint["+str(len(shaders))+"];\nshader_program = (ShaderProgram **)malloc(sizeof(ShaderProgram *)*"+str(len(shader_programs))+");\n"

    with open(shaders_outfile, "w") as fp:
        print(shader_header_buffer, file=fp)
        print(shader_files_buffer, file=fp)
        print(shader_programs_buffer, file=fp)

if __name__=="__main__":
    #SHADER_VARIABLES_IN = ../src/render/shaders/shader_variables.txt
    #SHADER_VARIABLES_OUT = ../src/render/shaders/ShaderVariables.h
    #SHADER_FOLDER = ../src/render/shaders/code/
    #SHADER_DESC = ../src/render/shaders/shaders.txt
    #SHADER_HEADER = ../src/render/shaders/ShaderPrograms.h
    #SHADER_LOADER = ../src/render/shaders/ShaderLoader.c
    variables_file = ""
    variables_outfile = ""
    shaders_folder = ""
    shaders_file = ""
    shaders_outfile = ""
    shader_programs_out = ""
    for i in range(len(sys.argv)):
        arg = sys.argv[i]
        if arg=="--variables-in":
            variables_file = sys.argv[i+1]
        elif arg=="--variables-out":
            variables_outfile = sys.argv[i+1]
        elif arg=="--shaders-folder":
            shaders_folder = sys.argv[i+1]
        elif arg=="--shaders-desc":
            shaders_file = sys.argv[i+1]
        elif arg=="--shaders-header":
            shader_programs_out = sys.argv[i+1]
        elif arg=="--shaders-loader":
            shaders_outfile = sys.argv[i+1]
    #parseShaders(sys.argv[2], sys.argv[4], sys.argv[6], sys.argv[8], sys.argv[10], sys.argv[12])
    parseShaders(variables_file, variables_outfile, shaders_folder, shaders_file, shaders_outfile, shader_programs_out)
