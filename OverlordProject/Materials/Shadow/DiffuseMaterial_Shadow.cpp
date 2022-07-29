#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow.fx")
{}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow::SetShadowBias(float value)
{
	SetVariable_Scalar(L"gShadowMapBias", value);
}

void DiffuseMaterial_Shadow::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	// Update The Shader Variables
	// 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	// 	LightWVP = model_world * light_viewprojection
	// 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	const auto pShadowMapRenderer = ShadowMapRenderer::Get();

	auto lightVP = pShadowMapRenderer->GetLightVP();
	auto world = pModel->GetTransform()->GetWorld();
	auto lightVPMat = XMLoadFloat4x4(&lightVP);
	auto worldMat = XMLoadFloat4x4(&world);
	auto lightWVPMat = worldMat * lightVPMat;
	XMFLOAT4X4 lightWVP;
	XMStoreFloat4x4(&lightWVP, lightWVPMat);

	SetVariable_Matrix(L"gWorldViewProj_Light", lightWVP);

	// 2. Update the ShadowMap texture
	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());

	// 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
}
