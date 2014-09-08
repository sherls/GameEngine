/**
 ****************************************************************************************************
 * \file		EntryPoint.cpp
 * \brief		Main procedure for SceneBuilder project
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Header Files
//=============

#include "cSceneBuilder.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	cs6963::cSceneBuilder sceneBuilder;
	return sceneBuilder.Build( i_argumentCount, i_arguments ) ? 0 : -1;
}
