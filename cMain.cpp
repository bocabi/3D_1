#include "DXUT.h"
#include "cMain.h"

cMain::cMain()
{
}

cMain::~cMain()
{
}

void cMain::Init()
{
	IMAGE->AddImage("f","./resource/Object001CompleteMap.png");
	mesh =  RENDER->LoadObj("B","./resource/c.obj");
	image = IMAGE->FindImage("f");
	Shader = SHADER->AddEffect("effect","./Lighting.fx");
}

void cMain::Update()
{
}

void cMain::Render()
{
	RENDER->Cam(Vec2(0,0));
	if (DXUTIsKeyDown('W'))
	{
		pos.y += 5;
	}
	if (DXUTIsKeyDown('D'))
	{
		pos.x += 5;

	}
	if (DXUTIsKeyDown('A'))
	{
		pos.x -= 5;

	}
	
	if (DXUTIsKeyDown('S'))
	{
		pos.y -= 5;
	}
	if (DXUTIsKeyDown(VK_DOWN))
	{
		rot += 0.1; 
	}
	if (DXUTIsKeyDown(VK_UP))
	{
		rot -= 0.1;
	}
	if (DXUTIsKeyDown(VK_LEFT))
	{
		rotL -= 0.1;
	}
	if (DXUTIsKeyDown(VK_RIGHT))
	{
		rotL += 0.1;
	}
	
	
	RENDER->ShaderRender(mesh,Shader,pos,Vec3(1,1,1),Vec3(rot,rotL,rotR));
	RENDER->Render(image, Vec2(0, 0), Vec2(1, 1), 0);

}

void cMain::Release()
{
	cGameManager::ReleaseInstance();
	cRenderManager::ReleaseInstance();
	cImageManager::ReleaseInstance();
	cMeshLoader::ReleaseInstance();
	cShaderRenderer::ReleaseInstance();
}

void cMain::ResetDevice()
{
}

void cMain::LostDevice()
{
}
