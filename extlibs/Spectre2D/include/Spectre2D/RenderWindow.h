#pragma once

#include "RenderTarget.h"
#include "Window.h"

namespace sp
{
	class RenderTexture;

	class RenderWindow : public Window, public RenderTarget
	{
	public:
		/*
		* Default constructor.
		*/
		RenderWindow();

		/*
		* Creates a window with size, caption. Allows to specify monitor for fullscreen window and sp::Window pointer to share resources with.
		* sp::RenderWindow also allows rendering to it.
		* 
		* @param size: Window's size.
		* @param caption: Window's caption. Empty by default.
		* @param monitor: Monitor for fullscreen. Leave nullptr if you need windowed mode. Nullptr by default.
		* @param share: Window to share resources with. Leave nullptr if you don't want to share resources with any other window. Nullptr by default.
		*/
		RenderWindow(const glm::ivec2& size, const std::string& caption = "", Monitor* monitor = 0, Window* share = 0);

		/*
		* Creates a window with size, caption. Allows to specify monitor for fullscreen window and sp::Window pointer to share resources with.
		* sp::RenderWindow also allows rendering to it.
		*
		* @param size: Window's size.
		* @param caption: Window's caption. Empty by default.
		* @param monitor: Monitor for fullscreen. Leave nullptr if you need windowed mode. Nullptr by default.
		* @param share: Window to share resources with. Leave nullptr if you don't want to share resources with any other window. Nullptr by default.
		*/
		void create(const glm::uvec2& size, const std::string& caption = "", Monitor* monitor = nullptr, Window* share = nullptr);

		/*
		* Sets the active render texture. Any drawing functions will now be rendered to this texture. Use NULL to start rendering to the window again.
		* 
		* @param texture: Texture to render to or NULL to render to the window.
		*/
		void setActiveRenderTexture(RenderTexture* texture = nullptr);

		/*
		* Sets the window's size.
		*
		* @param size: New window's size.
		*/
		void setSize(const glm::uvec2& size) override;

		/*
		* Updates size data. Call it if there is no way the window could have "known" about resizing.
		*/
		void updateSize() override;

		/*
		* Gets the default shader used by the window.
		* 
		* @returns The window's default shader.
		*/
		const Shader& getDefaultShader() const;

	protected:
		void setRenderActive(bool value) override;

	private:
		RenderTexture* activeRenderTexture;
		Shader defaultShader;
	};
}