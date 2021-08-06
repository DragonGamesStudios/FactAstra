#pragma once
#include "core.h"
#include "ObjectWithBuffer.h"

#include "../../extlibs/glm/glm/glm.hpp"
#include "../../extlibs/glm/glm/gtc/type_ptr.hpp"

#include <memory>
#include <vector>
#include <string>
#include <variant>
#include <map>
#include <tuple>
#include <type_traits>

namespace sp
{
	enum class VertexDataType
	{
		Empty,
		Ignore,
		Int,
		Uint,
		Float,
		Double,
		Bool,
		Texture2D,
	};

	struct VertexDataUnit
	{
		// Data type
		VertexDataType type;
		// The amount of vertex attributes described by this unit
		size_t size;
		// Size of a vector / columns of a matrix
		size_t innerSize;
		// Vertex attribute name. If size > 1, the attrib can be referred to as `<unit.name>[<index>]`
		std::string name;
		// Amount of rows in a matrix. 0 is `innerSize`, 1 is `not-a-matrix`. 1 by default.
		size_t matrixRows = 1;

		/*
		* Gets the size in bytes of that unit. E.g. if the unit was:
		* Ivec("myivec", 2) * 3
		* The size would be:
		* 3 * 2 * sizeof(int)
		*
		* @returns The size in bytes of the unit.
		*/
		size_t bytesize() const;

		/*
		* Gets the size in bytes of that unit single value. E.g. if the unit was:
		* Ivec("myivec", 2)
		* The size would be:
		* sizeof(int)
		*
		* @returns The size in bytes of the unit's single value.
		*/
		size_t valueBytesize() const;

		/*
		* Gets the location increase in glsl vertex attributes.
		* 
		* @returns The location increase.
		*/
		size_t locationIncrease() const;
	};

	class VertexDataStream
	{
	public:
		/*
		* Default constructor
		*/
		VertexDataStream();

		/*
		* Adds the units in the given stream to the stream.
		* 
		* @param right: The stream to copy.
		* @returns A reference to itself.
		*/
		VertexDataStream& operator<<(const VertexDataStream& right);

		/*
		* Adds the unit in the given stream to the stream.
		* 
		* @param right: The unit to add.
		* @returns A reference to itself.
		*/
		VertexDataStream& operator<<(const VertexDataUnit& right);

		/*
		* Adds the units in the given stream to the stream.
		*
		* @param right: The stream to copy.
		* @returns A reference to itself.
		*/
		void push(const VertexDataStream& right);

		/*
		* Adds the unit in the given stream to the stream.
		*
		* @param right: The unit to add.
		* @returns A reference to itself.
		*/
		void push(const VertexDataUnit& right);

		/*
		* Clears the units;
		*/
		void clear();

		/*
		* Gets the size in bytes of that stream. E.g. if the stream was: 
		* stream << Float("myfloat") * 3 << Ivec("myivec", 2); 
		* The size would be: 
		* 3 * sizeof(float) + 2 * sizeof(int)
		* 
		* @returns The size in bytes of the stream.
		*/
		size_t bytesize() const;

		/*
		* Gets the unit vector.
		* 
		* @returns The stream's unit vector.
		*/
		const std::vector<VertexDataUnit>& getUnits() const;

		/*
		* Creates a vertex data unit of type int.
		* 
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Int(const std::string& name);

		/*
		* Creates a vertex data unit of type unsigned int.
		*
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Uint(const std::string& name);

		/*
		* Creates a vertex data unit of type float.
		*
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Float(const std::string& name);

		/*
		* Creates a vertex data unit of type double.
		*
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Double(const std::string& name);

		/*
		* Creates a vertex data unit of type bool.
		*
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Bool(const std::string& name);

		/*
		* Creates an empty vertex data unit. Empty VDU does not count as vertex attribute, but it does increase the stride in the data, thus putting some empty space into data buffer. 
		* When used with ignored VDU, it can make Spectre ignore the attribute, but also reserve space for it, so that you can use it however you want. 
		* Note: you **can** write to empty VDUs.
		*
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Empty(const std::string& name);

		/*
		* Creates an ignored vertex data unit of type bool. Ignored VDU does not increase the stride in the data, but it does count as vertex attribute. This increases the VA index.
		* When used with empty VDU, it can make Spectre ignore the attribute, but also reserve space for it, so that you can use it however you want.
		*
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Ignore();

		/*
		* Creates a vertex data unit of type vec (vector of floats with the given size).
		*
		* @param name: The unit's name.
		* @param size: The vector's size.
		* @returns The created unit.
		*/
		static VertexDataUnit Vec(const std::string& name, size_t size);

		/*
		* Creates a vertex data unit of type ivec (vector of ints with the given size).
		*
		* @param name: The unit's name.
		* @param size: The vector's size.
		* @returns The created unit.
		*/
		static VertexDataUnit Ivec(const std::string& name, size_t size);

		/*
		* Creates a vertex data unit of type uvec (vector of unsigned ints with the given size).
		*
		* @param name: The unit's name.
		* @param size: The vector's size.
		* @returns The created unit.
		*/
		static VertexDataUnit Uvec(const std::string& name, size_t size);

		/*
		* Creates a vertex data unit of type dvec (vector of doubles with the given size).
		*
		* @param name: The unit's name.
		* @param size: The vector's size.
		* @returns The created unit.
		*/
		static VertexDataUnit Dvec(const std::string& name, size_t size);

		/*
		* Creates a vertex data unit of type bvec (vector of bools with the given size).
		*
		* @param name: The unit's name.
		* @param size: The vector's size.
		* @returns The created unit.
		*/
		static VertexDataUnit Bvec(const std::string& name, size_t size);

		/*
		* Creates a vertex data unit of type mat (matrix of floats with the given size).
		*
		* @param name: The unit's name.
		* @param columns: The amount of columns the matrix has.
		* @param rows: The amount of rows the matrix has. Set to 0 for the same amount as columns. 0 by default.
		* @returns The created unit.
		*/
		static VertexDataUnit Mat(const std::string& name, size_t columns, size_t rows = 0);

		/*
		* Creates a vertex data unit of type imat (matrix of ints with the given size).
		*
		* @param name: The unit's name.
		* @param columns: The amount of columns the matrix has.
		* @param rows: The amount of rows the matrix has. Set to 0 for the same amount as columns. 0 by default.
		* @returns The created unit.
		*/
		static VertexDataUnit Imat(const std::string& name, size_t columns, size_t rows = 0);

		/*
		* Creates a vertex data unit of type umat (matrix of unsigned ints with the given size).
		*
		* @param name: The unit's name.
		* @param columns: The amount of columns the matrix has.
		* @param rows: The amount of rows the matrix has. Set to 0 for the same amount as columns. 0 by default.
		* @returns The created unit.
		*/
		static VertexDataUnit Umat(const std::string& name, size_t columns, size_t rows = 0);

		/*
		* Creates a vertex data unit of type dmat (matrix of doubles with the given size).
		*
		* @param name: The unit's name.
		* @param columns: The amount of columns the matrix has.
		* @param rows: The amount of rows the matrix has. Set to 0 for the same amount as columns. 0 by default.
		* @returns The created unit.
		*/
		static VertexDataUnit Dmat(const std::string& name, size_t columns, size_t rows = 0);

		/*
		* Creates a vertex data unit of type bmat (matrix of bools with the given size).
		*
		* @param name: The unit's name.
		* @param columns: The amount of columns the matrix has.
		* @param rows: The amount of rows the matrix has. Set to 0 for the same amount as columns. 0 by default.
		* @returns The created unit.
		*/
		static VertexDataUnit Bmat(const std::string& name, size_t columns, size_t rows = 0);

		/*
		* Creates a vertex data unit of type sampler2D.
		*
		* @param name: The unit's name.
		* @returns The created unit.
		*/
		static VertexDataUnit Tex2D(const std::string& name);

	private:
		std::vector<VertexDataUnit> units;
		size_t size;

	};

	typedef std::variant<
		std::vector<double_t>,
		std::vector<float_t>,
		std::vector<bool>,
		std::vector<int32_t>,
		std::vector<uint32_t>
	> VertexAttribVector;

	class VertexData : public ObjectWithBuffer
	{
	public:
		// Default constructor.
		// Uses a default VertexDataStream.
		//
		// @param usage: The way the data should be used.
		// @param vertexAmount: The amount of vertices to allocate the memory for.
		// @param lock: Wether or not to buffer the data immediately.
		VertexData(DataUsage usage = DataUsage::Static, size_t vertexAmount = 0, bool lock = false);

		~VertexData();

		// Defines how to layout the data in the buffer. This should be the same as vertex interface of the shader this is drawn with.
		// Note: setting this after first buffering will recreate the buffer.
		//
		// @param stream: New layout.
		void setLayout(const VertexDataStream& stream);

		// Gets the data layout.
		//
		// @returns The vertex data layout.
		const VertexDataStream& getLayout() const;

		/*
		* Sets the amount of vertices this object holds data for.
		* 
		* @param vertices: The new amount.
		*/
		int32_t setVertexAmount(size_t amount, bool shrink = true, bool ignoreDiff = false);

		/*
		* Gets the amount of vertices this object holds data for.
		*
		* @returns The amount of vertices maintained by this object.
		*/
		size_t getVertexAmount() const;

		/*
		* Gets the size in bytes of the vertex buffer.
		* 
		* @returns Vertex buffer byte size.
		*/
		size_t bytesize() const;

		/*
		* Buffers all the changes made to the data.
		* This function runs when:
		* - The user calls it
		* - Changes are made to the data using for example `setVertexAttribValue()` and the object is not locked.
		* - The object is unlocked and there is non-buffered data, but there is also no need to rebuffer.
		* 
		* @returns Wether or not the data needed to be rebuffered.
		*/
		bool buffer() override;

		/*
		* Gets the OpenGL id of the vertex array object.
		* Warning: If a rebuffer happens, the ID returned by this function before the rebuffer will be invalidated.
		* Functions that cause rebuffer (if there is no lock):
		* - setLocked(false);
		* - setVertexAmount(...);
		* - setLayout(...);
		* - setDataUsage(...);
		*
		* @returns OpenGL ID (GLuint);
		*/
		uint32_t getOpenGLVAO() const;

		/*
		* Sets the value of a vertex attribute for the specified vertex.
		* 
		* @param attrib: Attribute to set.
		* @param vertexIndex: Index of the vertex to modify.
		* @param value: New value.
		* @returns Wether or not the call succeeded.
		*/
		template<typename T>
		typename std::enable_if_t<
			std::disjunction<
				std::is_same<T, bool>,
				std::is_same<T, double_t>,
				std::is_same<T, float_t>,
				std::is_same<T, int32_t>,
				std::is_same<T, uint32_t>
			>::value, bool
		> setVertexAttribValue(const std::string& attrib, size_t vertexIndex, T value, bool ignoreMissing = false);

		/*
		* Sets the value of a vertex attribute for the specified vertex.
		*
		* @param attrib: Attribute to set.
		* @param vertexIndex: Index of the vertex to modify.
		* @param value: New value.
		* @returns Wether or not the call succeeded.
		*/
		template<typename T, size_t _Size, glm::qualifier Q>
		bool setVertexAttribValue(const std::string& attrib, size_t vertexIndex, const glm::vec<_Size, T, Q>& value, bool ignoreMissing = false);

		/*
		* Sets the value of a vertex attribute for the specified vertex.
		*
		* @param attrib: Attribute to set.
		* @param vertexIndex: Index of the vertex to modify.
		* @param value: New value.
		* @returns Wether or not the call succeeded.
		*/
		template<typename T, size_t _Columns, size_t _Rows, glm::qualifier Q>
		bool setVertexAttribValue(const std::string& attrib, size_t vertexIndex, const glm::mat<_Columns, _Rows, T, Q>& value, bool ignoreMissing = false);

		/*
		* Sets the value of a vertex attribute for the specified vertex by a table.
		*
		* @param attrib: Attribute to set.
		* @param vertexIndex: Index of the vertex to modify.
		* @param value: The table of values (pointer).
		* @param size: The size of `value`.
		* @returns Wether or not the call succeeded.
		*/
		template<typename T>
		typename std::enable_if_t<
			std::disjunction<
				std::is_same<T, bool>,
				std::is_same<T, double_t>,
				std::is_same<T, float_t>,
				std::is_same<T, int32_t>,
				std::is_same<T, uint32_t>
			>::value, bool
		> setVertexAttribValue(const std::string& attrib, size_t vertexIndex, const T* value, size_t size, bool ignoreMissing = false);

		/*
		* Gets the value of a vertex attribute for the specified vertex by a table.
		* 
		* @param attrib: Attribute to get.
		* @param vertexIndex: Index of the vertex to query.
		* @returns The vertex attribute value.
		*/
		template<typename T, size_t _Columns, size_t _Rows>
		glm::mat<_Columns, _Rows, T> getVertexAttribValue(const std::string& attrib, size_t vertexIndex) const;

		/*
		* Gets the value of a vertex attribute for the specified vertex by a table.
		*
		* @param attrib: Attribute to get.
		* @param vertexIndex: Index of the vertex to query.
		* @returns The vertex attribute value.
		*/
		template<typename T, size_t _Columns>
		glm::vec<_Columns, T> getVertexAttribValue(const std::string& attrib, size_t vertexIndex) const;

		/*
		* Gets the value of a vertex attribute for the specified vertex by a table.
		*
		* @param attrib: Attribute to get.
		* @param vertexIndex: Index of the vertex to query.
		* @returns The vertex attribute value.
		*/
		template<typename T>
		T getVertexAttribValue(const std::string& attrib, size_t vertexIndex) const;

		/*
		* Checks wether or not a vertex attribute with the given name exists.
		* 
		* @param attrib: Attrib name.
		* @returns Wether or not the VA exists.
		*/
		bool hasVertexAttrib(const std::string& attrib) const;

		/*
		* Sets the position of the vertex.
		* Note: This function requires a vec3 layout unit with name `sp_vertexPosition` to work properly.
		*
		* @param vertexIndex: Vertex to modify.
		* @param position: New position.
		*/
		void setVertexPosition(size_t vertexIndex, const glm::vec2& position);

		/*
		* Sets the position of the vertex.
		* Note: This function requires a vec3 layout unit with name `sp_vertexPosition` to work properly.
		*
		* @param vertexIndex: Vertex to modify.
		* @param position: New position.
		*/
		void setVertexPosition(size_t vertexIndex, const glm::vec3& position);

		/*
		* Gets the position of the vertex.
		* Note: This function requires a vec3 layout unit with name `sp_position` to work properly.
		*
		* @param vertexIndex: Vertex to query.
		* @returns Vertex's position.
		*/
		glm::vec3 getVertexPosition(size_t vertexIndex) const;

		/*
		* Sets the color of the vertex.
		* Note: This function requires a vec4 layout unit with name `sp_vertexColor` to work properly.
		*
		* @param vertexIndex: Vertex to modify.
		* @param color: New color.
		*/
		void setVertexColor(size_t vertexIndex, const glm::vec4& color);

		/*
		* Gets the color of the vertex.
		* Note: This function requires a vec4 layout unit with name `sp_vertexColor` to work properly.
		*
		* @param vertexIndex: Vertex to query.
		* @returns Vertex's color.
		*/
		glm::vec4 getVertexColor(size_t vertexIndex) const;

		/*
		* Sets the UV coords of the vertex.
		* Note: This function requires a vec2 layout unit with name `sp_vertexUV` to work properly.
		*
		* @param vertexIndex: Vertex to modify.
		* @param uv: New UV coords.
		*/
		void setVertexUV(size_t vertexIndex, const glm::vec2& uv);

		/*
		* Gets the UV coords of the vertex.
		* Note: This function requires a vec2 layout unit with name `sp_vertexUV` to work properly.
		*
		* @param vertexIndex: Vertex to query.
		* @returns Vertex's UV coords.
		*/
		glm::vec2 getVertexUV(size_t vertexIndex) const;

		// Binds the object and thus makes it active.
		void bind() const override;

		/*
		* Gets the raw vertex data. The structure is:
		* Every argument's name is mapped to a tuple holding the offset in the data of the attribute, the size of single data unit (n in vecn) and vector variant for the values.
		* 
		* @returns The raw data.
		*/
		const std::map<std::string, std::tuple<size_t, size_t, VertexAttribVector>>& getRaw() const;

		/*
		* Shifts vertex attribute values from the given range to a new index.
		* 
		* @param start: The index of the first vertex to shift.
		* @param _size: How much vertices to shift.
		* @param idnex: New position of the vertices.
		*/
		void shift(size_t start, size_t _size, size_t index);

	protected:
		void rebuffer() override;

	private:
		uint32_t VAO;

		VertexDataStream layout;

		size_t vertexAmount;

		// Values iterator, vertex index
		std::vector<std::pair<std::map<std::string, std::tuple<size_t, size_t, VertexAttribVector>>::iterator, size_t>> bufferRequests;

		// VA name -> { VA offset, VA size (1 for scalar, 2 for vec2, 3 for vec3 etc), VA values }
		std::map<std::string, std::tuple<size_t, size_t, VertexAttribVector>> values;

		void bufferVAO();
	};

	typedef VertexDataStream VDS;
	typedef VertexDataUnit VDU;
	typedef VertexDataType VDT;
	template<typename T>
	inline typename std::enable_if_t<
		std::disjunction<
			std::is_same<T, bool>,
			std::is_same<T, double_t>,
			std::is_same<T, float_t>,
			std::is_same<T, int32_t>,
			std::is_same<T, uint32_t>
		>::value, bool
	> VertexData::setVertexAttribValue(const std::string& attrib, size_t vertexIndex, T value, bool ignoreMissing)
	{
		return setVertexAttribValue(attrib, vertexIndex, &value, ignoreMissing);
	}

	template<typename T, size_t _Size, glm::qualifier Q>
	inline bool VertexData::setVertexAttribValue(const std::string& attrib, size_t vertexIndex, const glm::vec<_Size, T, Q>& value, bool ignoreMissing)
	{
		return setVertexAttribValue(attrib, vertexIndex, glm::value_ptr(value), _Size, ignoreMissing);
	}

	template<typename T, size_t _Columns, size_t _Rows, glm::qualifier Q>
	inline bool VertexData::setVertexAttribValue(const std::string& attrib, size_t vertexIndex, const glm::mat<_Columns, _Rows, T, Q>& value, bool ignoreMissing)
	{
		bool success = true;

		for (size_t i = 0; i < _Rows; i++)
			success &= setVertexAttribValue(attrib + "[" + std::to_string(i) + "]", vertexIndex, value[i]);

		return success;
	}

	template<typename T>
	inline typename std::enable_if_t <
		std::disjunction<
			std::is_same<T, bool>,
			std::is_same<T, double_t>,
			std::is_same<T, float_t>,
			std::is_same<T, int32_t>,
			std::is_same<T, uint32_t>
		>::value, bool
	> VertexData::setVertexAttribValue(const std::string& attrib, size_t vertexIndex, const T* value, size_t size, bool ignoreMissing)
	{
		auto it = values.find(attrib);

		if (it == values.end())
		{
			if (!ignoreMissing)
				throw Error(SPECTRE_INVALID_VALUE, "Requested vertex attribute does not exist.");
			else
				return false;
		}

		std::vector<T>& vec = std::get<std::vector<T>>(std::get<2>(it->second));
		size_t vecSize = std::get<1>(it->second);

		for (size_t i = 0; i < std::min(vecSize, size); i++)
			vec[vertexIndex * vecSize + i] = value[i];

		bufferRequests.push_back({ it, vertexIndex });

		requestBuffer();

		return true;
	}

	template<typename T, size_t _Columns, size_t _Rows>
	inline glm::mat<_Columns, _Rows, T> VertexData::getVertexAttribValue(const std::string& attrib, size_t vertexIndex) const
	{
		glm::mat<_Columns, _Rows, T> matrix;

		for (size_t i = 0; i < _Rows; i++)
			matrix[i] = getVertexAttribValue<T, _Columns>(attrib + "[" + std::to_string(i) + "]", vertexIndex);

		return matrix;
	}

	template<typename T, size_t _Columns>
	inline glm::vec<_Columns, T> VertexData::getVertexAttribValue(const std::string& attrib, size_t vertexIndex) const
	{
		glm::vec<_Columns, T> vector(0);

		auto it = values.find(attrib);

		if (it == values.end())
			throw Error(SPECTRE_INVALID_VALUE, "Requested vertex attribute does not exist.");

		const std::vector<T>& vec = std::get<std::vector<T>>(std::get<2>(it->second));
		size_t vecSize = std::get<1>(it->second);

		for (size_t i = 0; i < std::min(vecSize, _Columns); i++)
			vector[i] = vec[vertexIndex * vecSize + i];

		return vector;
	}

	template<typename T>
	inline T VertexData::getVertexAttribValue(const std::string& attrib, size_t vertexIndex) const
	{
		auto it = values.find(attrib);

		if (it == values.end())
			throw Error(SPECTRE_INVALID_VALUE, "Requested vertex attribute does not exist.");

		const std::vector<T>& vec = std::get<std::vector<T>>(std::get<2>(it->second));
		size_t vecSize = std::get<1>(it->second);

		return vec[vertexIndex * vecSize];
	}

}

sp::VertexDataUnit operator*(const sp::VertexDataUnit& left, int32_t right);