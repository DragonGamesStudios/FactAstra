#pragma once

#include "LockableObject.h"

#include <memory>

namespace sp
{
	enum class DataUsage
	{
		// Set when the data will be modified rarely or never
		Static,
		// Set when the data will be modified often
		Dynamic
	};

	class ObjectWithBuffer : public LockableObject
	{
	public:
		// Default constructor
		ObjectWithBuffer(uint32_t _binding, DataUsage _usage = DataUsage::Static, bool lock = false);

		virtual ~ObjectWithBuffer();

		/*
		* Gets the OpenGL id of the buffer maintained by this object.
		* 
		* @returns Buffer id.
		*/
		uint32_t getOpenGLID() const;

		// Buffers all the unbuffered data.
		//
		// @returns Wether or not the data needed to be rebuffered.
		virtual bool buffer();

		/*
		* Sets the way the buffer is used.
		* 
		* @param _usage: New data usage.
		*/
		void setDataUsage(DataUsage _usage);

		/*
		* Gets the way the buffer is used.
		*
		* @returns Data usage.
		*/
		DataUsage getDataUsage() const;

		/*
		* Locks or unlocks the object. Locked object will not buffer any data. When unlocked, it will buffer all unbuffered data.
		*
		* @param lock: Wether or not to lock the object.
		*/
		void setLocked(bool value) override;

		// Binds the object and thus makes it active.
		virtual void bind() const;

	protected:
		void requestRebuffer();
		void requestBuffer();

		virtual void rebuffer();

		uint32_t getBinding() const;

		void bufferData(size_t size, const void* data);
		void bufferSubData(size_t offset, size_t size, const void* data);

	private:
		DataUsage usage;
		uint32_t id;
		uint32_t binding;
		bool rebufferNeeded;
	};
}