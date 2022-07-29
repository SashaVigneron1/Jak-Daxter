#include "stdafx.h"
#include "MainMenu.h"

#include "stdafx.h"

#include "JakAndDaxterScene.h"
#include "Managers/GameManager.h"
#include "Prefabs/Exam/UI/UI_Button.h"
#include "Prefabs/Exam/UI/UI_ButtonManager.h"
#include "Prefabs/Exam/UI/UI_Slider.h"

MainMenu::~MainMenu()
{
}

void MainMenu::Initialize()
{
	// Model
	/*auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/PeasantGirl_Diffuse.png");
	auto pObject = AddChild(new GameObject);
	auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Jack.ovm"));
	pModel->SetMaterial(pMaterial);*/

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = false;

	// GameManager

	// BackGround
	auto background = AddChild(new GameObject());
	const float referenceWidth{ 1920 };
	const float referenceHeight{ 1080 };
	background->AddComponent(new SpriteComponent(L"UI/Background.jpg", XMFLOAT2{ 0.5f,0.5f }));
	background->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);
	background->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.f);

	// Buttons
	m_pButtonManager = AddChild(new UI_ButtonManager(m_SceneContext));

	float buttonOffsetY = 170 * (m_SceneContext.windowHeight / referenceHeight);
	XMFLOAT3 buttonPos = { 100, (m_SceneContext.windowHeight / 2.f) + 20, .8f };

	m_pNewGameButton = AddChild(new UI_Button(m_pButtonManager, L"UI/NewGame_Default.png", L"UI/NewGame_Highlighted.png", L"UI/NewGame_Pressed.png"
		, { 0.0f,0.5f }));
	m_pNewGameButton->GetTransform()->Translate(buttonPos.x, buttonPos.y, .8f);
	m_pNewGameButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pNewGameButton);

	buttonPos.y += buttonOffsetY;

	m_pOptionsButton = AddChild(new UI_Button(m_pButtonManager, L"UI/Options_Default.png", L"UI/Options_Highlighted.png", L"UI/Options_Pressed.png"
		, { 0.0f,0.5f }));
	m_pOptionsButton->GetTransform()->Translate(buttonPos.x, buttonPos.y, .8f);
	m_pOptionsButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pOptionsButton);

	buttonPos.y += buttonOffsetY;

	m_pQuitGameButton = AddChild(new UI_Button(m_pButtonManager, L"UI/QuitGame_Default.png", L"UI/QuitGame_Highlighted.png", L"UI/QuitGame_Pressed.png"
		, { 0.0f,0.5f }));
	m_pQuitGameButton->GetTransform()->Translate(buttonPos.x, buttonPos.y, .8f);
	m_pQuitGameButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pQuitGameButton);

	// Set Button Links
	m_pNewGameButton->Select(true);
	m_pNewGameButton->SetButtonDown(m_pOptionsButton);
	m_pOptionsButton->SetButtonUp(m_pNewGameButton);
	m_pOptionsButton->SetButtonDown(m_pQuitGameButton);
	m_pQuitGameButton->SetButtonUp(m_pOptionsButton);

	// OPTIONS MENU
	m_pOptionsMenu = AddChild(new GameObject());
	m_pOptionsMenu->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .7f);

	background = m_pOptionsMenu->AddChild(new GameObject());
	background->AddComponent(new SpriteComponent(L"UI/Options_BG.png", XMFLOAT2{ 0.5f,0.5f }));
	background->GetTransform()->Translate(0, 0, 0.1f);
	background->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.f);

	// Audio Volume
	m_pVolumeSlider = m_pOptionsMenu->AddChild(new UI_Slider(0.0f, 1.f, L"UI/Slider_Fill.png", L"UI/Slider_BG.png", { 0,0.5f }));
	m_pVolumeSlider->GetTransform()->Translate(-50, -30, 0);
	m_pVolumeSlider->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.0f);
	m_pVolumeSlider->SetValue(0.5f);

	// Brightness
	m_pBrightnessSlider = m_pOptionsMenu->AddChild(new UI_Slider(0.5f, 2.f, L"UI/Slider_Fill.png", L"UI/Slider_BG.png", { 0,0.5f }));
	m_pBrightnessSlider->GetTransform()->Translate(-50, -78, 0);
	m_pBrightnessSlider->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.0f);
	m_pBrightnessSlider->SetValue(1.0f);

	// Return Button
	m_pReturnButton = m_pOptionsMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/Return_Default.png", L"UI/Return_Highlighted.png", L"UI/Return_Pressed.png"
		, { 0.5f,0.5f }));
	m_pReturnButton->GetTransform()->Translate(0, 175, 0);
	m_pReturnButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pReturnButton);

	m_pOptionsMenu->SetActive(false);


	// SOUNDS
	auto pFModSystem = SoundManager::Get()->GetSystem();

	FMOD::Sound* pSound{};
	pFModSystem->createStream("Resources/Sounds/Ambient/MainMenuMusic.mp3", FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pFModSystem->playSound(pSound, nullptr, false, &m_pMusicChannel);
}

void MainMenu::OnGUI()
{
}

void MainMenu::Update()
{
	if (m_pNewGameButton->IsPressed(m_SceneContext))
	{
		// Start New Game
		auto gameScene = reinterpret_cast<JakAndDaxterScene*>(SceneManager::Get()->GetScene(L"JakAndDaxterScene"));
		/*gameScene->SetBrightness(m_Brightness);
		gameScene->SetVolume(m_Volume);*/
		gameScene->Reset();

		m_pMusicChannel->setPaused(true);

		GameManager::SetGameState(GameManager::GameState::Playing);
		SceneManager::Get()->NextScene();
	}
	else if (m_pReturnButton->IsPressed(m_SceneContext))
	{
		m_pOptionsMenu->SetActive(false);
		m_pNewGameButton->Select(true);
	}
	else if (m_pOptionsButton->IsPressed(m_SceneContext))
	{
		// Open Options
		m_pOptionsMenu->SetActive(true);
		m_pReturnButton->Select(true);
	}
	else if (m_pQuitGameButton->IsPressed(m_SceneContext))
	{
		// Quit Game
		QuitGame();
	}
	

	if (m_pBrightnessSlider->isDirty())
	{
		GameManager::Get()->SetBrightness(m_pBrightnessSlider->GetValue());
		m_pBrightnessSlider->SetDirty(false);
	}
	if (m_pVolumeSlider->isDirty())
	{
		GameManager::Get()->SetVolume(m_pVolumeSlider->GetValue());
		m_pVolumeSlider->SetDirty(false);
		SetVolume(m_pVolumeSlider->GetValue());
	}
}

void MainMenu::PostDraw()
{
}


void MainMenu::UpdateSettings()
{
	SetVolume(GameManager::Get()->GetVolume());
	SetBrightness(GameManager::Get()->GetBrightness());
	m_pMusicChannel->setPaused(false);
}

void MainMenu::SetBrightness(float value)
{
	m_pBrightnessSlider->SetValue(value);
}

void MainMenu::SetVolume(float value)
{
	m_pVolumeSlider->SetValue(value);
	m_pMusicChannel->setVolume(value);
}