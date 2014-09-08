/**
 ****************************************************************************************************
 * \file		RingBuffer.inl
 * \brief		The inline functions implementation of RingBuffer.h
 ****************************************************************************************************
*/

#include "../Debug/Debug.h"

namespace Utilities
{
/**
 ****************************************************************************************************
	\fn			RingBuffer( UINT8 i_size )
	\brief		Create RingBuffer with the given u8Size
	\param		i_size u8Size of the created RingBuffer
	\return		NONE
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
RingBuffer<T, u8Size>::RingBuffer( UINT8 &i_u8Size )
{
	FUNCTION_START;

	assert( i_u8Size > 0 );

	_element = new T[i_u8Size];
	_u8Size = i_u8Size;
	_u8Count = 0;
	_u8CurrentIndex = 0;
	_u8LastIndex = 0;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			~RingBuffer( void )
	\brief		RingBuffer destuctor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
template< typename T, UINT8 u8Size >
RingBuffer<T, u8Size>::~RingBuffer( void )
{
	delete [] _element;
}

/**
 ****************************************************************************************************
	\fn			const UINT8 GetSize( void ) const
	\brief		Get the u8Size of RingBuffer
	\param		NONE
	\return		_u8Size the u8Size of RingBuffer
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
const UINT8 RingBuffer<T, u8Size>::GetSize( void ) const
{
	return _u8Size;
}

/**
 ****************************************************************************************************
	\fn			const UINT8 GetCount( void ) const
	\brief		Get total element in RingBuffer
	\param		NONE
	\return		_u8Count total element in RingBuffer
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
const UINT8 RingBuffer<T, u8Size>::GetCount( void ) const
{
	return _u8Count;
}

/**
 ****************************************************************************************************
	\fn			bool IsFull( void )
	\brief		Check whether RingBuffer is full
	\param		NONE
	\return		boolean the state of the RingBuffer
	\retval		true if RingBuffer is full
	\retval		false if RingBUffer is not full
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
bool RingBuffer<T, u8Size>::IsFull( void )
{
	if( _u8Count == _u8Size ) return true;
	return false;
}

/**
 ****************************************************************************************************
	\fn			bool IsEmpty( void )
	\brief		Check whether RingBuffer is empty
	\param		NONE
	\return		boolean the state of the RingBuffer
	\retval		true if RingBuffer is empty
	\retval		false if RingBUffer is not empty
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
bool RingBuffer<T, u8Size>::IsEmpty( void )
{
	if( _u8Count == 0 ) return true;
	return false;
}

/****************************************************************************************************
			NON INLINE FUNCTION IMPLEMENTATION
****************************************************************************************************/

/**
 ****************************************************************************************************
	\fn			void RingBuffer Push( const T &i_value )
	\brief		Push given value to RingBuffer
	\param		i_value value to be stored to RingBuffer
	\return		NONE
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
void RingBuffer<T, u8Size>::Push( const T &i_value )
{
	FUNCTION_START;

	_element[_u8CurrentIndex] = i_value;

	_u8LastIndex = _u8CurrentIndex;
	_u8CurrentIndex = (_u8CurrentIndex + 1) % _u8Size;

	if( _u8Count < _u8Size ) _u8Count++;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			const &T Pop( void ) const
	\brief		Pop the last enterred element of RingBuffer
	\param		NONE
	\return		_element the last enterred element
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
const T &RingBuffer<T, u8Size>::Pop( void ) const
{
	FUNCTION_START;

	assert( IsEmpty() );

	_u8Count--;
	_u8CurrentIndex = _u8LastIndex;
	
	(_u8CurrentIndex > 0)? _u8LastIndex = _u8CurrentIndex - 1;

	FUNCTION_FINISH;
	return T[_u8CurrentIndex];
}

/**
 ****************************************************************************************************
	\fn			const &T operator[]( UINT8 i_u8Offset ) const
	\brief		Get RingBuffer element at the given offset position
	\param		i_u8Offset the last N element stored in the 
	\return		_element the element at the given offset position
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
const T &RingBuffer<T, u8Size>::operator[]( UINT8 i_u8Offset ) const
{
	INT8 i8Index = _u8CurrentIndex - i_u8Offset - 1;

	FUNCTION_START;

	if( i8Index < 0 )
		i8Index = _u8Size + i8Index;

	FUNCTION_FINISH;
	return _element[i8Index];
}

/**
 ****************************************************************************************************
	\fn			const &T GetAt( UINT8 i_u8Offset ) const
	\brief		Get RingBuffer element at the given offset position
	\param		i_u8Offset the last N element stored in the 
	\return		_element the element at the given offset position
 ****************************************************************************************************
*/
template<typename T, UINT8 u8Size>
const T &RingBuffer<T, u8Size>::GetAt( UINT8 &i_u8Offset ) const
{
	INT8 i8Index = _u8CurrentIndex - i_u8Offset - 1;

	FUNCTION_START;

	if( i8Index < 0 )
		i8Index = _u8Size + i8Index;

	FUNCTION_FINISH;
	return _element[i8Index];
}
}	// namespace GameEngine