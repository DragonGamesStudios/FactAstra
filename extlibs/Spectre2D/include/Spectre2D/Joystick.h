#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <array>
#include <string>

// Hat states

#define SPECTRE_HAT_CENTERED 0
#define SPECTRE_HAT_UP 1
#define SPECTRE_HAT_RIGHT 2
#define SPECTRE_HAT_DOWN 4
#define SPECTRE_HAT_LEFT 8
#define SPECTRE_RIGHT_UP SPECTRE_HAT_UP | SPECTRE_HAT_RIGHT
#define SPECTRE_RIGHT_DOWN SPECTRE_HAT_DOWN | SPECTRE_HAT_RIGHT
#define SPECTRE_LEFT_DOWN SPECTRE_HAT_DOWN | SPECTRE_HAT_LEFT
#define SPECTRE_LEFT_UP SPECTRE_HAT_UP | SPECTRE_HAT_LEFT

// Gamepad buttons

#define	SPECTRE_GAMEPAD_BUTTON_A 0
#define	SPECTRE_GAMEPAD_BUTTON_B 1
#define	SPECTRE_GAMEPAD_BUTTON_X 2
#define	SPECTRE_GAMEPAD_BUTTON_Y 3
#define	SPECTRE_GAMEPAD_BUTTON_LEFT_BUMPER 4
#define	SPECTRE_GAMEPAD_BUTTON_RIGHT_BUMPER 5
#define	SPECTRE_GAMEPAD_BUTTON_BACK 6
#define	SPECTRE_GAMEPAD_BUTTON_START 7
#define	SPECTRE_GAMEPAD_BUTTON_GUIDE 8
#define	SPECTRE_GAMEPAD_BUTTON_LEFT_THUMB 9
#define	SPECTRE_GAMEPAD_BUTTON_RIGHT_THUMB 10
#define	SPECTRE_GAMEPAD_BUTTON_DPAD_UP 11
#define	SPECTRE_GAMEPAD_BUTTON_DPAD_RIGHT 12
#define	SPECTRE_GAMEPAD_BUTTON_DPAD_DOWN 13
#define	SPECTRE_GAMEPAD_BUTTON_DPAD_LEFT 14
#define	SPECTRE_GAMEPAD_BUTTON_LAST SPECTRE_GAMEPAD_BUTTON_DPAD_LEFT
#define	SPECTRE_GAMEPAD_BUTTON_CROSS SPECTRE_GAMEPAD_BUTTON_A
#define	SPECTRE_GAMEPAD_BUTTON_CIRCLE SPECTRE_GAMEPAD_BUTTON_B
#define	SPECTRE_GAMEPAD_BUTTON_SQUARE SPECTRE_GAMEPAD_BUTTON_X
#define	SPECTRE_GAMEPAD_BUTTON_TRIANGLE SPECTRE_GAMEPAD_BUTTON_Y

// Joysticks

#define SPECTRE_JOYSTICK_1 0
#define SPECTRE_JOYSTICK_2 1
#define SPECTRE_JOYSTICK_3 2
#define SPECTRE_JOYSTICK_4 3
#define SPECTRE_JOYSTICK_5 4
#define SPECTRE_JOYSTICK_6 5
#define SPECTRE_JOYSTICK_7 6
#define SPECTRE_JOYSTICK_8 7
#define SPECTRE_JOYSTICK_9 8
#define SPECTRE_JOYSTICK_10 9
#define SPECTRE_JOYSTICK_11 10
#define SPECTRE_JOYSTICK_12 11
#define SPECTRE_JOYSTICK_13 12
#define SPECTRE_JOYSTICK_14 13
#define SPECTRE_JOYSTICK_15 14
#define SPECTRE_JOYSTICK_16 15

// Gamepad axes

#define	SPECTRE_GAMEPAD_AXIS_LEFT_X 0
#define	SPECTRE_GAMEPAD_AXIS_LEFT_Y 1
#define	SPECTRE_GAMEPAD_AXIS_RIGHT_X 2
#define	SPECTRE_GAMEPAD_AXIS_RIGHT_Y 3
#define	SPECTRE_GAMEPAD_AXIS_LEFT_TRIGGER 4
#define	SPECTRE_GAMEPAD_AXIS_RIGHT_TRIGGER 5

namespace sp
{
	struct GamepadState
	{
		std::array<uint8_t, 15> buttons;
		std::array<float_t, 6> axes;
	};

	class Joystick
	{
	public:
		typedef std::function<void(Joystick*)> ConfigurationCallback;

		Joystick(int32_t id);
		~Joystick();

		bool isPresent() const;
		bool isGamepad() const;

		const std::vector<const float_t*>& getAxes() const;
		float_t getAxis(size_t i) const;
		size_t getAxesCount() const;

		const std::vector<const uint8_t*>& getButtons() const;
		uint8_t getButton(size_t i);
		size_t getButtonsCount() const;

		const std::vector<const uint8_t*>& getHats() const;
		uint8_t getHat(size_t i);
		size_t getHatsCount() const;

		void updateGamepadMappings(const std::string& mappings);

		const std::string& getGamepadName();

		int32_t getId() const;

		void onConnected(const ConfigurationCallback& callback);
		const ConfigurationCallback& getOnConnected() const;

		void onDisconnected(const ConfigurationCallback& callback);
		const ConfigurationCallback& getOnDisconnected() const;

		void getGamepadState(GamepadState* state) const;

		static void loadJoysticks();
		static void clearJoysticks();
		static Joystick* getJoystick(int32_t id);

	protected:
		void update();

	private:
		int32_t id;

		std::vector<const float_t*> axes;
		std::vector<const uint8_t*> buttons;
		std::vector<const uint8_t*> hats;

		bool gamepad;

		std::string name;
		std::string gamepadName;
		std::string GUID;

		ConfigurationCallback disconnectedCallback;
		ConfigurationCallback connectedCallback;

		friend class Internal;
	};
}