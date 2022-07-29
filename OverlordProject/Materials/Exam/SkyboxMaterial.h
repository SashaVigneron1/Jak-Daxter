#pragma once

class SkyboxMaterial :public Material<SkyboxMaterial>
{
public:
	SkyboxMaterial();
	~SkyboxMaterial() override;

	void SetCubeMapTexture(const std::wstring& assetFile);
protected:
	void InitializeEffectVariables() override;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyboxMaterial(const SkyboxMaterial& obj);
	SkyboxMaterial& operator=(const SkyboxMaterial& obj);

	TextureData* m_pCubemap = nullptr;
};