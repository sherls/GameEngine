#include <assert.h>
#include <fstream>

// Utilities header
#include <Math/Math.h>
#include <Debug/Debug.h>
#include <Parser/TagList.h>

#include "Octree.h"
#include "../../DebugMenu/DebugMenu.h"

bool GameEngine::Octree::_bShowOctree = false;

/****************************************************************************************************
			Octree class public functions implementation
****************************************************************************************************/
GameEngine::Octree::Octree( const char *i_filename ) :
	_root( NULL ),
	m_triangleDatabase( NULL )
{
	assert( i_filename != NULL );
	assert( i_filename[0] != '\0' );

	std::ifstream in;
	UINT32 u32StartAddress = 0;
	char fileName[MAX_FILENAME_INPUT];

	FUNCTION_START;

	strcpy_s( fileName, MAX_FILENAME_INPUT, "../../External/Data/Scenes/" );
	strcat_s( fileName, MAX_FILENAME_INPUT, i_filename );

	in.open( fileName, std::ios::binary );
	if( in.fail() )
	{
		FUNCTION_FINISH;
		return;
	}

#ifdef ENABLE_OCTREE_DISPLAY
	g_debugMenu::Get().AddCheckBox( "Show octree", _bShowOctree );
#endif	// #ifdef ENABLE_OCTREE

	if( !m_triangleDatabase )
		m_triangleDatabase = new std::vector<Utilities::S_TRIANGLE>;

	in.seekg( u32StartAddress, std::ios::beg );

	_root = new OctreeNode();
	_root->Load( this, in, u32StartAddress );

	in.close();

	FUNCTION_FINISH;
}

GameEngine::Octree::~Octree( void )
{
	if( _root )
	{
		delete _root;
		_root = NULL;
	}

	if( m_triangleDatabase )
	{
		delete m_triangleDatabase;
		m_triangleDatabase = NULL;
	}
}

void GameEngine::Octree::GetTriangleData( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint,
	std::vector<Utilities::S_TRIANGLE> &o_triangleData )
{
	for( UINT32 i = 0; i < _root->m_u32TotalData; ++i )
		o_triangleData.push_back( m_triangleDatabase->at(_root->m_u32StartIndex+i) );

	if( _bShowOctree )
		g_debugMenu::Get().DrawBox( _root->m_maxDimension, _root->m_size * 2 );

	if( _root->m_bHasChildren )
	{
		for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		{
			if( _root->m_children[i] != NULL )
				_root->m_children[i]->GetTriangleData( this, i_startPoint, i_endPoint, o_triangleData );
		}
	}
}

/****************************************************************************************************
			OctreeNode class public functions implementation
****************************************************************************************************/
GameEngine::Octree::OctreeNode::OctreeNode( void ) :
	m_u32StartIndex( 0 ),
	m_u32TotalData( 0 ),
	m_size( 0.0f ),
	m_parent( NULL ),
	m_bHasChildren( false )
{
	for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		m_children[i] = NULL;
}

GameEngine::Octree::OctreeNode::OctreeNode( OctreeNode *i_parent ) :
	m_u32StartIndex( 0 ),
	m_u32TotalData( 0 ),
	m_size( 0.0f ),
	m_parent( i_parent ),
	m_bHasChildren( false )
{
	for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		m_children[i] = NULL;
}

GameEngine::Octree::OctreeNode::OctreeNode( OctreeNode *i_parent,
	const D3DXVECTOR3& i_maxDimension, const float &i_size,
	const UINT32 i_u32StartIndex, const UINT32 i_u32TotalData, const bool &i_bHasChildren) :
	m_maxDimension( i_maxDimension ),
	m_u32StartIndex( i_u32StartIndex ),
	m_u32TotalData( i_u32TotalData ),
	m_size( i_size ),
	m_parent( i_parent ),
	m_bHasChildren( i_bHasChildren )
{
	for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		m_children[i] = NULL;
}

GameEngine::Octree::OctreeNode::OctreeNode( const D3DXVECTOR3& i_maxDimension, const float &i_size,
	const UINT32 i_u32StartIndex, const UINT32 i_u32TotalData, const bool &i_bHasChildren ) :
	m_maxDimension( i_maxDimension ),
	m_u32StartIndex( i_u32StartIndex ),
	m_u32TotalData( i_u32TotalData ),
	m_size( i_size ),
	m_parent( NULL ),
	m_bHasChildren( i_bHasChildren )
{
	for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		m_children[i] = NULL;
}

GameEngine::Octree::OctreeNode::~OctreeNode( void )
{
	if( m_bHasChildren )
	{
		for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		{
			if( m_children[i] )
			{
				delete m_children[i];
				m_children[i] = NULL;
			}
		}
	}
}

void GameEngine::Octree::OctreeNode::AddChildren( const UINT8 &i_u8Octant,
	const D3DXVECTOR3& i_maxDimension, const float &i_size,
	const UINT32 i_u32StartIndex, const UINT32 i_u32TotalData, const bool &i_bHasChildren )
{
	m_children[i_u8Octant] = new OctreeNode( this, i_maxDimension, i_size,
		i_u32StartIndex, i_u32TotalData, i_bHasChildren );
}

void GameEngine::Octree::OctreeNode::Load( Octree *i_octree, std::ifstream &i_inputStream, UINT32 &io_u32StartAddress )
{
	Utilities::S_TRIANGLE *triangle;

	FUNCTION_START;

	i_inputStream.seekg( io_u32StartAddress, std::ios::beg );

	i_inputStream.read( (char*) &m_size, sizeof(float) );
	io_u32StartAddress += sizeof( float );
	i_inputStream.read( (char*) &m_maxDimension, sizeof(D3DXVECTOR3) );
	io_u32StartAddress += sizeof( D3DXVECTOR3 );
	i_inputStream.read( (char*) &m_u32TotalData, sizeof(UINT32) );
	io_u32StartAddress += sizeof( UINT32 );
	i_inputStream.read( (char*) &m_bHasChildren, sizeof(bool) );
	io_u32StartAddress += sizeof( bool );

	char *tempTriangle = new char[m_u32TotalData * sizeof(Utilities::S_TRIANGLE)];
	i_inputStream.read( tempTriangle, m_u32TotalData*sizeof(Utilities::S_TRIANGLE) );
	io_u32StartAddress += m_u32TotalData * sizeof( Utilities::S_TRIANGLE );
	triangle = new Utilities::S_TRIANGLE[m_u32TotalData];
	memcpy( triangle, tempTriangle, m_u32TotalData * sizeof(Utilities::S_TRIANGLE) );

	m_u32StartIndex = i_octree->m_triangleDatabase->size();

	// Push the data to Octree's triangle list
	for( UINT32 i = 0; i < m_u32TotalData; ++i )
		i_octree->m_triangleDatabase->push_back( triangle[i] );

	if( m_bHasChildren )
	{
		for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		{
			float temp;
			i_inputStream.read( (char*) &temp, sizeof(float) );
			if( !Utilities::Math::AreRelativelyEqual(temp, 0.0f) )
			{
				m_children[i] = new OctreeNode( this );
				m_children[i]->Load( i_octree, i_inputStream, io_u32StartAddress );
			}
			else
			{
				io_u32StartAddress += sizeof( float );
			}
		}
	}

	FUNCTION_FINISH;

	delete [] tempTriangle;
	delete [] triangle;
}

void GameEngine::Octree::OctreeNode::GetTriangleData( const Octree *i_octree, const D3DXVECTOR3 &i_startPoint,
	const D3DXVECTOR3 &i_endPoint, std::vector<Utilities::S_TRIANGLE> &o_triangleData )
{
	float boxSize = 2 * m_size;
	D3DXVECTOR3 boxMax = m_maxDimension;
	D3DXVECTOR3 boxMin( boxMax.x - boxSize, boxMax.y - boxSize, boxMax.z - boxSize );

	D3DXVECTOR3 d = (i_endPoint - i_startPoint) * 0.5f;
	D3DXVECTOR3 e = (boxMax - boxMin) * 0.5f;
	D3DXVECTOR3 c = i_startPoint + d - (boxMin + boxMax) * 0.5f;
	D3DXVECTOR3 ad( abs(d.x), abs(d.y), abs(d.z) );

	if( fabsf(c.x) > (e.x + ad.x) )
		return;

	if( fabsf(c.y) > (e.y + ad.y) )
		return;

	if( fabsf(c.z) > (e.z + ad.z) )
		return;

	if( fabsf(d.y * c.z - d.z * c.y) > (e.y * ad.z + e.z * ad.y + FLT_EPSILON) )
		return;

	if( fabsf(d.z * c.x - d.x * c.z) > (e.z * ad.x + e.x * ad.z + FLT_EPSILON) )
		return;

	if( fabsf(d.x * c.y - d.y * c.x) > (e.x * ad.y + e.y * ad.x + FLT_EPSILON) )
		return;

	if( _bShowOctree )
		g_debugMenu::Get().DrawBox( m_maxDimension, m_size * 2 );

	for( UINT32 i = 0; i < m_u32TotalData; ++i )
		o_triangleData.push_back( i_octree->m_triangleDatabase->at(m_u32StartIndex + i) );

	if( m_bHasChildren )
	{
		for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; ++i )
		{
			if( m_children[i] != NULL )
				m_children[i]->GetTriangleData( i_octree, i_startPoint, i_endPoint, o_triangleData );
		}
	}
}