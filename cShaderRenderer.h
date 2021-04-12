#pragma once
#include "singleton.h"
class cShaderRenderer : public singleton<cShaderRenderer>
{
private:
	map<string, LPD3DXEFFECT> m_effect;
public:
	LPD3DXEFFECT effect;
	cShaderRenderer();
	~cShaderRenderer();
	void Render(Mesh * mesh, D3DXMATRIXA16 matW, D3DXMATRIXA16 matV, D3DXMATRIXA16 matP, LPD3DXEFFECT pEffect);
	LPD3DXEFFECT AddEffect(const string key,const string& path);
};

#define SHADER cShaderRenderer::Get()