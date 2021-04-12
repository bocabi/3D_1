#pragma once
#include "singleton.h"
class cTexture
{
private:
public:
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texturePtr;
	cTexture() {};
	cTexture(LPDIRECT3DTEXTURE9 texturePtr, D3DXIMAGE_INFO info)
		:texturePtr(texturePtr),info(info)
	{
	}
	


};
class cMultiTexture;
class cImageManager :public singleton<cImageManager>
{
private:
	LPDIRECT3DTEXTURE9 d3dTex;

	LPD3DXFONT pFont;
	map <string, cTexture*>m_image;
	LPD3DXSPRITE m_sprite = nullptr;
public:
	void Begin();
	void End();
	void ResetDevice();
	void LostDevice();
	cImageManager();
	~cImageManager();
	cTexture * AddImage(const string  & key, const string  & path, int cnt = 1);
	cTexture * FindImage(const string  & key, int cnt = 0);
	cMultiTexture* FindMultiTexture(const string & key);
	void TextDraw(string str, D3DXVECTOR2 pos, float size, bool Center, D3DCOLOR color);
	void Render(cTexture * texturePtr, D3DXVECTOR2 pos, D3DXVECTOR2 size = D3DXVECTOR2(1, 1), float rot = 0, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	void CenterRender(cTexture * texturePtr, D3DXVECTOR2 pos, D3DXVECTOR2 size = D3DXVECTOR2(1, 1), float rot = 0, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
};

#define IMAGE cImageManager::Get()
