#include "stdafx.h"
#include "SkyboxMaterial.h"


SkyboxMaterial::SkyboxMaterial()
	: Material<SkyboxMaterial>(L"Effects/Skybox.fx")
{
}

SkyboxMaterial::~SkyboxMaterial()
{
}

void SkyboxMaterial::SetCubeMapTexture(const std::wstring& assetFile)
{
	m_pCubemap = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gCubeMap", m_pCubemap);
}

void SkyboxMaterial::InitializeEffectVariables()
{

}

