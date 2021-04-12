#pragma once
#include "singleton.h"
class cRenderManager : public singleton<cRenderManager>
{
public:
	struct sVerTex
	{
		D3DXVECTOR3 Pos;
		D3DXVECTOR2 uv;
		D3DXVECTOR3 normal;
		inline static constexpr DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	};
	D3DLIGHT9 light;
	cMeshLoader * Loader;
	
	std::map<string, Mesh*>	mMeshs;
	cRenderManager();
	~cRenderManager();
public:
	Mesh * LoadObj(const string &key,const string & path);
	Mesh * FindObj(const string &key);
	LPDIRECT3DVERTEXBUFFER9 vb;
	LPDIRECT3DINDEXBUFFER9 ib;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;

	void Cam(Vec2 Pos);
	void ShaderRender(Mesh * mesh, LPD3DXEFFECT pEffect,D3DXVECTOR3 Pos, D3DXVECTOR3 size, D3DXVECTOR3 Rot = D3DXVECTOR3(0, 0, 0));
	D3DXVECTOR3 CamPos = D3DXVECTOR3(0.f,0.f,0), CamAt = D3DXVECTOR3(0.f, 0.f, 50000.f), CamUp = D3DXVECTOR3(0.f, 1.f,0.f);
	void MeshRender(Mesh * mesh, D3DXVECTOR3 Pos, D3DXVECTOR3 size, D3DXVECTOR3 Rot = D3DXVECTOR3(0,0,0));
	void Render(cTexture * texture,D3DXVECTOR2 Pos,D3DXVECTOR2 size,float rot);
};

#define RENDER cRenderManager::Get()