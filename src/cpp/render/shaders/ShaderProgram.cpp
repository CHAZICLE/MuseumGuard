#include "util/Globals.hpp"
#include <iostream>

#include "ShaderProgram.hpp"

using namespace render;
using namespace shaders;

GLint *ShaderProgram::shader_files = 0;
ShaderProgram **ShaderProgram::shader_program = 0;

ShaderProgram::ShaderProgram(int shaderprogram_id, std::string shaderprogram_name, std::vector<GLint> shaderFiles, ShaderMask shaderVar_mask, ShaderMask shaderVar_overrideMask, std::vector<std::string> shaderVar_overrides, std::vector<std::string> shaderVar_custom)
{
	this->shaderprogram_id        = shaderprogram_id;
	this->shaderprogram_name      = shaderprogram_name;
	this->shaderFiles             = shaderFiles;
	this->shaderVar_mask          = shaderVar_mask;
	this->shaderVar_overrideMask  = shaderVar_overrideMask;
	this->shaderVar_overrides     = shaderVar_overrides;
	this->shaderVar_custom        = shaderVar_custom;

	programID = glCreateProgram();
	for(auto shader : shaderFiles)
		glAttachShader(programID, shader);
	glLinkProgram(programID);
	
	GLint compileStatus,infoLogLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &compileStatus);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(compileStatus==GL_FALSE)
	{
		char *infoLog = new char[infoLogLength+1];
		glGetProgramInfoLog(programID, infoLogLength, NULL, infoLog);
		infoLog[infoLogLength] = 0;
		std::cerr << "Failed to link program: " << shaderprogram_name << "\n" << infoLog << std::endl;
		std::exit(1);
	}
	this->shaderVar_locationsCount = SHADERVAR_COUNT+shaderVar_custom.size();
	this->shaderVar_locations = new GLint[this->shaderVar_locationsCount];
	for(int i=0;i<this->shaderVar_locationsCount;i++)
	{
		this->shaderVar_locations[i] = -1;
	}
	this->currentMaterial.assetId = -1;
	this->currentMaterial.materialId = -1;
}
ShaderProgram::~ShaderProgram()
{
	delete [] this->shader_program;
}
void ShaderProgram::useShader()
{
	glUseProgram(this->programID);
}
inline GLint getNamedShaderLocation(bool uniform, GLuint programID, const char *name)
{
	GLint a;
	if(uniform)
		a = glGetUniformLocation(programID, name);
	else
		a = glGetAttribLocation(programID, name);
	return a;
}
GLint ShaderProgram::getShaderLocation(bool uniform, int shaderVar)
{
	if(shaderVar<0 || shaderVar>=this->shaderVar_locationsCount)
		util::Globals::fatalError("Attempt to access invalid shaderVar for program "+std::to_string(this->programID));
	GLint &sl = this->shaderVar_locations[shaderVar];
	if(sl==-1)
	{
		if(shaderVar<SHADERVAR_COUNT)//If common shadervar
		{
			if((1<<shaderVar)&this->shaderVar_overrideMask)//If shaderVar is overriden
			{
				int shaderVarCustomID = 0;
				for(int i=0;i<shaderVar;i++)
				{
					if((1<<i)&shaderVar_overrideMask)
					{
						shaderVarCustomID++;
					}
				}
				sl = getNamedShaderLocation(uniform, this->programID, this->shaderVar_overrides[shaderVarCustomID].c_str());
			}
			if((1<<shaderVar)&this->shaderVar_mask)
			{
				sl = getNamedShaderLocation(uniform, this->programID, shaderNames[shaderVar].c_str());
			}
		}
		else//If custom shaderVar
		{
			sl = getNamedShaderLocation(uniform, this->programID, this->shaderVar_custom[shaderVar-SHADERVAR_COUNT].c_str());
		}
	}
	return sl;
}
bool ShaderProgram::setVertexAttributePointer(int shaderVar, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
	int location = getShaderLocation(false, shaderVar);
	if(location!=-1)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, size, type, normalized, stride, pointer);
		return true;
	}
	return false;
}

bool ShaderProgram::setMaterial(MaterialAsset mat)
{
	if(this->currentMaterial!=mat)
	{
		this->currentMaterial = mat;
		util::Asset *asset = util::AssetManager::getAssetManager()->getAsset(mat.assetId);
		render::MaterialLibrary *mtlAsset = dynamic_cast<render::MaterialLibrary *>(asset);
		if(asset==0 || mtlAsset==0)
			util::Globals::fatalError("Failed to find material asset");
		mtlAsset->updateShader(this, mat.materialId);
		return true;
	}
	return false;
}
ShaderProgram *ShaderProgram::getShader(int shaderIndex)
{
	return shader_program[shaderIndex];
}
GLint ShaderProgram::compileShader(std::string name, int shaderTypeId, std::string shaderCode)
{
	int shaderType = shaderTypeId==0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	const GLchar *code = shaderCode.c_str();
	const GLint length = shaderCode.length();
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &code, &length);
	glCompileShader(shaderId);

	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus==GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		char *infoLog = new char [infoLogLength+1];
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, infoLog);
		infoLog[infoLogLength] = 0;
		std::cerr << "Failed to compile " << (shaderType==GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader " << name << ":" << std::endl << "Error Log [#" << (infoLogLength-1) << "]: " << infoLog << std::endl << "Vertcode:" << std::endl << shaderCode << std::endl;
		return -1;
	}
	return shaderId;
}
void ShaderProgram::loadShaders()
{
	#include "render/shaders/ShaderLoader.c"
}
