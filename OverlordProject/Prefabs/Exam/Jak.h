#pragma once


class Enemy;

struct JakDesc
{
	JakDesc(
		PxMaterial* pMaterial,
		float radius = .5f,
		float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
	}

	// MOVEMENT
	float maxMoveSpeed{ 15.f };
	float maxFallSpeed{ 15.f };

	float JumpSpeed{ 22.f };
	float DoubleJumpSpeed{ 15.f };
	float minTimeBeforeDoubleJump{ 0.3f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };

	float maxPitch{ 70.0f };
	float minPitch{ -30.0f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 60.f };

	// COMBAT
	float combatCooldown{ 1.5f };
	float combatHitTime{ 0.80f };
	int damage{ 1 };
	int health{ 6 };
	int maxHealth{ 6 };

	// ANIMATIONS
	int animationId_Idle{ 0 };
	int animationId_Running{ 1 };
	int animationId_Jump{ 2 };
	int animationId_Attack{ 3 };
	int animationId_Falling{ 4 };
	int animationId_Dying{ 5 };

	// INPUT
	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
	int actionId_Attack{ -1 };

};
class Jak : public GameObject
{
public:
	Jak(const JakDesc& characterDesc, PhysxProxy* pPhysxScene);
	~Jak() override;

	Jak(const Jak& other) = delete;
	Jak(Jak&& other) noexcept = delete;
	Jak& operator=(const Jak& other) = delete;
	Jak& operator=(Jak&& other) noexcept = delete;

	void DrawImGui();

	void Reset();
	int GetHealth() const { return m_CharacterDesc.health; }
	int GetMaxHealth() const { return m_CharacterDesc.maxHealth; }

	void TakeDamage(int amount);

	void SetVolume(float value);
	void StopAllSounds();
	TransformComponent* GetCameraTransform() const { return m_pCameraComponent->GetTransform(); }
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void SetAnimation(int clipId, float speed = 1.0f, bool shouldLoop = false, bool shouldOverride = false);
	void PlaySoundEffect(FMOD::Sound* clip);
private:
	PhysxProxy* m_pPhysxProxy;

	// Character Movement
	CameraComponent* m_pCameraComponent{};
	ControllerComponent* m_pControllerComponent{};
	ModelComponent* m_pModelComponent;

	JakDesc m_CharacterDesc;
	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	float m_ControllerRotationMultiplier{ 2.5f };      //When using a controller, the rotation speed is multiplied by this value
	bool m_HasDoubleJumped{ false };
	bool m_HasJustJumped{ false };
	float m_MinJumpAirTime{ 0.2f };
	float m_TimeInAir{ 0.0f };

	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)

	// GameObjects
	GameObject* m_pCameraObj;
	GameObject* m_pMeshObj;
	GameObject* m_pAttackHitBox;

	// Vulnerable Enemies
	std::vector<Enemy*> m_pVulnerableEnemies;

	float m_AccCombatTime{};
	bool m_IsAttacking{ false };
	bool m_HasDamaged{ false };

	// Sounds
	FMOD::Channel* m_pStepChannel{ };
	FMOD::ChannelGroup* m_pChannelGroup{  };

	FMOD::Sound* m_pStepSound{};
	FMOD::Sound* m_pAttackSound{};
	FMOD::Sound* m_pJumpSound{};
	FMOD::Sound* m_pOnHitSound{};
	FMOD::Sound* m_pDeathSound{};

	float m_AccParticlesTimer{0};
	float m_ParticlesTimer{0.5f};
	bool m_IsPlayingParticles{ false };
	GameObject* m_pParticles{};
};