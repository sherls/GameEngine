#ifndef _DISPLAY_PIVOTS_AND_LIMITS_H_
#define _DISPLAY_PIVOTS_AND_LIMITS_H_

#include "DisplayCommon.h"

#if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PIVOT_INFORMATION )
namespace DUMP_FBX_INFO
{
	namespace DisplayPivotAndLimits
	{
		void DisplayPivotsAndLimits( FbxNode *i_node );
	}
}
#endif	// #if defined( DUMP_FBX_INFO ) && defined( DISPLAY_PIVOT_INFORMATION )

#endif // #ifndef _DISPLAY_PIVOTS_AND_LIMITS_H_

