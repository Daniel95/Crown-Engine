#pragma once

#include "InputCommon.h"

#include <unordered_map>
#include <unordered_set>

namespace Crown {

	using Action = std::string;

	class KeyboardMap
	{
	public:
		void AddBinding(Action a_Action, Key a_Key);
		void RemoveBinding(Action a_Action, Key a_Key);

		void ClearBindings();
		void ClearBindings(Action a_Action);

		ButtonState GetState(Action a_Action) const;
		std::vector<Action> GetActions() const;

	private:
		std::unordered_map<Action, std::vector<Key>> m_ButtonMap;
	};

	class MouseMap
	{
	public:
		void AddBinding(Action a_Action, MouseButton a_Button);
		void RemoveBinding(Action a_Action, MouseButton a_Button);

		void ClearBindings();
		void ClearBindings(Action a_Action);

		ButtonState GetState(Action a_Action) const;
		std::vector<Action> GetActions() const;

	private:
		std::unordered_map<Action, std::vector<MouseButton>> m_ButtonMap;
	};

	class GamepadMap
	{
	public:
		void AddBinding(Action a_Action, Button a_Button);
		void RemoveBinding(Action a_Action, Button a_Button);

		void AddBinding(Action a_Action, Axis a_Axis);
		void RemoveBinding(Action a_Action, Axis a_Axis);
		void ClearBindings(Action a_Action);
		void ClearBindings();

		ButtonState GetState(Action a_Action) const;

		float GetDir(Action a_Action) const;
		float GetAxis(Action a_Action) const;

		std::vector<Action> GetActions() const;

	private:
		std::unordered_map<Action, std::vector<Button>> m_ButtonMap;
		std::unordered_map<Action, std::vector<Axis>> m_AxisMap;
	};

	class ActionMap
	{
	public:
		void AddAction(Action a_Action, MouseButton a_Button);
		void AddAction(Action a_Action, Key a_Button);
		void AddAction(Action a_Action, Button a_Button);
		void AddAction(Action a_Action, Axis a_Button, float a_MinValue = -1.0f, float a_MaxValue = 1.0f);

		void ClearAction(Action a_Action);
		void ClearActions();

		bool IsUp(Action a_Action) const;
		bool IsDown(Action a_Action) const;
		bool IsPressed(Action a_Action) const;
		bool IsReleased(Action a_Action) const;
		ButtonState GetState(Action a_Action) const;

		float GetDir(Action a_Action) const;
		float GetAxis(Action a_Action) const;

		std::unordered_set<Action> GetActions() const;

	private:
		KeyboardMap m_Keyboard;
		MouseMap m_Mouse;
		GamepadMap m_Gamepad;
	};

}