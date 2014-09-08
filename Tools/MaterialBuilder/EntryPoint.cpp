/**
 ****************************************************************************************************
 * \file		EntryPoint.cpp
 * \brief		Main procedure for MaterialBuilder project
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Header Files
//=============

#include "cMaterialBuilder.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	Tools::cMaterialBuilder materialBuilder;
	return materialBuilder.Build( i_argumentCount, i_arguments ) ? 0 : -1;
}
