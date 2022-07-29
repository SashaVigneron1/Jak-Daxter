#include "stdafx.h"
#include "UI_ButtonManager.h"

#include "UI_Button.h"

UI_ButtonManager::UI_ButtonManager(const SceneContext& sceneContext)
	: m_pSelectedButton{nullptr}
{
	auto input = sceneContext.pInput;

	auto inputAction = InputAction(moveUp, InputState::pressed, VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP);
	input->AddInputAction(inputAction);

	inputAction = InputAction(moveDown, InputState::pressed, VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN);
	input->AddInputAction(inputAction);

	inputAction = InputAction(moveLeft, InputState::pressed, VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT);
	input->AddInputAction(inputAction);

	inputAction = InputAction(moveRight, InputState::pressed, VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT);
	input->AddInputAction(inputAction);

	inputAction = InputAction(execute, InputState::pressed, VK_RETURN, -1, XINPUT_GAMEPAD_A);
	input->AddInputAction(inputAction);

}

void UI_ButtonManager::SelectButton(UI_Button* pButton)
{
	for(auto button : m_pButtons)
	{
		button->Select(false, false);
	}

	if (pButton == nullptr)
		return;

	pButton->Select(true, false);
	m_pSelectedButton = pButton;
}

void UI_ButtonManager::Initialize(const SceneContext& /*sceneContext*/)
{
}

void UI_ButtonManager::Update(const SceneContext& sceneContext)
{
	// Input
	auto input = sceneContext.pInput;

	if (input->IsActionTriggered(moveUp))
	{
		if (m_pSelectedButton) m_pSelectedButton->SelectButtonUp();
	}
	if (input->IsActionTriggered(moveDown))
	{
		if (m_pSelectedButton) m_pSelectedButton->SelectButtonDown();
	}
	if (input->IsActionTriggered(moveLeft))
	{
		if (m_pSelectedButton) m_pSelectedButton->SelectButtonLeft();
	}
	if (input->IsActionTriggered(moveRight))
	{
		if (m_pSelectedButton) m_pSelectedButton->SelectButtonRight();
	}

	if (input->IsActionTriggered(execute))
	{
		if (m_pSelectedButton) m_pSelectedButton->Execute();
	}
}
