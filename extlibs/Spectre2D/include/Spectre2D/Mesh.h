#pragma once

#include "ObjectWithBuffer.h"

#include <memory>
#include <vector>
#include <utility>

namespace sp
{
	enum class Primitive
	{
		Points,
		Lines,
		LineStrip,
		LineLoop,
		Triangles,
		TriangleStrip,
		TriangleFan
	};

	class Mesh : public ObjectWithBuffer
	{
	public:
		/*
		* Default constructor.
		* 
		* @param primitive: The primitive this mesh describes.
		* @param usage: Describes how the data stored by the mesh will be used.
		* @param vertexAmount: The amount of vertices this mesh consists of. 0 by default.
		* @param offset: The offset of vertex indices. This parameter allows storing data for multiple meshes in one vertex data.
		* @param _indices: Vector of indices to set. Empty by default;
		* @param lock: Wether or not the object should be locked upon creation. False by default.
		*/
		Mesh(Primitive primitive = Primitive::Triangles, DataUsage usage = DataUsage::Static, size_t vertexAmount = 0, size_t _offset = 0, const std::vector<size_t>& _indices = {}, bool lock = false);

		/*
		* Sets the amount of vertices the mesh consists of. Causes rebuffer.
		* 
		* @param amount: New vertex amount.
		*/
		int32_t setVertexAmount(size_t amount);

		/*
		* Gets the amount of vertices the mesh consists of.
		*
		* @returns Vertex amount.
		*/
		size_t getVertexAmount() const;

		/*
		* Sets the indices of vertices starting at the specified offset.
		* 
		* @param _indices: The values to set.
		* @param offset: The offset to start at. 0 by default. WARNING: This parameter IS NOT the same as the parameter in the constructor!
		*/
		void setIndices(const std::vector<size_t>& _indices, size_t offset = 0);

		/*
		* Gets the vector of indices.
		* 
		* @returns The offseted indices stored by the mesh. To subtract the offset, use getOffset().
		*/
		const std::vector<size_t>& getIndices() const;

		/*
		* Sets the index of the nth vertex.
		* 
		* @param vertexIndex: The vertex to change.
		* @param index: The new index.
		*/
		void setVertexIndex(size_t vertexIndex, size_t index);

		/*
		* Gets the index of nth vertex.
		* 
		* @param vertexIndex: The vertex to query.
		* @returns The index of the vertex.
		*/
		size_t getVertexIndex(size_t vertexIndex) const;

		/*
		* Buffers all the unbuffered data. This is the only function that can buffer data while the object is locked.
		* 
		* @returns Wether or not the data needed to be rebuffered.
		*/
		bool buffer() override;

		/*
		* Sets how much the vertex indices should be offset by. Can be used to have multiple meshes use one vertex data.
		* 
		* @param newOffset: New offset.
		* @param fixIndices: Wether or not to change vertex indices based on the new offset. True by default.
		*/
		void setOffset(size_t newOffset, bool fixIndices = true);

		/*
		* Gets how much the vertex indices should be offset by. Can be used to have multiple meshes use one vertex data.
		*
		* @returns Index offset.
		*/
		size_t getOffset() const;

		/*
		* Defines how the mesh is supposed to be interpreted (what primitive it should draw).
		* 
		* @param _primitive: Primitive to draw.
		*/
		void setPrimitive(Primitive _primitive);

		/*
		* Gets how the mesh is supposed to be interpreted (what primitive it should draw).
		*
		* @returns Primitive to draw.
		*/
		Primitive getPrimitive() const;

		/*
		* Gets raw OpenGL primitive.
		* 
		* @returns OpenGL enum of the primitive.
		*/
		uint32_t getRawPrimitive() const;

		/*
		* Converts Spectre2D primitive enum into OpenGL primitive enum.
		* 
		* @param spPrimitive: Spectre2D enum.
		* @returns OpenGL enum.
		*/
		static uint32_t getOpenGLPrimitive(Primitive spPrimitive);

		/*
		* Converts OpenGL primitive enum into Spectre2D primitive enum.
		*
		* @param glPrimitive: OpenGL enum.
		* @returns Spectre2D enum.
		*/
		static Primitive getSpectrePrimitive(uint32_t openglPrimitive);

	protected:
		void rebuffer() override;

	private:
		std::vector<size_t> indices;

		// request = {index offset, vertex count}
		std::vector<std::pair<size_t, size_t>> bufferRequests;

		uint32_t primitive;

		size_t offset;

	};
}