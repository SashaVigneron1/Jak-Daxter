#pragma once
class UI_Button;

class UI_ButtonManager : public GameObject
{
public:
	UI_ButtonManager(const SceneContext& sceneContext);
	~UI_ButtonManager() override = default;

	UI_ButtonManager(const UI_ButtonManager& other) = delete;
	UI_ButtonManager(UI_ButtonManager&& other) noexcept = delete;
	UI_ButtonManager& operator=(const UI_ButtonManager& other) = delete;
	UI_ButtonManager& operator=(UI_ButtonManager&& other) noexcept = delete;

	void SelectButton(UI_Button* pButton);
	void AddButton(UI_Button* pButton) { m_pButtons.push_back(pButton); }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	UI_Button* m_pSelectedButton;
	std::vector<UI_Button*> m_pButtons;

	enum InputIds
	{
		moveUp = 20,
		moveDown = 21,
		moveLeft = 22,
		moveRight = 23,
		execute = 24
	};
};

