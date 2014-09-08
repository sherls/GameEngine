/**
 ****************************************************************************************************
 * \file		FBXLoader.h
 * \brief		FBXLoader class declaration
 ****************************************************************************************************
*/

#ifndef _WORLD_EDITOR_FBX_LOADER_H_
#define _WORLD_EDITOR_FBX_LOADER_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <Singleton/Singleton.h>

namespace FBX
{
	class FBXLoader
	{
	public:
		bool OpenFBXLoader( const char *i_filename, wxTextCtrl *i_infoWindow, std::string &o_sceneFile );

	private:
		friend Utilities::Singleton<FBXLoader>;
	};
}

typedef Utilities::Singleton<FBX::FBXLoader> g_FBXLoader;

#endif	// #ifndef _WORLD_EDITOR_FBX_LOADER_H_