#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1() :
	GameScene(L"SoftwareSkinningScene_1") {}

SoftwareSkinningScene_1::~SoftwareSkinningScene_1()
{
	delete m_pBoneRot0;
	delete m_pBoneRot1;

}

void SoftwareSkinningScene_1::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	// Create ColorMaterial
	ColorMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();

	// Create root GameObject
	GameObject* pRoot = new GameObject();

	// Initialize First Bone
	m_pBone0 = new BoneObject(pMaterial, 15.0f);
	pRoot->AddChild(m_pBone0);

	// Initialize Second Bone
	m_pBone1 = new BoneObject(pMaterial, 15.0f);
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);

}

void SoftwareSkinningScene_1::Update()
{
	if (m_AutoRotate)
	{
		const float maxAngle{ 45.f };
		m_BoneRotation += m_RotationSign * maxAngle * m_SceneContext.pGameTime->GetElapsed();
		if (m_RotationSign < 0 && m_BoneRotation <= -maxAngle)
			m_RotationSign = 1;
		else if (m_RotationSign > 0 && m_BoneRotation >= maxAngle)
			m_RotationSign = -1;

		m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(0, 0, -m_BoneRotation * 2);
	}
}

void SoftwareSkinningScene_1::Draw()
{

}

void SoftwareSkinningScene_1::OnGUI()
{
	if (ImGui::DragFloat3("Bone 0 - ROT", m_pBoneRot0))
	{
		m_pBone0->GetTransform()->Rotate(m_pBoneRot0[0], m_pBoneRot0[1], m_pBoneRot0[2]);
	}
	if (ImGui::DragFloat3("Bone 1 - ROT", m_pBoneRot1))
	{
		m_pBone1->GetTransform()->Rotate(m_pBoneRot1[0], m_pBoneRot1[1], m_pBoneRot1[2]);
	}
	ImGui::Checkbox("Auto Rotate", &m_AutoRotate);

}
