#pragma once


class EnemySpawner;
class Jak;

struct EnemyDesc
{
	EnemyDesc()
	{
	}

	// MOVEMENT
	float wanderingSpeed{ 0.4f };
	float runningSpeed{ 0.5f };

	//float moveAccelerationTime{ .3f };
	//float fallAccelerationTime{ .3f };

	float minWanderingDistancePercentage{ 0.5f };

	float rotationSpeed{ 60.f };

	// ANIMATIONS
	int animationId_Idle{ 0 };
	int animationId_Walking{ 1 };
	int animationId_Running{ 2 };
	int animationId_Attack{ 3 };
	int animationId_Dying{ 4 };

	// COMBAT
	float attackTimer{ 2.0f };
	float damageTimer{ 0.50f };
	int health{ 3 };
	int damage{ 1 };

	float targetDistance{ 3.0f };
	float viewingDistance{16.3f};
	float attackingDistance{4.0f};

	// TIMERS
	float idleTimer{ 3.0f };
	float minIdleTime{ 3.0f };
	float maxIdleTime{ 3.0f };
	float dyingTimer{ 4.0f };
};

class Enemy : public GameObject
{
public:
	Enemy(Circlef movableSurface, EnemyDesc desc, Jak* jak, EnemySpawner* pEnemySpawner);
	~Enemy() override;

	Enemy(const Enemy& other) = delete;
	Enemy(Enemy&& other) noexcept = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy& operator=(Enemy&& other) noexcept = delete;

	void TakeDamage(int damage);
	void SetVolume(float value);
	void StopAllSounds();

	GameObject* GetCollider() const { return m_pCollider; }


protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

	void SetAnimation(int clipId, float speed = 1.0f, bool shouldLoop = false, bool shouldOverride = false);
	void PlaySoundEffect(FMOD::Sound* pClip);
private:

	ModelComponent* m_pModelComponent;

	enum class EnemyState
	{
		Idle,
		Attacking,
		Wandering,
		Dying
	};

	EnemyState m_State;
	EnemyDesc m_EnemyDesc;

	// Jak
	Jak* m_pJak;
	EnemySpawner* m_pEnemySpawner;

	// GameObjects
	GameObject* m_pMesh;
	GameObject* m_pCollider;

	void Die();

	// Movement
	XMFLOAT2 m_Velocity{};
	XMFLOAT2 m_Target{};

	float m_AccTimer{ 0.0f };
	float m_AccCombatTimer{ 0.0f };
	bool m_HasDamaged{ false };
	bool m_IsAttacking{ false };


	void SetState(EnemyState state);
	void SetTargetToRandomPosition();
	void LimitTargetToMovableSurface();
	void StartAttack();

	Circlef m_MoveableSurface;

	// SOUNDS
	FMOD::Channel* m_pStepChannel{ };
	FMOD::ChannelGroup* m_pChannelGroup{  };

	FMOD::Channel* m_pGrowlChannel{};
	FMOD::Sound* m_pOnHitSound{};
	FMOD::Channel* m_pDyingChannel{};


	float m_AccParticlesTimer{ 0 };
	float m_ParticlesTimer{ 0.5f };
	bool m_IsPlayingParticles{ false };
	GameObject* m_pParticles{};
};