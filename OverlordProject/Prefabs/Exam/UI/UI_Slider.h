#pragma once
class UI_Slider : public GameObject
{
public:
	UI_Slider(float minValue, float maxValue, const std::wstring& fillPath, const std::wstring& bgPath,
		XMFLOAT2 pivot = { 0.5f,0.5f }, XMFLOAT4 color = { 1,1,1,1 });
	~UI_Slider() override = default;

	UI_Slider(const UI_Slider& other) = delete;
	UI_Slider(UI_Slider&& other) noexcept = delete;
	UI_Slider& operator=(const UI_Slider& other) = delete;
	UI_Slider& operator=(UI_Slider&& other) noexcept = delete;

	float GetValue() const { return m_Value; }
	void SetValue(float value);
	bool isDirty() const { return m_IsDirty;  }
	void SetDirty(bool value) { m_IsDirty = value; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void UpdateUI();

private:
	std::wstring m_FillSpriteFilePath;
	std::wstring m_BackgroundSpriteFilePath;

	SpriteComponent* m_pFillSprite;
	SpriteComponent* m_pBackgroundSprite;
	XMFLOAT2 m_Pivot;
	XMFLOAT4 m_Color;

	bool m_IsDirty;

	float m_Value;
	float m_MinValue;
	float m_MaxValue;
};
