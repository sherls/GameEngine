/**
 ****************************************************************************************************
 * \file		Sphere.h
 * \brief		Sphere class declaration
 ****************************************************************************************************
*/

#ifndef _SPHERE_H_
#define _SPHERE_H_

// Utilities header
#include <Singleton/Singleton.h>

#include "../RendererEngine.h"
#include "../RendererEngineTypes.h"

namespace RendererEngine
{
	class Sphere
	{
		friend Utilities::Singleton<Sphere>;

		UINT32 _u32VertexCount;
		UINT32 _u32PrimitiveCount;
		IDirect3DIndexBuffer9* _indexBuffer;
		IDirect3DVertexBuffer9* _vertexBuffer;

		Sphere( void ){ }
		~Sphere( void ){ }

	public:
		Utilities::StringHash m_hashedVertexShader;
		Utilities::StringHash m_hashedFragmentShader;

		bool Initialize( void );
		void ShutDown( void );

		bool LoadVertexBuffer( const S_SPHERE_TO_DRAW &i_sphereData, const UINT8 &i_u8Slice = 10, const UINT8 &i_u8N = 10 );

		void Draw( void );
	};
}

typedef Utilities::Singleton<RendererEngine::Sphere> g_sphere;

#endif	// #ifndef _SPHERE_H_