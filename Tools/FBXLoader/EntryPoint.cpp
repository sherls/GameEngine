/**
 ****************************************************************************************************
 * \file		EntryPoint.cpp
 * \brief		Main procedure for SceneBuilder project
 ****************************************************************************************************
*/

// Header Files
//=============

#include "FBXLoader.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	Tools::FBXLoader TheFBXLoader;
	return TheFBXLoader.Build( i_argumentCount, i_arguments ) ? 0 : -1;
	//return 0;
}
