#include "stdafx.h"
#include "PongScene.h"

#include "Prefabs/SpherePrefab.h"
#include "Prefabs/CubePrefab.h"


PongScene::PongScene() :
	GameScene(L"PhongScene") {}

void PongScene::Initialize()
{
	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 1.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	// Objects
	m_pBall = AddChild(new SpherePrefab(0.5f, 10, XMFLOAT4{ Colors::Red }));
	auto pSphereActor = m_pBall->AddComponent(new RigidBodyComponent());
	pSphereActor->GetPxRigidActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	pSphereActor->AddCollider(PxSphereGeometry(0.5f), *pDefaultMaterial);

	m_pPlayerLeft = AddChild(new CubePrefab(1.f, 4, 1, XMFLOAT4{Colors::White}));
	auto pPlayerLeftActor = m_pPlayerLeft->AddComponent(new RigidBodyComponent());
	pPlayerLeftActor->AddCollider(PxBoxGeometry(0.5f, 2.f,0.5f) , *pDefaultMaterial);
	pPlayerLeftActor->SetKinematic(true);

	m_pPlayerRight = AddChild(new CubePrefab(1.f, 4, 1, XMFLOAT4{ Colors::White }));
	auto pPlayerRightActor = m_pPlayerRight->AddComponent(new RigidBodyComponent());
	pPlayerRightActor->AddCollider(PxBoxGeometry(0.5f, 2.f, 0.5f), *pDefaultMaterial);
	pPlayerRightActor->SetKinematic(true);

	// Walls
	m_pWallTop = AddChild(new CubePrefab(30, 2, 2, XMFLOAT4{ Colors::Transparent }));
	auto pTriggerActor = m_pWallTop->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	m_pWallTop->GetTransform()->Translate(0, 18, 0);
	pTriggerActor->AddCollider(PxBoxGeometry(15,1,1), *pDefaultMaterial);

	m_pWallBottom = AddChild(new CubePrefab(30, 2, 2, XMFLOAT4{ Colors::Transparent }));
	pTriggerActor = m_pWallBottom->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	m_pWallBottom->GetTransform()->Translate(0, 1, 0);
	pTriggerActor->AddCollider(PxBoxGeometry(15, 1, 1), *pDefaultMaterial);

	// Triggers
	m_pTriggerLeft = AddChild(new CubePrefab(2, 30, 2, XMFLOAT4{ Colors::Transparent }));
	pTriggerActor = m_pTriggerLeft->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	m_pTriggerLeft->GetTransform()->Translate(-18, 10, 0);

	UINT colliderId = pTriggerActor->AddCollider(PxBoxGeometry(1, 15, 1), *pDefaultMaterial);
	pTriggerActor->GetCollider(colliderId).SetTrigger(true);
	m_pTriggerLeft->SetOnTriggerCallBack([=](GameObject*, GameObject* pOther, PxTriggerAction action)
		{

			if (pOther == m_pBall)
			{
				if (action == PxTriggerAction::ENTER)
				{
					SetPositions();
				}
			}
		});

	m_pTriggerRight = AddChild(new CubePrefab(2, 30, 2, XMFLOAT4{ Colors::Transparent }));
	pTriggerActor = m_pTriggerRight->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);
	m_pTriggerRight->GetTransform()->Translate(18, 10, 0);

	colliderId = pTriggerActor->AddCollider(PxBoxGeometry(1, 15, 1), *pDefaultMaterial);
	pTriggerActor->GetCollider(colliderId).SetTrigger(true);
	m_pTriggerRight->SetOnTriggerCallBack([=](GameObject*, GameObject* pOther, PxTriggerAction action)
		{
			if (pOther == m_pBall)
			{
				if (action == PxTriggerAction::ENTER)
				{
					SetPositions();
				}
			}
		});

	// Settings
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = XMFLOAT4(0, 0, 0, 1);

	// Camera
	auto pCamera = AddChild(new FixedCamera());
	pCamera->GetTransform()->Rotate(0, 0, 0);
	pCamera->GetTransform()->Translate(0, 10, -20);
	SetActiveCamera(pCamera->GetComponent<CameraComponent>());

	SetPositions();
}

void PongScene::Update()
{
	float deltaTime = m_SceneContext.pGameTime->GetElapsed();
	constexpr float movementSpeed = 6.0f;

	if (!m_HasStarted)
	{
		m_pBall->GetTransform()->Translate(0, 10, 0);
	}

	if (m_SceneContext.pInput->IsKeyboardKey(InputState::pressed, VK_SPACE))
	{
		if (!m_HasStarted) StartGame();
	}
	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'R'))
	{
		SetPositions();
	}

	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_UP))
	{
		const XMFLOAT3 currPos = m_pPlayerRight->GetTransform()->GetPosition();
		const XMFLOAT3 newPos = XMFLOAT3(currPos.x, currPos.y + movementSpeed * deltaTime, currPos.z);

		m_pPlayerRight->GetTransform()->Translate(newPos);
	}
	else if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, VK_DOWN))
	{
		const XMFLOAT3 currPos = m_pPlayerRight->GetTransform()->GetPosition();
		const XMFLOAT3 newPos = XMFLOAT3(currPos.x, currPos.y - movementSpeed * deltaTime, currPos.z);

		m_pPlayerRight->GetTransform()->Translate(newPos);
	}

	if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'W'))
	{
		const XMFLOAT3 currPos = m_pPlayerLeft->GetTransform()->GetPosition();
		const XMFLOAT3 newPos = XMFLOAT3(currPos.x, currPos.y + movementSpeed * deltaTime, currPos.z);

		m_pPlayerLeft->GetTransform()->Translate(newPos);
	}
	else if (m_SceneContext.pInput->IsKeyboardKey(InputState::down, 'S'))
	{
		const XMFLOAT3 currPos = m_pPlayerLeft->GetTransform()->GetPosition();
		const XMFLOAT3 newPos = XMFLOAT3(currPos.x, currPos.y - movementSpeed * deltaTime, currPos.z);

		m_pPlayerLeft->GetTransform()->Translate(newPos);
	}
}

void PongScene::Draw()
{
	//Optional
}

void PongScene::OnGUI()
{
	
}

void PongScene::StartGame()
{
	m_HasStarted = true;

	constexpr XMFLOAT3 force{ 10,10,0 };
	m_pBall->GetComponent<RigidBodyComponent>()->AddForce(force, PxForceMode::eIMPULSE);
}

void PongScene::SetPositions()
{
	m_HasStarted = false;
	m_pBall->GetTransform()->Translate(0, 10, 0);
	m_pBall->GetTransform()->Rotate(0, 0, 0);

	m_pPlayerLeft->GetTransform()->Translate(-10, 10, 0);
	m_pPlayerRight->GetTransform()->Translate(10, 10, 0);
}