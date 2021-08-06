#pragma once
#include "LockableObject.h"

#include "../../extlibs/glm/glm/glm.hpp"

#include <vector>

namespace sp
{
	class TransformSeries : public LockableObject
	{
	public:
		/*
		* Default constructor
		* 
		* @param _size: The amount of transform matrices to allcoate.
		* @param lock: Wether or not to lock the object when creating.
		*/
		TransformSeries(size_t _size = 0, bool lock = false);

		/*
		* Gest the series size.
		* 
		* @returns series size.
		*/
		size_t getSize() const;

		/*
		* Sets the series size.
		* 
		* @param _size: New size.
		*/
		void setSize(size_t _size);

		/*
		* Sets the value of the matrix at specified index.
		* 
		* @param index: The matrix to change.
		* @param matrix: New value.
		*/
		void setMatrix(size_t index, const glm::mat4& matrix);

		/*
		* Sets the value of the matrix at specified index.
		*
		* @param index: The matrix to change.
		* @param matrix: New value.
		*/
		void setMatrix(size_t index, const glm::mat3& matrix);

		/*
		* Gets the value of the matrix at specified index.
		*
		* @param index: The matrix to change.
		* @returns The specified matrix.
		*/
		const glm::mat4& getMatrix(size_t index) const;

		/*
		* Sets the matrix at specified index to translation matrix described by the given vector.
		* 
		* @param index: The matrix to change.
		* @param vec: The translation vector.
		*/
		void setTranslation(size_t index, const glm::vec2& vec);

		/*
		* Sets the matrix at specified index to scale matrix described by the given vector.
		*
		* @param index: The matrix to change.
		* @param vec: The scale vector.
		*/
		void setScale(size_t index, const glm::vec2& vec);

		/*
		* Sets the matrix at specified index to rotation matrix described by the given angle.
		*
		* @param index: The matrix to change.
		* @param angle: The angle.
		*/
		void setRotation(size_t index, float_t angle);

		/*
		* Sets the matrix at specified index to x-shear matrix described by the given vector.
		*
		* @param index: The matrix to change.
		* @param factor: The x shear factor.
		*/
		void setShearX(size_t index, float_t factor);

		/*
		* Sets the matrix at specified index to y-shear matrix described by the given vector.
		*
		* @param index: The matrix to change.
		* @param factor: The y shear factor.
		*/
		void setShearY(size_t index, float_t factor);

		/*
		* Calculates the matrix of the entire transform series, if necessary, and returns it.
		* 
		* @returns The matrix of the entire series.
		*/
		const glm::mat4& evaluateTransform();

		/*
		* Gets the inverse transformation of the entire transform series.
		* 
		* @returns Inverse transformation of the series.
		*/
		const glm::mat4& getInverse();

		/*
		* Gets each individual member of the series.
		* 
		* @returns A vector of all matrices.
		*/
		const std::vector<glm::mat4>& getMatrices() const;

		/*
		* Locks or releases the object.
		* Locked object still saves the changes made to the matrices, but does not evaluate the general matrix.
		* Released object will immediately calculate the general matrix, if changes were made while the object was locked.
		* 
		* @param value: Wether or not to lock the object.
		*/
		void setLocked(bool value) override;

	private:
		size_t size;
		std::vector<glm::mat4> matrices;
		glm::mat4 transformed;
		glm::mat4 inverse;
		bool updateNeeded;
	};

	// Standard class for any transformable object. WARNING: Any function of this class will be most likely invalidated as soon as any function of `TransformSeries` is called.
	class Transformable : public TransformSeries
	{
	public:
		Transformable(bool lock = false);

		/*
		* Sets the position of the transformable. 
		*
		* @param vec: New position.
		*/
		void setPosition(const glm::vec2& vec);

		/*
		* Gets the position of the transformable.
		* 
		* @returns Position.
		*/
		const glm::vec2& getPosition() const;

		/*
		* Sets the local origin of the transformable.
		*
		* @param vec: New origin.
		*/
		void setOrigin(const glm::vec2& vec);

		/*
		* Gets the local origin of the transformable.
		*
		* @returns Origin.
		*/
		const glm::vec2& getOrigin() const;

		using TransformSeries::setScale;

		/*
		* Sets the scale of the transformable.
		*
		* @param vec: The scale vector.
		*/
		void setScale(const glm::vec2& vec);

		/*
		* Gets the scale of the transformable.
		*
		* @returns The scale vector.
		*/
		const glm::vec2& getScale() const;

		using TransformSeries::setRotation;

		/*
		* Sets the clockwise rotation of the transformable.
		* NOTE: With a coordinate system with y axis going up, the rotation would be counterclockwise.
		*
		* @param angle: The angle.
		*/
		void setRotation(float_t angle);

		/*
		* Gets the clockwise rotation of the transformable.
		* NOTE: With a coordinate system with y axis going up, the rotation would be counterclockwise.
		*
		* @returns Rotation angle.
		*/
		float_t getRotation() const;

		/*
		* Sets the x and y shear factors of the transformable. 0 is no shear.
		*
		* @param vec: The vector for x and y shear factors.
		*/
		void setShear(const glm::vec2& vec);

		/*
		* Gets the x and y shear factors of the transformable. 0 is no shear.
		*
		* @returns The vector for x and y shear factors.
		*/
		const glm::vec2& getShear() const;

	private:
		glm::vec2 position;
		glm::vec2 origin;
		float_t rotation;
		glm::vec2 scale;
		glm::vec2 shear;
	};
}