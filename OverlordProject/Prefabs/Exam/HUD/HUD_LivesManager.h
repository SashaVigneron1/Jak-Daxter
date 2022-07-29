#pragma once

class HUD_LifeIcon;

class HUD_LivesManager : public GameObject
{
public:
	HUD_LivesManager(int maxHealth, const std::wstring& emptyPath, const std::wstring& filledPath,
		XMFLOAT2 pivot = { 0.5f,0.5f }, XMFLOAT4 color = { 1,1,1,1 });
	~HUD_LivesManager() override = default;
	HUD_LivesManager(const HUD_LivesManager& other) = delete;
	HUD_LivesManager(HUD_LivesManager&& other) noexcept = delete;
	HUD_LivesManager& operator=(const HUD_LivesManager& other) = delete;
	HUD_LivesManager& operator=(HUD_LivesManager&& other) noexcept = delete;


	void SetHealth(int value);

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override {}

private:
	std::wstring m_EmptyFilePath;
	std::wstring m_FilledFilePath;

	XMFLOAT2 m_Pivot;
	XMFLOAT4 m_Color;

	int m_MaxHealth;
	float m_HeartOffset;
	float m_Scale;

	std::vector<HUD_LifeIcon*> m_pHearts;
};

