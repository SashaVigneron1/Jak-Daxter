#pragma once

class HUD_LifeIcon : public GameObject
{
public:
	HUD_LifeIcon(const std::wstring& emptyPath, const std::wstring& filledPath,
		XMFLOAT2 pivot = { 0.5f,0.5f }, XMFLOAT4 color = { 1,1,1,1 });
	~HUD_LifeIcon() override = default;
	HUD_LifeIcon(const HUD_LifeIcon& other) = delete;
	HUD_LifeIcon(HUD_LifeIcon&& other) noexcept = delete;
	HUD_LifeIcon& operator=(const HUD_LifeIcon& other) = delete;
	HUD_LifeIcon& operator=(HUD_LifeIcon&& other) noexcept = delete;

	void FillHart(bool value);

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	std::wstring m_EmptyFilePath;
	std::wstring m_FilledFilePath;

	SpriteComponent* m_pSprite;
	XMFLOAT2 m_Pivot;
	XMFLOAT4 m_Color;
};

