#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial()
	: Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
	, m_pSpikeHeight(new float{0.2f})
	, m_pDiffuseColor(new float[3]{1.0f, 1.0f, 1.0f})
{}

SpikyMaterial::~SpikyMaterial()
{
	delete m_pSpikeHeight;
	delete[] m_pDiffuseColor;
}


void SpikyMaterial::OnGui()
{
	// BaseMaterial::DrawImGui();

	ImGui::Spacing();
	ImGui::Text("Spiky Material");
	ImGui::Spacing();
	if (ImGui::InputFloat("Spike Height", m_pSpikeHeight, 0.01f, 0.1f))
	{
		SetVariable_Scalar(L"gSpikeLength", *m_pSpikeHeight);
	}
	if (ImGui::ColorEdit3("Diffuse Color", m_pDiffuseColor))
	{
		SetVariable_Vector(L"gColorDiffuse", XMFLOAT3(m_pDiffuseColor[0], m_pDiffuseColor[1], m_pDiffuseColor[2]));
	}
}

void SpikyMaterial::InitializeEffectVariables()
{
	SetVariable_Scalar(L"gSpikeLength", *m_pSpikeHeight);
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT3(m_pDiffuseColor[0], m_pDiffuseColor[1], m_pDiffuseColor[2]));
}
