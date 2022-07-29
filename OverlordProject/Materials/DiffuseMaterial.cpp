#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial() :
	Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fx")
{}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuse = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuse);
}

void DiffuseMaterial::InitializeEffectVariables()
{

}
