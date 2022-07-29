#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Prefabs/SpherePrefab.h"


ComponentTestScene::ComponentTestScene() :
	GameScene(L"ComponentTestScene") {}

void ComponentTestScene::Initialize()
{
	auto pDefaultMaterial = PxGetPhysics().createMaterial(.5f, .5f, 1.f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Sphere 1 (Group 1)
	auto pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{Colors::Red}));
	auto pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	pSphereObject->GetTransform()->Translate(0, 50, 0);

	pSphereActor->SetCollisionGroup(CollisionGroup::Group1);

	//Sphere 2 (Group 0)
	pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Green }));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	pSphereObject->GetTransform()->Translate(0, 40, 0);

	pSphereActor->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);

	//Sphere 3 (Group 0)
	pSphereObject = AddChild(new SpherePrefab(1.f, 10, XMFLOAT4{ Colors::Blue }));
	pSphereActor = pSphereObject->AddComponent(new RigidBodyComponent());
	pSphereActor->AddCollider(PxSphereGeometry(1.f), *pDefaultMaterial);
	pSphereObject->GetTransform()->Translate(0, 30, 0);


	m_SceneContext.settings.enableOnGUI = true;
}

void ComponentTestScene::Update()
{
	//Optional
}

void ComponentTestScene::Draw()
{
	//Optional
}

void ComponentTestScene::OnGUI()
{
	
}
