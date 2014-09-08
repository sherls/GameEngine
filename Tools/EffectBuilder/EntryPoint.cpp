/**
 ****************************************************************************************************
 * \file		EntryPoint.cpp
 * \brief		Main procedure for EffectBuilder project
 *
 * \date		1 February 2013
 * \author		Sherly Yunita \n
 *				Created \n
 *				\n\n
 ****************************************************************************************************
*/

// Header Files
//=============

#include "cEffectBuilder.h"

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	Tools::cEffectBuilder effectBuilder;
	return effectBuilder.Build( i_argumentCount, i_arguments ) ? 0 : -1;
}
