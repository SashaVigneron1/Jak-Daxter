#pragma once

class Enemy;
class Jak;

class EnemySpawner : public GameObject
{
public:
	EnemySpawner(Jak* pJak);
	~EnemySpawner() override = default;

	EnemySpawner(const EnemySpawner& other) = delete;
	EnemySpawner(EnemySpawner&& other) noexcept = delete;
	EnemySpawner& operator=(const EnemySpawner& other) = delete;
	EnemySpawner& operator=(EnemySpawner&& other) noexcept = delete;

	void AddSpawnLocation(XMFLOAT3 coordinates) { m_SpawnPositions.push_back(coordinates); }
	void SpawnEnemies();
	void DeleteAllEnemies();
	void KillEnemy(Enemy* pEnemy);
	void SetVolume(float volume);
	void StopAllSounds();


	bool AllEnemiesAreDead() const;

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	Jak* m_pJak;
	std::vector<XMFLOAT3> m_SpawnPositions;
	std::vector<Enemy*> m_pEnemies;

	bool m_HasSpawned;
};
