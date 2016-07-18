#include "util/StreamUtils.hpp"
#include "util/Globals.hpp"
#include "render/DDSImage.hpp"
#include "util/AssetUtils.hpp"
#include "render/shaders/ShaderProgram.hpp"

#include "render/MaterialLibrary.hpp"

#define readFloat3(v,x) do { v[0] = readFloat(x); v[1]= readFloat(x); v[2] = readFloat(x); } while(0);

using namespace render;
using namespace render::shaders;
using namespace util::StreamUtils;

MaterialLibrary::MaterialLibrary(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));

	int nMats = readInt(fp);

	for(int i=0;i<nMats;i++)
	{
		Material m;

		m.name = readString(fp);
		m.flags = readInt(fp);

		if(m.flags&MATERIAL_MASK_Ka) readFloat3(m.Ka, fp);
		if(m.flags&MATERIAL_MASK_Kd) readFloat3(m.Kd, fp);
		if(m.flags&MATERIAL_MASK_Ks) readFloat3(m.Ks, fp);
		if(m.flags&MATERIAL_MASK_Tf) readFloat3(m.Tf, fp);
		if(m.flags&MATERIAL_MASK_d) m.d = readFloat(fp);
		if(m.flags&MATERIAL_MASK_Ns) m.Ns = readFloat(fp);
		if(m.flags&MATERIAL_MASK_Ni) m.Ni = readFloat(fp);
		if(m.flags&MATERIAL_MASK_illum) m.illum = readInt(fp);
		if(m.flags&MATERIAL_MASK_sharpness) m.sharpness = readInt(fp);
		if(m.flags&MATERIAL_MASK_map_Ka) m.map_Ka = readInt(fp);
		if(m.flags&MATERIAL_MASK_map_Kd) m.map_Kd = readInt(fp);
		if(m.flags&MATERIAL_MASK_map_Ks) m.map_Ks = readInt(fp);
		if(m.flags&MATERIAL_MASK_map_Ns) m.map_Ns = readInt(fp);
		if(m.flags&MATERIAL_MASK_map_d) m.map_d = readInt(fp);
		if(m.flags&MATERIAL_MASK_disp) m.disp = readInt(fp);
		if(m.flags&MATERIAL_MASK_decal) m.decal = readInt(fp);
		if(m.flags&MATERIAL_MASK_bump) m.bump = readInt(fp);

		this->materials.push_back(m);
	}
}
MaterialLibrary::~MaterialLibrary()
{
}
void MaterialLibrary::write(std::ostream &ost) const
{
	ost << "[" << this->getAssetID() << ":" << this->getName() << ".mtl] " << this->materials.size() << " materials" << std::endl;
	for(const Material &m : this->materials)
		ost << "	" << m << std::endl;
}
Material *MaterialLibrary::getMaterial(int materialId)
{
	return &this->materials[materialId];
}
#define SET_SHADER_VEC3(name) do { \
	int shaderLoc = shader->getShaderLocation(true, SHADERVAR_material_##name); \
	if(shaderLoc!=-1) \
	{ \
		if(mat->flags&MATERIAL_MASK_##name) \
		{ \
			glUniform3f(shaderLoc, mat->name[0], mat->name[1], mat->name[2]); \
		} \
		else \
			util::Globals::fatalError("Material is missing field for shader uniform "#name); \
	} \
} while(0);
#define SET_SHADER_VALUE(name, gltype) do { \
	int shaderLoc = shader->getShaderLocation(true, SHADERVAR_material_##name); \
	if(shaderLoc!=-1) \
	{ \
		if(mat->flags&MATERIAL_MASK_##name) \
		{ \
			glUniform##gltype(shaderLoc, mat->name); \
		} \
		else \
			util::Globals::fatalError("Material is missing field for shader uniform "#name); \
	} \
} while(0);
#define SET_SHADER_TEXTURE(name) do { \
	int texLoc = shader->getShaderLocation(true, SHADERVAR_material_##name); \
	if(texLoc!=-1) \
	{ \
		if(mat->flags&MATERIAL_MASK_##name) \
		{ \
			glActiveTexture(GL_TEXTURE0+t); \
			util::AssetUtils::bindTexture(mat->name); \
			glUniform1i(texLoc, t); \
			t++; \
		} \
		else \
			util::Globals::fatalError("Material is missing field for shader uniform "#name); \
	} \
} while(0);
void MaterialLibrary::updateShader(shaders::ShaderProgram *shader, int materialId)
{
	Material *mat = getMaterial(materialId);
	int t = 0;

	SET_SHADER_VEC3(Ka);
	SET_SHADER_VEC3(Kd);
	SET_SHADER_VEC3(Ks);
	SET_SHADER_VEC3(Tf);
	SET_SHADER_VALUE(d, 1f);
	SET_SHADER_VALUE(Ns, 1f);
	SET_SHADER_VALUE(Ni, 1f);
	SET_SHADER_VALUE(illum, 1i);
	SET_SHADER_VALUE(sharpness, 1i);
	SET_SHADER_TEXTURE(map_Ka);
	SET_SHADER_TEXTURE(map_Kd);
	SET_SHADER_TEXTURE(map_Ks);
	SET_SHADER_TEXTURE(map_Ns);
	SET_SHADER_TEXTURE(map_d);
	SET_SHADER_TEXTURE(disp);
	SET_SHADER_TEXTURE(decal);
	SET_SHADER_TEXTURE(bump);
}
void MaterialLibrary::postload()
{

}
std::ostream &operator<<(std::ostream &ost, const render::Material &m)
{
	ost << "Material "<< m.name << " (";
	if(m.flags&MATERIAL_MASK_Ka		) ost <<   "Ka=("<< m.Ka[0] << ", "<< m.Ka[1] << ", "<< m.Ka[2] << ")";
	if(m.flags&MATERIAL_MASK_Kd		) ost << ", Kd=("<< m.Kd[0] << ", "<< m.Kd[1] << ", "<< m.Kd[2] << ")";
	if(m.flags&MATERIAL_MASK_Ks		) ost << ", Ks=("<< m.Ks[0] << ", "<< m.Ks[1] << ", "<< m.Ks[2] << ")";
	if(m.flags&MATERIAL_MASK_Tf		) ost << ", Tf=("<< m.Tf[0] << ", "<< m.Tf[1] << ", "<< m.Tf[2] << ")";
	if(m.flags&MATERIAL_MASK_d 		) ost << ", d="<< m.d;
	if(m.flags&MATERIAL_MASK_Ns		) ost << ", Ns="<< m.Ns;
	if(m.flags&MATERIAL_MASK_Ni		) ost << ", Ni="<< m.Ni;
	if(m.flags&MATERIAL_MASK_illum		) ost << ", illum="<< m.illum;
	if(m.flags&MATERIAL_MASK_sharpness	) ost << ", sharpness="<< m.sharpness;
	if(m.flags&MATERIAL_MASK_map_Ka		) ost << ", map_Ka="<< m.map_Ka;
	if(m.flags&MATERIAL_MASK_map_Kd		) ost << ", map_Kd="<< m.map_Kd;
	if(m.flags&MATERIAL_MASK_map_Ks		) ost << ", map_Ks="<< m.map_Ks;
	if(m.flags&MATERIAL_MASK_map_Ns		) ost << ", map_Ns="<< m.map_Ns;
	if(m.flags&MATERIAL_MASK_map_d		) ost << ", map_d="<< m.map_d;
	if(m.flags&MATERIAL_MASK_disp		) ost << ", disp="<< m.disp;
	if(m.flags&MATERIAL_MASK_decal		) ost << ", decal="<< m.decal;
	if(m.flags&MATERIAL_MASK_bump		) ost << ", bump="<< m.bump;
	return ost << ")";
}
bool operator==(MaterialAsset &a, MaterialAsset &b)
{
	return a.assetId==b.assetId && a.materialId==b.materialId;
}
bool operator!=(MaterialAsset &a, MaterialAsset &b)
{
	return !(a==b);
}
