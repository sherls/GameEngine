#ifndef _DISPLAY_USER_PROPERTIES_H_
#define _DISPLAY_USER_PROPERTIES_H_

#include "DisplayCommon.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PROPERTY_INFORMATION )
namespace Tools
{
	namespace DisplayUserProperties
	{
		void DisplayUserProperties( FbxObject *i_object );
	}
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PROPERTY_INFORMATION )

#endif // #ifndef _DISPLAY_USER_PROPERTIES_H_