#include "DXUT.h"
#include "cImageManager.h"
#include "cMultiTexture.h"
void cImageManager::Begin()
{
	m_sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void cImageManager::End()
{
	m_sprite->End();
}

void cImageManager::ResetDevice()
{
	m_sprite->OnResetDevice();
}

void cImageManager::LostDevice()
{
	m_sprite->OnLostDevice();
}

cImageManager::cImageManager()
{
	D3DXCreateSprite(Device, &m_sprite);
	

}

cImageManager::~cImageManager()
{
	for (auto iter : m_image)
	{
		iter.second->texturePtr->Release();
		SAFE_DELETE(iter.second);
	}
	m_sprite->Release();
}

cTexture * cImageManager::AddImage(const string & key, const string & path, int cnt)
{
	auto find = m_image.find(key);

	if (find != m_image.end())
		return find->second;

	LPDIRECT3DTEXTURE9 d3dTex;
	D3DXIMAGE_INFO info;

	if ((D3DXCreateTextureFromFileExA(Device, path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, nullptr, &d3dTex) == S_OK))
	{

		return m_image.insert(make_pair(key, new cTexture(d3dTex, info))).first->second;
	}

		return nullptr;
}

cTexture* cImageManager::FindImage(const string& key, int cnt)
{
	auto find = m_image.find(key);
	if (find == m_image.end())
	{
		return nullptr;
	}
	
	return find->second;
}

cMultiTexture* cImageManager::FindMultiTexture(const string& key)
{
	return nullptr;

}

void cImageManager::TextDraw(string str, D3DXVECTOR2 pos, float size, bool center, D3DCOLOR color)
{
	D3DXMATRIXA16 mat;
	if (!center)
	{
		D3DXCreateFontA(Device, size, 0, 10, 1, FALSE, DEFAULT_CHARSET, 0, 0, 0, "Black Ops One", &pFont);
		D3DXMatrixTranslation(&mat, pos.x, pos.y, 0);
	}
	else
	{
		D3DXCreateFontA(Device, size, 0, 0, 1, FALSE, DEFAULT_CHARSET, 0, 0, 0, "Black Ops One", &pFont);
		D3DXMatrixTranslation(&mat, pos.x - size * (str.size()*0.25), pos.y - size / 2.f, 0);
	}
	m_sprite->SetTransform(&mat);
	pFont->DrawTextA(m_sprite, str.c_str(), str.size(), nullptr, DT_NOCLIP, color);
	SAFE_RELEASE(pFont);

}

void cImageManager::Render(cTexture * texturePtr, D3DXVECTOR2 pos, D3DXVECTOR2 size, float rot, D3DCOLOR color)
{

	D3DXMATRIXA16 mat;
	if (!texturePtr)
	{
		return;
	}
	else
	{

		D3DXMatrixTransformation2D(&mat, nullptr, 0, &size, nullptr, rot, &pos);
		m_sprite->SetTransform(&mat);
		m_sprite->Draw(texturePtr->texturePtr, nullptr, nullptr, nullptr, color);
	}

}

void cImageManager::CenterRender(cTexture * texturePtr, D3DXVECTOR2 pos, D3DXVECTOR2 size, float rot, D3DCOLOR color)
{
	Render(texturePtr, Vec2(pos.x - texturePtr->info.Width / 2, pos.y - texturePtr->info.Height / 2), size, rot, color);
}
