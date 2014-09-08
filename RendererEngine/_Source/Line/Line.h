/**
 ****************************************************************************************************
 * \file		Line.h
 * \brief		Line class declaration
 ****************************************************************************************************
*/

#ifndef _LINE_H_
#define _LINE_H_

// Utilities header
#include <Singleton/Singleton.h>

#include "../RendererEngine.h"
#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	class Line
	{
		friend Utilities::Singleton<Line>;

		IDirect3DVertexBuffer9* _vertexBuffer;

		Line( void ){ }
		~Line( void ){ }

	public:
		Utilities::StringHash m_hashedVertexShader;
		Utilities::StringHash m_hashedFragmentShader;

		bool Initialize( void );
		void ShutDown( void );

		bool LoadVertexBuffer( const S_LINE_TO_DRAW &i_lineData );

		void Draw( void );
	};
}

typedef Utilities::Singleton<RendererEngine::Line> g_line;

#endif	// #ifndef _LINE_H_