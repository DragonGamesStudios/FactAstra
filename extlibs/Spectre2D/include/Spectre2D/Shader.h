#pragma once

#include "VertexData.h"
#include "core.h"

#include "../../extlibs/glm/glm/glm.hpp"
#include "../../extlibs/glm/glm/gtc/type_ptr.hpp"

#include <filesystem>
#include <string>
#include <map>
#include <variant>
#include <fstream>

namespace sp
{
	typedef std::variant<
		float_t,
		glm::vec2,
		glm::vec3,
		glm::vec4,
		int32_t,
		glm::ivec2,
		glm::ivec3,
		glm::ivec4,
		uint32_t,
		glm::uvec2,
		glm::uvec3,
		glm::uvec4,
		glm::mat2,
		glm::mat3,
		glm::mat4,
		glm::mat2x3,
		glm::mat3x2,
		glm::mat2x4,
		glm::mat4x2,
		glm::mat3x4,
		glm::mat4x3
	> UniformValue;

	// A container for shader source
	class ShaderSource
	{
	public:
		/*
		* Default constructor. Initializes an empty shader source.
		*/
		ShaderSource();

		/*
		* Loads a shader source from a file.
		* 
		* @param path: The file path.
		*/
		ShaderSource(const std::filesystem::path& path);

		/*
		* Load a shader source with specified size from an input stream. Set to -1 to load data until the stream ends. Warning: changes the stream current position.
		* 
		* @param input: The input stream.
		* @param size: The size of the source. Set to -1 to load until the stream ends. -1 by default.
		*/
		ShaderSource(std::istream& input, int32_t size = -1);

		/*
		* Loads a shader source from a file.
		*
		* @param path: The file path.
		*/
		void create(const std::filesystem::path& path);

		/*
		* Load a shader source with specified size from an input stream. Set to -1 to load data until the stream ends. Warning: changes the stream current position.
		*
		* @param input: The input stream.
		* @param size: The size of the source. Set to -1 to load until the stream ends. -1 by default.
		*/
		void create(std::istream& input, int32_t size = -1);

		/*
		* Load a shader source from a string.
		* 
		* @param shader: The shader source code.
		*/
		void createLiteral(const std::string& shader);

		/*
		* Gets the shader source's contents.
		* 
		* @returns The shader source's contents.
		*/
		const std::string& getText() const;

		/*
		* Gets the default source for vertex shader.
		* 
		* @returns Default vertex shader source.
		*/
		static const ShaderSource& getDefaultVertexShaderSource();

		/*
		* Gets the default source for fragment shader.
		*
		* @returns Default fragment shader source.
		*/
		static const ShaderSource& getDefaultFragmentShaderSource();

	private:
		std::string shader;
	};

	class RenderWindow;

	class Shader
	{
	public:
		/*
		* Default constructor. Creates an empty shader.
		*/
		Shader();

		/*
		* Loads the shader from given sources.
		* Warning: If a non-Spectr2D shader is active at the time of calling this function, the shader has to be activated again.
		* 
		* @param vertexShader: The source for vertex shader.
		* @param fragmentShader: The source for fragment shader.
		*/
		Shader(RenderWindow* _owner, const ShaderSource& vertexShader, const ShaderSource& fragmentShader);

		/*
		* Loads the shader from given set of sources.
		* Warning: If a non-Spectr2D shader is active at the time of calling this function, the shader has to be activated again.
		* 
		* @param vertexShaderSources: The sources for vertex shader.
		* @param fragmentShaderSources: The sources for fragment shader.
		*/
		Shader(RenderWindow* _owner, const std::vector<ShaderSource>& vertexShaderSources, const std::vector<ShaderSource>& fragmentShaderSources);

		~Shader();
		
		/*
		* Loads the shader from given sources.
		* Warning: If a non-Spectr2D shader is active at the time of calling this function, the shader has to be activated again.
		* 
		* @param vertexShader: The source for vertex shader.
		* @param fragmentShader: The source for fragment shader.
		*/
		void create(RenderWindow* _owner, const ShaderSource& vertexShader, const ShaderSource& fragmentShader);

		/*
		* Loads the shader from given set of sources.
		* Warning: If a non-Spectr2D shader is active at the time of calling this function, the shader has to be activated again.
		* 
		* @param vertexShaderSources: The sources for vertex shader.
		* @param fragmentShaderSources: The sources for fragment shader.
		*/
		void create(RenderWindow* _owner, const std::vector<ShaderSource>& vertexShaderSources, const std::vector<ShaderSource>& fragmentShaderSources);

		/*
		* Gets the OpenGL id of shader program this object represents.
		* 
		* @returns The OpenGL shader program id.
		*/
		uint32_t getOpenGLID() const;

		/*
		* Sets the shader's interface for vertex attributes.
		* All of the stream's units must have different names.
		* There can only be 16 vertex attributes used in the shader.
		* 
		* @param inter: The VDS defining the vertex attribute interface.
		*/
		void setVertexInterface(const VDS& inter);

		/*
		* Gets the shader's vertex attribute interface.
		* 
		* @returns The shader's vertex attribute interface.
		*/
		const VDS& getVertexInterface() const;

		/*
		* Sets the shader's interface between vertexshader and fragmentshader.
		* All of the stream's units must have different names.
		*
		* @param inter: The VDS defining the vertexshader <-> fragmentshader interface.
		*/
		void setVertFragInterface(const VDS& inter);

		/*
		* Gets the shader's vertexshader <-> fragmentshader interface.
		*
		* @returns The shader's vertexshader <-> fragmentshader interface.
		*/
		const VDS& getVertFragInterface() const;

		/*
		* Sets the shader's interface for uniform values.
		* All of the stream's units must have different names.
		*
		* @param inter: The VDS defining the uniform interface.
		*/
		void setUniformInterface(const VDS& inter);

		/*
		* Gets the shader's uniform interface.
		*
		* @returns The shader's uniform interface.
		*/
		const VDS& getUniformInterface() const;

		/*
		* Sets the shader's interface for output values.
		* All of the stream's units must have different names.
		*
		* @param inter: The VDS defining the output interface.
		*/
		void setOutputInterface(const VDS& inter);

		/*
		* Gets the shader's output interface.
		*
		* @returns The shader's output interface.
		*/
		const VDS& getOutputInterface() const;

		/*
		* Resets the interfaces to their default values.
		*/
		void resetInterfaces();

		/*
		* Gets the OpenGL uniform location. If the uniform is not registered in uniform interface, the shader must be active for this function to work properly.
		* 
		* @param uniform: The name of the uniform.
		* @returns the uniform location.
		*/
		int32_t getUniformLocation(const std::string& uniform) const;

		/*
		* Sets the uniform value. The shader must be active for this function to work properly.
		* Note: the values set by this function are not stored by the shader.
		* 
		* @param uniform: The name of the uniform.
		* @param value: The value to set.
		*/
		void setRawUniform(const std::string& uniform, const UniformValue& value) const;

		/*
		* Sets the uniform value. The shader must be active for this function to work properly.
		* Note: the values set by this function are not stored by the shader.
		*
		* @param uniform: The location of the uniform.
		* @param value: The value to set.
		*/
		void setRawUniform(int32_t uniformLocation, const UniformValue& value) const;

		/*
		* Sets all uniform values in GPU. The shader must be active for this function to work properly.
		*/
		void setRawUniforms() const;

		/*
		* Sets the uniform value. The shader doesn't have to be active for this function to work properly.
		*
		* @param uniform: The name of the uniform.
		* @param value: The value to set.
		*/
		void setUniform(const std::string& uniform, const UniformValue& value);

		/*
		* Gets the value of given uniform. The shader doesn't have to be active for this function to work properly.
		* 
		* @returns Uniform value.
		*/
		template<typename T>
		const T& getUniform(const std::string& uniform) const;

		/*
		* Gets the value of given uniform. The shader doesn't have to be active for this function to work properly.
		*
		* @returns Uniform value.
		*/
		template<>
		const UniformValue& getUniform(const std::string& uniform) const;

		/*
		* Gets the full vertex shader code.
		* 
		* @returns Vertex shader code.
		*/
		const ShaderSource& getVertexSource() const;

		/*
		* Gets the full fragment shader code.
		*
		* @returns fragment shader code.
		*/
		const ShaderSource& getFragmentSource() const;

		/*
		* Converts an interface to shader code defining it using the specified prefix.
		* 
		* @param inter: The interface to convert.
		* @param prefix: The prefix to use (out|in|uniform)
		* @returns The shader code.
		*/
		static std::string getInterfaceString(const VDS& inter, const std::string& prefix, bool useLayout = true);

		/*
		* Gets the default vertex interface.
		* 
		* @returns Default vertex interface.
		*/
		static const VDS& getDefaultVertexInterface();

		/*
		* Gets the default vertexshader <-> fragmentshader interface.
		*
		* @returns Default vertexshader <-> fragmentshader interface.
		*/
		static const VDS& getDefaultVertFragInterface();

		/*
		* Gets the default uniform interface.
		*
		* @returns Default uniform interface.
		*/
		static const VDS& getDefaultUniformInterface();

		/*
		* Gets the default output interface.
		*
		* @returns Default output interface.
		*/
		static const VDS& getDefaultOutputInterface();


	private:
		uint32_t programID;

		ShaderSource vertexSource;
		ShaderSource fragmentSource;

		VertexDataStream vertexInterface;
		VertexDataStream vertFragInterface;
		VertexDataStream uniformInterface;
		VertexDataStream outputInterface;

		RenderWindow* owner;

		std::map<std::string, std::pair<int32_t, UniformValue>> uniformValues;

		void clearShaders();
	};

	template<typename T>
	inline const T& Shader::getUniform(const std::string& uniform) const
	{
		auto it = uniformValues.find(uniform);

		if (it == uniformValues.end())
			throw Error(SPECTRE_INVALID_VALUE, "Attempt to get nonexistent uniform value.");

		return std::get<T>(it->second.second);
	}

	template<>
	inline const UniformValue& Shader::getUniform(const std::string& uniform) const
	{
		auto it = uniformValues.find(uniform);

		if (it == uniformValues.end())
			throw Error(SPECTRE_INVALID_VALUE, "Attempt to get nonexistent uniform value.");

		return it->second.second;
	}
}