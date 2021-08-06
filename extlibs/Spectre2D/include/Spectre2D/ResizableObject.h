#pragma once

#include "../../extlibs/glm/glm/glm.hpp"

#include <memory>

namespace sp
{
	// Base class for all resizable objects with size defined with 32bit uints.
	class ResizableObject
	{
	public:
		/*
		* Default constructor.
		* 
		* @param size: The object's size. {0, 0} by default.
		*/
		ResizableObject(const glm::uvec2& size = { 0, 0 });

		/*
		* Gets the object's size.
		* 
		* @returns The object's size.
		*/
		const glm::uvec2& getSize() const;

		/*
		* Sets the object's size.
		* 
		* @param size: New object's size.
		*/
		virtual void setSize(const glm::uvec2& size);

	private:
		glm::uvec2 size;

		friend class Internal;
	};
}