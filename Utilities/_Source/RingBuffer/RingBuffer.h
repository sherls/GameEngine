/**
 ****************************************************************************************************
 * \file		RingBuffer.h
 * \brief		The header of RingBuffer class
 ****************************************************************************************************
*/

#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include "../UtilitiesTypes.h"

#define DEFAULT_RING_BUFFER_SIZE	10

namespace Utilities
{
template<typename T, UINT8 u8Size = DEFAULT_RING_BUFFER_SIZE>
class RingBuffer{
	T *_element;
	UINT8 _u8Count;
	UINT8 _u8Size;
	UINT8 _u8CurrentIndex;
	UINT8 _u8LastIndex;

public:
	// Default constructor
	RingBuffer( void ) {
		_element = new T[u8Size];
		_u8Count = 0;
		_u8Size = u8Size;
		_u8CurrentIndex = 0;
		_u8LastIndex = 0;
	}

	// Standard constructor
	inline RingBuffer( UINT8 &i_u8Size );

	inline ~RingBuffer( void );

	inline const UINT8 GetSize( void ) const;
	inline const UINT8 GetCount( void ) const;

	inline bool IsFull( void );
	inline bool IsEmpty( void );

	void Push( const T &i_value );
	const T &Pop( void ) const;
	const T &operator[]( UINT8 i_u8Offset ) const;
	const T &GetAt( UINT8 &i_u8Offset ) const;
};
}	// namespace GameEngine

#include "RingBuffer.inl"

#endif	// #ifndef _RING_BUFFER_H_