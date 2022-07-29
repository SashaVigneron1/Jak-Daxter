#pragma once

class UI_ButtonManager;

class UI_Button : public GameObject
{
public:
	UI_Button(UI_ButtonManager* pButtonManager, const std::wstring& defaultPath, const std::wstring& hoveringPath, const std::wstring& pressedPath, 
		XMFLOAT2 pivot = {0.5f,0.5f}, XMFLOAT4 color = {1,1,1,1});
	~UI_Button() override = default;

	UI_Button(const UI_Button& other) = delete;
	UI_Button(UI_Button&& other) noexcept = delete;
	UI_Button& operator=(const UI_Button& other) = delete;
	UI_Button& operator=(UI_Button&& other) noexcept = delete;

	bool IsPressed(const SceneContext&);
	void Select(bool value, bool shouldNotifyButtonManager = true);
	void Execute() { m_IsExecuting = true; }

	void SelectButtonUp();
	void SelectButtonDown();
	void SelectButtonLeft();
	void SelectButtonRight();

	void SetButtonUp(UI_Button* pButton) { m_pButtonUp = pButton; }
	void SetButtonDown(UI_Button* pButton) { m_pButtonDown = pButton; }
	void SetButtonLeft(UI_Button* pButton) { m_pButtonLeft = pButton; }
	void SetButtonRight(UI_Button* pButton) { m_pButtonRight = pButton; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	std::wstring m_DefaultSpriteFilePath;
	std::wstring m_HoveringSpriteFilePath;
	std::wstring m_PressedSpriteFilePath;

	UI_ButtonManager* m_pButtonManager;
	UI_Button* m_pButtonUp;
	UI_Button* m_pButtonDown;
	UI_Button* m_pButtonLeft;
	UI_Button* m_pButtonRight;

	SpriteComponent* m_pSprite;
	XMFLOAT2 m_Pivot;
	XMFLOAT4 m_Color;

	bool m_IsExecuting;
	bool m_IsSelected;
	bool m_WasPressed;
	float m_PressedSpriteTimer;
	float m_AccPressedTime;
};
