#pragma once
#include "ResizableObject.h"

#include <memory>
#include <map>
#include <string>
#include <functional>
#include <filesystem>

struct GLFWwindow;

namespace sp
{
	class Monitor;
	class Internal;
	class Cursor;
	class ImageData;

	enum class ContextCreationAPIEnum
	{
		Native,
#ifndef __APPLE__
		Egl,
#endif
		OSMesa
	};
	
	enum class WindowFlagB
	{
		Focused,
		Resizable,
		Decorated,
		AutoIconify,
		Floating,
		FocusOnShow,
		Visible,
		Maximized,
		CenterCursor,
		TransparentFramebuffer,
		ScaleToMonitor,
		Stereo,
		SRGBCapable,
		ForwardCompatible,
		DebugContext,
		CocoaRetinaFramebuffer,
		CocoaGraphicsSwitching
	};

	enum class WindowFlagI
	{
		Samples,
		RefreshRate
	};

	enum class WindowFlagS
	{
		CocoaFrameName,
		X11ClassName,
		X11InstanceName
	};

	struct WindowFlagSet
	{
		WindowFlagSet();
		~WindowFlagSet();

		std::map<WindowFlagB, bool> bflags;
		std::map<WindowFlagI, int32_t> iflags;
		std::map<WindowFlagS, std::string> sflags;

		ContextCreationAPIEnum contextCreationAPI;

		void setFlag(WindowFlagB flag, bool value);
		void setFlag(WindowFlagI flag, int32_t value);
		void setFlag(WindowFlagS flag, const std::string& value);
		void setFlag(ContextCreationAPIEnum value);

		bool getFlag(WindowFlagB flag) const;
		int32_t getFlag(WindowFlagI flag) const;
		const std::string& getFlag(WindowFlagS flag) const;
		ContextCreationAPIEnum getContextCreationAPI() const;
	};

	enum class CursorMode
	{
		Normal,
		Hidden,
		Disabled
	};

	class  Window : public virtual ResizableObject
	{
	public:
		typedef std::function<void(Window*, const glm::uvec2&)> ResizeCallback;

		typedef std::function<void(Window*)> ClosedCallback;
		typedef std::function<void(Window*)> RefreshedCallback;

		typedef std::function<void(Window*)> FocusGainedCallback;
		typedef std::function<void(Window*)> FocusLostCallback;

		typedef std::function<void(Window*)> IconifyCallback;
		typedef std::function<void(Window*)> DeiconifyCallback;

		typedef std::function<void(Window*)> MaximizeCallback;
		typedef std::function<void(Window*)> DemaximizeCallback;

		// void(window, newSize)
		typedef std::function<void(Window*, const glm::ivec2&)> FramebufferResizeCallback;
		// void(window, newScale)
		typedef std::function<void(Window*, const glm::vec2&)> ContentScaleChangeCallback;

		// void(window, button, mods, repeat)
		typedef std::function<void(Window*, int32_t, int32_t, uint32_t)> MousePressedCallback;
		// void(window, button, mods, repeat)
		typedef std::function<void(Window*, int32_t, int32_t, uint32_t)> MouseReleasedCallback;
		// void(window, newPosition, positionIncrease)
		typedef std::function<void(Window*, const glm::dvec2&, const glm::dvec2&)> MouseMovedCallback;

		typedef std::function<void(Window*)> MouseEnteredCallback;
		typedef std::function<void(Window*)> MouseLeftCallback;

		// void(window, newPosition, positionIncrease)
		typedef std::function<void(Window*, const glm::dvec2&, const glm::dvec2&)> MouseScrolledCallback;

		// void(window, key, scancode, mods, repeat)
		typedef std::function<void(Window*, int32_t, int32_t, int32_t, uint32_t)> KeyPressedCallback;
		// void(window, key, scancode, mods, repeat)
		typedef std::function<void(Window*, int32_t, int32_t, int32_t, uint32_t)> KeyReleasedCallback;
		// void(window, key, scancode, mods, repeat)
		typedef std::function<void(Window*, int32_t, int32_t, int32_t, uint32_t)> KeyHeldCallback;

		// void(window, unicodeCode)
		typedef std::function<void(Window*, uint32_t)> CharInputCallback;

		// void(window, paths)
		typedef std::function<void(Window*, const std::vector<std::filesystem::path>&)> FileDroppedCallback;

		Window();
		Window(const glm::uvec2& size, const std::string& caption = "", Monitor* monitor = nullptr, Window* share = nullptr);

		virtual void create(const glm::uvec2& size, const std::string& caption = "", Monitor* monitor = nullptr, Window* share = nullptr);
		bool isCreated() const;

		bool isActive() const;
		void setActive(bool active);

		bool isVSyncEnabled() const;
		void setVSyncEnabled(bool value);

		bool shouldClose() const;
		void setShouldClose(bool value);

		const std::string& getCaption() const;
		void setCaption(const std::string& caption);

		void close();

		int32_t getSwapInterval() const;
		void setSwapInterval(int32_t interval);

		bool extensionSupported(const char* name);

		void* getOpenGLProcAddress(const char* name);

		void getOpenGLVersion(int32_t* major, int32_t* minor) const;
		void setOpenGLVersion(int32_t major, int32_t minor);

		void setFlag(WindowFlagB flag, bool value);
		void setFlag(WindowFlagI flag, int32_t value);
		void setFlag(WindowFlagS flag, const std::string& value);
		void setFlag(ContextCreationAPIEnum value);

		void setDefaultFlags();

		bool getFlag(WindowFlagB flag) const;
		int32_t getFlag(WindowFlagI flag) const;
		const std::string& getFlag(WindowFlagS flag) const;
		ContextCreationAPIEnum getContextCreationAPI() const;

		void setIcon(const std::vector<ImageData>& icons);
		void setIcon(const std::vector<std::filesystem::path>& icons);
		void restoreDefaultIcon();

		const glm::ivec2& getPosition() const;
		void setPosition(const glm::ivec2& pos);

		void setSizeLimits(const glm::uvec2& minSize, const glm::uvec2& maxSize);

		virtual void setSize(const glm::uvec2& size) override;

		void setAspectRatio(int32_t nominator, int32_t denominator);

		void getFrameSize(int32_t* top, int32_t* right, int32_t* bottom, int32_t* left);

		float getOpacity() const;
		void setOpacity(float opacity);

		void iconify();
		bool isIconified() const;

		void restore();

		void maximize();
		bool isMaximized() const;

		void show();
		void hide();

		void focus(bool value = true);
		bool isFocused() const;

		bool isVisible() const;

		bool isResizable() const;
		void setResizable(bool value);

		bool isHovered() const;

		bool isDecorated() const;
		void setDecorated(bool value);

		bool isAutoIconfied() const;
		void setAutoIconify(bool value);

		bool isFloating() const;
		void setFloating(bool value);

		bool hasTransparentFramebuffer() const;

		bool doesFocusOnShow() const;
		void setFocusOnShow(bool value);

		void requestAttention();

		Monitor* getMonitor() const;
		void setMonitor(Monitor* monitor, const glm::ivec2& pos, const glm::uvec2& size, int32_t refreshRate);

		const glm::ivec2& getPixelSize() const;

		void onResized(const ResizeCallback& callback);
		const ResizeCallback& getOnResized() const;

		void onClosed(const ClosedCallback& callback);
		const ClosedCallback& getOnClosed() const;

		void onRefreshed(const RefreshedCallback& callback);
		const RefreshedCallback& getOnRefreshed() const;

		void onFocusGained(const FocusGainedCallback& callback);
		const FocusGainedCallback& getOnFocusGained() const;

		void onFocusLost(const FocusLostCallback& callback);
		const FocusLostCallback& getOnFocusLost() const;

		void onIconified(const IconifyCallback& callback);
		const IconifyCallback& getOnIconify() const;

		void onDeiconified(const DeiconifyCallback& callback);
		const DeiconifyCallback& getOnDeiconified() const;

		void onMaximized(const MaximizeCallback& callback);
		const MaximizeCallback& getOnMaximized() const;

		void onDemaximized(const DemaximizeCallback& callback);
		const DemaximizeCallback& getOnDemaximized() const;

		void onFramebufferResized(const FramebufferResizeCallback& callback);
		const FramebufferResizeCallback& getOnFramebufferResized() const;

		void onContentScaleChanged(const ContentScaleChangeCallback& callback);
		const ContentScaleChangeCallback& getOnContentScaleChanged() const;

		void onMousePressed(const MousePressedCallback& callback);
		const MousePressedCallback& getOnMousePressed() const;

		void onMouseReleased(const MouseReleasedCallback& callback);
		const MouseReleasedCallback& getOnMouseReleased() const;

		void onMouseMoved(const MouseMovedCallback& callback);
		const MouseMovedCallback& getOnMouseMoved() const;

		void onMouseScrolled(const MouseScrolledCallback& callback);
		const MouseScrolledCallback& getOnMouseScrolled() const;

		void onMouseEntered(const MouseEnteredCallback& callback);
		const MouseEnteredCallback& getOnMouseEntered() const;

		void onMouseLeft(const MouseLeftCallback& callback);
		const MouseLeftCallback& getOnMouseLeft() const;

		void onKeyPressed(const KeyPressedCallback& callback);
		const KeyPressedCallback& getOnKeyPressed() const;

		void onKeyReleased(const KeyReleasedCallback& callback);
		const KeyReleasedCallback& getOnKeyReleased() const;

		void onKeyHeld(const KeyHeldCallback& callback);
		const KeyHeldCallback& getOnKeyHeld() const;

		void onCharInput(const CharInputCallback& callback);
		const CharInputCallback& getOnCharInput() const;

		void onFileDropped(const FileDroppedCallback& callback);
		const FileDroppedCallback& getOnFileDropped() const;

		double_t getRepeatedEventInterval() const;
		void setRepeatedEventInterval(double_t interval);

		const glm::dvec2& getMousePosition() const;
		void setMousePosition(double x, double y);
		void setMousePosition(const glm::dvec2& position);

		const glm::dvec2& getScrollPosition() const;

		void flip();

		bool getStickyMouseButtons() const;
		void setStickyMouseButtons(bool value);

		bool getStickyKeys() const;
		void setStickyKeys(bool value);

		bool getKeyModsLocked() const;
		void setKeyModsLocked(bool value);

		bool getRawMouseMotion() const;
		void setRawMouseMotion(bool value);

		CursorMode getCursorMode() const;
		void setCursorMode(CursorMode mode);

		bool keyPressed(int32_t keycode) const;
		bool mousePressed(int32_t button) const;

		void setCursor(const Cursor& cursor);

		/*
		* Updates size data. Call it if there is no way the window could have "known" about resizing.
		*/
		virtual void updateSize();

		static Window* getActiveWindow();

	protected:
		GLFWwindow* getGLFWWindow();

	private:
		glm::ivec2 framebufferSize;

		std::string caption;

		GLFWwindow* wnd;

		int32_t swapInterval;

		bool ext_WGL_EXT_swap_control_tear;
		bool ext_GLX_EXT_swap_control_tear;

		bool vsync;

		double_t repeatedEventInterval;

		int32_t opengl_major;
		int32_t opengl_minor;

		glm::ivec2 position;

		WindowFlagSet flagSet;

		ResizeCallback resizeCallback;

		ClosedCallback closedCallback;
		RefreshedCallback refreshedCallback;

		FocusGainedCallback focusGainedCallback;
		FocusLostCallback focusLostCallback;

		IconifyCallback iconifyCallback;
		DeiconifyCallback deiconifyCallback;

		MaximizeCallback maximizeCallback;
		DemaximizeCallback demaximizeCallback;

		FramebufferResizeCallback framebufferResizeCallback;
		ContentScaleChangeCallback contentScaleChangeCallback;

		MousePressedCallback mousePressedCallback;
		MouseReleasedCallback mouseReleasedCallback;
		MouseMovedCallback mouseMovedCallback;

		double_t lastMousePressTime;
		double_t lastMouseReleaseTime;

		uint32_t mousePressRepeat;
		uint32_t mouseReleaseRepeat;

		glm::dvec2 mousePosition;
		glm::dvec2 scrollPosition;

		MouseScrolledCallback mouseScrolledCallback;

		MouseEnteredCallback mouseEnteredCallback;
		MouseLeftCallback mouseLeftCallback;

		KeyPressedCallback keyPressedCallback;
		KeyReleasedCallback keyReleasedCallback;
		KeyHeldCallback keyHeldCallback;

		double_t lastKeyPressTime;
		double_t lastKeyReleaseTime;

		uint32_t keyPressRepeat;
		uint32_t keyReleaseRepeat;
		uint32_t keyHeldRepeat;

		CharInputCallback charInputCallback;

		FileDroppedCallback fileDroppedCallback;

		friend class Window;
		friend class Internal;
	};
}