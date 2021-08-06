#pragma once
#include "../../extlibs/glm/glm/glm.hpp"

#include <memory>
#include <vector>
#include <string>
#include <filesystem>

namespace sp
{
	/*
	* Enum specifying a channel. A channel is one of RGBA values.
	*/
	enum class Channel
	{
		R, G, B, A
	};

	/*
	* Enum specifying compression.
	*/
	enum class Compression
	{
		// Raw RGB/RGBA data
		RGB,
		// Run-length encoded 8bit compression
		RLE8,
		// Run-length encoded 4bit compression
		RLE4,
		// Raw RGB/RGBA data with value positions based on masks
		BITFIELDS,
		// JPEG compression
		JPEG,
		// PNG compression
		PNG
	};

	// Class holding image data in program memory. Allows for fast operations, but can't be displayed.
	class ImageData
	{
	public:
		typedef glm::vec4 value_type;
		typedef std::vector<glm::vec4> data_type;

		/*
		* Default constructor. No data is loaded.
		*/
		ImageData();

		/*
		* Constructs ImageData with width and height, possibly also filling it with the given data. If the data is not the same size, the pixels outside of the image will be cut out and the
		* missing pixels will be filled with transparent black (0, 0, 0, 0).
		* 
		* @param w: The width of image data.
		* @param h: The height of image data.
		* @param _data: The data to fill the image with.
		*/
		ImageData(int32_t w, int32_t h, const std::vector<glm::vec4>& _data = {});
		ImageData(const ImageData& other);
		ImageData(ImageData&& other) noexcept;
		~ImageData();

		ImageData& operator=(const ImageData& other);

		/*
		* Gets the image data width.
		* 
		* @returns The image data width.
		*/
		int32_t getWidth() const;

		/*
		* Gets the image data height.
		*
		* @returns The image data height.
		*/
		int32_t getHeight() const;

		/*
		* Gets the image raw data. The data is the image's rows packed next to each other. Top-to-bottom, left-to-right.
		*
		* @returns The image raw data.
		*/
		const std::vector<glm::vec4>& getRaw() const;

		/*
		* Gets the image data size.
		*
		* @returns The image data size.
		*/
		size_t getSize() const;

		/*
		* Gets the pixel color on coordinates (x, y). The coordinates are relative to topleft corner of the image.
		* 
		* @param x: X coordinate.
		* @param y: Y coordinate.
		* @returns Color of the given pixel.
		*/
		const glm::vec4& getPixel(int32_t x, int32_t y) const;

		/*
		* Gets the pixel color on coordinates (x, y). The coordinates are relative to topleft corner of the image.
		*
		* @param pos: Pixel position.
		* @returns Color of the given pixel.
		*/
		const glm::vec4& getPixel(const glm::ivec2& pos) const;

		/*
		* sets the pixel color on coordinates (x, y). The coordinates are relative to topleft corner of the image.
		*
		* @param x: X coordinate.
		* @param y: Y coordinate.
		* @param color: Color of the given pixel.
		*/
		void setPixel(int32_t x, int32_t y, const glm::vec4& color);

		/*
		* sets the pixel color on coordinates (x, y). The coordinates are relative to topleft corner of the image.
		*
		* @param pos: Pixel position.
		* @param color: Color of the given pixel.
		*/
		void setPixel(const glm::ivec2& pos, const glm::vec4& color);

	private:
		int32_t width;
		int32_t height;

		data_type data;
	};

	/*
	* A static member only class for loading/writing image data in different formats.
	*/
	class GenericImageLoader
	{
	public:
		/*
		* Static function loading the image data on the specified path. If you know the format, specify it. You can also flip the image vertically and/or horizontally.
		* 
		* @param path: The file path of the image.
		* @param format: The format of the image (e.g. "bmp", "png", "jpg"). This parameter is not case-sensitive. If unknown, leave empty. Empty by default.
		* @param flip_vertically: Wether or not to flip the image vertically when loading. False by default. Note, that some formats have to be flipped as they are stored bottom-to-top (Windows bitmap). In this case setting this to `true` disables the flip.
		* @param flip_horizontally: Wether or not to flip the imahe horizontally when loading. False by default.
		* @returns The loaded image data.
		*/
		static ImageData load(const std::filesystem::path& path, const std::string& format = "", bool flip_vertically = false, bool flip_horizontally = false);
		
		/*
		* Static function loading the image data on the specified path. If you know the format, specify it. You can also flip the image vertically and/or horizontally.
		*
		* @param file: The input stream of the image.
		* @param format: The format of the image (e.g. "bmp", "png", "jpg"). This parameter is not case-sensitive. If unknown, leave empty. Empty by default.
		* @param flip_vertically: Wether or not to flip the image vertically when loading. False by default. Note, that some formats have to be flipped as they are stored bottom-to-top (Windows bitmap). In this case setting this to `true` disables the flip.
		* @param flip_horizontally: Wether or not to flip the imahe horizontally when loading. False by default.
		* @returns The loaded image data.
		*/
		static ImageData load(std::istream& file, const std::string& format = "", bool flip_vertically = false, bool flip_horizontally = false);
	};

#pragma pack(push, 1)

	/*
	* See MSDN pages for BITMAPV5HEADER for details.
	*/
	struct BMPColorSpacePoint
	{
		int32_t x;
		int32_t y;
		int32_t z;
	};

	/*
	* See MSDN pages for BITMAPV5HEADER for details.
	*/
	struct BMPColorSpaceEndpoints
	{
		BMPColorSpacePoint r;
		BMPColorSpacePoint g;
		BMPColorSpacePoint b;
	};

	/*
	* The color in the format of Windows bitmap file.
	*/
	struct BMPColor
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		char reserved;
	};

	/*
	* The Windows bitmap file header.
	*/
	struct BMPFileHeader
	{
		// Should always be "BM" (0x4D42)
		uint16_t headerSignature;
		// The size of .bmp file
		uint32_t fileSize;
		// Reserved
		char reserved1[2];
		// Reserved
		char reserved2[2];
		// The offset from the beginning of the file, where the pixel data begins.
		uint32_t pixelDataOffset;
	};

	/*
	* See MSDN pages for BITMAPV5HEADER for details.
	*/
	struct BMPInfoHeader
	{
		uint32_t infoHeaderSize;
		int32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bitCount;
		uint32_t compression;
		uint32_t compressedSize;
		int32_t xPixelsPerMeter;
		int32_t yPixelsPerMeter;
		uint32_t colorsUsed;
		uint32_t colorsImportant;
	};

	/*
	* See MSDN pages for BITMAPV5HEADER for details.
	*/
	struct BMPColorHeader
	{
		uint32_t redMask;
		uint32_t blueMask;
		uint32_t greenMask;
		uint32_t alphaMask;
		uint32_t colorSpaceType;
		BMPColorSpaceEndpoints endpoints;
		uint32_t gammaRed;
		uint32_t gammaGreen;
		uint32_t gammaBlue;
		uint32_t intent;
		uint32_t profileData;
		uint32_t profileSize;
		char reserved[4];
	};

#pragma pack(pop)

	/*
	* A struct holding a BMP file data.
	*/
	struct BMPFile
	{
		// File header
		BMPFileHeader fileHeader;
		// Info header
		BMPInfoHeader infoHeader;
		// Color header
		BMPColorHeader colorHeader;
		// Color table (not used in 16bit, 24bit and 32bit)
		BMPColor* colorTable;

		// Data
		ImageData data;
	};

	/*
	* A static member only class for loading and writing BMP files. Also contains some functions for loading compressed data.
	*/
	class BMPLoader
	{
	public:
		/*
		* Static function loading the image data on the specified path. If you know the format, specify it. You can also flip the image vertically and/or horizontally.
		*
		* @param path: The file path of the image.
		* @param flip_vertically: Wether or not to flip the image vertically when loading. False by default. Note, that some formats have to be flipped as they are stored bottom-to-top (Windows bitmap). In this case setting this to `true` disables the flip.
		* @param flip_horizontally: Wether or not to flip the imahe horizontally when loading. False by default.
		* @param fileStruct: a sp::BMPFile struct pointer this function will fill with data if present. NULL by default.
		* @returns The loaded image data.
		*/
		static ImageData load(const std::filesystem::path& path, bool flipVertically = false, bool flipHorizontally = false, bool loadColorTable = false, BMPFile* fileStruct = nullptr);

		/*
		* Static function loading the image data on the specified path. If you know the format, specify it. You can also flip the image vertically and/or horizontally.
		*
		* @param input: The input stream of the image.
		* @param flipVertically: Wether or not to flip the image vertically when loading. False by default. Note, that some formats have to be flipped as they are stored bottom-to-top (Windows bitmap). In this case setting this to `true` disables the flip.
		* @param flipHorizontally: Wether or not to flip the imahe horizontally when loading. False by default.
		* @param fileStruct: a sp::BMPFile struct pointer this function will fill with data if present. NULL by default.
		* @returns The loaded image data.
		*/
		static ImageData load(std::istream& input, bool filpVertically = false, bool flipHorizontally = false, bool loadColorTable = false, BMPFile* fileStruct = nullptr);

		/*
		* Checks wether the given file is a valid BMP file.
		* 
		* @param path: Path of the file to check.
		* @returns Wether or not the file is valid.
		*/
		static bool isBMP(const std::filesystem::path& path);

		/*
		* Checks wether the given file is a valid BMP file.
		*
		* @param path: Input stream with the image to check.
		* @returns Wether or not the file is valid.
		*/
		static bool isBMP(std::istream& input);

		/*
		* Loads raw uncompressed data into data type accepted by ImageData constructor. Each pixel has to consist of RGBA/RGB values. Supports 32, 24 and 16 bit images.
		* For each missing channel it will set 0.0. For missing alpha channel, however, it will set 1.0 (opaque).
		* 
		* @param input: The input stream containing the data.
		* @param data: Reference to vector of sp::Color to fill with data. The vector does not need to be sized properly, as the function will resize it.
		* @param bytesPerPixel: Byte size of each pixel (4 for 32bit images, 3 for 24bit etc).
		* @param width: The width of the image.
		* @param height: The height of the image.
		* @param redMask: The mask of the red channel. 0x00ff0000 by default (BGRA format used in BMP files).
		* @param greenMask: The mask of the green channel. 0x0000ff00 by default (BGRA format used in BMP files).
		* @param blueMask: The mask of the blue channel. 0x000000ff by default (BGRA format used in BMP files).
		* @param alphaMask: The mask of the alpha channel. 0xff000000 by default (BGRA format used in BMP files).
		* @param flipVertically: Wether or not to flip the image vertically when loading. False by default.
		* @param flipHorizontally: Wether or not to flip the image horizontally when loading. False by default.
		*/
		static void loadUncompressed(
			std::istream& input, std::vector<glm::vec4>& data, uint8_t bytesPerPixel, int32_t width, int32_t height,
			uint32_t redMask =		0x00ff0000,
			uint32_t greenMask =	0x0000ff00,
			uint32_t blueMask =		0x000000ff,
			uint32_t alphaMask =	0xff000000,
			bool flipVertically = false, bool flipHorizontally = false
		);

		/*
		* Loads raw uncompressed data into data type accepted by ImageData constructor. Each pixel is defined by index to color table. Supports 8 and 4 bit images.
		*
		* @param input: The input stream containing the data.
		* @param data: Reference to vector of sp::Color to fill with data. The vector does not need to be sized properly, as the function will resize it.
		* @param colorTable: A table of BMPColor. The pixels contain indices pointing to colors in this table.
		* @param colorTable: The size of color table.
		* @param bpp: Bits per pixel (8 for 8bit, 4 for 4bit)
		* @param width: The width of the image.
		* @param height: The height of the image.
		* @param flipVertically: Wether or not to flip the image vertically when loading. False by default.
		* @param flipHorizontally: Wether or not to flip the image horizontally when loading. False by default.
		*/
		static void loadUncompressed(
			std::istream& input, std::vector<glm::vec4>& data, BMPColor* colorTable, size_t colorTableSize, uint8_t bpp,
			int32_t width, int32_t height, bool flipVertically = false, bool flipHorizontally = false
		);

		/*
		* Loads run-length encoded data into data type accepted by ImageData constructor. Each pixel is defined by index to color table. Supports 8 bit images.
		*
		* @param input: The input stream containing the data.
		* @param data: Reference to vector of sp::Color to fill with data. The vector does not need to be sized properly, as the function will resize it.
		* @param colorTable: A table of BMPColor. The pixels contain indices pointing to colors in this table.
		* @param colorTable: The size of color table.
		* @param width: The width of the image.
		* @param height: The height of the image.
		* @param flipVertically: Wether or not to flip the image vertically when loading. False by default.
		* @param flipHorizontally: Wether or not to flip the image horizontally when loading. False by default.
		*/
		static void loadRLE8Compressed(
			std::istream& input, std::vector<glm::vec4>& data, BMPColor* colorTable, size_t colorTableSize,
			int32_t width, int32_t height, bool flipVertically = false, bool flipHorizontally = false
		);

		/*
		* Loads run-length encoded data into data type accepted by ImageData constructor. Each pixel is defined by index to color table. Supports 4 bit images.
		*
		* @param input: The input stream containing the data.
		* @param data: Reference to vector of sp::Color to fill with data. The vector does not need to be sized properly, as the function will resize it.
		* @param colorTable: A table of BMPColor. The pixels contain indices pointing to colors in this table.
		* @param colorTableSize: The size of color table.
		* @param width: The width of the image.
		* @param height: The height of the image.
		* @param flipVertically: Wether or not to flip the image vertically when loading. False by default.
		* @param flipHorizontally: Wether or not to flip the image horizontally when loading. False by default.
		*/
		static void loadRLE4Compressed(
			std::istream& input, std::vector<glm::vec4>& data, BMPColor* colorTable, size_t colorTableSize,
			int32_t width, int32_t height, bool flipVertically = false, bool flipHorizontally = false
		);

		/*
		* Loads raw pixel data into data type accepted by ImageData constructor. Each pixel is defined by index to color table. Supports 1 bit images.
		*
		* @param input: The input stream containing the data.
		* @param data: Reference to vector of sp::Color to fill with data. The vector does not need to be sized properly, as the function will resize it.
		* @param colorTable: A table of BMPColor. The pixels contain indices pointing to colors in this table.
		* @param colorTableSize: The size of color table.
		* @param width: The width of the image.
		* @param height: The height of the image.
		* @param flipVertically: Wether or not to flip the image vertically when loading. False by default.
		* @param flipHorizontally: Wether or not to flip the image horizontally when loading. False by default.
		*/
		static void load1Bit(
			std::istream& input, std::vector<glm::vec4>& data, BMPColor* colorTable, size_t colorTableSize,
			int32_t width, int32_t height, bool flipVertically = false, bool flipHorizontally = false
		);
	};

}