/**
 ****************************************************************************************************
 * \file		FastVector3.cpp
 * \brief		Implementation of non-inline functions of FastVector3 class
 ****************************************************************************************************
*/

#include <assert.h>

// Utilities header
#include <Debug/Debug.h>

#include "FastVector3.h"

#ifdef _DEBUG
/**
 ****************************************************************************************************
	\fn			void UnitTest( void )
	\brief		Unit test for FastVector3 class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Math::FastVector3::UnitTest( void )
{
	UINT8 i;

	FUNCTION_START;

	/*CachedVector3 vector3_1( 1.0f, 2.0f, 3.0f );
	CachedVector3 *vector3_2 = new CachedVector3( 0.1f, 0.2f, 0.3f );
	CachedVector3 *vector3_3 = new CachedVector3[2];

	assert( (int)&vector3_1 % CACHE_LINE == 0 );
	printf( "Address of vector3_1: %d\n", &vector3_1 );
	assert( (int)vector3_2 % CACHE_LINE == 0 );
	//assert( (int)vector3_2 % sizeof(Vector3) == 0 );
	printf( "Address of vector3_2: %d\n", vector3_2 );
	assert( (int)vector3_3 % CACHE_LINE == 0 );
	//assert( (int)vector3_2 % sizeof(Vector3) == 0 );
	printf( "Address of vector3_3: %d\n", vector3_3 );
	for( i = 0; i < 2; ++i )
	{
		assert( (int)&vector3_3[i] % CACHE_LINE == 0 );
		//assert( (int)&vector3_3[i] % sizeof(Vector3) == 0 );
		printf( "Address of vector3_3[%d]: %d\n", i, &vector3_3[i] );
	}*/

	FastVector3 fastVector1( 1.0f, 2.0f, 3.0f );
	FastVector3 *fastVector2 = new FastVector3( 0.1f, 0.2f, 0.3f );
	FastVector3 *fastVector3 = new FastVector3[2];
	
	assert( (int)&fastVector1 % CACHE_LINE == 0 );
	DBG_MSG_LEVEL( D_UNIT_TEST, "Address of fastVector1: %d\n", &fastVector1 );
	assert( (int)fastVector2 % CACHE_LINE == 0 );
	DBG_MSG_LEVEL( D_UNIT_TEST, "Address of fastVector2: %d\n", fastVector2 );
	assert( (int)fastVector3 % CACHE_LINE == 0 );
	DBG_MSG_LEVEL( D_UNIT_TEST, "Address of fastVector3: %d\n", fastVector3 );
	for( i = 0; i < 2; ++i )
	{
		assert( (int)&fastVector3[i] % CACHE_LINE == 0 );
		DBG_MSG_LEVEL( D_UNIT_TEST, "Address of fastVector3[%d]: %d\n", i, &fastVector3[i] );
	}

	DBG_MSG_LEVEL( D_UNIT_TEST, "Cached fast vector 3 sucessfully tested\n" );

	delete fastVector2;
	delete fastVector3;

	FUNCTION_FINISH;
}
#endif	// #ifdef _DEBUG