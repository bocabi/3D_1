#pragma once
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;
const int WINSIZEX = 1920;
const int WINSIZEY = 1080;

#define Device DXUTGetD3D9Device()
#define VERTEXFVF D3DFVF_XYZ | D3DFVF_NORMAL| D3DFVF_TEX1
#define Vec2 D3DXVECTOR2
#define Vec3 D3DXVECTOR3

#include "cGameManager.h"
#include "cImageManager.h"
#include "MeshLoader.h"
#include "cMeshLoader.h"
#include "cShaderRenderer.h"
#include "cRenderManager.h"
#define DTIME DXUTGetElapsedTime() * GAME->TimeScale 
