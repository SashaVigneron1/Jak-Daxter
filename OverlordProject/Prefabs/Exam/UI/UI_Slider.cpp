#include "stdafx.h"
#include "UI_Slider.h"

UI_Slider::UI_Slider(float minValue, float maxValue, const std::wstring& fillPath, const std::wstring& bgPath, XMFLOAT2 pivot, XMFLOAT4 color)
	: m_FillSpriteFilePath{fillPath}
	, m_BackgroundSpriteFilePath{bgPath}
	, m_Pivot{pivot}
	, m_Color{color}
	, m_pFillSprite{}
	, m_pBackgroundSprite{}
	, m_MinValue{minValue}
	, m_MaxValue{maxValue}
	, m_IsDirty{false}
{
}

void UI_Slider::SetValue(float value)
{
	// Apply Value
	m_Value = value;
	MathHelper::Clamp(m_Value, m_MaxValue, m_MinValue);
	m_IsDirty = true;
	UpdateUI();
}

void UI_Slider::Initialize(const SceneContext& )
{
	auto bgObj = AddChild(new GameObject());
	auto fillObj = AddChild(new GameObject());

	m_pBackgroundSprite = bgObj->AddComponent(new SpriteComponent(m_BackgroundSpriteFilePath,
		m_Pivot, m_Color));
	m_pFillSprite = fillObj->AddComponent(new SpriteComponent(m_FillSpriteFilePath,
		m_Pivot, m_Color));

}

void UI_Slider::Update(const SceneContext& sceneContext)
{
	auto& mousePos = sceneContext.pInput->GetMousePosition();
	auto spriteDimensions = m_pBackgroundSprite->GetTexture()->GetDimension();
	auto& spritePos = m_pBackgroundSprite->GetTransform()->GetWorldPosition();
	auto& spriteScale = GetTransform()->GetWorldScale();
	auto& pivot = m_Pivot;
	spriteDimensions.x -= m_pFillSprite->GetTexture()->GetDimension().x;

	// Calculate IsHovering
	float minX = spritePos.x - (pivot.x * spriteDimensions.x * spriteScale.x);
	float maxX = spritePos.x + ((1 - pivot.x) * spriteDimensions.x * spriteScale.x);
	float minY = spritePos.y - ((1 - pivot.y) * spriteDimensions.y * spriteScale.y);
	float maxY = spritePos.y + (pivot.y * spriteDimensions.y * spriteScale.y);

	bool isHovering = (mousePos.x > minX && mousePos.x < maxX) && (mousePos.y > minY && mousePos.y < maxY);
	bool isPressed = isHovering && (sceneContext.pInput->IsMouseButton(InputState::pressed, 1)
		|| sceneContext.pInput->IsMouseButton(InputState::down, 1));

	if (isPressed)
	{
		float normalizedValue = ((float)mousePos.x - minX) / (maxX - minX);
		m_Value = m_MinValue + normalizedValue * (m_MaxValue - m_MinValue);

		m_IsDirty = true;
		UpdateUI();
	}
}

void UI_Slider::UpdateUI()
{
	// Update UI
	m_pBackgroundSprite->GetTransform()->Scale(GetTransform()->GetScale());
	m_pFillSprite->GetTransform()->Scale(GetTransform()->GetScale());

	auto spriteDimensions = m_pBackgroundSprite->GetTexture()->GetDimension();
	spriteDimensions.x -= m_pFillSprite->GetTexture()->GetDimension().x;
	//auto& spritePos = m_pBackgroundSprite->GetTransform()->GetWorldPosition();
	auto& pivot = m_Pivot;

	// Calculate IsHovering
	float maxX = (1 - pivot.x) * spriteDimensions.x;
	float normalizedValue = (m_Value - m_MinValue) / (m_MaxValue - m_MinValue);

	XMFLOAT3 fillPos{};
	fillPos.x = normalizedValue * maxX;
	m_pFillSprite->GetTransform()->Translate(fillPos);
}
