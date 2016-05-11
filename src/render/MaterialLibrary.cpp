#include "util/StreamUtils.hpp"

#include "MaterialLibrary.hpp"

#define readFloat3(v,x) do { v[0] = readFloat(x); v[1]= readFloat(x); v[2] = readFloat(x); } while(0);

using namespace render;
using namespace util::StreamUtils;

MaterialLibrary::MaterialLibrary(int assetId, std::istream &fp) : Asset(assetId)
{
	this->setName(readString(fp));

	int nMats = readInt(fp);

	for(int i=0;i<nMats;i++)
	{

		Material *m = new Material;

		m->name = readString(fp);

		m->flags = readInt(fp);

		if(m->flags&MASK_Ka) readFloat3(m->Ka, fp);
		if(m->flags&MASK_Kd) readFloat3(m->Kd, fp);
		if(m->flags&MASK_Ks) readFloat3(m->Ks, fp);
		if(m->flags&MASK_Tf) readFloat3(m->Tf, fp);
		if(m->flags&MASK_d) m->d = readFloat(fp);
		if(m->flags&MASK_Ns) m->Ns = readFloat(fp);
		if(m->flags&MASK_Ni) m->Ni = readFloat(fp);
		if(m->flags&MASK_illum) m->illum = readInt(fp);
		if(m->flags&MASK_sharpness) m->sharpness = readInt(fp);
		if(m->flags&MASK_map_Ka) m->map_Ka = readString(fp);
		if(m->flags&MASK_map_Kd) m->map_Kd = readString(fp);
		if(m->flags&MASK_map_Ks) m->map_Ks = readString(fp);
		if(m->flags&MASK_map_Ns) m->map_Ns = readString(fp);
		if(m->flags&MASK_map_d) m->map_d = readString(fp);
		if(m->flags&MASK_disp) m->disp = readString(fp);
		if(m->flags&MASK_decal) m->decal = readString(fp);
		if(m->flags&MASK_bump) m->bump = readString(fp);

		this->materials.push_back(*m);
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
void MaterialLibrary::postload()
{

}
std::ostream &operator<<(std::ostream &ost, const render::Material &m)
{
	ost << "Material "<< m.name << " (";
	if(m.flags&MASK_Ka)		ost <<   "Ka=("<< m.Ka[0] << ", "<< m.Ka[1] << ", "<< m.Ka[2] << ")";
	if(m.flags&MASK_Kd)		ost << ", Kd=("<< m.Kd[0] << ", "<< m.Kd[1] << ", "<< m.Kd[2] << ")";
	if(m.flags&MASK_Ks)		ost << ", Ks=("<< m.Ks[0] << ", "<< m.Ks[1] << ", "<< m.Ks[2] << ")";
	if(m.flags&MASK_Tf)		ost << ", Tf=("<< m.Tf[0] << ", "<< m.Tf[1] << ", "<< m.Tf[2] << ")";
	if(m.flags&MASK_d) 		ost << ", d="<< m.d;
	if(m.flags&MASK_Ns)		ost << ", Ns="<< m.Ns;
	if(m.flags&MASK_Ni)		ost << ", Ni="<< m.Ni;
	if(m.flags&MASK_illum)		ost << ", illum="<< m.illum;
	if(m.flags&MASK_sharpness)	ost << ", sharpness="<< m.sharpness;
	if(m.flags&MASK_map_Ka)		ost << ", map_Ka="<< m.map_Ka;
	if(m.flags&MASK_map_Kd)		ost << ", map_Kd="<< m.map_Kd;
	if(m.flags&MASK_map_Ks)		ost << ", map_Ks="<< m.map_Ks;
	if(m.flags&MASK_map_Ns)		ost << ", map_Ns="<< m.map_Ns;
	if(m.flags&MASK_map_d)		ost << ", map_d="<< m.map_d;
	if(m.flags&MASK_disp)		ost << ", disp="<< m.disp;
	if(m.flags&MASK_decal)		ost << ", decal="<< m.decal;
	if(m.flags&MASK_bump)		ost << ", bump="<< m.bump;
	return ost << ")";
}
