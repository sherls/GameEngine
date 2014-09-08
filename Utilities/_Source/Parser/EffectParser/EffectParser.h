/**
 ****************************************************************************************************
 * \file		EffectParser.h
 * \brief		Effect parser class declaration
 ****************************************************************************************************
*/

#ifndef _EFFECTPARSER_H_
#define _EFFECTPARSER_H_

#include <vector>

#include "../../UtilitiesTypes.h"

namespace Utilities
{
	class EffectParser
	{
		// Constructor
		EffectParser( void );

		void LoadEffectData( const char *i_fileName );

	public:
		std::string m_vertexShader;
		std::vector<std::string> *m_fragmentShader;
		E_ALPHA_MODE m_renderState;
		UINT8 m_textureMode;

		// Constructor
		inline EffectParser( const char *i_filename );

		// Destructor
		inline ~EffectParser( void );
	};
}

#include "EffectParser.inl"
#endif	// #ifndef _EFFECTPARSER_H_