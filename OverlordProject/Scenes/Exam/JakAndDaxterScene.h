#pragma once
#include "Materials/Post/PostBrightness.h"
#include "Prefabs/Exam/UI/UI_Slider.h"
class HUD_LivesManager;
class UI_Button;
class UI_ButtonManager;
class UI_Slider;
class EnemySpawner;
class PostBrightness;
class Jak;

class JakAndDaxterScene : public GameScene
{
public:
	JakAndDaxterScene() : GameScene(L"JakAndDaxterScene") {}
	~JakAndDaxterScene() override;
	JakAndDaxterScene(const JakAndDaxterScene& other) = delete;
	JakAndDaxterScene(JakAndDaxterScene&& other) noexcept = delete;
	JakAndDaxterScene& operator=(const JakAndDaxterScene& other) = delete;
	JakAndDaxterScene& operator=(JakAndDaxterScene&& other) noexcept = delete;

	void Reset();

protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void PostDraw() override;

	void SetBrightness(float value);
	void SetVolume(float value);

	void GoToMainMenu();
	void RestartGame();

	FMOD_VECTOR ToFModVector(XMFLOAT3 v);
private:
	enum InputIds
	{
		JakMoveLeft,
		JakMoveRight,
		JakMoveForward,
		JakMoveBackward,
		JakJump,

		JakAttack,

		ToggleOptions,
		ShowControls,
	};

	XMFLOAT3 m_SpawnPos{};
	Jak* m_pJak{};


	EnemySpawner* m_pEnemyManager{};

	PostBrightness* m_pPostBrightness{};

	// UI
	UI_ButtonManager* m_pButtonManager{};

	GameObject* m_pOptionsMenu{};
	UI_Button* m_pOptionsReturnButton{};
	UI_Button* m_pOptionsRestartButton{};
	UI_Button* m_pOptionsReturnToMainButton{};
	UI_Slider* m_pOptionsBrightnessSlider{};
	UI_Slider* m_pOptionsVolumeSlider{};

	GameObject* m_pGameOverMenu{};
	UI_Button* m_pGameOverReturnToMainButton{};
	UI_Button* m_pGameOverRestartButton{};

	GameObject* m_pGameWonMenu{};
	UI_Button* m_pGameWonReturnToMainButton{};
	UI_Button* m_pGameWonRestartButton{};

	GameObject* m_pControllerLayout{};

	// HUD
	HUD_LivesManager* m_pLivesManager{};

	float m_ControllerTime{3.0f};
	float m_AccControllerTime{0};
	bool m_IsShowingController{false};


	// SOUNDS
	FMOD_VECTOR m_PrevCameraPos{};
	FMOD::Channel* m_pMusicChannel{};

	FMOD::ChannelGroup* m_pChannelGroup{  };
	FMOD::Sound* m_pGameOverSound{};
	FMOD::Sound* m_pGameWonSound{};
};