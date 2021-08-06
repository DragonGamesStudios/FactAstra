#pragma once
#include <filesystem>
#include "../../extlibs/glm/glm/glm.hpp"

#define SPECTRE_ARROW_CURSOR 0x00036001
#define SPECTRE_IBEAM_CURSOR 0x00036002
#define SPECTRE_CROSSHAIR_CURSOR 0x00036003
#define SPECTRE_HAND_CURSOR 0x00036004
#define SPECTRE_HRESIZE_CURSOR 0x00036005
#define SPECTRE_VRESIZE_CURSOR 0x00036006

struct GLFWcursor;

namespace sp
{
	class ImageData;
	class Window;

	/*
	* Cursor class. Provides functions for creating custom cursors and using system cursors.
	*/
	class Cursor
	{
	public:
		/*
		* Default constructor. Creates cursor from platform-specific shapes. Creates arrow cursor if no arguments are given.
		* 
		* @param shape: The shape of requested cursor.
		*/
		Cursor(int32_t shape = SPECTRE_ARROW_CURSOR);

		/*
		* Creates a cursor from the given file.
		*
		* @param path: The image file path.
		* @param hotspot: The cursor hotspot coordinates relative to its top-left corner. Hotspot is the exact place the system gets cursor coordinates from.
		*/
		Cursor(const std::filesystem::path& path, const glm::ivec2& hotspot);

		/*
		* Creates a cursor from the given image data.
		*
		* @param path: The image data to create cursor from.
		* @param hotspot: The cursor hotspot coordinates relative to its top-left corner. Hotspot is the exact place the system gets cursor coordinates from.
		*/
		Cursor(const ImageData& data, const glm::ivec2& hotspot);

		~Cursor();

		/*
		* Creates a cursor from the given file.
		*
		* @param path: The image file path.
		* @param hotspot: The cursor hotspot coordinates relative to its top-left corner. Hotspot is the exact place the system gets cursor coordinates from.
		*/
		void create(const std::filesystem::path& path, const glm::ivec2& hotspot);

		/*
		* Creates a cursor from the given image data.
		*
		* @param path: The image data to create cursor from.
		* @param hotspot: The cursor hotspot coordinates relative to its top-left corner. Hotspot is the exact place the system gets cursor coordinates from.
		*/
		void create(const ImageData& data, const glm::ivec2& hotspot);

		/*
		* Default constructor. Creates cursor from platform-specific shapes.
		*
		* @param shape: The shape of requested cursor.
		*/
		void create(int32_t shape);

	private:
		GLFWcursor* cursor;
		friend class Window;
	};
}