#pragma once
class SpikyMaterial;

class SpikyScene final : public GameScene
{
public:
	SpikyScene();
	~SpikyScene() override = default;

	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	GameObject* m_pSphere{};
	SpikyMaterial* m_pMaterial{};

	float m_RotateSpeed{ 30.f };
	float m_CurrAngle{ 0.f };
};


