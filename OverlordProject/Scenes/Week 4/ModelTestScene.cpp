//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "ModelTestScene.h"

#include "Materials/DiffuseMaterial.h"

ModelTestScene::ModelTestScene() :
	GameScene(L"ModelTestScene") {}

void ModelTestScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 0.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	m_pChair = AddChild(new GameObject());

	auto modelComp = m_pChair->AddComponent(new ModelComponent(L"Meshes/Chair.ovm"));
	modelComp->SetMaterial(pMaterial);

	auto convexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");

	auto pSphereActor = m_pChair->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxConvexMeshGeometry{ convexMesh }, *pDefaultMaterial);

	m_pChair->GetTransform()->Translate(0, 1, 0);
}

void ModelTestScene::Update()
{
	//Optional

}

void ModelTestScene::Draw()
{
	//Optional

}

void ModelTestScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
