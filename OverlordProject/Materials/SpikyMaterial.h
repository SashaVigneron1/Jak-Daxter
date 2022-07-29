#pragma once
class SpikyMaterial final : public Material<SpikyMaterial>
{
public:
	SpikyMaterial();
	~SpikyMaterial() override;

	SpikyMaterial(const SpikyMaterial& other) = delete;
	SpikyMaterial(SpikyMaterial&& other) noexcept = delete;
	SpikyMaterial& operator=(const SpikyMaterial& other) = delete;
	SpikyMaterial& operator=(SpikyMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

	void OnGui();

protected:
	void InitializeEffectVariables() override;

	float* m_pSpikeHeight;
	float* m_pDiffuseColor;
};

