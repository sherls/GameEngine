/*
	The main() function in this file is the entry point of this program
*/

// Header Files
//=============

#include "cTextureBuilder.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	Tools::cTextureBuilder textureBuilder;
	return textureBuilder.Build( i_argumentCount, i_arguments ) ? 0 : -1;
}
