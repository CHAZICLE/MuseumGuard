#!/bin/env python3

import os

def writeModelObject(name, object_name, object_usemtl, ohf, osf, indexMap, indexArr, indicies, enableTexCoords, enableNormals):
	name = name.replace(".", "_")
	object_name = object_name.replace(".", "_")
	print("GLuint _vertexIndicies_"+name+"_"+object_name+"[] = {",end="", file=osf)
	for i in indicies:
		print(i, end=",", file=osf)
	print("};", file=osf)
	print("extern struct ModelObject ModelObject_"+name+"_"+object_name+";", file=ohf)
	print("struct ModelObject ModelObject_"+name+"_"+object_name+" = {_vertexIndicies_"+name+"_"+object_name+", "+str(len(indicies))+", "+("NULL" if object_usemtl==None else "&Material_"+name+"_"+object_usemtl)+"};", file=osf)


def writeMaterial(name, material_name, ohf, osf, m):
	name = name.replace(".", "_")
	material_name = material_name.replace(".", "_")
	print("extern struct Material Material_"+name+"_"+material_name+";", file=ohf)
	print("struct Material Material_"+name+"_"+material_name+" = {", file=osf, end="")
	for i in range(1,5):
		if m[i]==None:
			m[i] = ""
	
	print("\""+m[1]+"\", \""+m[2]+"\", \""+m[3]+"\", \""+m[4]+"\", ", file=osf, end="")
	for i in range(5,9):
		smat = m[i]
		if smat==None:
		    smat = ["0.0", "0.0", "0.0"]
		print("{"+smat[0]+", "+smat[1]+", "+smat[2]+"}, ", file=osf, end="")
	print(m[9]+", "+m[10]+", "+m[11]+", "+m[12]+"};", file=osf)

def searchIndex(indexArr, indexMap, V):
	key = ""
	for v in V:
		for av in v:
		    key = key+","+str(av)
	key = key[1:]
	try:
	    return indexMap[key]
	except KeyError:
	    index = len(indexArr)
	    indexArr.append(V)
	    indexMap[key] = index
	    return index

def convertStringArrayToFloat(sa):
	r = []
	for a in sa:
		r.append(float(a))
	return r

def expandBoundingBox(vertex, boundingBox):
	if vertex[0]<boundingBox[0]:
	    boundingBox[0] = vertex[0]
	if vertex[0]>boundingBox[1]:
	    boundingBox[1] = vertex[0]

	if vertex[1]<boundingBox[2]:
	    boundingBox[2] = vertex[1]
	if vertex[1]>boundingBox[3]:
	    boundingBox[3] = vertex[1]

	if vertex[2]<boundingBox[4]:
	    boundingBox[4] = vertex[2]
	if vertex[2]>boundingBox[5]:
	    boundingBox[5] = vertex[2]

def processWavefrontFile(modelsFolder, wavefrontsFile, name, ohf, osf, wfFile):
	print("convert-model "+wavefrontsFile+"/"+name+".obj")
	openWFFile = open(wavefrontsFile+"/"+name+".obj");
	vertexPositions = []
	vertexTexCoords = []
	vertexNormals = []
	indexMap = dict()
	indexArr = []
	indicies = []
	enableNormals = False
	enableTexCoords = False
	object_name = ""
	object_usemtl = None
	selected_materials = []
	indexKeys = []
	model_objects = []
	print("\n// Begin "+name, file=osf)
	boundingBox = [0, 0, 0, 0, 0, 0]
	for line in openWFFile:
		if line[-1]=='\n':
			line = line[0:-1]
		if line[0]=='#':
			continue
		if line[0:7]=='usemtl ':
			object_usemtl = line[7:].replace(".", "_")
			if object_usemtl=="None":
			    object_usemtl = None
		if line[0]=='o':
			if len(indicies)>0:
				writeModelObject(name, object_name, object_usemtl, ohf, osf, indexMap, indexArr, indicies, enableTexCoords, enableNormals)
				model_objects.append("ModelObject_"+name+"_"+object_name)
				indicies = []
			object_name = line[2:].replace(".", "_")
		if line[0]=='v': # Process vertex data
			if line[1]==' ': # VPosition
				vp = convertStringArrayToFloat(line[2:].split(' '))
				expandBoundingBox(vp, boundingBox)
				vertexPositions.append(vp)
			elif line[1]=='t':
				vertexTexCoords.append(convertStringArrayToFloat(line[3:].split(' ')))
				enableTexCoords = True
			elif line[1]=='n':
				vertexNormals.append(convertStringArrayToFloat(line[3:].split(' ')))
				enableNormals = True
		if line[0]=='f': # Face w/ stuff
			facepoint = line[2:].split(" ")
			if len(facepoint)!=3:
				print("Wavefront file is not triangular with", len(facepoint),"face points, Skipping...")
				return
			for fp in facepoint:
				fpa = fp.split("/")
				V = [vertexPositions[int(fpa[0])-1]]
				if enableTexCoords:
					if len(fpa)>1 and fpa[1]!='':
						V.append(vertexTexCoords[int(fpa[1])-1])
					else:
						V.append(vertexTexCoords[0])
				if enableNormals:
					if len(fpa)>2 and fpa[2]!='':
						V.append(vertexNormals[int(fpa[2])-1])
					else:
						V.append(vertexNormals[0])
				indicies.append(searchIndex(indexArr, indexMap, V))
	if len(indicies)>0:
		writeModelObject(name, object_name, object_usemtl, ohf, osf, indexMap, indexArr, indicies, enableTexCoords, enableNormals)
		model_objects.append("ModelObject_"+name+"_"+object_name)
	# mm_writeVertexData(name, object_name, ohf, osf, indexMap, indexArr, indicies, enableTexCoords, enableNormals)
	def mm_writeVertexData(name, object_name, ohf, osf, indexMap, indexArr, indicies, enableTexCoords, enableNormals):
		print("};\nstruct ModelData ModelData_"+name+" = {};", file=osf);
		print("extern struct ModelData ModelData_"+name+";", file=ohf);
	# Write model data
	if len(model_objects)>0:
		print("struct ModelObject _ModelObjects_"+name+"[] = {", end="", file=osf)
		for o in model_objects:
			print(o+", ", end="", file=osf)
		print("};\nGLfloat _ModelData_"+name+"[] = {", end="", file=osf)
		vertexDataCount = 0
		# ic[vertex][vertex attribute][vertex attribute value]
		for vi in range(len(indexArr)):#Each vertex
			for vai in range(len(indexArr[vi])):#Each vertex attribute
				for vavi in range(len(indexArr[vi][vai])):#Each vertex attribute value
					print(indexArr[vi][vai][vavi],end="f,", file=osf)
					vertexDataCount += 1
		print("extern struct Model Model_"+name+";", file=ohf)
		print("};\nstruct Model Model_"+name+" = {_ModelData_"+name+", "+str(len(indexArr))+", "+str(vertexDataCount)+", "+("true" if enableTexCoords else "false")+", "+("true" if enableNormals else "false")+", _ModelObjects_"+name+", "+str(len(model_objects))+", "+str(boundingBox[0])+", "+str(boundingBox[1])+", "+str(boundingBox[2])+", "+str(boundingBox[3])+", "+str(boundingBox[4])+", "+str(boundingBox[5])+"};", sep="", file=osf)
	# Material
	try:
		openMTFile = open(wavefrontsFile+"/"+name+".mtl")
		print("convert-material "+wavefrontsFile+"/"+name+".mtl")
	except:
		return
	
	currentMaterial = None
	for line in openMTFile:
		line = line[0:-1]
		if line[0:7]=="newmtl ":
			if currentMaterial!=None:
				writeMaterial(name, currentMaterial[0], ohf, osf, currentMaterial)
			if line[7:]=="None":
				currentMaterial = None
			else:
				currentMaterial = [line[7:],
						None, None, None, None,
						None, None, None, None,
						"0", "0", "0", "0"]
		if currentMaterial!=None:
			# Texture Maps
			if line[0:7]=="map_Ka ":
				currentMaterial[1] = line[7:]
			elif line[0:7]=="map_Kd ":
				currentMaterial[2] = line[7:]
			elif line[0:7]=="map_Ks ":
				currentMaterial[3] = line[7:]
			elif line[0:7]=="map_Ke ":
				currentMaterial[4] = line[7:]
			# Global Lighting
			elif line[0:3]=="Ka ":
				currentMaterial[5] = line[3:].split(" ")
			elif line[0:3]=="Kd ":
				currentMaterial[6] = line[3:].split(" ")
			elif line[0:3]=="Ks ":
				currentMaterial[7] = line[3:].split(" ")
			elif line[0:3]=="Ke ":
				currentMaterial[8] = line[3:].split(" ")
			# Other
			elif line[0:3]=="Ns ":
				currentMaterial[9] = line[3:]
			elif line[0:3]=="Ni ":
				currentMaterial[10] = line[3:]
			elif line[0:2]=="d ":
				currentMaterial[11] = line[2:]
			elif line[0:6]=="illum ":
				currentMaterial[12] = line[6:]
		# Printed update
	if currentMaterial!=None:
		writeMaterial(name, currentMaterial[0], ohf, osf, currentMaterial)

def processFolder(modelsFolder, wavefrontsFile):
	# Header file
	ohf = open(modelsFolder+"/wavefront/wavefront_export.h.txt", "w")
	osf = open(modelsFolder+"/wavefront/wavefront_export.c.txt", "w")
	# Process files
	for somefile in os.listdir(wavefrontsFile):
		if somefile.find(".obj") >= 0 and somefile[0]!=".":
			processWavefrontFile(modelsFolder, wavefrontsFile, somefile.replace(".obj",""), ohf, osf, wavefrontsFile+"/"+somefile);
	osf.close()
	ohf.close()

processFolder("src/render/", "src/render/wavefront");
