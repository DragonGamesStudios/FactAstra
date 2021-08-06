#pragma once

#include "../../extlibs/glm/glm/glm.hpp"

namespace sp
{
	class RenderTarget;

	class Drawable
	{
	public:
		Drawable();

		/*
		* Draws the drawable.
		* 
		* @param target: The target to draw on.
		*/
		virtual void draw(RenderTarget* target) = 0;

		/*
		* Gets the drawable's tint.
		* 
		* @returns The drawable's tint as vec4.
		*/
		const glm::vec4& getTint() const;

		/*
		* Sets the drawable's tint.
		*
		* @param _tint: The drawable's tint as vec4.
		*/
		void setTint(const glm::vec4& _tint);

	private:
		glm::vec4 tint;
	};
}