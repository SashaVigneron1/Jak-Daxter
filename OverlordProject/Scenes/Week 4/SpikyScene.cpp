//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "SpikyScene.h"

#include "Materials/SpikyMaterial.h"

SpikyScene::SpikyScene() :
	GameScene(L"SpikyScene") {}

void SpikyScene::Initialize()
{
	m_SceneContext.settings.showInfoOverlay = true;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	// Material
	m_pMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();

	// Object
	m_pSphere = AddChild(new GameObject());
	m_pSphere->GetTransform()->Scale(20, 20, 20);

	auto modelComp = m_pSphere->AddComponent(new ModelComponent(L"Meshes/OctaSphere.ovm"));
	modelComp->SetMaterial(m_pMaterial);
}

void SpikyScene::Update()
{
	m_CurrAngle += m_RotateSpeed * m_SceneContext.pGameTime->GetElapsed();
	m_pSphere->GetTransform()->Rotate(0, m_CurrAngle, 0, true);
}

void SpikyScene::Draw()
{
}

void SpikyScene::OnGUI()
{
	m_pMaterial->OnGui();
}
