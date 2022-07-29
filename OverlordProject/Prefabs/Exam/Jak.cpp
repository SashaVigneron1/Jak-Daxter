#include "stdafx.h"
#include "Jak.h"

#include "Enemy.h"
#include "Managers/GameManager.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

Jak::Jak(const JakDesc& characterDesc, PhysxProxy* pPhysxScene) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime),
	m_pPhysxProxy{pPhysxScene}
{}

Jak::~Jak()
{
	m_pChannelGroup->release();
	m_pAttackSound->release();
	m_pJumpSound->release();
	m_pOnHitSound->release();
	m_pDeathSound->release();
	m_pStepChannel->stop();
}

void Jak::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	// Model
	m_pMeshObj = AddChild(new GameObject);
	m_pModelComponent = m_pMeshObj->AddComponent(new ModelComponent(L"Meshes/Jak.ovm"));

	// Textures
	auto pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Face.png");
	m_pModelComponent->SetMaterial(pMaterial, 1);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Goggles.png");
	m_pModelComponent->SetMaterial(pMaterial, 2);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Arms.png");
	m_pModelComponent->SetMaterial(pMaterial, 3);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/MetalShine.png");
	m_pModelComponent->SetMaterial(pMaterial, 4);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Legs.png");
	m_pModelComponent->SetMaterial(pMaterial, 5);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/StrapLegs.png");
	m_pModelComponent->SetMaterial(pMaterial, 6);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Shirt.png");
	m_pModelComponent->SetMaterial(pMaterial, 7);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Pouch.png");
	m_pModelComponent->SetMaterial(pMaterial, 8);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Scarf.png");
	m_pModelComponent->SetMaterial(pMaterial, 9);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Ears.png");
	m_pModelComponent->SetMaterial(pMaterial, 10);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Shoes.png");
	m_pModelComponent->SetMaterial(pMaterial, 11);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/StrapsBlue.png");
	m_pModelComponent->SetMaterial(pMaterial, 12);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/GunHolster.png");
	m_pModelComponent->SetMaterial(pMaterial, 13);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Belt.png");
	m_pModelComponent->SetMaterial(pMaterial, 14);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/FacialHair.png");
	m_pModelComponent->SetMaterial(pMaterial, 15);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/EyeL.png");
	m_pModelComponent->SetMaterial(pMaterial, 16);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/LensShine.png");
	m_pModelComponent->SetMaterial(pMaterial, 17);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Hair.png");
	m_pModelComponent->SetMaterial(pMaterial, 18);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Gloves.png");
	m_pModelComponent->SetMaterial(pMaterial, 19);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Armor.png");
	m_pModelComponent->SetMaterial(pMaterial, 20);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Goggles.png");
	m_pModelComponent->SetMaterial(pMaterial, 21);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/EyeR.png");
	m_pModelComponent->SetMaterial(pMaterial, 22);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/StrapsChest.png");
	m_pModelComponent->SetMaterial(pMaterial, 23);
	pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
	pMaterial->SetDiffuseTexture(L"Textures/Jak/Ears.png");
	m_pModelComponent->SetMaterial(pMaterial, 24);

	m_pMeshObj->GetTransform()->Scale(0.001f);
	m_pMeshObj->GetTransform()->Rotate(0, 180, 0, true);
	m_pMeshObj->GetTransform()->Translate(0, -1.7f, 0);

	SetAnimation(m_CharacterDesc.animationId_Idle);

	//Camera
	m_pCameraObj = AddChild(new GameObject);
	const auto pCamera = m_pCameraObj->AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true);

	pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * .5f, -7.f);

	// StartAttack TriggerBox
	auto pDefaultMaterial = PxGetPhysics().createMaterial(0.f, 0.f, 1.f);

	m_pAttackHitBox = AddChild(new GameObject());
	auto pTriggerActor = m_pAttackHitBox->AddComponent(new RigidBodyComponent());
	pTriggerActor->SetKinematic(true);

	UINT colliderId = pTriggerActor->AddCollider(PxBoxGeometry(2, 2, 2), *pDefaultMaterial);
	pTriggerActor->GetCollider(colliderId).SetTrigger(true);
	m_pAttackHitBox->SetOnTriggerCallBack([=](GameObject*, GameObject* pOther, PxTriggerAction action)
		{
			// If Other = Enemy
			auto enemy = dynamic_cast<Enemy*>(pOther->GetParent());
			if (enemy && pOther == enemy->GetCollider())
			{
				if (action == PxTriggerAction::ENTER)
				{
					// Add to vulnerable
					m_pVulnerableEnemies.push_back(enemy);
				}
				else if (action == PxTriggerAction::LEAVE)
				{
					// Remove from vulnerable
					for (size_t i = 0; i < m_pVulnerableEnemies.size(); i++)
					{
						if (m_pVulnerableEnemies[i] == enemy)
						{
							m_pVulnerableEnemies[i] = m_pVulnerableEnemies.back();
							m_pVulnerableEnemies.pop_back();
							return;
						}
					}
				}
			}
		});

	// Sounds
	auto pFModSystem = SoundManager::Get()->GetSystem();

	//// ChannelGroup
	pFModSystem->createChannelGroup("PlayerChannel", &m_pChannelGroup);

	//// Step
	pFModSystem->createStream("Resources/Sounds/Character/Step.wav", FMOD_2D, nullptr, &m_pStepSound);
	m_pStepSound->setMode(FMOD_LOOP_NORMAL);
	pFModSystem->playSound(m_pStepSound, m_pChannelGroup, true, &m_pStepChannel);

	//// Attack
	pFModSystem->createSound("Resources/Sounds/Character/Attack.mp3", FMOD_2D, nullptr, &m_pAttackSound);

	//// Jump
	pFModSystem->createSound("Resources/Sounds/Character/Jump.wav", FMOD_2D, nullptr, &m_pJumpSound);

	//// OnHit
	pFModSystem->createSound("Resources/Sounds/Character/OnHit.wav", FMOD_2D, nullptr, &m_pOnHitSound);

	//// Death
	pFModSystem->createSound("Resources/Sounds/Character/Dying.wav", FMOD_2D, nullptr, &m_pDeathSound);
}

void Jak::Update(const SceneContext& sceneContext)
{
	const float deltaT{ sceneContext.pGameTime->GetElapsed() };


	if (m_pCameraComponent->IsActive() && GameManager::GetGameState() == GameManager::GameState::Playing)
	{
		#pragma region Movement
		constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero


		#pragma region InputGathering
		//***************
		//HANDLE INPUT
		#pragma region Move
		//## Input Gathering (move)
		XMFLOAT2 move = XMFLOAT2{ 0.0f,0.0f };
		//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward)) move.y = 1.0f;
		else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward)) move.y = -1.0f;
		//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement
		if (abs(move.y) <= epsilon)
		{
			move.y = sceneContext.pInput->GetThumbstickPosition(true).y;
		}

		//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
		if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight)) move.x = 1.0f;
		else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft)) move.x = -1.0f;
		//Optional: if move.x is near zero (abs(move.x) < epsilon), you could use the Left ThumbStickPosition.x for movement
		if (abs(move.x) <= epsilon)
		{
			move.x = sceneContext.pInput->GetThumbstickPosition(true).x;
		}

		bool isMoving = false;
		if (abs(move.x) > epsilon || abs(move.y) > epsilon)
			isMoving = true;
		m_pStepChannel->setPaused(!isMoving);
		#pragma endregion

		#pragma region Look
		//## Input Gathering (look)
		XMFLOAT2 look{ 0.f, 0.f };
		//Only if the Left Mouse Button is Down >
			// Store the MouseMovement in the local 'look' variable (cast is required)
		//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
		if (sceneContext.pInput->IsMouseButton(InputState::down, 2))
		{
			auto mouseMovement = sceneContext.pInput->GetMouseMovement();
			look.x = (float)mouseMovement.x;
			look.y = (float)mouseMovement.y;
		}
		else
		{
			// Controller Camera Movement
			look.x = sceneContext.pInput->GetThumbstickPosition(false).x * m_ControllerRotationMultiplier;
			look.y = -sceneContext.pInput->GetThumbstickPosition(false).y * m_ControllerRotationMultiplier;
		}
		#pragma endregion
		#pragma endregion

		#pragma region TransformInfo
		//************************
		//GATHERING TRANSFORM INFO
		//Retrieve the TransformComponent
		TransformComponent* pCameraTransform = m_pCameraObj->GetTransform();
		//TransformComponent* pModelTransform = m_pMeshObj->GetTransform();

		//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent
		// COMING FROM CAMERA
		XMVECTOR forward = XMLoadFloat3(&pCameraTransform->GetForward());
		forward.m128_f32[1] = 0;
		XMVECTOR right = XMLoadFloat3(&pCameraTransform->GetRight());
		

		#pragma endregion
		
		#pragma region CameraRotation
		//***************
		//CAMERA ROTATION
		//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
		//Make sure this calculated on a framerate independent way and uses JakDesc::rotationSpeed.
		//Rotate this Jak based on the TotalPitch (X) and TotalYaw (Y)
		m_TotalYaw += look.x * m_CharacterDesc.rotationSpeed * deltaT;
		m_TotalPitch += look.y * m_CharacterDesc.rotationSpeed * deltaT;
		// Clamp Pitch
		if (m_TotalPitch > m_CharacterDesc.maxPitch) m_TotalPitch = m_CharacterDesc.maxPitch;
		else if (m_TotalPitch < m_CharacterDesc.minPitch) m_TotalPitch = m_CharacterDesc.minPitch;

		pCameraTransform->Rotate(m_TotalPitch, m_TotalYaw, 0);
		//pModelTransform->Rotate(0, m_TotalYaw + 180, 0);
		#pragma endregion
		
		#pragma region ApplyMovement
		//********
		//MOVEMENT
		//## Horizontal Velocity (Forward/Backward/Right/Left)
		//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
		float currentAcceleration = m_MoveAcceleration * deltaT;

		//If Jak is moving (= input is pressed)
		if (abs(move.x) >= epsilon || abs(move.y) >= epsilon)
		{
			//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
			XMVECTOR dir{};
			dir += forward * move.y;
			dir += right * move.x;
			dir = XMVector3Normalize(dir);
			XMStoreFloat3(&m_CurrentDirection, dir);

			//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			m_MoveSpeed += currentAcceleration;

			//Make sure the current MoveSpeed stays below the maximum MoveSpeed (JakDesc::maxMoveSpeed)
			if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed)
				m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
		}
		//Else (Jak is not moving, or stopped moving)
		else
		{
			//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			m_MoveSpeed -= m_MoveAcceleration;

			//Make sure the current MoveSpeed doesn't get smaller than zero
			if (m_MoveSpeed < 0)
				m_MoveSpeed = 0;
		}

		//Now we can calculate the Horizontal Velocity which should be stored in m_TotalVelocity.xz
		//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
		//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
		//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)
		float tempY = m_TotalVelocity.y;

		XMVECTOR currDir = XMLoadFloat3(&m_CurrentDirection);
		currDir = currDir * m_MoveSpeed;
		XMFLOAT3 velocity;
		XMStoreFloat3(&velocity, currDir);

		m_TotalVelocity = velocity;
		m_TotalVelocity.y = tempY;

		// Rotate Mesh In Direction
		float angleToRotate = -90.0f;
		angleToRotate += -atan2(move.y, move.x) * (180 / 3.14f);
		angleToRotate += m_TotalYaw;
		if (abs(move.x) <= epsilon && abs(move.y) <= epsilon) angleToRotate -= 90.0f;

		m_pMeshObj->GetTransform()->Rotate(XMFLOAT3{0,angleToRotate,0}, true);

		#pragma endregion


		#pragma region Jumping
		//## Vertical Movement (Jump/Fall)
		//If the Controller Component is NOT grounded (= freefall)
		const PxU32 bufferSize = 256;
		PxRaycastHit hitBuffer[bufferSize];
		PxRaycastBuffer buffer(hitBuffer, bufferSize);
		bool isInput = sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump);
		bool canDoubleJump = m_TimeInAir >= m_CharacterDesc.minTimeBeforeDoubleJump;
		//bool isGrounded = m_pControllerComponent->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN);

		PxVec3 position{m_pControllerComponent->GetFootPosition().x, m_pControllerComponent->GetFootPosition().y, m_pControllerComponent->GetFootPosition().z};
		bool isGrounded = false;
		PxQueryFilterData fd;
		fd.flags |= PxQueryFlag::eNO_BLOCK;
		if(m_pPhysxProxy->Raycast(position, PxVec3(0, -1, 0), 0.2f, buffer, PxHitFlag::eMESH_MULTIPLE, fd))
		{
			auto hitCount = buffer.getNbAnyHits();
			for (PxU32 i = 0; i < hitCount; i++)
			{
				auto actor = buffer.getAnyHit(i).actor;
				if (actor)
				{
					auto userData = actor->userData;

					if (userData)
					{
						BaseComponent* obj = reinterpret_cast<BaseComponent*>(userData);
						if (obj->GetGameObject() != this && obj->GetGameObject() != m_pAttackHitBox)
						{
							isGrounded = true;
						}
					}
				}
			}
		}

		//If the jump action is triggered
		bool hasJumped = false;
		if (isInput && isGrounded)
		{
			//Set m_TotalVelocity.y equal to JakDesc::JumpSpeed
			m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;

			hasJumped = true;
			m_HasJustJumped = true;
			m_TimeInAir = 0.0f;
			PlaySoundEffect(m_pJumpSound);

		}
		else if (!m_HasDoubleJumped && canDoubleJump && !isGrounded && isInput)
		{
			// Double Jump
			m_TotalVelocity.y = m_CharacterDesc.DoubleJumpSpeed;
			m_HasDoubleJumped = true;
			hasJumped = true;
			m_TimeInAir = 0.0f;
			PlaySoundEffect(m_pJumpSound);
		}
		else if (!isGrounded)
		{
			//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
			m_TotalVelocity.y -= m_FallAcceleration * deltaT;

			//Make sure that the minimum speed stays above -JakDesc::maxFallSpeed (negative!)
			if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed)
				m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;

			m_TimeInAir += deltaT;
		}
		//Else (=Jak is grounded, no input pressed)
		else if (!(m_HasJustJumped && m_TimeInAir < m_MinJumpAirTime))
		{
			//m_TotalVelocity.y is zero
			m_TotalVelocity.y = 0;
			m_TimeInAir = 0.0f;
			m_HasDoubleJumped = false;
		}
		#pragma endregion

		#pragma region Displacement
		//************
		//DISPLACEMENT
		//The displacement required to move the Jak Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
		//Calculate the displacement (m) for the current frame and move the ControllerComponent
		auto vel = XMLoadFloat3(&m_TotalVelocity);
		XMFLOAT3 displacement;
		XMStoreFloat3(&displacement, vel * sceneContext.pGameTime->GetElapsed());
		m_pControllerComponent->Move(displacement);

		//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
		//Also, it can be usefull to use a seperate RayCast to check if the Jak is grounded (more responsive)
		#pragma endregion
		#pragma endregion

		#pragma region HandleCombat
		// Move TriggerBox
		m_pAttackHitBox->GetTransform()->Translate(GetTransform()->GetPosition());

		bool hasAttacked = false;
		if (!m_IsAttacking)
		{
			bool isAttackingInput = sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Attack);
			if (isAttackingInput && isGrounded)
			{
				
				hasAttacked = true;
				m_IsAttacking = true;
				m_AccCombatTime = 0.0f;
				m_HasDamaged = false;
				PlaySoundEffect(m_pAttackSound);
			}
		}
		else
		{
			m_AccCombatTime += deltaT;

			if (m_AccCombatTime >= m_CharacterDesc.combatCooldown)
				m_IsAttacking = false;
			else if (!m_HasDamaged && m_AccCombatTime >= m_CharacterDesc.combatHitTime)
			{
				if (m_pVulnerableEnemies.size() > 0)
				{
					// For all enemies in triggerBox
					for (auto enemy : m_pVulnerableEnemies)
					{
						// Damage
						enemy->TakeDamage(m_CharacterDesc.damage);
					}
				}
				m_HasDamaged = true;
			}
		}
		#pragma endregion

		#pragma region Animations
		auto pAnimator = m_pModelComponent->GetAnimator();

		bool isAttacking = pAnimator->GetCurrentClipId() == m_CharacterDesc.animationId_Attack;
		bool isJumping = pAnimator->GetCurrentClipId() == m_CharacterDesc.animationId_Jump;
		bool isDying = pAnimator->GetCurrentClipId() == m_CharacterDesc.animationId_Dying;
		bool isFalling = (m_TimeInAir > 0.50f && m_HasDoubleJumped) || m_TimeInAir > 1.0f;

		if (isMoving && (!isJumping || !pAnimator->IsPlaying()) && (!isAttacking || !pAnimator->IsPlaying()) && !isFalling && !isDying)
			SetAnimation(m_CharacterDesc.animationId_Running, 1.0f, true);
		else if (isFalling)
		{
			if (m_TotalVelocity.y < -1)
				SetAnimation(m_CharacterDesc.animationId_Falling, 1.0f, true);
		}

		if (hasJumped)
			SetAnimation(m_CharacterDesc.animationId_Jump, 1.0f, false, true);
		if (hasAttacked)
			SetAnimation(m_CharacterDesc.animationId_Attack, 2.0f, false);

		// If animation is not playing, go back to idle
		if (!isMoving && !isFalling && !isDying && (!pAnimator->IsPlaying() || pAnimator->GetCurrentClipId() == m_CharacterDesc.animationId_Running
			|| pAnimator->GetCurrentClipId() == m_CharacterDesc.animationId_Falling))
			SetAnimation(m_CharacterDesc.animationId_Idle, 1.0f, true);
		#pragma endregion

		#pragma region ShadowMap
		// LIGHTS
		float positionMultiplier{ 10 };
		XMFLOAT3 direction{ MathHelper::Deg2Rad(45), MathHelper::Deg2Rad(-60), MathHelper::Deg2Rad(45) };
		XMFLOAT3 playerPos{ GetTransform()->GetPosition() };
		XMFLOAT3 lightPos{ (-direction.x * positionMultiplier) + playerPos.x, (-direction.y * positionMultiplier) + playerPos.y, (-direction.z * positionMultiplier) + playerPos.z};
		sceneContext.pLights->SetDirectionalLight(lightPos, direction);
		ShadowMapRenderer::Get()->SetShadowmapScaleMultiplier(0.25f);
		#pragma endregion

		// Falling Death State
		if (GetTransform()->GetPosition().y < -30)
			GameManager::SetGameState(GameManager::GameState::GameOver);

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

			// Only End Game When Particles Were Shown
			GameManager::SetGameState(GameManager::GameState::GameOver);

		}
	}
}



void Jak::SetAnimation(int clipId, float speed, bool shouldLoop, bool shouldOverride)
{
	auto pAnimator = m_pModelComponent->GetAnimator();

	if (pAnimator->GetCurrentClipId() == clipId && !shouldOverride) return;

	pAnimator->SetAnimation(clipId);
	pAnimator->SetAnimationSpeed(speed);
	pAnimator->EnableLooping(shouldLoop);
	pAnimator->Play();
}

void Jak::PlaySoundEffect(FMOD::Sound* clip)
{
	SoundManager::Get()->GetSystem()->playSound(clip, m_pChannelGroup, false, nullptr);
}

void Jak::DrawImGui()
{
	if (ImGui::CollapsingHeader("Jak"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Double Jump Speed", &m_CharacterDesc.DoubleJumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if (ImGui::Checkbox("Jak Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
		ImGui::Checkbox("Is Double Jumping", &m_HasDoubleJumped);
	}
}

void Jak::Reset()
{
	m_CharacterDesc.health = m_CharacterDesc.maxHealth;
	SetAnimation(m_CharacterDesc.animationId_Idle);
	m_TotalYaw = 0;
	m_TotalPitch = 0;

	float volume = GameManager::GetVolume();
	SetVolume(volume);
}

void Jak::TakeDamage(int amount)
{
	m_CharacterDesc.health -= amount;
	PlaySoundEffect(m_pOnHitSound);
	if (m_CharacterDesc.health <= 0)
	{
		// Die
		StopAllSounds();
		PlaySoundEffect(m_pDeathSound);
		SetAnimation(m_CharacterDesc.animationId_Dying);

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
			position.y += .5f;
			m_pParticles = AddChild(new GameObject());
			m_pParticles->AddComponent(new ParticleEmitterComponent(L"Textures/Particles/JakDeath.png", settings, 200));
			m_pParticles->GetTransform()->Translate(position);

			m_IsPlayingParticles = true;
		}
	}
}

void Jak::SetVolume(float value)
{
	// StepSound Is Quite Loud By Default, So It's multiplied by 0.75f
	m_pStepChannel->setVolume(value * 0.75f);
	m_pChannelGroup->setVolume(value);
}

void Jak::StopAllSounds()
{
	m_pStepChannel->setPaused(true);
}
