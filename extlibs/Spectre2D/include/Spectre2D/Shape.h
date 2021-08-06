#pragma once

#include "Mesh.h"
#include "VertexData.h"
#include "Drawable.h"
#include "Transformable.h"

#include <functional>

namespace sp
{
	enum class ShapeType
	{
		Line,
		Fill
	};

	class Shape : public Drawable, public Transformable
	{
	public:
		typedef std::function<std::vector<size_t>(const std::vector<glm::vec2>&)> TriangulationFunc;

		/*
		* Default constructor.
		*
		* @param vdata: The vertex data of the shape.
		* @param _mesh: The mesh of the shape.
		* @param lock: Wether or not to lock the object or not. Locking the shape locks its transforms, not the mesh and vertex data.
		*/
		Shape(VertexData* vdata = nullptr, Mesh* _mesh = nullptr, bool lock = false);

		virtual ~Shape();

		/*
		* Default constructor.
		*
		* @param vdata: The vertex data of the shape.
		* @param _mesh: The mesh of the shape.
		*/
		void create(VertexData* vdata = nullptr, Mesh* _mesh = nullptr);

		/*
		* Creates the shape as a rectangle. Requires mesh and vertex data set.
		* 
		* @param type: The way to draw the rectangle (line/fill).
		* @param size: The width and height of rect as vec2.
		* @returns How the mesh changed when creating.
		*/
		int32_t createRect(ShapeType type, const glm::vec2& size);

		/*
		* Creates the shape as a circle. Requires mesh and vertex data set.
		* 
		* @param type: The way to draw the circle (line/fill).
		* @param radius: The radius of the circle.
		* @param segments: The amount of segments to create the circle from. The more segments the more round the circle is, but also the more triangles it takes.
		* @param sideLength: Pointer to a float_t value. The function fills it with length of the polygon size. Nullptr by default.
		* @returns How the mesh changed when creating.
		*/
		int32_t createCircleSegments(ShapeType type, float_t radius, size_t segments, float_t* sideLength = nullptr);

		/*
		* Creates the shape as a circle. Requires mesh and vertex data set. Adjusts the segment count based of maximum side length.
		*
		* @param type: The way to draw the circle (line/fill).
		* @param radius: The radius of the circle.
		* @param maxSideLength: Maxmimum length in pixels of a side of the created polygon. 5 by default.
		* @param sideLength: Pointer to a float_t value. The function fills it with length of the polygon side. Nullptr by default.
		* @param segments: Pointer to a size_t value. The function fills it with how much segments is used.
		* @returns How the mesh changed when creating.
		*/
		int32_t createCircle(ShapeType type, float_t radius, float_t maxSideLength = 5.0f, float_t* sideLength = nullptr, size_t* segments = nullptr);

		/*
		* Creates the shape as a regular polygon from the given radius. Requires mesh and vertex data set.
		*
		* @param type: The way to draw the polygon (line/fill).
		* @param radius: The radius of the polygon (distance between origin and one of the points).
		* @param segments: The amount of segments to create the polygon from.
		* @param angleOffset: How the first point should be rotated around the origin.
		* @param sideLength: Pointer to a float_t value. The function fills is with length of the polygon side. Nullptr by default.
		* @returns How the mesh changed when creating.
		*/
		int32_t createRegularPolygonRadius(ShapeType type, float_t radius, size_t segments, float_t angleOffset = 0.0f, float_t* sideLength = nullptr);

		/*
		* Creates the shape as a regular polygon from the given side length. Requires mesh and vertex data set.
		*
		* @param type: The way to draw the polygon (line/fill).
		* @param sideLength: Length of one side of the polygon.
		* @param segments: The amount of segments to create the polygon from.
		* @param angleOffset: How the first point should be rotated around the origin.
		* @param radius: Pointer to a float_t value. The function fills is with polygon's radius. Nullptr by default.
		* @returns How the mesh changed when creating.
		*/
		int32_t createRegularPolygonSide(ShapeType type, float_t sideLength, size_t segments, float_t angleOffset, float_t* radius = nullptr);

		/*
		* Creates the shape as a triangle from the given points. Requires mesh and vertex data set.
		* 
		* @param type: The way to draw the triangle (line/fill).
		* @param p0: 1st point of the triangle.
		* @param p1: 2nd point of the triangle.
		* @param p2: 3rd point of the triangle.
		* @returns How the mesh changed when creating.
		*/
		int32_t createTriangle(ShapeType type, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2);

		/*
		* Creates the shape as a set of points.
		* 
		* @param points: The points.
		* @returns How the mesh changed when creating.
		*/
		int32_t createPoints(const std::vector<glm::vec2>& points);

		/*
		* Creates the shape as a continuous line.
		*
		* @param points: The points of the line.
		* @returns How the mesh changed when creating.
		*/
		int32_t createLine(const std::vector<glm::vec2>& points);

		/*
		* Creates the shape as a simple (non-intersecting) convex polygon.
		*
		* @param type: The way to draw the polygon (line/fill).
		* @param points: The points of the polygon.
		* @returns How the mesh changed when creating.
		*/
		int32_t createPolygonConvex(ShapeType type, const std::vector<glm::vec2>& points);

		/*
		* Creates the shape's mesh.
		*
		* @param primitive: The primitive this mesh describes.
		* @param usage: Describes how the data stored by the mesh will be used.
		* @param vertexAmount: The amount of vertices this mesh consists of. 0 by default.
		* @param offset: The offset of vertex indices. This parameter allows storing data for multiple meshes in one vertex data.
		* @param _indices: Vector of indices to set. Empty by default;
		* @param lock: Wether or not the object should be locked upon creation. False by default.
		*/
		void createMesh(
			Primitive primitive = Primitive::Triangles, DataUsage usage = DataUsage::Static, size_t vertexAmount = 0, size_t offset = 0, const std::vector<size_t>& _indices = {}, bool lock = false
		);

		// Creates the vertex data of the shape.
		// Uses a default VertexDataStream.
		//
		// @param usage: The way the data should be used.
		// @param vertexAmount: The amount of vertices to allocate the memory for.
		// @param lock: Wether or not to buffer the data immediately.
		void createVertexData(DataUsage usage = DataUsage::Static, size_t vertexAmount = 0, bool lock = false);

		/*
		* Sets the mesh of the shape.
		*
		* @param _mesh: New mesh.
		*/
		void setMesh(Mesh* _mesh);

		/*
		* Sets the vertex data of the shape.
		*
		* @param vdata: New vertex data.
		*/
		void setVertexData(VertexData* vdata);

		/*
		* Gets the mesh of the shape.
		*
		* @returns Shape's mesh.
		*/
		Mesh* getMesh();

		/*
		* Gets the mesh of the shape.
		*
		* @returns Shape's mesh.
		*/
		const Mesh* getMesh() const;

		/*
		* Gets the vertex data of the shape.
		*
		* @returns Shape's vertex data.
		*/
		VertexData* getVertexData();

		/*
		* Gets the vertex data of the shape.
		*
		* @returns Shape's vertex data.
		*/
		const VertexData* getVertexData() const;

		/*
		* Sets the usage of both mesh and vertex data.
		*
		* @param usage: The way the data is used.
		*/
		void setDataUsage(DataUsage usage);

		/*
		* Gets wether or not the object maintains the mesh pointer.
		*
		* @returns Wether or not the object maintains the mesh pointer.
		*/
		bool getMaintainsMesh() const;

		/*
		* Sets wether or not the object maintains the mesh pointer.
		*
		* @param value: Wether or not the object maintains the mesh pointer.
		*/
		void setMaintainsMesh(bool value);

		/*
		* Sets wether or not the object maintains the vertex data pointer.
		*
		* @param value: Wether or not the object maintains the mesh pointer.
		*/
		bool getMaintainsVertexData() const;

		/*
		* Sets wether or not the object maintains the vertex data pointer.
		*
		* @param value: Wether or not the object maintains the vertex data pointer.
		*/
		void setMaintainsVertexData(bool value);

		/*
		* Draws the shape.
		* 
		* @param target: The target to draw on.
		*/
		void draw(RenderTarget* target) override;

		/*
		* Adjusts the mesh and the vertex data to the given vertex amount.
		*
		* @param vertices: Vertex amount to adjust vertex data to.
		* @param indices: Index amount to adjust mesh to.
		* @returns How the mesh changed (current size - previous size).
		*/
		int32_t adjust(size_t vertices, size_t indices);

		/*
		* Sets wether or not to shift the vertex data when resizing it. This can be used to keep the vertex data with multiple meshes unharmed, when editing one of them. True by default.
		* 
		* @param value: New value.
		*/
		void setShiftOnResize(bool value);

		/*
		* Gets wether or not to shift the vertex data when resizing it. This can be used to keep the vertex data with multiple meshes unharmed, when editing one of them. True by default.
		*
		* @return: Current value.
		*/
		bool getShiftOnResize() const;

	protected:
		void checkObjects();

	private:
		VertexData* vertexData;
		Mesh* mesh;

		bool maintainsVData;
		bool maintainsMesh;
		bool shiftOnReisze;
	};
}