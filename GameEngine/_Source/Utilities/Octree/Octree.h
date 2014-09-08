#ifndef _OCTREE_H_
#define _OCTREE_H_

#include <vector>

#include "../GameEngineTypes.h"

namespace GameEngine
{
	class Octree
	{
		class OctreeNode
		{
		public:
			D3DXVECTOR3 m_maxDimension;
			UINT32 m_u32StartIndex;
			UINT32 m_u32TotalData;
			float m_size;
			OctreeNode *m_parent;
			OctreeNode *m_children[Utilities::E_OCTANT_TOTAL];
			bool m_bHasChildren;

			OctreeNode( void );
			OctreeNode( OctreeNode *i_parent );
			OctreeNode( OctreeNode *i_parent, const D3DXVECTOR3& i_maxDimension, const float &i_size,
				const UINT32 i_u32StartIndex, const UINT32 i_u32TotalData, const bool &i_bHasChildren );
			OctreeNode( const D3DXVECTOR3& i_maxDimension, const float &i_size,
				const UINT32 i_u32StartIndex, const UINT32 i_u32TotalData, const bool &i_bHasChildren );
			~OctreeNode( void );

			void AddChildren( const UINT8 &i_u8Octant,
				const D3DXVECTOR3& i_maxDimension, const float &i_size,
				const UINT32 i_u32StartIndex, const UINT32 i_u32TotalData, const bool &i_bHasChildren );

			void Load( Octree *i_octree, std::ifstream &i_inputStream, UINT32 &io_u32StartAddress );
			void GetTriangleData( const Octree *i_octree, const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint,
				std::vector<Utilities::S_TRIANGLE> &o_triangleData );
		};

		OctreeNode *_root;
		static bool _bShowOctree;

	public:
		std::vector<Utilities::S_TRIANGLE> *m_triangleDatabase;

		Octree( const char *i_filename );
		~Octree( void );

		void GetTriangleData( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint,
			std::vector<Utilities::S_TRIANGLE> &o_triangleData );
	};
}

#endif	// _OCTREE_H_