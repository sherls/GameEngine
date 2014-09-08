/**
 ****************************************************************************************************
 * \file		Singleton.inl
 * \brief		The inline functions implementation of Singleton class
 ****************************************************************************************************
*/

#include <assert.h>

#include "../Debug/Debug.h"

namespace Utilities
{
template< class T >
T* Singleton<T>::_instance = NULL;

/**
 ****************************************************************************************************
	\fn			Singleton( void )
	\brief		Default constructor of Singleton
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
Singleton<T>::Singleton( void )
{
}

/**
 ****************************************************************************************************
	\fn			~Singleton( void )
	\brief		Default destructor of Singleton
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
Singleton<T>::~Singleton( void )
{
	FUNCTION_START;

	if( _instance )
	{
		delete _instance;

		_instance = NULL;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			T &Get( void )
	\brief		Get the instance of singleton class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
T &Singleton<T>::Get( void )
{
	FUNCTION_START;

	if( _instance == NULL)
		_instance = new T;

	assert( _instance );

	FUNCTION_FINISH;
	return *_instance;
}

/**
 ****************************************************************************************************
	\fn			void Release( void )
	\brief		Release the instance of singleton class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
void Singleton<T>::Release( void )
{
	FUNCTION_START;

	if( _instance )
	{
		delete _instance;

		_instance = NULL;
	}

	FUNCTION_FINISH;
}

}	// namespace GameEngine