#include "stdafx.h"
#include "PostBrightness.h"

PostBrightness::PostBrightness()
	: PostProcessingMaterial(L"Effects/Post/Brightness.fx")
{
}

void PostBrightness::SetBrightness(float value)
{
	m_Brightness = value;
	MathHelper::Clamp(m_Brightness, m_MaxBrightness, m_MinBrightness);
	m_pBaseEffect->GetVariableByName("gBrightness")->AsScalar()->SetFloat(m_Brightness);
}

void PostBrightness::UpdateBaseEffectVariables(const SceneContext& /*sceneContext*/, RenderTarget* pSource)
{
	const auto pSourceSRV = pSource->GetColorShaderResourceView();
	m_pBaseEffect->GetVariableByName("gTexture")->AsShaderResource()->SetResource(pSourceSRV);
}
