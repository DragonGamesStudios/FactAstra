#pragma once
#include "keycodes.h"

#include <memory>
#include <functional>
#include <string>

// Special defines

#define SPECTRE_DONT_CARE -1

// Error codes

#define SPECTRE_OK 0
#define SPECTRE_NOT_INITIALIZED 1
#define SPECTRE_WINDOW_NOT_CREATED 2
#define SPECTRE_PLATFORM_ERROR 3
#define SPECTRE_INVALID_VALUE 4
#define SPECTRE_NO_ACTIVE_WINDOW 5
#define SPECTRE_UNKNOWN_ERROR 6
#define SPECTRE_INVALID_ENUM 7
#define SPECTRE_MONITOR_NOT_LINKED 8
#define SPECTRE_WINDOW_ACTIVE 9
#define SPECTRE_PIXEL_OUT_OF_BOUNDS 10
#define SPECTRE_UNSUPPORTED_FORMAT 11
#define SPECTRE_COULD_NOT_OPEN_FILE 12
#define SPECTRE_INVALID_FILE 13
#define SPECTRE_SETTING_AFTER_CREATION 14
#define SPECTRE_MISSING_OBJECT 15

// Mouse buttons

#define SPECTRE_MOUSE_BUTTON_LEFT 0
#define SPECTRE_MOUSE_BUTTON_RIGHT 1
#define SPECTRE_MOUSE_BUTTON_MIDDLE 2

namespace sp
{
	/*
	* Error structure. Catch it with `try/catch` clause to capture exceptions the library emits.
	*/
	struct Error
	{
		/*
		* The error code. 0 is OK.
		*/
		uint16_t code;

		/*
		* The more detailed error description.
		*/
		std::string description;

		Error(uint16_t _code, const std::string& _description);
	};

	/*
	* Initializes the library. This must be called before any other functions.
	*/
	void init();

	/*
	* Terminates the library. This must be called at the end of the program. Eventually, after termination, you can re-initialize using `sp::init()`.
	*/
	void finish();

	/*
	* Polls the events. If an event occured, the corresponding callback will be called.
	*/
	void pollEvents();

	/*
	* Waits until an event occurs. As long as there is no event to dispatch, this function blocks the program.
	*/
	void waitForEvents();

	/*
	* Waits until an event occurs or the specified time passes. As long as there is no dispatch and the timeout is not over, this function blocks the program.
	* 
	* @param timeout: The time the function is supposed to wait for events.
	*/
	void waitForEventsWithTimeout(double timeout);

	/*
	* Raises an empty event. This unblocks `sp::waitForEvents()` and `sp::waitForEventsWithTimeout()`.
	*/
	void raiseEmptyEvent();

	/*
	* Checks if raw mouse motion (not scaled, not accelerated) is supported on the current platform.
	* 
	* @returns Boolean specifying wether or not current platform supports raw mouse input.
	*/
	bool rawMouseMotionSupported();
	/*
	* Gets the name of the specified key. This function SHOULD NOT be used for character input. See `sp::Window::onCharInput()` instead.
	* 
	* @param keycode: The code of the key to query.
	* 
	* @returns The name of the key.
	*/
	std::string getKeyName(int32_t keycode);

	/*
	* Gets the platform-specific scancode of the specified key.
	*
	* @param keycode: The code of the key to query.
	*
	* @returns The scancode of the key.
	*/
	int32_t getKeyScancode(int32_t keycode);

	/*
	* Gets the elapsed time since library initialization. Can be used for time measurement. For more advanced time management, see `sp::Clock`.
	* 
	* @returns Elapsed time since library initialization.
	*/
	double_t getTime();

	/*
	* Gets the text stored in the clipboard. If the clipboard is empty or its contents are not convertible to string, returns an empty string.
	* 
	* @returns UTF-8 encode clipboard text.
	*/
	std::string getClipboardText();

	/*
	* Sets the text stored in the clipboard.
	* 
	* @param text: UTF-8 encoded new clipboard text.
	*/
	void setClipboardText(const std::string& text);

	extern bool BIG_ENDIAN;
}