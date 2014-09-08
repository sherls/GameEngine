/*
	The main() function in this file is the entry point of this program
*/

// Header Files
//=============

#include "cAssetBuilder.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	return g_assetBuilder.Build( i_argumentCount, i_arguments ) ? 0 : -1;
}
