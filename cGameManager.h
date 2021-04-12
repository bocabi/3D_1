#pragma once
#include "singleton.h"
class cGameManager : public singleton<cGameManager>
{
public:
	cGameManager() {};
	~cGameManager() {};
	float TimeScale = 1;
};

#define GAME cGameManager::Get()