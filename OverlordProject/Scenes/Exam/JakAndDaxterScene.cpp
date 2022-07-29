#include "stdafx.h"
#include "JakAndDaxterScene.h"

#include "MainMenu.h"
#include "Managers/GameManager.h"
#include "Materials/Deferred/DiffuseMaterial_Shadow_Deferred.h"
#include "Materials/Post/PostBrightness.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Exam/EnemySpawner.h"
#include "Prefabs/Exam/Jak.h"
#include "Prefabs/Exam/SkyboxPrefab.h"
#include "Prefabs/Exam/HUD/HUD_LivesManager.h"
#include "Prefabs/Exam/UI/UI_Button.h"
#include "Prefabs/Exam/UI/UI_ButtonManager.h"
#include "Prefabs/Exam/UI/UI_Slider.h"

JakAndDaxterScene::~JakAndDaxterScene()
{
}

void JakAndDaxterScene::Initialize()
{
	m_SceneContext.useDeferredRendering = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.enableOnGUI = false;
	m_SceneContext.settings.drawGrid = false;

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	//GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Character
	#pragma region Character
	m_SpawnPos = XMFLOAT3{ 25, 5.f, 20.5f };

	JakDesc jakDesc{ pDefaultMaterial };
	jakDesc.actionId_MoveForward = JakMoveForward;
	jakDesc.actionId_MoveBackward = JakMoveBackward;
	jakDesc.actionId_MoveLeft = JakMoveLeft;
	jakDesc.actionId_MoveRight = JakMoveRight;
	jakDesc.actionId_Jump = JakJump;
	jakDesc.actionId_Attack = JakAttack;

	m_pJak = AddChild(new Jak(jakDesc, GetPhysxProxy()));
	m_pJak->GetTransform()->Translate(m_SpawnPos);
	m_pJak->GetTransform()->Rotate(0, -90, 0);
	#pragma endregion

	// Enemies
	#pragma region Enemies
	m_pEnemyManager = AddChild(new EnemySpawner(m_pJak));
	m_pEnemyManager->AddSpawnLocation({ -17,2.5f,-7 });
	m_pEnemyManager->AddSpawnLocation({ -17,2.5f,-13 });
	m_pEnemyManager->SpawnEnemies();
	#pragma endregion

	#pragma region Level
	// SKYBOX
	AddChild(new SkyboxPrefab(L"Textures/Skybox.dds"));

	//Simple Level
	// This isn't the best way to add the level, but the submeshes weren't working properly with my materials.
	const auto pLevelObject = AddChild(new GameObject());
	auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Terrain/Terrain.ovm"));
	auto material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/terrain_diffuse.jpeg");
	pLevelMesh->SetMaterial(material, 0);

	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/palm_p1-Diffuse.png");
	pLevelMesh->SetMaterial(material, 1);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/iskele_albedo.jpeg");
	pLevelMesh->SetMaterial(material, 2);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/objeler_albedo.jpeg");
	pLevelMesh->SetMaterial(material, 3);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/rock3-Diffuse.png");
	pLevelMesh->SetMaterial(material, 4);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/rock1-Diffuse.png");
	pLevelMesh->SetMaterial(material, 5);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/rock2-Diffuse.png");
	pLevelMesh->SetMaterial(material, 6);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/rock4-Diffuse.png");
	pLevelMesh->SetMaterial(material, 7);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/platform-A-Diffuse.jpeg");
	pLevelMesh->SetMaterial(material, 8);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/platform-stairs-Diffuse.jpeg");
	pLevelMesh->SetMaterial(material, 9);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/platform-pl-Diffuse.jpeg");
	pLevelMesh->SetMaterial(material, 10);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/sea_sea_BaseColor.png");
	pLevelMesh->SetMaterial(material, 11);


	const float levelSize = 40.f;
	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Terrain/Terrain.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ levelSize,levelSize,levelSize })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale(levelSize, levelSize, levelSize);
	pLevelObject->GetTransform()->Translate(0, -10, 0);
	pLevelObject->GetTransform()->Rotate(90, 0, 0);

	// Campfire
	const auto pCampfireObj = AddChild(new GameObject());
	auto pCampfireMesh = pCampfireObj->AddComponent(new ModelComponent(L"Meshes/Terrain/Campfire.ovm"));
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/Campfire.png");
	pCampfireMesh->SetMaterial(material, 0);
	material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Deferred>();
	material->SetDiffuseTexture(L"Textures/Terrain/Log.png");
	pCampfireMesh->SetMaterial(material, 1);

	const auto pCampfireActor = pCampfireObj->AddComponent(new RigidBodyComponent(true));
	const auto pPxCampfireMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Terrain/Campfire.ovpt");
	pCampfireActor->AddCollider(PxTriangleMeshGeometry(pPxCampfireMesh, PxMeshScale({ 1,1,1 })), *pDefaultMaterial);
	pCampfireObj->GetTransform()->Scale(1, 1, 1);
	pCampfireObj->GetTransform()->Translate(0, 0, 0);

	//Particle System
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,3.f,0.f };
	settings.minSize = 1.f;
	settings.maxSize = 2.f;
	settings.minEnergy = 0.3f;
	settings.maxEnergy = 1.f;
	settings.minScale = .5f;
	settings.maxScale = 1.5f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f,1.f,1.f, .3f };

	const float campfireSize{ 3 };
	XMFLOAT3 campfirePos{ 13,-10,25 };
	const auto pParticles = AddChild(new GameObject());
	pParticles->AddComponent(new ParticleEmitterComponent(L"Textures/Particles/Fire.png", settings, 200));
	pParticles->GetTransform()->Translate(campfirePos.x, campfirePos.y + 1, campfirePos.z);

	pCampfireObj->GetTransform()->Translate(campfirePos);
	pCampfireObj->GetTransform()->Scale(campfireSize, campfireSize, campfireSize);
	#pragma endregion

	//Input
	#pragma region Input
	auto inputAction = InputAction(JakMoveLeft, InputState::down, 'A', -1, XINPUT_GAMEPAD_DPAD_LEFT);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(JakMoveRight, InputState::down, 'D', -1, XINPUT_GAMEPAD_DPAD_RIGHT);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(JakMoveForward, InputState::down, 'W', -1, XINPUT_GAMEPAD_DPAD_UP);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(JakMoveBackward, InputState::down, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(JakJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(JakAttack, InputState::pressed, -1, 1, XINPUT_GAMEPAD_B);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(ToggleOptions, InputState::pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_START);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(ShowControls, InputState::pressed, 'I', -1, XINPUT_GAMEPAD_BACK);
	m_SceneContext.pInput->AddInputAction(inputAction);
	#pragma endregion

	// POST PROCESSING
	#pragma region PostProcessing
	m_pPostBrightness = MaterialManager::Get()->CreateMaterial<PostBrightness>();
	AddPostProcessingEffect(m_pPostBrightness);
	#pragma endregion

	// UI
	#pragma region UI
	m_pButtonManager = AddChild(new UI_ButtonManager(m_SceneContext));
	const float referenceWidth{ 1920 };
	const float referenceHeight{ 1080 };

	// Controller Layout
	#pragma region ControllerLayoutImage
	const float controllerScale{ 0.75f };
	m_pControllerLayout = AddChild(new GameObject());
	m_pControllerLayout->GetTransform()->Translate(m_SceneContext.windowWidth, m_SceneContext.windowHeight / 8.f, .6f);
	auto image = m_pControllerLayout->AddChild(new GameObject());
	image->AddComponent(new SpriteComponent(L"UI/ControllerLayout.png", XMFLOAT2{ 1.0f,0.0f }));
	image->GetTransform()->Translate(0, 0, 0.1f);
	image->GetTransform()->Scale((controllerScale* m_SceneContext.windowWidth / referenceWidth), controllerScale * (m_SceneContext.windowHeight / referenceHeight), 1.f);
	#pragma endregion

	// OPTIONS MENU
	#pragma region OptionsMenu
	m_pOptionsMenu = AddChild(new GameObject());
	m_pOptionsMenu->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .7f);

	auto background = m_pOptionsMenu->AddChild(new GameObject());
	
	background->AddComponent(new SpriteComponent(L"UI/Options_BG.png", XMFLOAT2{ 0.5f,0.5f }));
	background->GetTransform()->Translate(0, 0, 0.1f);
	background->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.f);

	// SLIDERS
	// Audio Volume
	m_pOptionsVolumeSlider = m_pOptionsMenu->AddChild(new UI_Slider(0.0f, 1.f, L"UI/Slider_Fill.png", L"UI/Slider_BG.png", { 0,0.5f }));
	m_pOptionsVolumeSlider->GetTransform()->Translate(-50, -30, 0);
	m_pOptionsVolumeSlider->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.0f);
	m_pOptionsVolumeSlider->SetValue(0.5f);

	// Brightness
	m_pOptionsBrightnessSlider = m_pOptionsMenu->AddChild(new UI_Slider(0.5f, 2.f, L"UI/Slider_Fill.png", L"UI/Slider_BG.png", { 0,0.5f }));
	m_pOptionsBrightnessSlider->GetTransform()->Translate(-50, -78, 0);
	m_pOptionsBrightnessSlider->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.0f);
	m_pOptionsBrightnessSlider->SetValue(1.0f);

	// BUTTONS
	const float scaleMultiplier{ 0.75f };
	// Restart Button
	m_pOptionsRestartButton = m_pOptionsMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/Restart_Default.png", L"UI/Restart_Highlighted.png", L"UI/Restart_Pressed.png"
		, { 0.5f,0.5f }));
	m_pOptionsRestartButton->GetTransform()->Translate(0, 50, 0);
	m_pOptionsRestartButton->GetTransform()->Scale(scaleMultiplier * (m_SceneContext.windowHeight / referenceHeight), scaleMultiplier* (m_SceneContext.windowHeight / referenceHeight), 1.f);
	m_pButtonManager->AddButton(m_pOptionsRestartButton);

	// Return To Main Button
	m_pOptionsReturnToMainButton = m_pOptionsMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/ReturnMenu_Default.png", L"UI/ReturnMenu_Highlighted.png", L"UI/ReturnMenu_Pressed.png"
		, { 0.5f,0.5f }));
	m_pOptionsReturnToMainButton->GetTransform()->Translate(0, 125, 0);
	m_pOptionsReturnToMainButton->GetTransform()->Scale(scaleMultiplier* (m_SceneContext.windowHeight / referenceHeight), scaleMultiplier* (m_SceneContext.windowHeight / referenceHeight), 1.f);
	m_pButtonManager->AddButton(m_pOptionsReturnToMainButton);

	// Return Button
	m_pOptionsReturnButton = m_pOptionsMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/Return_Default.png", L"UI/Return_Highlighted.png", L"UI/Return_Pressed.png"
		, { 0.5f,0.5f }));
	m_pOptionsReturnButton->GetTransform()->Translate(0, 200, 0);
	m_pOptionsReturnButton->GetTransform()->Scale(scaleMultiplier* (m_SceneContext.windowHeight / referenceHeight), scaleMultiplier* (m_SceneContext.windowHeight / referenceHeight), 1.f);
	m_pButtonManager->AddButton(m_pOptionsReturnButton);

	// Button Links
	m_pOptionsRestartButton->SetButtonDown(m_pOptionsReturnToMainButton);
	m_pOptionsReturnToMainButton->SetButtonUp(m_pOptionsRestartButton);
	m_pOptionsReturnToMainButton->SetButtonDown(m_pOptionsReturnButton);
	m_pOptionsReturnButton->SetButtonUp(m_pOptionsReturnToMainButton);
	#pragma endregion

	// GAMEOVERMENU
	#pragma region GameOverMenu
	m_pGameOverMenu = AddChild(new GameObject());
	m_pGameOverMenu->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .7f);

	background = m_pGameOverMenu->AddChild(new GameObject());
	background->AddComponent(new SpriteComponent(L"UI/GameOver_BG.png", XMFLOAT2{ 0.5f,0.5f }));
	background->GetTransform()->Translate(0, 0, 0.1f);
	background->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.f);

	// Restart Button
	m_pGameOverRestartButton = m_pGameOverMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/Restart_Default.png", L"UI/Restart_Highlighted.png", L"UI/Restart_Pressed.png"
		, { 0.5f,0.5f }));
	m_pGameOverRestartButton->GetTransform()->Translate(0, 50, 0);
	m_pGameOverRestartButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pGameOverRestartButton);

	// Return To Main Button
	m_pGameOverReturnToMainButton = m_pGameOverMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/ReturnMenu_Default.png", L"UI/ReturnMenu_Highlighted.png", L"UI/ReturnMenu_Pressed.png"
		, { 0.5f,0.5f }));
	m_pGameOverReturnToMainButton->GetTransform()->Translate(0, 150, 0);
	m_pGameOverReturnToMainButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pGameOverReturnToMainButton);

	// Button Links
	m_pGameOverRestartButton->SetButtonDown(m_pGameOverReturnToMainButton);
	m_pGameOverReturnToMainButton->SetButtonUp(m_pGameOverRestartButton);
	#pragma endregion

	#pragma region GameWonMenu
	m_pGameWonMenu = AddChild(new GameObject());
	m_pGameWonMenu->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .7f);

	background = m_pGameWonMenu->AddChild(new GameObject());
	background->AddComponent(new SpriteComponent(L"UI/GameWon_BG.png", XMFLOAT2{ 0.5f,0.5f }));
	background->GetTransform()->Translate(0, 0, 0.1f);
	background->GetTransform()->Scale(m_SceneContext.windowWidth / referenceWidth, m_SceneContext.windowHeight / referenceHeight, 1.f);

	// Restart Button
	m_pGameWonRestartButton = m_pGameWonMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/Restart_Default.png", L"UI/Restart_Highlighted.png", L"UI/Restart_Pressed.png"
		, { 0.5f,0.5f }));
	m_pGameWonRestartButton->GetTransform()->Translate(0, 50, 0);
	m_pGameWonRestartButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pGameWonRestartButton);

	// Return To Main Button
	m_pGameWonReturnToMainButton = m_pGameWonMenu->AddChild(new UI_Button(m_pButtonManager, L"UI/ReturnMenu_Default.png", L"UI/ReturnMenu_Highlighted.png", L"UI/ReturnMenu_Pressed.png"
		, { 0.5f,0.5f }));
	m_pGameWonReturnToMainButton->GetTransform()->Translate(0, 150, 0);
	m_pGameWonReturnToMainButton->GetTransform()->Scale(m_SceneContext.windowHeight / referenceHeight, m_SceneContext.windowHeight / referenceHeight, 1.f);
	m_pButtonManager->AddButton(m_pGameWonReturnToMainButton);

	// Button Links
	m_pGameWonRestartButton->SetButtonDown(m_pGameWonReturnToMainButton);
	m_pGameWonReturnToMainButton->SetButtonUp(m_pGameWonRestartButton);
	#pragma endregion
	#pragma endregion

	#pragma region HUD
	m_pLivesManager = AddChild(new HUD_LivesManager(m_pJak->GetMaxHealth(), L"HUD/Life_Empty.png", L"HUD/Life_Filled.png", { 0,0 }));
	m_pLivesManager->GetTransform()->Translate(30, 30, 0);
	#pragma endregion

	m_pOptionsMenu->SetActive(false);
	m_pGameOverMenu->SetActive(false);
	m_pGameWonMenu->SetActive(false);
	m_pControllerLayout->SetActive(false);

	// SOUND
	auto pFModSystem = SoundManager::Get()->GetSystem();

	pFModSystem->createChannelGroup("MainChannel", &m_pChannelGroup);

	pFModSystem->createStream("Resources/Sounds/GameLoop/GameOver.wav", FMOD_2D, nullptr, &m_pGameOverSound);
	pFModSystem->createStream("Resources/Sounds/GameLoop/GameWon.mp3", FMOD_2D, nullptr, &m_pGameWonSound);

	// Lighting
	//Directional
	auto& dirLight = m_SceneContext.pLights->GetDirectionalLight();
	dirLight.intensity = 0.6f;
	dirLight.isEnabled = true;
	dirLight.direction = { -0.577f, -0.577f, 0.577f , 1.0f };

	//Point Light
	Light light = {};
	light.isEnabled = true;
	light.position = { campfirePos.x, campfirePos.y + 1, campfirePos.z, 1.0f };
	light.color = { 1.0f,.6f,0.f,1.f };
	light.intensity = 1.0f;
	light.range = 10.0f;
	light.type = LightType::Point;
	m_SceneContext.pLights->AddLight(light);

}

void JakAndDaxterScene::OnGUI()
{
	m_pJak->DrawImGui();
}

void JakAndDaxterScene::Update()
{
	// Camera Pos
	//GLOBAL LISTENER SETTINGS (3D Sound)
	auto camera = m_pJak->GetCameraTransform();
	auto forward = ToFModVector(camera->GetForward());
	auto up = ToFModVector(camera->GetUp());
	auto pos = ToFModVector(camera->GetWorldPosition());
	FMOD_VECTOR velocity{ };
	auto deltaTime = m_SceneContext.pGameTime->GetElapsed();
	velocity.x = (pos.x - m_PrevCameraPos.x) / deltaTime;
	velocity.y = (pos.y - m_PrevCameraPos.y) / deltaTime;
	velocity.z = (pos.z - m_PrevCameraPos.z) / deltaTime;

	m_PrevCameraPos = pos;
	SoundManager::Get()->GetSystem()->set3DListenerAttributes(0, &pos, &velocity, &forward, &up);


	// INPUT
	if (m_SceneContext.pInput->IsActionTriggered(ToggleOptions))
	{
		bool isAlreadyShowing = m_pOptionsMenu->IsActive();

		m_pOptionsMenu->SetActive(!isAlreadyShowing);
		if (!isAlreadyShowing) 
		{
			m_pOptionsReturnButton->Select(true);
			GameManager::SetGameState(GameManager::GameState::Paused);
		}
		else
		{
			m_pButtonManager->SelectButton(nullptr);
			GameManager::SetGameState(GameManager::GameState::Playing);
		} 
	}
	if (m_SceneContext.pInput->IsActionTriggered(ShowControls))
	{
		m_pControllerLayout->SetActive(true);
		m_IsShowingController = true;
	}

	#pragma region UI
	// OPTIONSMENU
	#pragma region Options
	// BUTTONS
	if (m_pOptionsReturnToMainButton->IsPressed(m_SceneContext))
	{
		GoToMainMenu();
	}
	else if (m_pOptionsReturnButton->IsPressed(m_SceneContext))
	{
		m_pOptionsMenu->SetActive(false);
		m_pButtonManager->SelectButton(nullptr);
		GameManager::SetGameState(GameManager::GameState::Playing);
	}
	else if (m_pOptionsRestartButton->IsPressed(m_SceneContext))
	{
		RestartGame();
	}
	
	// SLIDERS
	if (m_pOptionsBrightnessSlider->isDirty())
	{
		m_pPostBrightness->SetBrightness(m_pOptionsBrightnessSlider->GetValue());
		GameManager::Get()->SetBrightness(m_pOptionsBrightnessSlider->GetValue());
		m_pOptionsBrightnessSlider->SetDirty(false);
	}
	if (m_pOptionsVolumeSlider->isDirty())
	{
		GameManager::Get()->SetVolume(m_pOptionsVolumeSlider->GetValue());
		m_pOptionsVolumeSlider->SetDirty(false);
		SetVolume(m_pOptionsVolumeSlider->GetValue());
	}
	#pragma endregion

	// GAMEOVERMENU
	#pragma region GameOver
	// GAMEOVERSTATE
	if (!m_pGameOverMenu->IsActive() && GameManager::GetGameState() == GameManager::GameState::GameOver)
	{
		m_pGameOverMenu->SetActive(true);
		m_pGameOverRestartButton->Select(true);

		SoundManager::Get()->GetSystem()->playSound(m_pGameOverSound, m_pChannelGroup, false, nullptr);
	}

	// BUTTONS
	if (m_pGameOverRestartButton->IsPressed(m_SceneContext))
	{
		RestartGame();
	}
	else if (m_pGameOverReturnToMainButton->IsPressed(m_SceneContext))
	{
		GoToMainMenu();
	}
	#pragma endregion

	// GAMEWONMENU
	#pragma region GameWon
	// Winning State
	if (!m_pGameWonMenu->IsActive() && m_pEnemyManager->AllEnemiesAreDead())
	{
		m_pGameWonMenu->SetActive(true);
		m_pGameWonRestartButton->Select(true);
		GameManager::SetGameState(GameManager::GameState::GameWon);

		SoundManager::Get()->GetSystem()->playSound(m_pGameWonSound, m_pChannelGroup, false, nullptr);
	}

	// Buttons
	if (m_pGameWonRestartButton->IsPressed(m_SceneContext))
	{
		RestartGame();
	}
	else if (m_pGameWonReturnToMainButton->IsPressed(m_SceneContext))
	{
		GoToMainMenu();

	}
	#pragma endregion

	// CONTROLLERLAYOUT
	if (m_IsShowingController)
	{
		m_AccControllerTime += m_SceneContext.pGameTime->GetElapsed();
		if (m_AccControllerTime >= m_ControllerTime)
		{
			m_AccControllerTime = 0.0f;
			m_IsShowingController = false;
			m_pControllerLayout->SetActive(false);
		}
	}
	#pragma endregion

	#pragma region HUD
	if (GameManager::GetGameState() != GameManager::GameState::GameOver)
		m_pLivesManager->SetHealth(m_pJak->GetHealth());
	else
		m_pLivesManager->SetHealth(0);
	#pragma endregion

	// Update CampfireLight
	m_SceneContext.pLights->GetLight(0).intensity = MathHelper::randF(0.5f, 1.2f);
}

void JakAndDaxterScene::PostDraw()
{
}

void JakAndDaxterScene::SetBrightness(float value)
{
	m_pPostBrightness->SetBrightness(value);
	m_pOptionsBrightnessSlider->SetValue(value);
}

void JakAndDaxterScene::SetVolume(float value)
{
	m_pOptionsVolumeSlider->SetValue(value);
	m_pJak->SetVolume(value);
	m_pEnemyManager->SetVolume(value);
	m_pChannelGroup->setVolume(value);

	// BGMusic is divided by 3
	m_pMusicChannel->setVolume(value * 0.3f);
}

void JakAndDaxterScene::GoToMainMenu()
{
	auto menuScene = reinterpret_cast<MainMenu*>(SceneManager::Get()->GetScene(L"MainMenu"));
	menuScene->UpdateSettings();

	m_pMusicChannel->stop();
	m_pJak->StopAllSounds();
	m_pEnemyManager->StopAllSounds();

	GameManager::SetGameState(GameManager::GameState::MainMenu);
	SceneManager::Get()->PreviousScene();
}

void JakAndDaxterScene::RestartGame()
{
	Reset();
	GameManager::SetGameState(GameManager::GameState::Playing);
}

void JakAndDaxterScene::Reset()
{
	// Reset Jak Position & Stats
	m_pJak->Reset();
	m_pJak->GetTransform()->Translate(m_SpawnPos);
	m_pJak->GetTransform()->Rotate(0, -90, 0);

	// Reset Enemies
	m_pEnemyManager->DeleteAllEnemies();
	m_pEnemyManager->SpawnEnemies();

	// UI
	m_pOptionsMenu->SetActive(false);
	m_pGameOverMenu->SetActive(false);
	m_pGameWonMenu->SetActive(false);
	m_pButtonManager->SelectButton(nullptr);

	// Set Volume & Brightness
	SetVolume(GameManager::Get()->GetVolume());
	SetBrightness(GameManager::Get()->GetBrightness());

	// Show Controller Layout
	m_pControllerLayout->SetActive(true);
	m_IsShowingController = true;
	m_AccControllerTime = 0;

	// Music
	m_pMusicChannel->stop();
	auto pFModSystem = SoundManager::Get()->GetSystem();

	FMOD::Sound* pSound{};
	pFModSystem->createStream("Resources/Sounds/Ambient/GameMusic.flac", FMOD_2D, nullptr, &pSound);
	pSound->setMode(FMOD_LOOP_NORMAL);
	pFModSystem->playSound(pSound, nullptr, false, &m_pMusicChannel);
}

FMOD_VECTOR JakAndDaxterScene::ToFModVector(XMFLOAT3 v)
{
	return FMOD_VECTOR{ v.x, v.y, v.z };
}