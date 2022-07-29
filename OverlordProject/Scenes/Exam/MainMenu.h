#pragma once

class UI_Slider;
class UI_ButtonManager;
class UI_Button;

class MainMenu : public GameScene
{
public:
	MainMenu() : GameScene(L"MainMenu") {}
	~MainMenu() override;
	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) noexcept = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) noexcept = delete;

	void UpdateSettings();
protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void PostDraw() override;

	void SetBrightness(float value);
	void SetVolume(float value);
private:
	UI_Button* m_pNewGameButton{};
	UI_Button* m_pOptionsButton{};
	UI_Button* m_pQuitGameButton{};
	UI_Button* m_pReturnButton{};
	UI_Slider* m_pBrightnessSlider{};
	UI_Slider* m_pVolumeSlider{};

	UI_ButtonManager* m_pButtonManager{};

	GameObject* m_pOptionsMenu{};

	FMOD::Channel* m_pMusicChannel{};
};
