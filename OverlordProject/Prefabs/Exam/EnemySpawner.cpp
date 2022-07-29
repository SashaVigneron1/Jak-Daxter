#include "stdafx.h"
#include "EnemySpawner.h"

#include "Enemy.h"

EnemySpawner::EnemySpawner(Jak* pJak)
	: m_pJak{pJak}
	, m_HasSpawned{false}
{
}

void EnemySpawner::Initialize(const SceneContext& /*sceneContext*/)
{
}

void EnemySpawner::Update(const SceneContext& /*sceneContext*/)
{

}

void EnemySpawner::SpawnEnemies()
{
	// Spawn Enemies At Predefined Locations (for now)
	for (auto coords : m_SpawnPositions)
	{
		EnemyDesc desc{};
		Circlef movableSurface{ {-13,2.5f,-10}, 10.0f };
		auto obj = AddChild(new Enemy(movableSurface, desc, m_pJak, this));
		obj->GetTransform()->Translate(coords);
		m_pEnemies.push_back(obj);
	}

	m_HasSpawned = true;
}

void EnemySpawner::DeleteAllEnemies()
{
	for (auto enemy : m_pEnemies)
	{
		RemoveChild(enemy, true);
	}
	m_pEnemies.clear();
}

void EnemySpawner::KillEnemy(Enemy* pEnemy)
{
	// Remove From List
	for (int i{}; i < m_pEnemies.size(); ++i)
	{
		if (m_pEnemies[i] == pEnemy)
		{
			m_pEnemies[i] = m_pEnemies.back();
			m_pEnemies.pop_back();
		}
	}

	// Delete Obj
	RemoveChild(pEnemy, true);
}

void EnemySpawner::SetVolume(float volume)
{
	for (auto enemy : m_pEnemies)
	{
		enemy->SetVolume(volume);
	}
}

void EnemySpawner::StopAllSounds()
{
	for (auto enemy : m_pEnemies)
	{
		enemy->StopAllSounds();
	}
}

bool EnemySpawner::AllEnemiesAreDead() const
{
	return m_HasSpawned && m_pEnemies.size() <= 0;
}

