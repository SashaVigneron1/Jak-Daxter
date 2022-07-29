#pragma once
class BoneObject;

class SoftwareSkinningScene_3 final : public GameScene
{
public:
	SoftwareSkinningScene_3();
	~SoftwareSkinningScene_3() override;

	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	#pragma region Part1

	BoneObject* m_pBone0{}, * m_pBone1{};

	float m_BoneRotation{};
	int m_RotationSign{ 1 };

	float* m_pBoneRot0{ new float[3]{} };
	float* m_pBoneRot1{ new float[3]{} };

	bool m_AutoRotate{ false };

	#pragma endregion

	#pragma region Part2&3

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float weight0, float weight1)
			: transformedVertex{ position, normal, color }
			, originalVertex{ position, normal, color }
			, blendWeight0{ weight0 }
			, blendWeight1{ weight1 }
		{

		}
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float weight0)
			: transformedVertex{ position, normal, color }
			, originalVertex{ position, normal, color }
			, blendWeight0{ weight0 }
			, blendWeight1{ 1 - weight0 }
		{

		}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
		float blendWeight0{};
		float blendWeight1{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};

	#pragma endregion
};


