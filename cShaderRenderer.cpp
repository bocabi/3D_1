#include "DXUT.h"
#include "cShaderRenderer.h"

cShaderRenderer::cShaderRenderer()
{
}

cShaderRenderer::~cShaderRenderer()
{
}

void cShaderRenderer::Render(Mesh * mesh, D3DXMATRIXA16 matW, D3DXMATRIXA16 matV, D3DXMATRIXA16 matP, LPD3DXEFFECT pEffect)
{
	pEffect->SetMatrix(D3DXHANDLE("gWorldMatrix"), &matW);
	pEffect->SetMatrix(D3DXHANDLE("gViewMatrix"), &matV);
	pEffect->SetMatrix(D3DXHANDLE("gProjMatrix"), &matP);

	UINT numPasses = 0;
	pEffect->Begin(&numPasses, NULL);

	for (UINT i = 0; i < numPasses; ++i)
	{
		pEffect->BeginPass(i);

		for (size_t j = 0; j < mesh->vMaterial.size(); ++j)
			mesh->LPD3DMesh->DrawSubset(j);

		pEffect->EndPass();
	}
	pEffect->End();
}

LPD3DXEFFECT cShaderRenderer::AddEffect(const string key, const string& path)
{

	auto find = m_effect.find(key);
	LPD3DXBUFFER buffer;
	DWORD flags = D3DXSHADER_DEBUG;
	if (find == m_effect.end())
	{
		if (D3DXCreateEffectFromFileA(Device, path.c_str(), NULL, NULL, flags, NULL, &effect, &buffer) == S_OK)
		{
			m_effect[key] = effect;
			return effect;
		}
	}

	return nullptr;
}
