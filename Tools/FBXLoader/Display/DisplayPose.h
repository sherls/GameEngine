#ifndef _DISPLAY_POSE_H_
#define _DISPLAY_POSE_H_

#include "DisplayCommon.h"

namespace Tools
{
	namespace DisplayPose
	{
#ifdef DUMP_FBX_INFO
		void DisplayPose( FbxScene *i_scene );
#else
		inline void DisplayPose( FbxScene *i_scene ){ }
#endif	// #ifdef DUMP_FBX_INFO
	}
}

#endif // #ifndef _DISPLAY_POSE_H_