#ifndef __SHADER_H_INCLUDED__
#define __SHADER_H_INCLUDED__

#include "render/MaterialData.h"

#define SHADER_LOCATION_M 0
#define SHADER_LOCATION_V 1
#define SHADER_LOCATION_P 2
#define SHADER_LOCATION_MV 3
#define SHADER_LOCATION_VP 4
#define SHADER_LOCATION_MVP 5
#define SHADER_LOCATION_IM 6
#define SHADER_LOCATION_IV 7
#define SHADER_LOCATION_IP 8
#define SHADER_LOCATION_IMV 9
#define SHADER_LOCATION_IVP 10
#define SHADER_LOCATION_IMVP 11
#define SHADER_LOCATION_VERTEX_POSITION 12
#define SHADER_LOCATION_VERTEX_TEXTURE 13
#define SHADER_LOCATION_VERTEX_NORMAL 14

#define SHADER_LOCATION_MATERIAL_OFFSET 15
#define materialShaderLocation(x) (x+SHADER_LOCATION_MATERIAL_OFFSET)

namespace render {
	class Shader {
		public:
			Shader();
			~Shader();
			void bindAttributeBuffer();
	};
}

#endif
