#include "DXUT.h"
#include "cMeshLoader.h"
#include "cImageManager.h"
cMeshLoader::cMeshLoader()
{
}

cMeshLoader::~cMeshLoader()
{
	
	Pos.clear();
	TexCoord.clear();
	Normal.clear();
	Ver.clear();
	Index.clear();
	Attribute.clear();
	VertexCache.clear();

}


void cMeshLoader::Create(LPD3DXMESH * mesh)
{
	if (FAILED(D3DXCreateMeshFVF(Index.size() / 3, Ver.size(), D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEXFVF, Device, mesh)))
	{
		return;
	}

	VERTEX * vertices = nullptr;
	if (FAILED((*mesh)->LockVertexBuffer(0, (void**)&vertices)))
	{
		return;
	}
	memcpy(vertices, &Ver[0], sizeof(VERTEX)*Ver.size());
	(*mesh)->UnlockVertexBuffer();
	Ver.clear();

	DWORD* indices = nullptr;
	if (FAILED((*mesh)->LockIndexBuffer(0, (void**)&indices)))
	{
		return;
	}
	memcpy(indices, &Index[0], sizeof(DWORD)*Index.size());
	(*mesh)->UnlockIndexBuffer();
	Index.clear();

	DWORD * attribute = nullptr;
	if (FAILED((*mesh)->LockAttributeBuffer(0, &attribute)))
	{
		return;

	}
	memcpy(attribute, &Attribute[0], sizeof(DWORD)*Attribute.size());
	(*mesh)->UnlockAttributeBuffer();
	Attribute.clear();
	for (size_t i = 0; i < VertexCache.size(); ++i)
	{
		CacheEntry* pEntry = VertexCache[i];
		while (pEntry != NULL)
		{
			CacheEntry* pNext = pEntry->pNext;
			SAFE_DELETE(pEntry);
			pEntry = pNext;
		}
	}
	VertexCache.clear();
	TexCoord.clear();
	Pos.clear();
	Normal.clear();
}

void cMeshLoader::MeshLoad(const string & key, Mesh * mesh, const string & mapPath)
{

	std::ifstream mtlLoader;
	mtlLoader.open(mtlPath);
	if (mtlLoader.fail())
	{
		return;
	}
	sMaterial * lpMtl = nullptr;
	D3DMATERIAL9 * d3dMtl = nullptr;
	bool findMtl = false;
	while (!mtlLoader.eof())
	{
		string tag;
		mtlLoader >> tag;
		if (!findMtl)
		{
			if (tag == "newmtl")
			{
				mtlLoader >> tag;
				if (tag == key)
				{
					lpMtl = new sMaterial;
					lpMtl->mtlName = key;
					mesh->vMaterial.push_back(lpMtl);
					d3dMtl = &lpMtl->material;
					findMtl = true;
				}
			}

		}
		else
		{
			if (tag == "Ka")
			{
				float r, g, b;
				r = g = b = 0.f;
				mtlLoader >> tag;
				r = (float)atof(tag.c_str());
				mtlLoader >> tag;
				g = (float)atof(tag.c_str());
				mtlLoader >> tag;
				b = (float)atof(tag.c_str());
				d3dMtl->Ambient = { r,g,b,1.f };
			}
			if (tag == "Kd")
			{
				float r, g, b;
				r = g = b = 0.f;
				mtlLoader >> tag;
				r = (float)atof(tag.c_str());
				mtlLoader >> tag;
				g = (float)atof(tag.c_str());
				mtlLoader >> tag;
				b = (float)atof(tag.c_str());
				d3dMtl->Diffuse = { r,g,b,1.f };
			}
			if (tag == "Ks")
			{
				float r, g, b;
				r = g = b = 0.f;

				mtlLoader >> tag;
				r = (float)atof(tag.c_str());

				mtlLoader >> tag;
				g = (float)atof(tag.c_str());

				mtlLoader >> tag;
				b = (float)atof(tag.c_str());

				d3dMtl->Specular = { r, g, b, 1.f };
			}
			if (tag == "Ke")
			{
				float r, g, b;
				r = g = b = 0.f;

				mtlLoader >> tag;
				r = (float)atof(tag.c_str());
				mtlLoader >> tag;
				g = (float)atof(tag.c_str());
				mtlLoader >> tag;
				b = (float)atof(tag.c_str());

				d3dMtl->Emissive = { r,g,b, 1.f };
			}
			if (tag == "illum")
			{
				float l;

				mtlLoader >> tag;
				l = (float)atof(tag.c_str());

				lpMtl->fIllum = l;
			}

			if (tag == "Ns")
			{
				float n;

				mtlLoader >> tag;
				n = (float)atof(tag.c_str());

				lpMtl->fNs = n;
			}

			if (tag == "Tr" || tag == "d")
			{
				float a;

				mtlLoader >> tag;

				if (tag == "Tr")
					a = 1.f - (float)atof(tag.c_str());
				else
					a = (float)atof(tag.c_str());

				lpMtl->fAlpha = a;
			}

			if (tag == "map_Kd")
			{
				mtlLoader >> tag;

				std::string ImagePath;

				if (mapPath == "None")
				{
					int pathIndex = objPath.rfind("/"); // 마지막으로 사용된 해당 문자의 인덱스를 가져옴
					ImagePath = objPath.substr(0, pathIndex + 1) + tag; // 첫번쨰 인자부터 두번째 인자의 수만큼 글자를 가져옴
				}
				else
					ImagePath = mapPath + tag;

				lpMtl->DiffuseMap = IMAGE->AddImage(ImagePath, ImagePath, true);
			}
			if (tag == "map_Ks")
			{
				mtlLoader >> tag;

				std::string ImagePath;

				if (mapPath == "None")
				{
					int pathIndex = objPath.rfind("/"); // 마지막으로 사용된 해당 문자의 인덱스를 가져옴
					ImagePath = objPath.substr(0, pathIndex + 1) + tag; // 첫번쨰 인자부터 두번째 인자의 수만큼 글자를 가져옴
				}
				else
					ImagePath = mapPath + tag;

				lpMtl->specularMap = IMAGE->AddImage(ImagePath, ImagePath, true);
			}


			if (tag == "newmtl")
				break;
		}
	}

	mtlLoader.close();



}

DWORD cMeshLoader::AddVerTex(UINT hash, VERTEX * pVerTex)
{
	bool bFoundInList = false;
	DWORD index = 0;

	// Since it's very slow to check every element in the vertex list, a hashtable stores
	// vertex indices according to the vertex position's index as reported by the OBJ file
	if ((UINT)VertexCache.size() > hash)
	{
		CacheEntry* pEntry = VertexCache[hash];
		while (pEntry != NULL)
		{
			VERTEX* pCacheVertex = &Ver[pEntry->index];

			// If this vertex is identical to the vertex already in the list, simply
			// point the index buffer to the existing vertex
			if (0 == memcmp(pVerTex, pCacheVertex, sizeof(VERTEX)))
			{
				bFoundInList = true;
				index = pEntry->index;
				break;
			}

			pEntry = pEntry->pNext;
		}
	}

	// Vertex was not found in the list. Create a new entry, both within the Vertices list
	// and also within the hashtable cache
	if (!bFoundInList)
	{
		// Add to the Vertices list
		index = Ver.size();
		Ver.push_back(*pVerTex);

		// Add this to the hashtable
		CacheEntry* pNewEntry = new CacheEntry;
		if (pNewEntry == NULL)
			return (DWORD)-1;

		pNewEntry->index = index;
		pNewEntry->pNext = NULL;

		// Grow the cache if needed
		while ((UINT)VertexCache.size() <= hash)
		{
			VertexCache.push_back(NULL);
		}

		// Add to the end of the linked list
		CacheEntry* pCurEntry = VertexCache[hash];
		if (pCurEntry == NULL)
		{
			// This is the head element
			//m_VertexCache.SetAt(hash, pNewEntry);
			VertexCache[hash] = pNewEntry;
		}
		else
		{
			// Find the tail
			while (pCurEntry->pNext != NULL)
			{
				pCurEntry = pCurEntry->pNext;
			}

			pCurEntry->pNext = pNewEntry;
		}
	}

	return index;
}

void cMeshLoader::ObjLoad(Mesh * mesh, const string & objpath, const string & mapPath)
{
	objPath = objpath;

	DWORD dwAttribute = 0;

	ifstream loader;

	loader.open(objpath.c_str());

	if (loader.fail())
	{
		
		return;
	}
	if (loader.fail())
	{
		
		return;
	}
	while (!loader.eof())
	{
		std::string tag;
		loader >> tag;

		if (tag == "v")
		{
			Vec3 position;
			loader >> position.x >> position.y >> position.z;
			Pos.push_back(position);
			continue;
		}
		else if (tag == "vt")
		{
			Vec2 texCoord;
			loader >> texCoord.x >> texCoord.y;
			TexCoord.push_back(Vec2(texCoord.x, 1.f - texCoord.y));
			continue;
		}
		else if (tag == "vn")
		{
			Vec3 normal;
			loader >> normal.x >> normal.y >> normal.z;
			Normal.push_back(normal);
			continue;
		}
		else if (tag == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				std::string str;
				loader >> str;

				UINT iPosition;
				UINT iTexCoord;
				UINT iNormal;

				sscanf(str.c_str(), "%u/%u/%u", &iPosition, &iTexCoord, &iNormal);

				VERTEX* vertex = new VERTEX(
					Pos[iPosition - 1],
					Normal[iNormal - 1],
					TexCoord[iTexCoord - 1]);
				
				DWORD index = AddVerTex(iPosition, vertex);
				Index.push_back(index);
				SAFE_DELETE(vertex);
			}

			Attribute.push_back(dwAttribute);
			continue;
		}
		else if (tag == "mtllib")
		{
			loader >> mtlPath;

			int pathIndex = objPath.rfind("/"); // 마지막으로 사용된 해당 문자의 인덱스를 가져옴

			mtlPath = objPath.substr(0, pathIndex + 1) + mtlPath; // 첫번쨰 인자부터 두번째 인자의 수만큼 글자를 가져옴
		}
		else if (tag == "usemtl")
		{
			loader >> tag;

			bool bFind = false;

			for (size_t iMtl = 0; iMtl < mesh->vMaterial.size(); ++iMtl)
			{
				if (mesh->vMaterial[iMtl]->mtlName == tag)
				{
					dwAttribute = iMtl;
					bFind = true;
				}
			}

			if (!bFind)
			{
				dwAttribute = mesh->vMaterial.size();
				MeshLoad(tag, mesh, mapPath);
			}

			continue;
		}
	}

	return Create(&mesh->LPD3DMesh);

}
