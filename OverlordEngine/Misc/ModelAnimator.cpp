#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = std::max(std::min(passedTicks, m_CurrentClip.duration), 0.0f);

		//2.
		if (m_Reversed)
		{
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;
			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if (m_TickCount < 0)
			{
				m_TickCount += m_CurrentClip.duration;
				if (!m_IsLooping) m_IsPlaying = false;
			}
		}
		else
		{
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;
			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if (m_TickCount > m_CurrentClip.duration)
			{
				if (!m_IsLooping)
				{
					m_IsPlaying = false;
					m_TickCount = m_CurrentClip.duration - 1;
				}
				else
				{
					m_TickCount -= m_CurrentClip.duration;
				}
			}
		}


		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for (size_t i = 0; i < m_CurrentClip.keys.size(); ++i)
		{
			if (m_CurrentClip.keys[i].tick > m_TickCount)
			{
				keyA = m_CurrentClip.keys[i - 1];
				keyB = m_CurrentClip.keys[i];
				break;
			}
		}


		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendFactor = (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (auto i = 0; i < m_pMeshFilter->m_BoneCount; ++i)
		{
			//	Retrieve the transform from keyA (transformA)
			auto transformA = keyA.boneTransforms[i];
			XMMATRIX transformAMat = XMLoadFloat4x4(&transformA);
			// 	Retrieve the transform from keyB (transformB)
			auto transformB = keyB.boneTransforms[i];
			XMMATRIX transformBMat = XMLoadFloat4x4(&transformB);

			//	Decompose both transforms
			XMVECTOR scaleA, rotA, transA;
			XMMatrixDecompose(&scaleA, &rotA, &transA, transformAMat);
			XMVECTOR scaleB, rotB, transB;
			XMMatrixDecompose(&scaleB, &rotB, &transB, transformBMat);

			//	Lerp between all the transformations (Position, Scale, Rotation)
			auto trans = XMVectorLerp(transA, transB, blendFactor);
			auto scale = XMVectorLerp(scaleA, scaleB, blendFactor);
			auto rot = XMQuaternionSlerp(rotA, rotB, blendFactor);

			//	Compose a transformation matrix with the lerp-results
			XMMATRIX transMat = XMMatrixTranslationFromVector(trans);
			XMMATRIX scaleMat = XMMatrixScalingFromVector(scale);
			XMMATRIX rotMat = XMMatrixRotationQuaternion(rot);

			//	Add the resulting matrix to the m_Transforms vector
			auto mat = scaleMat * rotMat * transMat;
			XMFLOAT4X4 finalmat;
			XMStoreFloat4x4(&finalmat, mat);
			m_Transforms.push_back(finalmat);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;

	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	bool succes = false;
	AnimationClip animationClip;
	for (int i{}; i < m_pMeshFilter->m_AnimationClips.size(); ++i)
	{
		auto clip = m_pMeshFilter->m_AnimationClips[i];
		if (clip.name == clipName)
		{
			animationClip = clip;
			succes = true;
			m_CurrentClipId = i;
			break;
		}
	}

	if (succes)
	{
		//	Call SetAnimation(Animation Clip) with the found clip
		SetAnimation(animationClip);
	}
	else
	{
		//	Call Reset
		Reset();
		//	Log a warning with an appropriate message
		Logger::LogWarning(L"ModelAnimator -> Couldn't find animation with name: {}", clipName);
	}

}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		auto clip = m_pMeshFilter->m_AnimationClips[clipNumber];
		//	Call SetAnimation(AnimationClip clip)
		m_CurrentClipId = clipNumber;
		SetAnimation(clip, clipNumber);
	}
	else
	{
		//	Call Reset
		Reset();
		//	Log a warning with an appropriate message
		Logger::LogWarning(L"ModelAnimator -> Couldn't find animation with id: {}, the max id allowed is {}", clipNumber, m_pMeshFilter->m_AnimationClips.size());
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip, int clipId)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	m_CurrentClipId = clipId;

	if (clipId == -1)
	{
		for (int i{}; i < m_pMeshFilter->m_AnimationClips.size(); ++i)
		{
			auto animClip = m_pMeshFilter->m_AnimationClips[i];
			if (animClip.name == m_CurrentClip.name)
			{
				m_CurrentClipId = i;
				break;
			}
		}
	}
	

	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause) m_IsPlaying = false;

	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;

	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(m_CurrentClip.keys[0].boneTransforms.begin(), m_CurrentClip.keys[0].boneTransforms.end());
	}
	else
	{
		//	Create an IdentityMatrix
		auto mat = XMMatrixIdentity();
		XMFLOAT4X4 xmat;
		XMStoreFloat4x4(&xmat, mat);

		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, xmat);
	}

}
