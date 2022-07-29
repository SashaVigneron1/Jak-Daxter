#pragma once
class PongScene final : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

	void StartGame();
	void SetPositions();

private:
	bool m_HasStarted{ false };
	GameObject* m_pBall{};
	GameObject* m_pPlayerLeft{};
	GameObject* m_pPlayerRight{};

	GameObject* m_pTriggerLeft{};
	GameObject* m_pTriggerRight{};
	GameObject* m_pWallTop{};
	GameObject* m_pWallBottom{};

};

