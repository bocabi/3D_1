#pragma once
class cMain
{
private:
	Mesh * mesh;
	LPD3DXEFFECT Shader;
	cTexture * image;
	float rot= 0;
	float rotL = 0;
	
	float rotR = 0;
	INT A = 0;
	Vec3 pos = Vec3(0,0,1000);
public:
	cMain();
	~cMain();
public:
	void Init();
	void Update();
	void Render();
	void Release();
	void ResetDevice();
	void LostDevice();
};

