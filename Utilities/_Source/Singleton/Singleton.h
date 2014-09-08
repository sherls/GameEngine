/**
 ****************************************************************************************************
 * \file		Singleton.h
 * \brief		The header of Singleton class
 ****************************************************************************************************
*/

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

namespace Utilities
{
	template< class T >
	class Singleton
	{
		static T* _instance;

		// Make non-copyable
		Singleton( Singleton const &T );
		Singleton &operator=( Singleton const & T );

	public:
		Singleton( void );
		~Singleton( void );

		static T &Get( void );
		static void Release( void );
	};
}	// namespace GameEngine

#include "Singleton.inl"

#endif	// #ifndef _SINGLETON_H_