#include "stdafx.h"
#include "UI_Button.h"

#include "UI_ButtonManager.h"

UI_Button::UI_Button(UI_ButtonManager* pButtonManager, const std::wstring& defaultPath, const std::wstring& hoveringPath, const std::wstring& pressedPath,
                     XMFLOAT2 pivot, XMFLOAT4 color)
	: m_DefaultSpriteFilePath{ defaultPath }
	, m_HoveringSpriteFilePath{ hoveringPath }
	, m_PressedSpriteFilePath{ pressedPath }
	, m_pSprite{ nullptr }
	, m_Pivot{ pivot }
	, m_Color{ color }
	, m_WasPressed{ false }
	, m_IsExecuting{ false }
	, m_PressedSpriteTimer{ 0.30f }
	, m_AccPressedTime{}
	, m_pButtonManager{ pButtonManager }

	, m_pButtonUp{nullptr}
	, m_pButtonDown{nullptr}
	, m_pButtonLeft{nullptr}
	, m_pButtonRight{nullptr}
{
}

void UI_Button::Update(const SceneContext& sceneContext)
{
	if (m_WasPressed)
	{
		m_AccPressedTime += sceneContext.pGameTime->GetElapsed();
		if (m_AccPressedTime >= m_PressedSpriteTimer)
		{
			m_AccPressedTime = 0.0f;
			m_WasPressed = false;
			m_pSprite->SetTexture(m_DefaultSpriteFilePath);
		}
	}
}

bool UI_Button::IsPressed(const SceneContext& sceneContext)
{
	if (!m_IsActive) 
		return false;

	// KEYBOARD

	// Get Data
	auto& mousePos = sceneContext.pInput->GetMousePosition();
	auto& spriteDimensions = m_pSprite->GetTexture()->GetDimension();
	auto& spritePos = m_pSprite->GetTransform()->GetWorldPosition();
	auto& spriteScale = m_pSprite->GetTransform()->GetScale();
	auto& pivot = m_Pivot;

	// Calculate IsHovering
	float minX = spritePos.x - (pivot.x * spriteDimensions.x * spriteScale.x);
	float maxX = spritePos.x + ((1 - pivot.x) * spriteDimensions.x * spriteScale.x);
	float minY = spritePos.y - ((1 - pivot.y) * spriteDimensions.y * spriteScale.y);
	float maxY = spritePos.y + (pivot.y * spriteDimensions.y * spriteScale.y);

	bool isHovering = (mousePos.x > minX && mousePos.x < maxX) && (mousePos.y > minY && mousePos.y < maxY);
	bool isPressed = isHovering && sceneContext.pInput->IsMouseButton(InputState::released, 1);

	// If Hovering: Show Other Texture
	if (isHovering)
	{
		if (!m_WasPressed) m_pSprite->SetTexture(m_HoveringSpriteFilePath);
	}
	else
	{
		if (!m_WasPressed) m_pSprite->SetTexture(m_DefaultSpriteFilePath);
	}

	// If isPressed: Return True & Show Other Texture
	if (isPressed)
	{
		m_pSprite->SetTexture(m_PressedSpriteFilePath);
		m_WasPressed = true;
		return true;
	}

	// CONTROLLER
	if (m_IsSelected)
	{
		m_pSprite->SetTexture(m_HoveringSpriteFilePath);
		if (m_IsExecuting)
		{
			m_pSprite->SetTexture(m_PressedSpriteFilePath);
			m_WasPressed = true;
			m_IsExecuting = false;
			return true;
		}
		return false;
	}

	

	return false;
}

void UI_Button::Select(bool value, bool shouldNotifyButtonManager)
{
	m_IsSelected = value;
	if (shouldNotifyButtonManager) m_pButtonManager->SelectButton(this);
}

void UI_Button::SelectButtonUp()
{
	if (!m_pButtonUp)
		return;

	m_pButtonManager->SelectButton(m_pButtonUp);
}

void UI_Button::SelectButtonDown()
{
	if (!m_pButtonDown)
		return;

	m_pButtonManager->SelectButton(m_pButtonDown);
}

void UI_Button::SelectButtonLeft()
{
	if (!m_pButtonLeft)
		return;

	m_pButtonManager->SelectButton(m_pButtonLeft);
}

void UI_Button::SelectButtonRight()
{
	if (!m_pButtonRight)
		return;

	m_pButtonManager->SelectButton(m_pButtonRight);
}

void UI_Button::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pSprite = AddComponent(new SpriteComponent(m_DefaultSpriteFilePath, 
		m_Pivot, m_Color));
}
