#pragma once
#include "ResizableObject.h"
#include "Shader.h"
#include "Drawable.h"
#include "Transformable.h"

namespace sp
{
	class RenderTarget : public virtual ResizableObject, public Transformable
	{
	public:
		/*
		* Default constructor.
		* 
		* @param size: The target's size. {0, 0} by default.
		* @param lock: Wether or not to lock the target's transforms upon creation.
		*/
		RenderTarget(const glm::uvec2& size = { 0, 0 }, bool lock = false);

		/*
		* Creates the target.
		* 
		* @param size: The target's size.
		*/
		void create(const glm::uvec2& size);

		/*
		* Clears the target to the specified background color.
		*/
		void clear();

		/*
		* Sets the target's background color.
		* 
		* @param color: New background color.
		*/
		void setBackgroundColor(const glm::vec4& color);

		/*
		* Gets the target's background color.
		* 
		* @returns Target's background color.
		*/
		const glm::vec4& getBackgroundColor() const;

		/*
		* Gets the current projection matrix. The default projection is orthographical.
		*
		* @returns The current projection matrix.
		*/
		const glm::mat4& getProjection() const;

		/*
		* Sets the current projection matrix. The default projection is orthographical.
		*
		* @param matrix: New projection matrix.
		*/
		virtual void setProjection(const glm::mat4& matrix);

		/*
		* Gets the current view matrix. The default view is identity matrix.
		*
		* @returns The current view matrix.
		*/
		const glm::mat4& getView() const;

		/*
		* Sets the current view matrix. The default view is identity matrix.
		*
		* @param matrix: New view matrix.
		*/
		virtual void setView(const glm::mat4& matrix);

		/*
		* Gets the current model matrix. The default model is identity matrix.
		*
		* @returns The current model matrix.
		*/
		const glm::mat4& getModel() const;

		/*
		* Sets the current model matrix. The default model is identity matrix.
		*
		* @param matrix: New model matrix.
		*/
		virtual void setModel(const glm::mat4& matrix);

		/*
		* Sets the current tint. The default tint opaque white.
		*
		* @param _tint: New tint.
		*/
		virtual void setTint(const glm::vec4& _tint);

		/*
		* Gets the current tint. The default tint opaque white.
		*
		* @returns: Tint as vec4.
		*/
		const glm::vec4& getTint() const;

		/*
		* Sets the object's size.
		*
		* @param size: New object's size.
		*/
		virtual void setSize(const glm::uvec2& size);

		/*
		* Gets the object's size.
		*
		* @returns Object's size.
		*/
		const glm::uvec2& getSize() const;

		/*
		* Sets the size of the target's transform series size.
		* 
		* @param _size: New size.
		*/
		void setTransformSize(size_t _size);

		/*
		* Gets the size of the target's transform series size.
		*
		* @returns Series size.
		*/
		size_t getTransformSize() const;

		/*
		* Sets wether or not to update projection matrix on resize.
		* 
		* @param value: New value.
		*/
		void setUpdateProjection(bool value);

		/*
		* Gets wether or not to update projection matrix on resize.
		*
		* @returns Wether or not to update projection matrix on resize.
		*/
		bool getUpdateProjection() const;

		// Updates size data. Call it if there is no way the window could have "known" about resizing.
		void updateSize();

		/*
		* Draws the specified drawable onto the target.
		*/
		void draw(Drawable* drawable);

		/*
		* Gets wether or not the render target is active. (The draw calls will draw to it).
		* 
		* @returns Wether or not the target is active.
		*/
		bool getRenderActive() const;

		/*
		* Sets the active shader.
		* Warning: If you're activating a program using gluseProgram, make sure to allways call `Shader::setActiveShader(nullptr);` first.
		*
		* @param shader: The shader to activate.
		*/
		void setActiveShader(Shader* shader);

		/*
		* Gets the active shader.
		* Warning: This only keeps track of Spectre2D shaders.
		*
		* @returns The currently active shader.
		*/
		Shader* getActiveShader() const;

		/*
		* Sets the current point size of the target. This value is used in shaders if sp::Primitive::Points is used.
		* 
		* @param value: New point size.
		*/
		void setPointSize(float_t value);

		/*
		* Gets the current point size of the target.
		* 
		* @returns Current point size.
		*/
		float_t getPointSize() const;

		/*
		* Sets the current line width of the target. This value is used if sp::Primitive::Lines is used.
		*
		* @param value: New line width.
		*/
		void setLineWidth(float_t value);

		/*
		* Gets the current line width of the target.
		*
		* @returns Current line width.
		*/
		float_t getLineWidth() const;

		/*
		* Sets wether or not the lines are smooth (anti-aliased) when drawn. This is false by default.
		*
		* @param smooth: Wether or not the lines are smooth.
		*/
		void setSmoothLines(bool smooth);

		/*
		* Gets wether or not the lines are smooth (anti-aliased) when drawn. This is false by default.
		* 
		* @returns Wether or not the lines are smooth.
		*/
		bool getSmoothLines() const;

		protected:
			virtual void setRenderActive(bool value);

	private:
		glm::vec4 backgroundColor;

		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec4 tint;
		Shader* activeShader;

		float_t pointSize;
		float_t lineWidth;

		bool active;

		bool updateProjection;

		bool smoothLines;
	};
}