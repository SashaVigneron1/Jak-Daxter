#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial()
	: Material<UberMaterial>(L"Effects/UberShader.fx")

	// Diffuse
	, m_UseDiffuseTexture{ new bool{true} }
	, m_DiffuseColor(new float[3]{1,1,1})

	// Normal
	, m_UseNormalTexture{ new bool{true} }
	, m_FlipGreenChannel{ new bool{true} }

	// Specular
	, m_UseSpecularTexture{ new bool{false} }
	, m_UseBlinn{ new bool{true} }
	, m_UsePhong{ new bool{false} }
	, m_Shininess{ new int{15} }
	, m_SpecularColor(new float[3]{1,1,1})

	// Ambient
	, m_AmbientIntensity{ new float{0.f} }
	, m_AmbientColor(new float[4]{1,1,1})

	// Fresnel
	, m_UseFresnelFalloff(new bool{true})
	, m_FresnelColor(new float[3]{1,1,1})
	, m_FresnelPower(new float{2.0f})
	, m_FresnelMultiplier(new float{1.0f})
	, m_FresnelHardness(new float{0.0f})

	// CubeMap
	, m_UseCubeMap{ new bool{true} }
	, m_ReflectionStrength(new float{ 1.0f })
	, m_RefractionStrength(new float{ 0.0f })
	, m_RefractionIndex(new float{ 0.3f })

	// Opacity
	, m_UseOpacity(new bool{false})
	, m_OpacityIntensity(new float{1.0f})
{
}

UberMaterial::~UberMaterial()
{
	// Diffuse
	delete[] m_DiffuseColor;
	delete m_UseDiffuseTexture;

	// Normal
	delete m_UseNormalTexture;
	delete m_FlipGreenChannel;

	// Specular
	delete[] m_SpecularColor;
	delete m_UseSpecularTexture;
	delete m_UseBlinn;
	delete m_UsePhong;
	delete m_Shininess;

	// Ambient
	delete[] m_AmbientColor;
	delete m_AmbientIntensity;

	// Fresnel
	delete[] m_FresnelColor;
	delete m_FresnelPower;
	delete m_FresnelMultiplier;
	delete m_FresnelHardness;
	delete m_UseFresnelFalloff;

	// EnvironmentMapping
	delete m_UseCubeMap;
	delete m_ReflectionStrength;
	delete m_RefractionStrength;
	delete m_RefractionIndex;

	// Opacity
	delete m_UseOpacity;
	delete m_OpacityIntensity;
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuse = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuse);
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormal = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureNormal", m_pNormal);
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetFile)
{
	m_pSpecular = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecular);
}

void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacity = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gTextureOpacity", m_pOpacity);
}

void UberMaterial::SetCubeMapTexture(const std::wstring& assetFile)
{
	m_pCubeMap = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gCubeEnvironment", m_pCubeMap);
}

void UberMaterial::OnGui()
{
	BaseMaterial::DrawImGui();

	//ImGui::Spacing();
	//ImGui::Text("DIFFUSE");
	//ImGui::Spacing();
	//if (ImGui::Checkbox("Use Diffuse Texture", m_UseDiffuseTexture))
	//{
	//	SetVariable_Scalar(L"gUseTextureDiffuse", *m_UseDiffuseTexture);
	//}
	//if (ImGui::ColorEdit3("Diffuse Color", m_DiffuseColor, ImGuiColorEditFlags_NoInputs))
	//{
	//	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4(m_DiffuseColor[0], m_DiffuseColor[1], m_DiffuseColor[2], 1));
	//}

	//ImGui::Spacing();
	//ImGui::Text("NORMAL");
	//ImGui::Spacing();
	//if (ImGui::Checkbox("Use Normal Texture", m_UseNormalTexture))
	//{
	//	SetVariable_Scalar(L"gUseTextureNormal", *m_UseNormalTexture);
	//}
	//if (ImGui::Checkbox("Flip Green Channel", m_FlipGreenChannel))
	//{
	//	SetVariable_Scalar(L"gFlipGreenChannel", *m_FlipGreenChannel);
	//}

	//ImGui::Spacing();
	//ImGui::Text("SPECULAR");
	//ImGui::Spacing();
	//if (ImGui::Checkbox("Use Specular Texture", m_UseSpecularTexture))
	//{
	//	SetVariable_Scalar(L"gUseTextureSpecularIntensity", *m_UseSpecularTexture);
	//}
	//if (ImGui::Checkbox("Use Specular Blinn", m_UseBlinn))
	//{
	//	SetVariable_Scalar(L"gUseSpecularBlinn", *m_UseBlinn);
	//}
	//if (ImGui::Checkbox("Use Specular Phong", m_UsePhong))
	//{
	//	SetVariable_Scalar(L"gUseSpecularPhong", *m_UsePhong);
	//}
	//if (ImGui::ColorEdit3("Specular Color", m_SpecularColor, ImGuiColorEditFlags_NoInputs))
	//{
	//	SetVariable_Vector(L"gColorSpecular", XMFLOAT4(m_SpecularColor[0], m_SpecularColor[1], m_SpecularColor[2], 1));
	//}
	//if (ImGui::InputInt("Shininess", m_Shininess, 1, 10))
	//{
	//	SetVariable_Scalar(L"gShininess", *m_Shininess);
	//}

	//ImGui::Spacing();
	//ImGui::Text("AMBIENT");
	//ImGui::Spacing();
	//if (ImGui::ColorEdit4("Ambient Color", m_AmbientColor, ImGuiColorEditFlags_NoInputs))
	//{
	//	SetVariable_Vector(L"gColorAmbient", XMFLOAT4(m_AmbientColor[0], m_AmbientColor[1], m_AmbientColor[2], m_AmbientColor[3]));
	//}
	//if (ImGui::InputFloat("Ambient Intensity", m_AmbientIntensity, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gAmbientIntensity", *m_AmbientIntensity);
	//}

	//ImGui::Spacing();
	//ImGui::Text("ENVIRONMENT");
	//ImGui::Spacing();
	//if (ImGui::Checkbox("Use CubeMap", m_UseCubeMap))
	//{
	//	SetVariable_Scalar(L"gUseEnvironmentMapping", *m_UseCubeMap);
	//}
	//if (ImGui::InputFloat("Reflection Strength", m_ReflectionStrength, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gReflectionStrength", *m_ReflectionStrength);
	//}
	//if (ImGui::InputFloat("Refraction Strength", m_RefractionStrength, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gRefractionStrength", *m_RefractionStrength);
	//}
	//if (ImGui::InputFloat("Refraction Index", m_RefractionIndex, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gRefractionIndex", *m_RefractionIndex);
	//}

	//ImGui::Spacing();
	//ImGui::Text("FRESNEL");
	//ImGui::Spacing();
	//if (ImGui::Checkbox("Use Fresnel", m_UseFresnelFalloff))
	//{
	//	SetVariable_Scalar(L"gUseFresnelFalloff", *m_UseFresnelFalloff);
	//}
	//if (ImGui::ColorEdit3("Fresnel Color", m_FresnelColor, ImGuiColorEditFlags_NoInputs))
	//{
	//	SetVariable_Vector(L"gColorFresnel", XMFLOAT4(m_FresnelColor[0], m_FresnelColor[1], m_FresnelColor[2], 1));
	//}
	//if (ImGui::InputFloat("Fresnel Power", m_FresnelPower, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gFresnelPower", *m_FresnelPower);
	//}
	//if (ImGui::InputFloat("Fresnel Multiplier", m_FresnelMultiplier, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gFresnelMultiplier", *m_FresnelMultiplier);
	//}
	//if (ImGui::InputFloat("Fresnel Hardness", m_FresnelHardness, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gFresnelHardness", *m_FresnelHardness);
	//}

	//ImGui::Spacing();
	//ImGui::Text("Opacity");
	//ImGui::Spacing();
	//if (ImGui::Checkbox("Use Opacity", m_UseOpacity))
	//{
	//	SetVariable_Scalar(L"gTextureOpacityIntensity", *m_UseOpacity);
	//}
	//if (ImGui::InputFloat("Opacity Intensity", m_OpacityIntensity, 0.1f, 0.25f))
	//{
	//	SetVariable_Scalar(L"gOpacityIntensity", *m_OpacityIntensity);
	//}
}

void UberMaterial::InitializeEffectVariables()
{
	// Diffuse
	SetVariable_Scalar(L"gUseTextureDiffuse", *m_UseDiffuseTexture);
	SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	SetVariable_Vector(L"gColorDiffuse", XMFLOAT4(m_DiffuseColor[0], m_DiffuseColor[1], m_DiffuseColor[2], 1));

	// Specular
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", *m_UseSpecularTexture);
	SetSpecularTexture(L"Textures/Specular_Level.tga");
	SetVariable_Scalar(L"gUseSpecularBlinn", *m_UseBlinn);
	SetVariable_Scalar(L"gShininess", *m_Shininess);
	SetVariable_Vector(L"gColorSpecular", XMFLOAT4(m_SpecularColor[0], m_SpecularColor[1], m_SpecularColor[2], 1));

	// Ambient
	SetVariable_Scalar(L"gAmbientIntensity", *m_AmbientIntensity);
	SetVariable_Vector(L"gColorAmbient", XMFLOAT4(m_AmbientColor[0], m_AmbientColor[1], m_AmbientColor[2], m_AmbientColor[3]));

	// Normal
	SetVariable_Scalar(L"gUseTextureNormal", *m_UseNormalTexture);
	SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	SetVariable_Scalar(L"gFlipGreenChannel", *m_FlipGreenChannel);

	// Environment Mapping
	SetVariable_Scalar(L"gUseEnvironmentMapping", *m_UseCubeMap);
	SetCubeMapTexture(L"Textures/Sunol_Cubemap.dds");
	SetVariable_Scalar(L"gReflectionStrength", *m_ReflectionStrength);
	SetVariable_Scalar(L"gRefractionStrength", *m_RefractionStrength);
	SetVariable_Scalar(L"gRefractionIndex", *m_RefractionIndex);

	// Fresnel Fallof
	SetVariable_Scalar(L"gUseFresnelFalloff", *m_UseFresnelFalloff);
	SetVariable_Scalar(L"gFresnelPower", *m_FresnelPower);
	SetVariable_Scalar(L"gFresnelMultiplier", *m_FresnelMultiplier);
	SetVariable_Scalar(L"gFresnelHardness", *m_FresnelHardness);
	SetVariable_Vector(L"gColorFresnel", XMFLOAT4(m_FresnelColor[0], m_FresnelColor[1], m_FresnelColor[2], 1));

	// Opacity
	SetVariable_Scalar(L"gTextureOpacityIntensity", *m_UseOpacity);
	SetVariable_Scalar(L"gOpacityIntensity", *m_OpacityIntensity);
	SetOpacityTexture(L"Textures/Skulls_Diffusemap.tga");


}
