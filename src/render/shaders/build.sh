#!/bin/env bash

shadersfolder="src/render/shaders"

echo -en "/*\n * THIS FILE IS AUTO GENERATED\n * DO NOT EDIT!\n */\nvoid compileShaders();\n" > $shadersfolder/shadercode.h
echo -en "/*\n * THIS FILE IS AUTO GENERATED\n * DO NOT EDIT!\n */\nvoid compileShaders() {\n\tconst char *vertex_code;\n\tconst char *fragment_code;\n\n" > $shadersfolder/shadercode.c

initimportcode=""
for file in $shadersfolder/glsl/*.glsl.c; do
	echo "convert-shader $file"
	shadername=`echo "$file" | sed -rn 's/.*\/([^\.]*).glsl.c/\1/p'`
	initimportcode="${initimportcode}\tGLuint program_${shadername} = 0;\n"
	codeType=""
	vertexShaderCode=""
	fragmentShaderCode=""
	declareimportcode=""
	defineimportcode=""
	while read -r line; do
		if [ "$line" == "#vertex" ]; then
			codeType="vertex"
		elif [ "$line" == "#fragment" ]; then
			codeType="fragment"
		else
			if [[ "$line" =~ import.* ]]; then
				varname="$(echo "$line" | sed -rn 's/.*\W(.*?);/\1/p')"
				declareimportcode="$declareimportcode,program_${shadername}_$varname"
				initimportcode="${initimportcode}\tGLint program_${shadername}_$varname = 0;\n"
				if [[ "$line" =~ import.*uniform.* ]]; then
					defineimportcode="${defineimportcode}\t\tprogram_${shadername}_$varname = getUniformLocation(program_${shadername}, \"$varname\");\n"
				elif [[ "$line" =~ import.*in.* ]]; then
					defineimportcode="${defineimportcode}\t\tprogram_${shadername}_$varname = getAttributeLocation(program_${shadername}, \"$varname\");\n"
				fi
				line="$(echo "$line" | sed -rn 's/import\W(.*)/\1/p')"
			fi
			if [ "$codeType" == "vertex" ]; then
				vertexShaderCode="$vertexShaderCode$line\\\n"
			elif [ "$codeType" == "fragment" ]; then
				fragmentShaderCode="$fragmentShaderCode$line\\\n"
			else
				echo "Code type not specified in $file"
				exit 1
			fi
		fi
	done < "$file"
	echo -en "extern GLuint program_${shadername};\nextern GLint ${declareimportcode:1};\n" >> $shadersfolder/shadercode.h
	echo -en "\tvertex_code=\"$vertexShaderCode\";\n\tfragment_code=\"$fragmentShaderCode\";\n\tprogram_${shadername} = loadShaders(\"${shadername}\", vertex_code, fragment_code);\n$defineimportcode\n" >> $shadersfolder/shadercode.c
done

echo -e "\t}\n$initimportcode" >> $shadersfolder/shadercode.c
