/**
 ****************************************************************************************************
 * \file		EntryPoint.cpp
 * \brief		Main procedure for MeshBuilder project
 *
 * \date		12 January 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

#include "cMeshBuilder.h"

int main( int i_argumentCount, char** i_arguments )
{
	Tools::cMeshBuilder meshBuilder;
	return meshBuilder.Build( i_argumentCount, i_arguments ) ? 0 : -1;
}
