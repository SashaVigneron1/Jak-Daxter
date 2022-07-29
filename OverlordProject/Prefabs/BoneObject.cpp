#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: m_Length{length}
    , m_pMaterial{pMaterial}
{
	

}

void BoneObject::AddBone(BoneObject* pBone)
{
	// Translate pBone along the x axis so it's located next tho this bone
	pBone->GetTransform()->Translate(XMFLOAT3(m_Length, 0, 0));
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	auto worldMatrix = XMLoadFloat4x4(&GetTransform()->GetWorld());
	auto worldInverse = XMMatrixInverse(nullptr, worldMatrix);
	XMStoreFloat4x4(&m_BindPose, worldInverse);

	auto children = GetChildren<BoneObject>();
	for (auto c : children)
	{
		c->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext& /*sceneContext*/)
{
	// Create a gameobject
	auto pEmpty = new GameObject();

	// Add pEmpty as a child to the boneObject
	AddChild(pEmpty);

	// Add a modelcomponent to pEmpty(pModel), use Bone.ovm
	auto pModel = new ModelComponent(L"Meshes/Bone.ovm");

	// Assign the BoneObject's material (m_pMaterial) to pModel
	pModel->SetMaterial(m_pMaterial);
	pEmpty->AddComponent(pModel);

	// Rotate pEmpty -90 degrees around the Y axis
	pEmpty->GetTransform()->Rotate(0, -90.0f, 0, true);	

	// Uniformly scale pEmpty to match the boneobjects length
	pEmpty->GetTransform()->Scale(m_Length);
}

