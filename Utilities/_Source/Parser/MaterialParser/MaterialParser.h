/**
 ****************************************************************************************************
 * \file		MaterialParser.h
 * \brief		Material parser class declaration
 ****************************************************************************************************
*/

#ifndef _MATERIALPARSER_H_
#define _MATERIALPARSER_H_

#include <vector>

namespace Utilities
{
	class MaterialParser
	{
		// Constructor
		MaterialParser( void );

		void LoadMaterialData( const char *i_fileName );

	public:
		float m_shininess;
		float m_transparency;
		float m_reflectance;
		std::string m_effectData;
		//std::vector<std::string> *m_textureData;
		std::string m_diffuseColorTexture;
		std::string m_normalMapTexture;
		std::string m_transparentColourTexture;

		// Constructor
		inline MaterialParser( const char *i_filename );

		// Destructor
		inline ~MaterialParser( void );
	};
}

#include "MaterialParser.inl"

#endif	// #ifndef _MATERIALPARSER_H_