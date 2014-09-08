/**
 ****************************************************************************************************
 * \file		Types.h
 * \brief		Custom types
 ****************************************************************************************************
*/

#ifndef _UTILITIES_TYPES_H_
#define _UTILITIES_TYPES_H_

#include <float.h>
#include <BaseTsd.h>

// For renderer
#include <stdint.h>
#include <d3dx9math.h>

// Utilities header
#include "StringHash/StringHash.h"

#define SUCCESS					1
#define FAIL						0

#define DIFFUSE_MAP				0x01
#define NORMAL_MAP				0x02
#define ENVIRONMENT_MAP		0x04
#define OPAQUE_BUFFER			0x08
#define Z_BUFFER				0x10

namespace Utilities
{
	class StringHash;

	typedef struct _s_size_
	{
		float width;
		float height;
		float depth;
	} S_SIZE;

	typedef struct _s_normal_map_vertex_data_
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 biNormal;
		D3DXVECTOR2 texcoord;
		D3DCOLOR colour;
	} S_NORMAL_MAP_VERTEX_DATA;

	typedef struct _s_basic_vertex_data_
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texcoord;
		D3DCOLOR colour;
	} S_BASIC_VERTEX_DATA;

	typedef struct _s_triangle_
	{
		D3DXVECTOR3 a;
		D3DXVECTOR3 b;
		D3DXVECTOR3 c;
		Utilities::StringHash hashedTag;
	} S_TRIANGLE;

	typedef enum _e_alpha_mode_
	{
		E_ALPHA_MODE_NONE = 0,
		E_ALPHA_MODE_BINARY,
		E_ALPHA_MODE_BLEND,
		E_ALPHA_MODE_ADDITIVE,
		E_ALPHA_MODE_MAX
	} E_ALPHA_MODE;

	typedef enum _e_octant_
	{
		E_OCTANT_UPPER_FRONT_RIGHT = 0,
		E_OCTANT_UPPER_FRONT_LEFT,
		E_OCTACT_UPPER_BACK_RIGHT,
		E_OCTANT_UPPER_BACK_LEFT,
		E_OCTANT_LOWER_FRONT_RIGHT,
		E_OCTANT_LOWER_FRONT_LEFT,
		E_OCTACT_LOWER_BACK_RIGHT,
		E_OCTANT_LOWER_BACK_LEFT,
		E_OCTANT_TOTAL
	} E_OCTANT;
}

#endif	// #ifndef _UTILITIES_TYPES_H_