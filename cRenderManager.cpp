#include "DXUT.h"
#include "cRenderManager.h"

cRenderManager::cRenderManager()
{
	Loader = new cMeshLoader;
	Device->CreateVertexBuffer(sizeof(sVerTex)*4,D3DUSAGE_WRITEONLY,sVerTex::FVF,D3DPOOL_DEFAULT,&vb,NULL);
	
	
	sVerTex * pVerTices = nullptr;

	vb->Lock(0,0,(void**)&pVerTices,0);

	pVerTices[0].Pos = Vec3(-0.5, 0.5, 0);
	pVerTices[1].Pos = Vec3(0.5, 0.5, 0);
	pVerTices[2].Pos = Vec3(-0.5, -0.5, 0);
	pVerTices[3].Pos = Vec3(0.5, -0.5, 0);

	pVerTices[0].uv = Vec2(0, 0);
	pVerTices[1].uv = Vec2(1, 0);
	pVerTices[2].uv = Vec2(0, 1);
	pVerTices[3].uv = Vec2(1, 1);

	pVerTices[0].normal = Vec3(0,0,0);
	pVerTices[1].normal = Vec3(0, 0, 0);
	pVerTices[2].normal = Vec3(0, 0, 0);
	pVerTices[3].normal = Vec3(0, 0, 0);

	vb->Unlock();
	

	Device->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL);
	void * pIndice = NULL;
	
	WORD idx[] = { 1,2,0,1,3,2 };

	ib->Lock(0, 0, &pIndice, 0);
	memcpy(pIndice, idx, sizeof(WORD) * 6);
	ib->Unlock();
	

	DWORD* Attribute = nullptr;
	

	
	ZeroMemory(&light,sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_POINT;
	light.Specular.r = 255.f;
	light.Specular.g = 255.f;
	light.Specular.b = 255.f;
	 
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;


	light.Range = FLT_MAX;
	light.Attenuation0 = 0.00000001f;
	light.Position.x = 0;
	light.Position.y = 100;
	light.Position.z = 0;

	light.Direction.x = 0 ;
	light.Direction.y = 0;
	light.Direction.z = 0;

	light.Theta = 10;
	light.Phi = 1;
	light.Falloff = 100.f;
	Device->SetLight(1,&light);
	Device->LightEnable(1,TRUE);
	Device->SetRenderState(D3DRS_AMBIENT,0x00202020);
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetFVF(sVerTex::FVF);
	Device->SetStreamSource(0, vb, 0, sizeof(sVerTex));
	Device->SetIndices(ib);

	D3DXMatrixOrthoLH(&matProj,WINSIZEX,WINSIZEY,0,100);
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60), 16.f / 9.f, 1.f, 50000.f);
	Device->SetTransform(D3DTS_PROJECTION, &matProj);

	CamPos = D3DXVECTOR3(0, 0, -0);
	CamAt = D3DXVECTOR3(0, 0, 10);
	CamUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXMatrixLookAtLH(&matView, &CamPos, &CamAt, &CamUp);

	Device->SetTransform(D3DTS_VIEW, &matView);
}

cRenderManager::~cRenderManager()
{
	vb->Release();
	ib->Release();
	for (auto iter : mMeshs)
	{
		SAFE_DELETE(iter.second);
	}
	mMeshs.clear();
	SAFE_DELETE(Loader);
}

Mesh * cRenderManager::LoadObj(const string & key, const string & path)
{
	auto find = mMeshs.find(key);
	if (find != mMeshs.end())
	{
		return find->second;
	}
	Mesh*mesh = new Mesh;
	Loader->ObjLoad(mesh, path);
	mMeshs[key] = mesh;
	return mesh;
}

Mesh * cRenderManager::FindObj(const string & key)
{
	auto find = mMeshs.find(key);
	if (find == mMeshs.end())
	{
		return nullptr;
	}
	return find->second;
}

void cRenderManager::Cam(Vec2 Pos)
{

}

void cRenderManager::ShaderRender(Mesh * mesh, LPD3DXEFFECT pEffect, D3DXVECTOR3 Pos, D3DXVECTOR3 size, D3DXVECTOR3 Rot)
{
	D3DXMATRIXA16 matP, matS, matR, matR1, matR2, matR3, matW;
	D3DXMatrixTranslation(&matP, Pos.x, Pos.y, Pos.z);
	D3DXMatrixScaling(&matS, size.x, size.y, size.z);
	D3DXMatrixRotationX(&matR1, Rot.x);
	D3DXMatrixRotationY(&matR2, Rot.y);
	D3DXMatrixRotationZ(&matR3, Rot.z);
	matR = matR1 * matR2*matR3;
	matW = matR * matP;
	Device->SetTransform(D3DTS_WORLD, &matW);

	pEffect->SetMatrix(D3DXHANDLE("gWorldMatrix"), &matW);
	pEffect->SetMatrix(D3DXHANDLE("gViewMatrix"), &matView);
	pEffect->SetMatrix(D3DXHANDLE("gProjMatrix"), &matProj);

	UINT numPasses = 0;
	pEffect->Begin(&numPasses, NULL);

	for (UINT i = 0; i < numPasses; ++i)
	{
		pEffect->BeginPass(i);
		auto mat = mesh->vMaterial[i];


		Device->SetMaterial(&mat->material);

		if (mat->DiffuseMap != nullptr)
			Device->SetTexture(0, mat->DiffuseMap->texturePtr);
		else
			Device->SetTexture(0, nullptr);
		for (size_t j = 0; j < mesh->vMaterial.size(); ++j)
			mesh->LPD3DMesh->DrawSubset(j);

		pEffect->EndPass();
	}
	pEffect->End();
}

void cRenderManager::MeshRender(Mesh * mesh, D3DXVECTOR3 Pos , D3DXVECTOR3 size, D3DXVECTOR3 Rot)
{

	D3DXMATRIXA16 matP, matS,matR, matR1, matR2, matR3, matW;
	D3DXMatrixTranslation(&matP, Pos.x,Pos.y,Pos.z);
	D3DXMatrixScaling(&matS,size.x,size.y,size.z);
	D3DXMatrixRotationX(&matR1,Rot.x);
	D3DXMatrixRotationY(&matR2, Rot.y);
	D3DXMatrixRotationZ(&matR3, Rot.z);
	matR = matR1 * matR2*matR3;
	matW = matR*matP;
	Device->SetTransform(D3DTS_WORLD,&matW);
	for (int i = 0; i < mesh->vMaterial.size(); ++i)
	{
		auto mat = mesh->vMaterial[i];


		Device->SetMaterial(&mat->material);

		if (mat->DiffuseMap != nullptr)
			Device->SetTexture(0, mat->DiffuseMap->texturePtr);
		else
			Device->SetTexture(0, nullptr);

		mesh->LPD3DMesh->DrawSubset(i);
	}


}

void cRenderManager::Render(cTexture * texture, D3DXVECTOR2 Pos, D3DXVECTOR2 size, float rot)
{
	D3DXMATRIXA16 matP, matS, matR, matW;
	D3DXMatrixTranslation(&matP, Pos.x, Pos.y, 0);
	D3DXMatrixScaling(&matS, texture->info.Width*size.x,texture->info.Height* size.y, 0);

	D3DXMatrixRotationZ(&matR,rot);
	matW = matS * matR*matP;

	Device->SetTransform(D3DTS_WORLD, &matW);
	Device->SetTexture(0,texture->texturePtr);
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

}
