#pragma once

class PostBrightness : public PostProcessingMaterial
{
public:
	PostBrightness();
	~PostBrightness() override = default;
	PostBrightness(const PostBrightness& other) = delete;
	PostBrightness(PostBrightness&& other) noexcept = delete;
	PostBrightness& operator=(const PostBrightness& other) = delete;
	PostBrightness& operator=(PostBrightness&& other) noexcept = delete;

	void SetBrightness(float value);
	float GetBrightness() const { return m_Brightness; }

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
	void UpdateBaseEffectVariables(const SceneContext& sceneContext, RenderTarget* pSource) override;

	float m_Brightness = 1.0f;
	float m_MaxBrightness = 2.0f;
	float m_MinBrightness = 0.5f;
};