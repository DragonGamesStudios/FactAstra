#pragma once

#include "RenderTarget.h"

namespace sp
{
	class RenderTexture : public RenderTarget
	{
	public:
		// Default constructor.
		RenderTexture();

	private:
		friend class RenderWindow;
	};
}