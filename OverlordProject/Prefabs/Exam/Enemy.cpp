#include "stdafx.h"
#include "Enemy.h"

#include "EnemySpawner.h"
#include "Jak.h"
#include "Managers/GameManager.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Prefabs/CubePrefab.h"

Enemy::Enemy(Circlef movableSurface, EnemyDesc desc, Jak* jak, EnemySpawner* pEnemySpawner)
	: m_State{EnemyState::Idle}
	, m_EnemyDesc{desc}
	, m_pCollider{nullptr}
	, m_pMesh{nullptr}
	, m_IsPlayingParticles{false}
	, m_pJak{jak}
	, m_pEnemySpawner{pEnemySpawner}
	, m_MoveableSurface{ movableSurface }
{
}

Enemy::~Enemy()
{
	m_pOnHitSound->release();
	m_pChannelGroup->release();
	m_pStepChannel->stop();
	m_pGrowlChannel->stop();
	m_pDyingChannel->stop();
}

void Enemy::Initialize(const SceneContext&)
{
	// Model
	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 1.f);


	m_pMesh = AddChild(new GameObject);
	m_pModelComponent = m_pMesh->AddComponent(new ModelComponent(L"Meshes/Enemy.ovm"));
	auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Enemy/Enemy.png");
	m_pModelComponent->SetMaterial(pMaterial);

	m_pMesh->GetTransform()->Scale(0.004f);
	m_pMesh->GetTransform()->Rotate(0, 180, 0, true);
	m_pMesh->GetTransform()->Translate(0, -1, 0);

	SetAnimation(m_EnemyDesc.animationId_Idle, 1.0f, true, true);

	// Collision
	m_pCollider = AddChild(new GameObject());
	auto pTriggerActor = m_pCollider->AddComponent(new RigidBodyComponent());
	pTriggerActor->AddCollider(PxBoxGeometry(1, 1, 1), *pDefaultMaterial);
	pTriggerActor->SetKinematic(true);

	// Transform to position
	GetTransform()->Translate(0, 1, 2);

	m_Target = {GetTransform()->GetWorldPosition().x, GetTransform()->GetWorldPosition().z};

	// Random idle timer
	m_EnemyDesc.idleTimer = MathHelper::randF(m_EnemyDesc.minIdleTime, m_EnemyDesc.maxIdleTime);

	// SOUNDS
	auto pFModSystem = SoundManager::Get()->GetSystem();

	//// ChannelGroup
	pFModSystem->createChannelGroup("EnemyChannel", &m_pChannelGroup);

	float maxDistance = 50.f;

	//// Step
	FMOD::Sound* pSound3D{ };
	pFModSystem->createSound("Resources/Sounds/Enemy/Step.wav", FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	pSound3D->setMode(FMOD_LOOP_NORMAL);
	pFModSystem->playSound(pSound3D, m_pChannelGroup, true, &m_pStepChannel);
	m_pStepChannel->set3DMinMaxDistance(0, maxDistance);

	//// Growl
	pFModSystem->createSound("Resources/Sounds/Enemy/Growl.wav", FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	pFModSystem->playSound(pSound3D, m_pChannelGroup, true, &m_pGrowlChannel);
	pSound3D->setMode(FMOD_LOOP_NORMAL);
	m_pGrowlChannel->set3DMinMaxDistance(0, maxDistance);

	//// Dying
	pFModSystem->createSound("Resources/Sounds/Enemy/Dying.wav", FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	pFModSystem->playSound(pSound3D, m_pChannelGroup, true, &m_pDyingChannel);
	m_pDyingChannel->set3DMinMaxDistance(0, maxDistance);

	//// OnHit
	pFModSystem->createSound("Resources/Sounds/Enemy/OnHit.wav", FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &m_pOnHitSound);
}
void Enemy::Update(const SceneContext& sceneContext)
{
	if (GameManager::GetGameState() != GameManager::GameState::Paused 
		&& GameManager::GetGameState() != GameManager::GameState::GameOver
		&& GameManager::GetGameState() != GameManager::GameState::GameWon)
	{
		// Update Trigger Position
		m_pCollider->GetTransform()->Translate(GetTransform()->GetPosition());

		// Variables
		float deltaT = sceneContext.pGameTime->GetElapsed();
		float distance2DToPlayerSqr = MathHelper::Distance2DSqr(GetTransform()->GetPosition(), m_pJak->GetTransform()->GetPosition());
		float distanceToPlayerSqr = MathHelper::DistanceSqr(GetTransform()->GetPosition(), m_pJak->GetTransform()->GetPosition());
		float distance2DToTargetSqr = MathHelper::Distance2DSqr(GetTransform()->GetPosition(), m_Target);
		float movementSpeed = m_EnemyDesc.wanderingSpeed;

		// State Logic
		switch (m_State)
		{
		case EnemyState::Attacking:
			// set target to player
			m_Target = XMFLOAT2{ m_pJak->GetTransform()->GetPosition().x, m_pJak->GetTransform()->GetPosition().z };
			// Limit Target To Movable Space
			LimitTargetToMovableSurface();

			

			// Player In Attacking Range
			if (!m_IsAttacking)
			{
				// Run instead of walk
				movementSpeed = m_EnemyDesc.runningSpeed;

				if (distanceToPlayerSqr <= m_EnemyDesc.attackingDistance * m_EnemyDesc.attackingDistance)
				{
					// Start StartAttack
					StartAttack();
				}
				else
				{
					SetState(EnemyState::Attacking);
				}
			}
			else
			{
				// Disable Movement (movementspeed is set to a very low value in order to calculate direction correctly)
				movementSpeed = 0.00001f;

				m_AccCombatTimer += deltaT;
				if (!m_HasDamaged && m_AccCombatTimer >= m_EnemyDesc.damageTimer)
				{
					if (distanceToPlayerSqr <= m_EnemyDesc.attackingDistance * m_EnemyDesc.attackingDistance)
						m_pJak->TakeDamage(m_EnemyDesc.damage);
					m_HasDamaged = true;
				}
				else if (m_AccCombatTimer >= m_EnemyDesc.attackTimer)
				{
					m_IsAttacking = false;
					m_AccCombatTimer = 0.0f;
				}
			}



			// Player To Far
			if (distance2DToPlayerSqr > m_EnemyDesc.viewingDistance * m_EnemyDesc.viewingDistance)
			{
				// Idle
				SetState(EnemyState::Idle);
			}
			break;
		case EnemyState::Idle:
			// Wait and wander
			movementSpeed = 0.0f;
			m_AccTimer += deltaT;
			if (m_AccTimer >= m_EnemyDesc.idleTimer)
			{
				SetState(EnemyState::Wandering);
				SetTargetToRandomPosition();
			}

			// Player CloseBy
			if (distance2DToPlayerSqr <= m_EnemyDesc.viewingDistance * m_EnemyDesc.viewingDistance)
			{
				// StartAttack
				SetState(EnemyState::Attacking);
			}

			break;
		case EnemyState::Wandering:
			// If Close To Target
			if (distance2DToTargetSqr <= m_EnemyDesc.targetDistance * m_EnemyDesc.targetDistance)
			{
				// Idle
				SetState(EnemyState::Idle);
			}

			// Player CloseBy
			if (distance2DToPlayerSqr <= m_EnemyDesc.viewingDistance * m_EnemyDesc.viewingDistance)
			{
				// StartAttack
				SetState(EnemyState::Attacking);
			}
			break;
		case EnemyState::Dying:
			movementSpeed = 0.0f;
			m_AccTimer += deltaT;
			if (m_AccTimer >= m_EnemyDesc.dyingTimer)
			{
				Die();
				return;
			}

			// Update Particles
			if (m_IsPlayingParticles)
			{
				m_AccParticlesTimer += deltaT;
				if (m_AccParticlesTimer >= m_ParticlesTimer)
				{
					if (m_pParticles)
					{
						RemoveChild(m_pParticles, true);
						m_pParticles = nullptr;
					}
					m_IsPlayingParticles = false;
				}
			}
			break;
		}

		// Move To Target
		{
			// Get Positions
			auto position = GetTransform()->GetPosition();
			auto currPos = XMLoadFloat3(&position);

			// Get Direction
			XMFLOAT2 direction = { m_Target.x - position.x, m_Target.y - position.z };
			auto directionVector = XMLoadFloat2(&direction);
			XMVector2Normalize(directionVector);
			XMStoreFloat2(&direction, directionVector);

			// Calculate New Position
			XMFLOAT3 positionOffset{ direction.x * movementSpeed * deltaT,0,direction.y * movementSpeed * deltaT };
			auto newPos = currPos + XMLoadFloat3(&positionOffset);
			XMFLOAT3 newPosition;
			XMStoreFloat3(&newPosition, newPos);
			GetTransform()->Translate(newPosition);

			// Rotate Mesh In Direction
			float angleToRotate = 90.0f;
			angleToRotate += -atan2(direction.y, direction.x);
			GetTransform()->Rotate(XMFLOAT3{ 0,angleToRotate,0 }, false);
		}
		
	}
	else
	{
		StopAllSounds();
	}
}

void Enemy::SetAnimation(int clipId, float speed, bool shouldLoop, bool shouldOverride)
{
	auto pAnimator = m_pModelComponent->GetAnimator();

	if (pAnimator->GetCurrentClipId() == clipId && !shouldOverride) return;

	pAnimator->SetAnimation(clipId);
	pAnimator->SetAnimationSpeed(speed);
	pAnimator->EnableLooping(shouldLoop);
	pAnimator->Play();
}

void Enemy::PlaySoundEffect(FMOD::Sound* pClip)
{
	SoundManager::Get()->GetSystem()->playSound(pClip, m_pChannelGroup, false, nullptr);
}

void Enemy::Die()
{
	m_pEnemySpawner->KillEnemy(this);
}

void Enemy::SetState(EnemyState state)
{
	m_State = state;
	m_AccTimer = 0.0f;

	switch(state)
	{
	case EnemyState::Attacking:
		SetAnimation(m_EnemyDesc.animationId_Running, 1.0f, true, false);
		m_pStepChannel->setPaused(false);
		m_pGrowlChannel->setPaused(true);
		m_pDyingChannel->setPaused(true);
		break;
	case EnemyState::Idle:
		m_EnemyDesc.idleTimer = MathHelper::randF(m_EnemyDesc.minIdleTime, m_EnemyDesc.maxIdleTime);

		SetAnimation(m_EnemyDesc.animationId_Idle, 1.0f, true, false);
		m_pStepChannel->setPaused(true);
		m_pGrowlChannel->setPaused(false);
		m_pDyingChannel->setPaused(true);
		break;
	case EnemyState::Wandering:
		SetAnimation(m_EnemyDesc.animationId_Walking, 1.0f, true, false);
		m_pStepChannel->setPaused(false);
		m_pGrowlChannel->setPaused(true);
		m_pDyingChannel->setPaused(true);
		break;
	case EnemyState::Dying:
		SetAnimation(m_EnemyDesc.animationId_Dying, 1.0f, false, false);
		m_pStepChannel->setPaused(true);
		m_pGrowlChannel->setPaused(true);
		m_pDyingChannel->setPaused(false);
		break;
	}
}

void Enemy::SetTargetToRandomPosition()
{
	// Get Random Distance In Rage
	float distance = MathHelper::RandomFloat(m_EnemyDesc.minWanderingDistancePercentage * m_MoveableSurface.radius, m_MoveableSurface.radius);

	// Add Distance To Random Direction
	XMFLOAT2 direction{ MathHelper::RandomFloat(0.0f, 2.0f) - 1.0f, MathHelper::RandomFloat(0.0f, 2.0f) - 1.0f };
	auto directionVector = XMLoadFloat2(&direction);
	directionVector *= distance;
	XMStoreFloat2(&direction, directionVector);

	auto startPointVec = XMLoadFloat3(&m_MoveableSurface.position);
	XMFLOAT3 startPoint;
	XMStoreFloat3(&startPoint, startPointVec);

	// Set m_Target
	m_Target = XMFLOAT2{ startPoint.x + direction.x, startPoint.z + direction.y };
}

void Enemy::LimitTargetToMovableSurface()
{
	float distanceToTarget = MathHelper::Distance2DSqr(m_MoveableSurface.position, m_Target);
	if (distanceToTarget > m_MoveableSurface.radius * m_MoveableSurface.radius)
	{
		auto target = XMLoadFloat2(&m_Target);
		XMFLOAT2 centerPoint{ m_MoveableSurface.position.x, m_MoveableSurface.position.z };
		auto center = XMLoadFloat2(&centerPoint);

		auto dir = target - center;
		dir = XMVector2Normalize(dir);
		dir *= m_MoveableSurface.radius;

		auto newPos = center + dir;
		XMStoreFloat2(&m_Target, newPos);
	}
}

void Enemy::StartAttack()
{
	m_IsAttacking = true;
	m_HasDamaged = false;
	m_AccCombatTimer = 0.0f;
	SetAnimation(m_EnemyDesc.animationId_Attack, 0.70f, false, true);
}

void Enemy::TakeDamage(int damage)
{
	m_EnemyDesc.health -= damage;

	SoundManager::Get()->GetSystem()->playSound(m_pOnHitSound, m_pChannelGroup, false, nullptr);


	// Start Timer To Die
	if (m_EnemyDesc.health <= 0)
	{
		SetState(EnemyState::Dying);
		if (!m_IsPlayingParticles)
		{
			//Particle System
			ParticleEmitterSettings settings{};
			settings.velocity = { 0.f,3.f,0.f };
			settings.minSize = 1.f;
			settings.maxSize = 2.f;
			settings.minEnergy = 0.3f;
			settings.maxEnergy = 1.f;
			settings.minScale = .5f;
			settings.maxScale = 1.5f;
			settings.minEmitterRadius = .2f;
			settings.maxEmitterRadius = .5f;
			settings.color = { 1.f,1.f,1.f, .3f };

			auto position = GetTransform()->GetWorldPosition();
			position.y += 1.5f;
			m_pParticles = AddChild(new GameObject());
			m_pParticles->AddComponent(new ParticleEmitterComponent(L"Textures/Particles/EnemyDeath.png", settings, 200));
			m_pParticles->GetTransform()->Translate(position);

			m_IsPlayingParticles = true;
		}
	}
	
}

void Enemy::SetVolume(float value)
{
	// StepSound Is Quite Loud By Default, So It's multiplied by 0.75f
	m_pStepChannel->setVolume(value * 0.75f);
	m_pChannelGroup->setVolume(value);
}

void Enemy::StopAllSounds()
{
	m_pStepChannel->setPaused(true);
	m_pGrowlChannel->setPaused(true);
	m_pDyingChannel->setPaused(true);
}
