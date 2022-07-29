#include "stdafx.h"
#include "HUD_LifeIcon.h"

HUD_LifeIcon::HUD_LifeIcon(const std::wstring& emptyPath, const std::wstring& filledPath, XMFLOAT2 pivot, XMFLOAT4 color)
	: m_FilledFilePath{ filledPath }
	, m_EmptyFilePath{ emptyPath }
	, m_pSprite{ nullptr }
	, m_Pivot{ pivot }
	, m_Color{ color }
{
}

void HUD_LifeIcon::FillHart(bool value)
{
	if (value)
		m_pSprite->SetTexture(m_FilledFilePath);
	else
		m_pSprite->SetTexture(m_EmptyFilePath);
}

void HUD_LifeIcon::Initialize(const SceneContext&)
{
	m_pSprite = AddComponent(new SpriteComponent(m_FilledFilePath,
		m_Pivot, m_Color));
}

void HUD_LifeIcon::Update(const SceneContext&)
{
}
