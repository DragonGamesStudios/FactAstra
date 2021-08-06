#pragma once

namespace sp
{
	class LockableObject
	{
	public:
		/*
		* Default constructor.
		* 
		* @param lock: Wether or not to lock the object upon creation.
		*/
		LockableObject(bool lock = false);

		/*
		* Locks or unlocks the object.
		*
		* @param lock: Wether or not to lock the object.
		*/
		virtual void setLocked(bool value);

		/*
		* Gets wether or not the object is locked.
		*
		* @returns Wether or not the object is locked.
		*/
		bool getLocked() const;

	private:
		bool locked;
	};
}