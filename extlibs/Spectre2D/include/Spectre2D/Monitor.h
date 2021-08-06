#pragma once
#include <memory>
#include <string>
#include <functional>
#include <vector>
#include "../../extlibs/glm/glm/glm.hpp"

struct GLFWmonitor;

namespace sp
{
	class Window;
	class Internal;

	struct VideoMode
	{
		uint32_t width;
		uint32_t height;
		uint32_t redBits;
		uint32_t greenBits;
		uint32_t blueBits;
		uint32_t refreshRate;
	};

	struct GammaRamp
	{
		uint16_t* red;
		uint16_t* blue;
		uint16_t* green;
		size_t size;

		~GammaRamp();
	};

	class Monitor
	{
	public:
		typedef std::function<void(Monitor*)> ConfigurationCallback;

		Monitor(GLFWmonitor* m);
		~Monitor();

		glm::ivec2 getPosition() const;
		glm::ivec2 getWorkAreaPosition() const;
		glm::ivec2 getWorkAreaSize() const;
		glm::ivec2 getPhysicalSize() const;
		glm::ivec2 getContentScale() const;
			
		const std::string& getName() const;

		const VideoMode* getVideoModes(size_t* count) const;
		const VideoMode& getCurrentVideoMode() const;

		void setGamma(float_t gamma);
			
		void setGammaRamp(const GammaRamp* ramp);
		const GammaRamp* getGammaRamp() const;

		static Monitor* getPrimary();
		static const std::vector<Monitor*>& getAll();
		static void loadMonitors();
		static void clearMonitors();

	protected:

	private:
		GLFWmonitor* monitor;

		glm::ivec2 position;
		glm::ivec2 workAreaPosition;
		glm::ivec2 workAreaSize;
		glm::ivec2 physicalSize;
		glm::vec2 contentScale;

		std::string name;

		VideoMode* videoModes;
		size_t videoModeCount;
		VideoMode currentVideoMode;

		GammaRamp* gammaRamp;

		static void onMonitorConnected(const ConfigurationCallback& callback);
		static const ConfigurationCallback& getOnMonitorConnected();

		static void onMonitorDisconnected(const ConfigurationCallback& callback);
		static const ConfigurationCallback& getOnMonitorDisconnected();

		static void deleteMonitor(Monitor* monitor);
		static Monitor* pushMonitor(GLFWmonitor* monitor);

		friend class Monitor;
		friend class Window;
		friend class Internal;

	};
}