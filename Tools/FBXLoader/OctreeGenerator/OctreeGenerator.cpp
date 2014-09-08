#include <vector>
#include <fstream>
#include <assert.h>

#include <fbxsdk.h>

// Utilities header
#include<Utilities.h>
#include <Math/Math.h>

#include "OctreeGenerator.h"

//#define OUTPUT_TRIANGLE_LIST
//#define OUTPUT_OCTREE_TXT

/****************************************************************************************************
			Class / variable declaration
****************************************************************************************************/
namespace Tools
{
	namespace OctreeGenerator
	{
		typedef struct _s_triangle_data_
		{
			Utilities::S_TRIANGLE triangle;
			UINT8 octant;
			bool bFitChildren;
		} S_TRIANGLE_DATA;

		class OctreeNode
		{
			bool _bHasChildren;

			OctreeNode( OctreeNode *parent, const UINT8 &i_u8Octant );

			static bool IsLastTwoTrianglePointsFit( const Utilities::S_TRIANGLE &i_triangle,
				const D3DXVECTOR3 &i_maxDimension, const UINT8 &i_u8Octant, const float &i_octSize );
			void InsertTriangleToThisNode( const Utilities::S_TRIANGLE &i_triangle, const UINT8 &i_u8Octant );

			public:
				D3DXVECTOR3 m_maxDimension;
				float m_octSize;
				std::vector<S_TRIANGLE_DATA> *m_data;
				OctreeNode *m_children[Utilities::E_OCTANT_TOTAL];
				OctreeNode *m_parent;

				OctreeNode( const float &i_octSize, const D3DXVECTOR3 &i_maxDimension );
				~OctreeNode( void );

				void InsertTriangle( const Utilities::S_TRIANGLE &i_triangle );
				void Save( std::ofstream &o_octTreeFile
				#ifdef OUTPUT_OCTREE_TXT
					, std::ofstream &o_octTreeTxtFile
				#endif	// #ifdef OUTPUT_OCTREE_TXT
					);
		};

		const UINT8 MIN_DATA_IN_NODE = 24;
		D3DXVECTOR3 maxDimension;
		std::vector<Utilities::S_TRIANGLE> *triangleDatabase = NULL;
		OctreeNode *root;
#ifdef OUTPUT_OCTREE_TXT
		UINT32 u32TotalOctreeData = 0;
#endif	// #ifdef OUTPUT_OCTREE_TXT
	}
}

/****************************************************************************************************
			Global functions implementation
****************************************************************************************************/
void Tools::OctreeGenerator::Initialize( void )
{
	triangleDatabase = new std::vector< Utilities::S_TRIANGLE >;
	root = NULL;
}

void Tools::OctreeGenerator::ShutDown( void )
{
	if( triangleDatabase )
	{
		delete triangleDatabase;
		triangleDatabase = NULL;
	}

	if( root )
	{
		delete root;
		root = NULL;
	}
}

void Tools::OctreeGenerator::AddTriangle( const D3DXVECTOR3 &i_a, const D3DXVECTOR3 &i_b, const D3DXVECTOR3 &i_c, \
	Utilities::StringHash &i_u32hashedTag )
{
	Utilities::S_TRIANGLE newData = { i_a, i_b, i_c, i_u32hashedTag };
	triangleDatabase->push_back( newData );
}

void Tools::OctreeGenerator::SetMaxSize( const float &i_maxX, const float &i_maxY, const float &i_maxZ )
{
	if( maxDimension.x < abs(i_maxX) )
		maxDimension.x = abs(i_maxX);

	if( maxDimension.y < abs(i_maxY) )
		maxDimension.y = abs(i_maxY);

	if( maxDimension.z < abs(i_maxZ) )
		maxDimension.z = abs(i_maxZ);
}

void Tools::OctreeGenerator::GenerateOctree( const char *i_sourceName )
{
	FbxString octreeFileName = "../../External/Data/Scenes/" + FbxString(i_sourceName) + FbxString( "Octree.txt" );
	std::ofstream octreeFile;

#ifdef OUTPUT_TRIANGLE_LIST
	std::ofstream debugFile;

	debugFile.open( "octreeDebug.txt" );
	if( debugFile.fail() )
		return;

	debugFile << octreeFileName.Buffer() << std::endl;
#endif	// #ifdef OUTPUT_TRIANGLE_LIST


	octreeFile.open( octreeFileName.Buffer(), std::ios::binary );
	if( octreeFile.fail() )
		return;
#ifdef OUTPUT_OCTREE_TXT
	std::ofstream octreeTxtFile;

	octreeFileName += "b";

	octreeTxtFile.open( octreeFileName.Buffer() );
	if( octreeTxtFile.fail() )
		return;

	u32TotalOctreeData = 0;
#endif	// #ifdef OUTPUT_OCTREE_TXT

	if( root )
	{
		delete root;
	}

	UINT32 totalTriangles = triangleDatabase->size();

	maxDimension *= 1.05f;	// Expand a little bit the max to handle triangles just in the max

	float size = Utilities::Math::MaxFloats( maxDimension.x, maxDimension.y, maxDimension.z );

#ifdef OUTPUT_TRIANGLE_LIST
	debugFile << "Total triangles: " << totalTriangles << std::endl;
	debugFile << "Size: " << size << std::endl;
	debugFile << "Max dimension: " << maxDimension.x << ", " << maxDimension.y << ", " << maxDimension.z << std::endl;
#endif	// #ifdef OUTPUT_TRIANGLE_LIST

	size *= 2.0f;
	root = new OctreeNode( size, maxDimension );

	for( UINT32 i = 0; i < totalTriangles; i++ )
	{
		root->InsertTriangle( triangleDatabase->at(i) );

#ifdef OUTPUT_TRIANGLE_LIST
		debugFile << "{" << std::endl;
		debugFile << "     " << triangleDatabase->at(i).a.x << ", " << triangleDatabase->at(i).a.y << ", " \
			<< triangleDatabase->at(i).a.z << std::endl;
		debugFile << "     " << triangleDatabase->at(i).b.x << ", " << triangleDatabase->at(i).b.y << ", " \
			<< triangleDatabase->at(i).b.z << std::endl;
		debugFile << "     " << triangleDatabase->at(i).c.x << ", " << triangleDatabase->at(i).c.y << ", " \
			<< triangleDatabase->at(i).c.z << std::endl;
		debugFile << "}" << std::endl;
#endif	// #ifdef OUTPUT_TRIANGLE_LIST
	}

	root->Save( octreeFile
#ifdef OUTPUT_OCTREE_TXT
		, octreeTxtFile
#endif	// #ifdef OUTPUT_OCTREE_TXT
		);

	octreeFile.close();

#ifdef OUTPUT_OCTREE_TXT
	octreeTxtFile << "Total data in octree: " << u32TotalOctreeData << std::endl;
	octreeTxtFile.close();
#endif	// #ifdef OUTPUT_OCTREE_TXT
#ifdef OUTPUT_TRIANGLE_LIST
	debugFile.close();
#endif	// #ifdef OUTPUT_TRIANGLE_LIST
}

/****************************************************************************************************
			OctreeNode class public functions implementation
****************************************************************************************************/
Tools::OctreeGenerator::OctreeNode::OctreeNode( const float &i_octSize, const D3DXVECTOR3 &i_maxDimension ) :
	m_octSize( i_octSize / 2.0f ),
	m_maxDimension (i_maxDimension ),
	m_parent( NULL ),
	m_data( NULL ),
	_bHasChildren( FALSE )
{
	m_data = new std::vector<S_TRIANGLE_DATA>;

	for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; i++ )
		m_children[i] = NULL;
}

Tools::OctreeGenerator::OctreeNode::~OctreeNode( void )
{
	if( m_data )
	{
		delete m_data;
		m_data = NULL;
	}

	if( _bHasChildren )
	{
		for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; i++ )
		{
			if( m_children[i] )
			{
				delete m_children[i];
				m_children[i] = NULL;
			}
		}
	}
}

void Tools::OctreeGenerator::OctreeNode::InsertTriangle( const Utilities::S_TRIANGLE &i_triangle )
{
	UINT8 octant = 0;

	if( i_triangle.a.y < (maxDimension.y - m_octSize) )
		octant += 4;

	if( i_triangle.a.z < (maxDimension.z - m_octSize) )
		octant += 2;

	if( i_triangle.a.x < (maxDimension.x - m_octSize) )
		octant += 1;

	if( !IsLastTwoTrianglePointsFit(i_triangle, m_maxDimension, octant, m_octSize) )
	{
		S_TRIANGLE_DATA newData = { i_triangle, octant, false };
		m_data->push_back( newData );
	}
	else
	{
		if( m_children[octant] != NULL )
			m_children[octant]->InsertTriangle( i_triangle );
		else
			InsertTriangleToThisNode( i_triangle, octant );
	}
}

void Tools::OctreeGenerator::OctreeNode::Save( std::ofstream &o_octreeFile
#ifdef OUTPUT_OCTREE_TXT
	, std::ofstream &o_octreeTxtFile
#endif	// #ifdef OUTPUT_OCTREE_TXT
	)
{
	UINT32 u32DataSize = m_data->size();

	o_octreeFile.write( reinterpret_cast<const char*> (&m_octSize), sizeof(m_octSize) );
	o_octreeFile.write( reinterpret_cast<const char*> (&m_maxDimension), sizeof(m_maxDimension) );
	o_octreeFile.write( reinterpret_cast<const char*> (&u32DataSize), sizeof(u32DataSize) );
	o_octreeFile.write( reinterpret_cast<const char*> (&_bHasChildren), sizeof(_bHasChildren) );

#ifdef OUTPUT_OCTREE_TXT
	u32TotalOctreeData += u32DataSize;
	o_octreeTxtFile << "Size: " << m_octSize << std::endl;
	o_octreeTxtFile << "Max dimension: " << m_maxDimension.x << ", " << m_maxDimension.y << ", " \
		<< m_maxDimension.z << std::endl;
	o_octreeTxtFile << "Total data: " << u32DataSize << std::endl;
	o_octreeTxtFile << "Has children: " << _bHasChildren << std::endl;
#endif	// #ifdef OUTPUT_OCTREE_TXT

	for( UINT32 i = 0; i < u32DataSize; i++ )
	{
		o_octreeFile.write( reinterpret_cast<const char*> (&m_data->at(i).triangle), sizeof(m_data->at(i).triangle) );
#ifdef OUTPUT_OCTREE_TXT
		o_octreeTxtFile << "{" << std::endl;
		o_octreeTxtFile << "    " << m_data->at(i).triangle.a.x << ", " << m_data->at(i).triangle.a.y << ", " \
			<< m_data->at(i).triangle.a.z << std::endl;
		o_octreeTxtFile << "    " << m_data->at(i).triangle.b.x << ", " << m_data->at(i).triangle.b.y << ", " \
			<< m_data->at(i).triangle.b.z << std::endl;
		o_octreeTxtFile << "    " << m_data->at(i).triangle.c.x << ", " << m_data->at(i).triangle.c.y << ", " \
			<< m_data->at(i).triangle.c.z << std::endl;
		o_octreeTxtFile << "    " << m_data->at(i).triangle.hashedTag << std::endl;
		o_octreeTxtFile << "}" << std::endl;
#endif	// #ifdef OUTPUT_OCTREE_TXT
	}

	if( _bHasChildren )
	{
		for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; i++ )
		{
			if( m_children[i] != NULL )
			{
				m_children[i]->Save( o_octreeFile
				#ifdef OUTPUT_OCTREE_TXT
					, o_octreeTxtFile
				#endif	// #ifdef OUTPUT_OCTREE_TXT
					);
			}
			else
			{
				float size = 0.0f;
				o_octreeFile.write( reinterpret_cast<const char*> (&size), sizeof(size) );
			#ifdef OUTPUT_OCTREE_TXT
				o_octreeTxtFile << "Node " << static_cast<UINT32>(i) << ": 0 " << std::endl;
			#endif	// #ifdef OUTPUT_OCTREE_TXT
			}
		}
	}
}

/****************************************************************************************************
			OctreeNode class private functions implementation
****************************************************************************************************/
Tools::OctreeGenerator::OctreeNode::OctreeNode( OctreeNode *i_parent, const UINT8 &i_u8Octant ) :
	m_parent( i_parent ),
	m_data( NULL ),
	_bHasChildren( FALSE )
{
	float octMinRange = 0.0f;
	float octMaxRange = 0.0f;
	float parentOctSize = i_parent->m_octSize;
	UINT8 u8Octant = i_u8Octant;
	bool bLargerX = true;
	bool bLargerY = true;
	bool bLargerZ = true;

	m_data = new std::vector<S_TRIANGLE_DATA>;

	for( UINT8 i = 0; i < Utilities::E_OCTANT_TOTAL; i++ )
		m_children[i] = NULL;

	assert( u8Octant < Utilities::E_OCTANT_TOTAL );

	if( u8Octant >= 4 )
	{
		u8Octant -= 4;
		bLargerY = false;
	}

	if( u8Octant >= 2 )
	{
		u8Octant -= 2;
		bLargerZ = false;
	}

	if( u8Octant >= 1 )
		bLargerX = false;

	// Get the x range
	if( bLargerX )
		m_maxDimension.x = i_parent->m_maxDimension.x;
	else
		m_maxDimension.x = i_parent->m_maxDimension.x - parentOctSize;

	if( bLargerY )
		m_maxDimension.y = i_parent->m_maxDimension.y;
	else
		m_maxDimension.y = i_parent->m_maxDimension.y - parentOctSize;

	if( bLargerZ )
		m_maxDimension.z = i_parent->m_maxDimension.z;
	else
		m_maxDimension.z = i_parent->m_maxDimension.z - parentOctSize;

	m_octSize = parentOctSize / 2;
}

void Tools::OctreeGenerator::OctreeNode::InsertTriangleToThisNode( const Utilities::S_TRIANGLE &i_triangle, 
	const UINT8 &i_u8Octant )
{
	if( _bHasChildren )
	{
		m_children[i_u8Octant] = new OctreeNode( this, i_u8Octant );
		m_children[i_u8Octant]->InsertTriangle( i_triangle );
	}
	else
	{
		S_TRIANGLE_DATA newData = { i_triangle, i_u8Octant, true };
		m_data->push_back( newData );

		if( m_data->size() > MIN_DATA_IN_NODE )
		{
			std::vector<S_TRIANGLE_DATA> *newData = new std::vector<S_TRIANGLE_DATA>;
			for( UINT32 i = 0; i < m_data->size(); i++ )
			{
				if( m_data->at(i).bFitChildren )
				{
					if( m_children[m_data->at(i).octant] == NULL )
						m_children[m_data->at(i).octant] = new OctreeNode( this, m_data->at(i).octant );
					_bHasChildren = true;
					m_children[m_data->at(i).octant]->InsertTriangle( m_data->at(i).triangle );
				}
				else
				{
					newData->push_back( m_data->at(i) );
				}
			}

			m_data->clear();
			for( UINT32 i = 0; i < newData->size(); i++ )
				m_data->push_back( newData->at(i) );

			delete newData;
		}
	}
}

/****************************************************************************************************
			OctreeNode class static functions implementation
****************************************************************************************************/
bool Tools::OctreeGenerator::OctreeNode::IsLastTwoTrianglePointsFit( const Utilities::S_TRIANGLE &i_triangle,
	const D3DXVECTOR3 &i_maxDimension, const UINT8 &i_u8Octant, const float &i_octSize )
{
	float octMinRange = 0.0f;
	float octMaxRange = 0.0f;
	UINT8 u8Octant = i_u8Octant;
	bool bLargerX = true;
	bool bLargerY = true;
	bool bLargerZ = true;

	assert( u8Octant < Utilities::E_OCTANT_TOTAL );

	if( u8Octant >= 4 )
	{
		u8Octant -= 4;
		bLargerY = false;
	}

	if( u8Octant >= 2 )
	{
		u8Octant -= 2;
		bLargerZ = false;
	}

	if( u8Octant >= 1 )
		bLargerX = false;

	// Get the x range
	if( bLargerX )
		octMaxRange = i_maxDimension.x;
	else
		octMaxRange = i_maxDimension.x - i_octSize;
	octMinRange = octMaxRange - i_octSize;

	// If either of the x not within the range
	if( (i_triangle.b.x > octMaxRange) || (i_triangle.b.x < octMinRange)
		|| (i_triangle.c.x > octMaxRange) || (i_triangle.c.x < octMinRange)
		)
		return false;

	// Get the y range
	if( bLargerY )
		octMaxRange = i_maxDimension.y;
	else
		octMaxRange = i_maxDimension.y - i_octSize;
	octMinRange = octMaxRange - i_octSize;

	// If either of the y not within the range
	if( (i_triangle.b.y > octMaxRange) || (i_triangle.b.y < octMinRange)
		|| (i_triangle.c.y > octMaxRange) || (i_triangle.c.y < octMinRange)
		)
		return false;

	// Get the z range
	if( bLargerZ )
		octMaxRange = i_maxDimension.z;
	else
		octMaxRange = i_maxDimension.z - i_octSize;
	octMinRange = octMaxRange - i_octSize;

	// If either of the z not within the range
	if( (i_triangle.b.z > octMaxRange) || (i_triangle.b.z < octMinRange)
		|| (i_triangle.c.z > octMaxRange) || (i_triangle.c.z < octMinRange)
		)
		return false;

	return true;
}

