#include "CrownPCH.h"
#include "ActionMap.h"

#include "Core/Application.h"
#include "Input.h"
#include "Keyboard.h"

namespace Crown {

	void KeyboardMap::AddBinding(Action a_Action, Key a_Key)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end())
		{
			bindings->second.push_back(a_Key);
		}
		else
		{
			m_ButtonMap[a_Action] = std::vector<Key>{ a_Key };
		}
	}

	void KeyboardMap::RemoveBinding(Action a_Action, Key a_Key)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end())
		{
			auto& v = bindings->second;
			v.erase(std::remove(v.begin(), v.end(), a_Key), v.end());
		}
	}

	void KeyboardMap::ClearBindings()
	{
		m_ButtonMap.clear();
	}

	void KeyboardMap::ClearBindings(Action a_Action)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) {
			bindings->second.clear();
		}
	}

	ButtonState KeyboardMap::GetState(Action a_Action) const
	{
		auto& keyboard = Application::Get().GetInput()->GetKeyboard();
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) 
		{
			for (auto& b : bindings->second) 
			{
				const auto state = keyboard.GetState(static_cast<unsigned>(b));
				if (state != ButtonState::Up) 
					return state;
			}
			return ButtonState::Up;
		}

		return ButtonState::Up;
	}

	std::vector<Action> KeyboardMap::GetActions() const
	{
		std::vector<Action> actions;
		for (const auto& btn : m_ButtonMap) {
			actions.push_back(btn.first);
		}
		return actions;
	}

	void MouseMap::AddBinding(Action a_Action, MouseButton a_Button)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) 
		{
			bindings->second.push_back(a_Button);
		}
		else 
		{
			m_ButtonMap[a_Action] = std::vector<MouseButton>{ a_Button };
		}
	}

	void MouseMap::RemoveBinding(Action a_Action, MouseButton a_Button)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) 
		{
			auto& v = bindings->second;
			v.erase(std::remove(v.begin(), v.end(), a_Button), v.end());
		}
	}

	void MouseMap::ClearBindings()
	{
		m_ButtonMap.clear();
	}

	void MouseMap::ClearBindings(Action a_Action)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) 
		{
			bindings->second.clear();
		}
	}

	ButtonState MouseMap::GetState(Action a_Action) const
	{
		auto& mouse = Application::Get().GetInput()->GetMouse();
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) 
		{
			for (auto& b : bindings->second) 
			{
				const auto state = mouse.GetState(b);
				if (state != ButtonState::Up) 
					return state;
			}
			return ButtonState::Up;
		}

		return ButtonState::Up;
	}

	std::vector<Action> MouseMap::GetActions() const
	{
		std::vector<Action> actions;
		for (auto& btn : m_ButtonMap) {
			actions.push_back(btn.first);
		}
		return actions;
	}

	void GamepadMap::AddBinding(Action a_Action, Button a_Button)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) {
			bindings->second.push_back(a_Button);
		}
		else {
			m_ButtonMap[a_Action] = std::vector<Button>{ a_Button };
		}
	}

	void GamepadMap::RemoveBinding(Action a_Action, Button a_Button)
	{
		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) {
			auto& v = bindings->second;
			v.erase(std::remove(v.begin(), v.end(), a_Button), v.end());
		}
	}

	void GamepadMap::AddBinding(Action a_Action, Axis a_Axis)
	{
		const auto& bindings = m_AxisMap.find(a_Action);
		if (bindings != m_AxisMap.end()) {
			bindings->second.push_back(a_Axis);
		}
		else {
			m_AxisMap[a_Action] = std::vector<Axis>{ a_Axis };
		}
	}

	void GamepadMap::RemoveBinding(Action a_Action, Axis a_Axis)
	{
		const auto& bindings = m_AxisMap.find(a_Action);
		if (bindings != m_AxisMap.end()) {
			auto& v = bindings->second;
			v.erase(std::remove(v.begin(), v.end(), a_Axis), v.end());
		}
	}

	void GamepadMap::ClearBindings(Action action)
	{
		const auto& buttons = m_ButtonMap.find(action);
		if (buttons != m_ButtonMap.end()) {
			buttons->second.clear();
		}

		const auto& axes = m_AxisMap.find(action);
		if (axes != m_AxisMap.end()) {
			axes->second.clear();
		}
	}
	
	void GamepadMap::ClearBindings()
	{
		m_ButtonMap.clear();
		m_AxisMap.clear();
	}

	ButtonState GamepadMap::GetState(Action a_Action) const
	{
		auto& gamepad = Application::Get().GetInput()->GetGamepad(0);

		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) 
		{
			for (auto& b : bindings->second) 
			{
				const auto state = gamepad.GetButtonState(b);
				if (state != ButtonState::Up)
					return state;
			}
			return ButtonState::Up;
		}

		return ButtonState::Up;
	}

	float GamepadMap::GetDir(Action a_Action) const
	{
		auto& gamepad = Application::Get().GetInput()->GetGamepad(0);

		const auto& bindings = m_ButtonMap.find(a_Action);
		if (bindings != m_ButtonMap.end()) 
		{
			for (auto& b : bindings->second) 
			{
				const auto state = gamepad.GetButtonState(b);
				switch (state) {
				case ButtonState::Up:
				case ButtonState::Released:
					return 0.0f;
				case ButtonState::Down:
				case ButtonState::Pressed:
					return 1.0f;
				}
			}
			return 0.0f;
		}
		return 0.0f;
	}

	float GamepadMap::GetAxis(Action a_Action) const
	{
		auto& gamepad = Application::Get().GetInput()->GetGamepad(0);

		const auto& bindings = m_AxisMap.find(a_Action);
		if (bindings != m_AxisMap.end()) 
		{
			for (auto& b : bindings->second) 
			{
				const auto state = gamepad.GetAxis(b);
				if (state > 0.0f) {
					return state;
				}
			}
			return 0.0f;
		}

		return 0.0f;
	}

	std::vector<Action> GamepadMap::GetActions() const
	{
		std::vector<Action> actions;
		for (auto& btn : m_ButtonMap) {
			actions.push_back(btn.first);
		}
		return actions;
	}

	void ActionMap::AddAction(Action a_Action, MouseButton a_Button)
	{
		m_Mouse.AddBinding(a_Action, a_Button);
	}

	void ActionMap::AddAction(Action a_Action, Key a_Button)
	{
		m_Keyboard.AddBinding(a_Action, a_Button);
	}

	void ActionMap::AddAction(Action a_Action, Button a_Button)
	{
		m_Gamepad.AddBinding(a_Action, a_Button);
	}

	void ActionMap::AddAction(Action a_Action, Axis a_Button, float a_MinValue, float a_MaxValue)
	{
		m_Gamepad.AddBinding(a_Action, a_Button);
	}

	void ActionMap::ClearAction(Action a_Action)
	{
		m_Mouse.ClearBindings(a_Action);
		m_Keyboard.ClearBindings(a_Action);
		m_Gamepad.ClearBindings(a_Action);
	}

	void ActionMap::ClearActions()
	{
		m_Mouse.ClearBindings();
		m_Keyboard.ClearBindings();
		m_Gamepad.ClearBindings();
	}

	bool ActionMap::IsUp(Action a_Action) const
	{
		return m_Gamepad.GetState(a_Action) == ButtonState::Up
			|| m_Keyboard.GetState(a_Action) == ButtonState::Up
			|| m_Mouse.GetState(a_Action) == ButtonState::Up;

	}

	bool ActionMap::IsDown(Action a_Action) const
	{
		return m_Gamepad.GetState(a_Action) == ButtonState::Down
			|| m_Keyboard.GetState(a_Action) == ButtonState::Down
			|| m_Mouse.GetState(a_Action) == ButtonState::Down;
	}

	bool ActionMap::IsPressed(Action a_Action) const
	{
		return m_Gamepad.GetState(a_Action) == ButtonState::Pressed
			|| m_Keyboard.GetState(a_Action) == ButtonState::Pressed
			|| m_Mouse.GetState(a_Action) == ButtonState::Pressed;
	}

	bool ActionMap::IsReleased(Action a_Action) const
	{
		return m_Gamepad.GetState(a_Action) == ButtonState::Released
			|| m_Keyboard.GetState(a_Action) == ButtonState::Released
			|| m_Mouse.GetState(a_Action) == ButtonState::Released;
	}

	ButtonState ActionMap::GetState(Action a_Action) const
	{
		const auto gamepad = m_Gamepad.GetState(a_Action);
		if (gamepad != ButtonState::Up) return gamepad;
		
		const auto keyboard = m_Keyboard.GetState(a_Action);
		if (keyboard != ButtonState::Up) return keyboard;
		
		const auto mouse = m_Mouse.GetState(a_Action);
		if (mouse != ButtonState::Up) return mouse;

		return ButtonState::Up;
	}

	float ActionMap::GetDir(Action a_Action) const
	{
		return Sign(m_Gamepad.GetAxis(a_Action));
	}

	float ActionMap::GetAxis(Action a_Action) const
	{
		return m_Gamepad.GetAxis(a_Action);
	}

	std::unordered_set<Action> ActionMap::GetActions() const
	{
		std::unordered_set<Action> actions;
		for (const auto &action : m_Keyboard.GetActions()) actions.emplace(action);
		for (const auto &action : m_Mouse.GetActions()) actions.emplace(action);
		for (const auto &action : m_Gamepad.GetActions()) actions.emplace(action);
		return actions;
	}

}
