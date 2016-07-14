#ifndef 3YP_RENDER_SHADERS_SHADERPROGRAM_HPP_
#define 3YP_RENDER_SHADERS_SHADERPROGRAM_HPP_

namespace render {
	class RenderManager;
}

#include <string>
#include <cstdint>
#include <vector>

#include "ShaderVariables.h"
#include "util/gl.h"
#include "render/MaterialLibrary.hpp"


namespace render {
	namespace shaders {
		typedef std::int32_t ShaderMask;
		const std::vector<std::string> shaderNames = {SHADERVAR_NAMES};
		class ShaderProgram {
			public:
				ShaderProgram(int shaderprogram_id, std::string shaderprogram_name, std::vector<GLint> shaderFiles, ShaderMask shaderVar_mask, ShaderMask shaderVar_overrideMask, std::vector<std::string> shaderVar_overrides, std::vector<std::string> shaderVar_custom);
				~ShaderProgram();
				static void loadShaders();
				static ShaderProgram *getShader(int shaderIndex);
				void useShader();
				GLint getShaderLocation(bool uniform, int shaderVar);
				bool setVertexAttributePointer(int shaderVar, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
				bool setMaterial(render::MaterialAsset mat);
			private:
				int shaderprogram_id;
				std::string shaderprogram_name;
				std::vector<GLint> shaderFiles;
				ShaderMask shaderVar_mask;
				ShaderMask shaderVar_overrideMask;
				std::vector<std::string> shaderVar_overrides;
				std::vector<std::string> shaderVar_custom;
				// GL spec
				GLuint programID;
				int shaderVar_locationsCount;
				GLint *shaderVar_locations;

				MaterialAsset currentMaterial;

				static GLint compileShader(std::string name, int mask, std::string code);
				static GLint *shader_files;
				static ShaderProgram **shader_program;
		};
	}
}

#endif
