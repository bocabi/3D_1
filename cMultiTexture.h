#pragma once
#include "cImageManager.h"
class cMultiTexture : public cTexture
{
private:
	vector<cTexture*> m_multiImage;
public:
	cMultiTexture();
	virtual ~cMultiTexture();
	void AddImage(string path, int cnt = 1);
	cTexture* FindImage(int cnt = 0);
	size_t GetImageSize() { return m_multiImage.size(); };
};

