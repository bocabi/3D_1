#include "DXUT.h"
#include "cMultiTexture.h"

cMultiTexture::cMultiTexture()
{
}

cMultiTexture::~cMultiTexture()
{
	for (auto iter : m_multiImage)
	{
		iter->texturePtr->Release();
		SAFE_DELETE(iter);
	}
	m_multiImage.clear();
}

void cMultiTexture::AddImage(string path, int cnt)
{
	/*char sz[128];
	for (int i = 0; i < cnt; i++)
	{
		sprintf(sz, path.c_str(), i);
		m_multiImage.push_back(new cTexture(sz));
	}*/
}

cTexture* cMultiTexture::FindImage(int cnt)
{
	if (cnt < 0)
	{
		return nullptr;
	}
	return m_multiImage[cnt];
}
