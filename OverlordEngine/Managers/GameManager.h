#pragma once


class GameManager : public Singleton<GameManager>
{
public:
	enum class GameState
	{
		MainMenu,
		Playing,
		Paused,
		GameOver,
		GameWon
	};

	GameManager(const GameManager& other) = delete;
	GameManager(GameManager&& other) noexcept = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager& operator=(GameManager&& other) noexcept = delete;

	static float GetBrightness() { return m_Brightness; }
	static void SetBrightness(float value) { m_Brightness = value; }

	static float GetVolume() { return m_Volume; }
	static void SetVolume(float value) { m_Volume = value; }

	static GameState GetGameState() { return m_GameState; }
	static void SetGameState(GameState state) { m_GameState = state; }
protected:
	void Initialize() override;

private:
	friend class Singleton<GameManager>;
	GameManager() = default;
	~GameManager() = default;

	static GameState m_GameState;

	static float m_Brightness;
	static float m_Volume;
};

