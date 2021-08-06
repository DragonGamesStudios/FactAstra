#pragma once
#include "core.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace sp
{
	class Internal
	{
	public:
		static void onResized(GLFWwindow* window, int32_t width, int32_t height);
		static void onClosed(GLFWwindow* window);
		static void onRefreshed(GLFWwindow* window);
		static void onFocusChanged(GLFWwindow* window, int32_t focused);
		static void onIconifiedChanged(GLFWwindow* window, int32_t iconified);
		static void onMaximizedChanged(GLFWwindow* window, int32_t maximized);
		static void onFramebufferResized(GLFWwindow* window, int32_t width, int32_t height);
		static void onContentScaleChanged(GLFWwindow* window, float_t xscale, float_t yscale);

		static void onMouseButtonChanged(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
		static void onMouseMoved(GLFWwindow* window, double_t x, double_t y);
		static void onMouseEntered(GLFWwindow* window, int32_t entered);
		static void onMouseScrolled(GLFWwindow* window, double_t x, double_t y);
		static void onKeyChanged(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
		static void onCharInput(GLFWwindow* window, uint32_t code);
		static void onFileDropped(GLFWwindow* window, int32_t pathCount, const char* paths[]);

		static void onMonitorConfigurationChanged(GLFWmonitor* monitor, int32_t event);
		static void onJoystickConfigurationChanged(int32_t id, int32_t event);
	};

	void _onResized(GLFWwindow* window, int32_t width, int32_t height);
	void _onClosed(GLFWwindow* window);
	void _onRefreshed(GLFWwindow* window);
	void _onFocusChanged(GLFWwindow* window, int32_t focused);
	void _onIconifiedChanged(GLFWwindow* window, int32_t iconified);
	void _onMaximizedChanged(GLFWwindow* window, int32_t maximized);
	void _onFramebufferResized(GLFWwindow* window, int32_t width, int32_t height);
	void _onContentScaleChanged(GLFWwindow* window, float_t xscale, float_t yscale);

	void _onMouseButtonChanged(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
	void _onMouseMoved(GLFWwindow* window, double_t x, double_t y);
	void _onMouseEntered(GLFWwindow* window, int32_t entered);
	void _onMouseScrolled(GLFWwindow* window, double_t x, double_t y);
	void _onKeyChanged(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
	void _onCharInput(GLFWwindow* window, uint32_t code);
	void _onFileDropped(GLFWwindow* window, int32_t pathCount, const char* paths[]);

	void _onMonitorConfigurationChanged(GLFWmonitor* monitor, int32_t action);
	void _onJoystickConfigurationChanged(int32_t id, int32_t action);
}