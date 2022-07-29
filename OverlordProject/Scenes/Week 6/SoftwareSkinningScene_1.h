#pragma once

class BoneObject;

class SoftwareSkinningScene_1 final : public GameScene
{
public:
	SoftwareSkinningScene_1();
	~SoftwareSkinningScene_1() override;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};

	float m_BoneRotation{};
	int m_RotationSign{ 1 };

	float* m_pBoneRot0{ new float[3]{} };
	float* m_pBoneRot1{ new float[3]{} };

	bool m_AutoRotate{ false };
};


