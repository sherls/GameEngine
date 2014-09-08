/**
 ****************************************************************************************************
 * \file		UnitTest.cpp
 * \brief		The implementation of Unit Test function
 ****************************************************************************************************
*/

#ifdef _DEBUG
#include <conio.h>
#include <assert.h>

// Utilities header
#include <Math/Math.h>
#include <Debug/Debug.h>
#include <RingBuffer/RingBuffer.h>

#include "UnitTest.h"
#include "../Utilities/GameEngineTypes.h"

/**
 ****************************************************************************************************
	\fn			void RingBufferUnitTest( void )
	\brief		Unit test for RingBuffer class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::RingBufferUnitTest( void )
{
	using namespace GameEngine;

	const UINT8 ringBufferSize = 10;
	const UINT8 inputSize = 15;

	Utilities::RingBuffer <int, ringBufferSize> intRingBuffer;
	Utilities::RingBuffer <char, ringBufferSize> charRingBuffer;

	int intInput[inputSize] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	char charInput[inputSize] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O' };

	UINT8 i;
	UINT8 index;

	FUNCTION_START;

	for( i = 0; i < inputSize; ++i )
	{
		intRingBuffer.Push( intInput[i] );
		charRingBuffer.Push( charInput[i] );
	}

	index = 0;
	assert( intRingBuffer[index] == intInput[inputSize-index-1] );
	DBG_MSG_LEVEL( D_UNIT_TEST, "Last %d stored integer is %d\n", index, intRingBuffer[index] );

	index = 9;
	assert( charRingBuffer[index] == charInput[inputSize-index-1] );
	DBG_MSG_LEVEL( D_UNIT_TEST, "Last %d stored char is %c\n", index, charRingBuffer[index] );

	DBG_MSG_LEVEL( D_UNIT_TEST, "Ring buffer test success\n\n" );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void FloatNumberPrecisionTest( void )
	\brief		Test float number precision
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::FloatNumberPrecisionTest( void )
{
	using namespace GameEngine;

	FUNCTION_START;

	float a = 1.1f;
	float b = a * 0.2f;
	float c = a / 5.0f;

	assert( Utilities::Math::AreRelativelyEqual(c, b) );

	FUNCTION_FINISH;
}
#endif	// #ifdef _DEBUG
