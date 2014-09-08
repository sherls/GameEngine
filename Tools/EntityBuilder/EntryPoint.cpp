/**
 ****************************************************************************************************
 * \file		EntryPoint.cpp
 * \brief		Main procedure for EntityBuilder project
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Header Files
//=============

#include "cEntityBuilder.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	Tools::cEntityBuilder entityBuilder;
	return entityBuilder.Build( i_argumentCount, i_arguments ) ? 0 : -1;
}
