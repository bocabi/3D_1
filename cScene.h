#pragma once
class cScene abstract
{
public:
	cScene();
	~cScene();

	void Init();
	void Update();
	void Render();
	void Release();
};

