/**
 ****************************************************************************************************
 * \file		TagList.h
 * \brief		The common tag used in parser
 ****************************************************************************************************
*/

#ifndef _TAG_LIST_H_
#define _TAG_LIST_H_

#define MAX_INPUT_LEN		128
#define MAX_FILENAME_INPUT	256

namespace Utilities
{
	namespace Parser
	{
		typedef enum _e_input_type_
		{
			E_TYPE_VERTEX = 0,
			E_TYPE_INDEX,
			E_TYPE_POSITION,
			E_TYPE_COLOUR,
			E_TYPE_NORMAL,
			E_TYPE_UV,
			E_TYPE_ENTITY,
			E_TYPE_FILE,
			E_TYPE_ORIENTATION,
			E_TYPE_CAMERA,
			E_TYPE_LOOK_AT,
			E_TYPE_UP,
			E_TYPE_ASPECT,
			E_TYPE_NEAR,
			E_TYPE_FAR,
			E_TYPE_LIGHT,
			E_TYPE_INTENSITY,
			E_TYPE_MESH,
			E_TYPE_MATERIAL,
			E_TYPE_EFFECT,
			E_TYPE_TEXTURE,
			E_TYPE_VERTEX_SHADER,
			E_TYPE_FRAGMENT_SHADER,
			E_TYPE_RENDER_STATE,
			E_TYPE_TEXTURE_MODE,
			E_TYPE_AMBIENT,
			E_TYPE_ATTENTUATOR,
			E_TYPE_RADIUS,
			E_TYPE_SHININESS,
			E_TYPE_TRANSPARENCY,
			E_TYPE_TANGENT,
			E_TYPE_BI_NORMAL,
			E_TYPE_REFLECTANCE,
			E_TYPE_DIRECTIONAL_LIGHT,
			E_TYPE_WIDTH,
			E_TYPE_HEIGHT,
			E_TYPE_DIFFUSE_COLOUR_TEXTURE,
			E_TYPE_NORMAL_MAP_TEXTURE,
			E_TYPE_TRANSPARENT_COLOUR_TEXTURE,
			E_INPUT_TYPE_TOTAL
		} E_INPUT_TYPE;
	}
}

#endif	// #ifndef _TAG_LIST_H_