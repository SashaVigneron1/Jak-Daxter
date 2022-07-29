#pragma once
class DiffuseMaterial_Shadow_Deferred final : public Material<DiffuseMaterial_Shadow_Deferred>
{
public:
	DiffuseMaterial_Shadow_Deferred();
	~DiffuseMaterial_Shadow_Deferred() override = default;

	DiffuseMaterial_Shadow_Deferred(const DiffuseMaterial_Shadow_Deferred& other) = delete;
	DiffuseMaterial_Shadow_Deferred(DiffuseMaterial_Shadow_Deferred&& other) noexcept = delete;
	DiffuseMaterial_Shadow_Deferred& operator=(const DiffuseMaterial_Shadow_Deferred& other) = delete;
	DiffuseMaterial_Shadow_Deferred& operator=(DiffuseMaterial_Shadow_Deferred&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext&, const ModelComponent*) const override;

private:
	TextureData* m_pDiffuseTexture{};
};
