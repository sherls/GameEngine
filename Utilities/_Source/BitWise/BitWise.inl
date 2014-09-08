/**
 ****************************************************************************************************
 * \file		BitWise.inl
 * \brief		The inline functions implementation of BitWise.h
 ****************************************************************************************************
*/
#include <stdio.h>
#include <memory.h>
#include <assert.h>
#include "../UtilitiesTypes.h"

/****************************************************************************************************
			Public functions implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void Destroy( void )
	\brief		Destroy BitWise class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void Utilities::BitWise::Destroy( void )
{
	FUNCTION_START;

	_u32NumElements = 0;
	_aligned_free( _u32pBits );
	delete this;

	FUNCTION_FINISH;
}
