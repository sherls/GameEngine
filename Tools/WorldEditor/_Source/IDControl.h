/**
 ****************************************************************************************************
 * \file		IDControl.h
 * \brief		IDs for the controls and the menu commands
 ****************************************************************************************************
*/

#ifndef _ID_CONTROL_H_
#define _ID_CONTROL_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

enum
{
	// File menu
	WorldEditor_OPEN_FBX = 1,
	WorldEditor_OPEN_SCN,
	WorldEditor_EXPORT,
	WorldEditor_EXPORT_OCTREE,

	// Entity property
	WorldEditor_TAG_NAME,

	// Trigger box
	WorldEditor_TRIGGER_BOX_ADD,
	WorldEditor_TRIGGER_BOX_REMOVE,
	WorldEditor_TRIGGER_BOX_UPDATE,
	WorldEditor_TRIGGER_BOX_LIST,

	// Way point page
	WorldEditor_WAY_POINT_ADD,
	WorldEditor_WAY_POINT_REMOVE,
	WorldEditor_WAY_POINT_UPDATE,
	WorldEditor_WAY_POINT_LIST,

	// Way point link page
	WorldEditor_WAY_POINT_LINK_ADD,
	WorldEditor_WAY_POINT_LINK_REMOVE,
	WorldEditor_WAY_POINT_LINK_LIST,

	// Menu items
	WorldEditor_QUIT = wxID_EXIT,

	// it is important for the id corresponding to the "About" command to have
	// this standard value as otherwise it won't be handled properly under Mac
	// (where it is special and put into the "Apple" menu)
	WorldEditor_ABOUT = wxID_ABOUT
};

#define DEFAULT_WAY_POINT_RADIUS	20.0f

#endif	// #ifndef _ID_CONTROL_H_