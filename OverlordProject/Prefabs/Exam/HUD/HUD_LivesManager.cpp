#include "stdafx.h"
#include "HUD_LivesManager.h"

#include "HUD_LifeIcon.h"

HUD_LivesManager::HUD_LivesManager(int maxHealth, const std::wstring& emptyPath, const std::wstring& filledPath,
                                   XMFLOAT2 pivot, XMFLOAT4 color)
	: m_MaxHealth{ maxHealth }
	, m_EmptyFilePath{ emptyPath }
	, m_FilledFilePath{ filledPath }
	, m_Pivot{ pivot }
	, m_Color{ color }
	, m_HeartOffset{ 80 }
	, m_Scale{ 0.35f}
{
}

void HUD_LivesManager::SetHealth(int value)
{
	for (size_t i = 0; i < m_MaxHealth; i++)
	{
		bool isFilled = (i < value);
		m_pHearts[i]->FillHart(isFilled);
	}
}

void HUD_LivesManager::Initialize(const SceneContext&)
{
	for (size_t i = 0; i < m_MaxHealth; i++)
	{
		// Create Heart Object
		auto heartObj = AddChild(new HUD_LifeIcon(m_EmptyFilePath, m_FilledFilePath, m_Pivot, m_Color));
		heartObj->GetTransform()->Translate(i * m_HeartOffset, 0, 0);
		heartObj->GetTransform()->Scale(m_Scale);
		m_pHearts.push_back(heartObj);
	}
}
