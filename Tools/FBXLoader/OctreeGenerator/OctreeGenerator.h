#ifndef _OCTREE_GENERATOR_H_
#define _OCTREE_GENERATOR_H_

// Utilities header
#include <UtilitiesTypes.h>

namespace Tools
{
	namespace OctreeGenerator
	{
		void Initialize( void );
		void ShutDown( void );
		void AddTriangle( const D3DXVECTOR3 &i_a, const D3DXVECTOR3 &i_b, const D3DXVECTOR3 &i_c, \
			Utilities::StringHash &i_hashedTag );
		void SetMaxSize( const float &i_maxX, const float &i_maxY, const float &i_maxZ );
		void GenerateOctree( const char *i_sourceName );
	}
}

#endif	// #ifndef _OCTREE_GENERATOR_H_