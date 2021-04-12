#pragma once
#include "singleton.h"
struct sMaterial
{
	string mtlName;

	D3DMATERIAL9 material;
	cTexture* DiffuseMap = nullptr;
	cTexture* specularMap = nullptr;

	float fAlpha;
	float fIllum;
	float fNs;

	sMaterial()
		:fAlpha(1.f), fIllum(0.f), fNs(0.f)
	{
		material.Power = 1;
	}
};

struct Mesh
{
	bool bNoneDelete = false;
	LPD3DXMESH LPD3DMesh;
		vector<sMaterial * >vMaterial;
	sMaterial * GetMaterial(int index)
	{
		return vMaterial[index];
	}

	cTexture * GetDiffuseMap(int index)
	{
		return vMaterial[index]->DiffuseMap;
	}
	cTexture* GetSpecularMap(int index)
	{
		return vMaterial[index]->specularMap;
	}

	~Mesh()
	{
		for (auto Iter : vMaterial)
			SAFE_DELETE(Iter);

		vMaterial.clear();

		SAFE_RELEASE(LPD3DMesh);
	}
};
class cMeshLoader : public singleton<cMeshLoader>
{
private:
	string objPath;
	vector<D3DXVECTOR3> Pos;
	vector<D3DXVECTOR2> TexCoord;
	vector<D3DXVECTOR3> Normal;
	vector<VERTEX> Ver;
	vector<DWORD> Index;
	vector<DWORD> Attribute;
	vector <CacheEntry*> VertexCache;
	string mtlPath;
public:
	cMeshLoader();
	~cMeshLoader();
	
	void Create(LPD3DXMESH * mesh);
	void MeshLoad(const string & key,Mesh * mesh,const string & mapPath = "None");
	DWORD AddVerTex(UINT hash, VERTEX *pVerTex);
	void ObjLoad(Mesh * mesh,const string & objpath,const string &mapPath = "None");
};

#define MESHLOADER cMeshLoader::Get()