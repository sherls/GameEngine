/**
 ****************************************************************************************************
 * \file		Quad.h
 * \brief		Quad class declaration
 ****************************************************************************************************
*/

#ifndef _QUAD_H_
#define _QUAD_H_

// Utilities header
#include <Singleton/Singleton.h>
#include <StringHash/StringHash.h>

#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	class Quad
	{
		friend Utilities::Singleton<Quad>;

		IDirect3DVertexBuffer9* _vertexBuffer;

		Quad( void ){ }
		~Quad( void ){ }

	public:
		Utilities::StringHash m_hashedVertexShader;
		Utilities::StringHash m_hashedFragmentShader;

		bool Initialize( void );
		void ShutDown( void );

		bool LoadVertexBuffer( const D3DCOLOR &i_colour );

		void Draw( void );
	};
}

typedef Utilities::Singleton<RendererEngine::Quad> g_quad;

#endif	// #ifndef _QUAD_H_