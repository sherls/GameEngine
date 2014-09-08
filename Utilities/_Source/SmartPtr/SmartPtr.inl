/**
 ****************************************************************************************************
 * \file		SmartPtr.inl
 * \brief		The inline functions implementation of SmartPtr.h
 ****************************************************************************************************
*/

#include <assert.h>

#include "../Debug/Debug.h"

namespace Utilities
{
	namespace Pointer
	{
/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			SmartPtr( void )
	\brief		SmartPtr class default constructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
SmartPtr<T>::SmartPtr( void ) :
	_ptr( NULL ),
	_count( new(AllocateCount()) UINT32(1) )
{
}

/**
 ****************************************************************************************************
	\fn			SmartPtr( const T *i_ptr )
	\brief		SmartPtr class copy constructor
	\param		*i_ptr pointer to be copied
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
SmartPtr<T>::SmartPtr( T *i_ptr ) :
	_ptr( i_ptr ),
	_count( new(AllocateCount()) UINT32(1) )
{
}

/**
 ****************************************************************************************************
	\fn			SmartPtr( const SmartPtr &i_other )
	\brief		SmartPtr class copy constructor
	\param		*i_other SmartPtr to be copied
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
SmartPtr<T>::SmartPtr( const SmartPtr &i_other ) :
	_ptr( i_other._ptr ),
	_count( i_other._count )
{
	assert( i_other._count );

	(*_count)++;
}

/**
 ****************************************************************************************************
	\fn			~SmartPtr( void )
	\brief		Standard destructor of SmartPtr class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
SmartPtr<T>::~SmartPtr( void )
{
	Release();
}

/**
 ****************************************************************************************************
	\fn			bool operator==( const T *i_other )
	\brief		== operator of SmartPtr class
	\param		*i_ptr pointer to be compared
	\return		boolean
	\retval		TRUE equal
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
template< class T >
bool SmartPtr<T>::operator==( const T *i_other ) const
{
	return _ptr == i_other;
}

/**
 ****************************************************************************************************
	\fn			bool operator==( const SmartPtr &i_other )
	\brief		== operator of SmartPtr class
	\param		i_other SmartPtr to be compared
	\return		boolean
	\retval		TRUE equal
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
template< class T >
bool SmartPtr<T>::operator==( const SmartPtr &i_other ) const
{
	return _ptr == i_other._ptr;
}

/**
 ****************************************************************************************************
	\fn			bool operator!=( const T *i_other )
	\brief		!= operator of SmartPtr class
	\param		*i_ptr pointer to be compared
	\return		boolean
	\retval		TRUE equal
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
// template< class T >
// bool SmartPtr<T>::operator!=( const T *i_other ) const
// {
// 	return _ptr != i_other;
// }

/**
 ****************************************************************************************************
	\fn			bool operator!=( const SmartPtr &i_other )
	\brief		!= operator of SmartPtr class
	\param		i_other SmartPtr to be compared
	\return		boolean
	\retval		TRUE equal
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
// template< class T >
// bool SmartPtr<T>::operator!=( const SmartPtr &i_other ) const
// {
// 	return _ptr != i_other._ptr;
// }

/**
 ****************************************************************************************************
	\fn			const SmartPtr &operator=( const SmartPtr &i_other )
	\brief		= operator of SmartPtr class
	\param		&i_other SmartPtr class source
	\return		New assigned SmartPtr
 ****************************************************************************************************
*/
template< class T >
SmartPtr<T> &SmartPtr<T>::operator=( const SmartPtr &i_other )
{
	if( this != &i_other )
	{
		Release();

		assert( i_other._count );
		_ptr = i_other._ptr;
		_count = i_other._count;

		(*_count)++;
	}

	return *this;
}

/**
 ****************************************************************************************************
	\fn			const SmartPtr &operator=( const T *i_other )
	\brief		= operator of SmartPtr class
	\param		*i_other pointer source
	\return		New assigned SmartPtr
 ****************************************************************************************************
*/
template< class T >
SmartPtr<T> &SmartPtr<T>::operator=( T *i_other )
{
	if( _ptr != i_other )
	{
		Release();

		_ptr = i_other;
		_count = new( AllocateCount() ) UINT32(1) ;
	}

	return *this;
}

/**
 ****************************************************************************************************
	\fn			T *operator->( void )
	\brief		-> operator of SmartPtr class
	\param		NONE
	\return		The pointer of SmartPtr
 ****************************************************************************************************
*/
template< class T >
T *SmartPtr<T>::operator->( void )
{
	return _ptr;
}

/**
 ****************************************************************************************************
	\fn			T &operator*( void )
	\brief		* operator of SmartPtr class
	\param		NONE
	\return		The address of SmartPtr
 ****************************************************************************************************
*/
template< class T >
T &SmartPtr<T>::operator*( void )
{
	assert( _ptr );
	return *_ptr;
}

/**
 ****************************************************************************************************
	\fn			const T *operator->( void )
	\brief		-> operator of SmartPtr class
	\param		NONE
	\return		The pointer of SmartPtr
 ****************************************************************************************************
*/
template< class T >
const T *SmartPtr<T>::operator->( void ) const
{
	return _ptr;
}

/**
 ****************************************************************************************************
	\fn			const T &operator*( void )
	\brief		* operator of SmartPtr class
	\param		NONE
	\return		The address of SmartPtr
 ****************************************************************************************************
*/
template< class T >
const T &SmartPtr<T>::operator*( void ) const
{
	assert( _ptr );
	return *_ptr;
}
/**
 ****************************************************************************************************
	\fn			operator bool( void ) const
	\brief		Operator bool of SmartPtr class
	\param		NONE
	\return		boolean
	\retval		TRUE pointer is not NULL
	\retval		FALSE otherwise
 ****************************************************************************************************
*/
template< class T >
SmartPtr<T>::operator bool( void ) const
{
	return _ptr != 0;
}

/****************************************************************************************************
			Private functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void Release( void )
	\brief		Decrease counter to pinter by one and delete the pointer when no one use it
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< class T >
void SmartPtr<T>::Release( void )
{
	assert( _count );
	assert( *_count> 0  );

	if( --(*_count) == 0 )
	{
		FreeCount( _count );

		_count = NULL;

		if( _ptr )
			delete _ptr;

		_ptr = NULL;
	}
}

	} // namespace Pointer
}	// namespace GameEngine