#include "stdafx.h"
#include "SkyboxPrefab.h"

#include "stdafx.h"
#include "SkyBoxPrefab.h"
#include "Materials/Exam/SkyboxMaterial.h"

SkyboxPrefab::SkyboxPrefab(const std::wstring& assetFile)
	: m_AssetFile(assetFile)
{
	auto pModel = AddComponent(new ModelComponent(L"Meshes/Box.ovm"));
	auto skyboxMaterial = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	skyboxMaterial->SetCubeMapTexture(m_AssetFile);
	pModel->SetMaterial(skyboxMaterial, 1);
}

SkyboxPrefab::~SkyboxPrefab()
{
}

