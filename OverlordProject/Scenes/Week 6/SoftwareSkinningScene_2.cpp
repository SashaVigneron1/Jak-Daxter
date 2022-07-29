#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

SoftwareSkinningScene_2::SoftwareSkinningScene_2() :
	GameScene(L"SoftwareSkinningScene_2") {}

SoftwareSkinningScene_2::~SoftwareSkinningScene_2()
{
	delete m_pBoneRot0;
	delete m_pBoneRot1;
}

void SoftwareSkinningScene_2::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	#pragma region Part1
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
	#pragma endregion

	#pragma region Part2
	m_pBone0->CalculateBindPose();

	auto pBoxDrawer = new GameObject();
	m_pMeshDrawer = new MeshDrawComponent(24, true);
	pBoxDrawer->AddComponent(m_pMeshDrawer);
	AddChild(pBoxDrawer);

	InitializeVertices(15.0f);
#pragma endregion
}

void SoftwareSkinningScene_2::Update()
{
	#pragma region Part1
	if (m_AutoRotate)
	{
		const float maxAngle{ 45.f };
		m_BoneRotation += m_RotationSign * maxAngle * m_SceneContext.pGameTime->GetElapsed();
		if (m_RotationSign < 0 && m_BoneRotation <= -maxAngle)
			m_RotationSign = 1;
		if (m_RotationSign > 0 && m_BoneRotation >= maxAngle)
			m_RotationSign = -1;

		m_pBone0->GetTransform()->Rotate(0, 0, m_BoneRotation);
		m_pBone1->GetTransform()->Rotate(0, 0, -m_BoneRotation * 2);

	}
	#pragma endregion

	// Remove Current Triangles
	m_pMeshDrawer->RemoveTriangles();

	// Transform skinned vertices to bones
	auto boneBindPose = XMLoadFloat4x4(&m_pBone0->GetBindPose());
	auto boneWorld = XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld());
	auto boneTransform = XMMatrixMultiply(boneBindPose, boneWorld);

	auto boneBindPose1 = XMLoadFloat4x4(&m_pBone1->GetBindPose());
	auto boneWorld1 = XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld());
	auto boneTransform1 = XMMatrixMultiply(boneBindPose1, boneWorld1);

	for (unsigned int i = 0; i < m_SkinnedVertices.size(); ++i)
	{
		if (i < 24)
		{
			XMVECTOR originalVert = XMLoadFloat3(&m_SkinnedVertices[i].originalVertex.Position);
			XMStoreFloat3(&m_SkinnedVertices[i].transformedVertex.Position, XMVector3TransformCoord(originalVert, boneTransform));
		}
		else
		{
			XMVECTOR originalVert = XMLoadFloat3(&m_SkinnedVertices[i].originalVertex.Position);
			XMStoreFloat3(&m_SkinnedVertices[i].transformedVertex.Position, XMVector3TransformCoord(originalVert, boneTransform1));
		}
	}

	// Iterate vector & append
	for (unsigned int i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		auto quad = QuadPosNormCol(m_SkinnedVertices[i].transformedVertex,
			m_SkinnedVertices[i + 1].transformedVertex,
			m_SkinnedVertices[i + 2].transformedVertex,
			m_SkinnedVertices[i + 3].transformedVertex);

		m_pMeshDrawer->AddQuad(quad);
	}

	// Update Buffer
	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::Draw()
{
}

void SoftwareSkinningScene_2::OnGUI()
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

void SoftwareSkinningScene_2::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion
}
