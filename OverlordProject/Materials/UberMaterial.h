#pragma once
class UberMaterial final : public Material<UberMaterial>
{
public:
	UberMaterial();
	~UberMaterial() override;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetNormalTexture(const std::wstring& assetFile);
	void SetSpecularTexture(const std::wstring& assetFile);
	void SetOpacityTexture(const std::wstring& assetFile);
	void SetCubeMapTexture(const std::wstring& assetFile);

	void OnGui();

protected:
	void InitializeEffectVariables() override;

	TextureData* m_pDiffuse;
	TextureData* m_pNormal;
	TextureData* m_pSpecular;
	TextureData* m_pOpacity;

	TextureData* m_pCubeMap;

	// Diffuse
	bool* m_UseDiffuseTexture;
	float* m_DiffuseColor;

	// Normal
	bool* m_UseNormalTexture;
	bool* m_FlipGreenChannel;

	// Specular
	bool* m_UseSpecularTexture;
	bool* m_UseBlinn;
	bool* m_UsePhong;
	float* m_SpecularColor;
	int* m_Shininess;

	// Environment
	bool* m_UseCubeMap;
	float* m_ReflectionStrength;
	float* m_RefractionStrength;
	float* m_RefractionIndex;

	// Ambient
	float* m_AmbientColor;
	float* m_AmbientIntensity;

	// Fresnel
	bool* m_UseFresnelFalloff;
	float* m_FresnelPower;
	float* m_FresnelMultiplier;
	float* m_FresnelHardness;
	float* m_FresnelColor;

	// Opacity
	bool* m_UseOpacity;
	float* m_OpacityIntensity;
};

