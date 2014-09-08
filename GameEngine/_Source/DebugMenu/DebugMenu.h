/**
 ****************************************************************************************************
 * \file		DebugMenu.h
 * \brief		Debug menu class
 ****************************************************************************************************
*/

#ifndef _DEBUG_MENU_H_
#define _DEBUG_MENU_H_
#include <string>
#include <map>

// Utilities header
#include <UtilitiesDefault.h>
#include <Singleton/Singleton.h>
#include <StringHash/StringHash.h>

// Renderer header
#include <RendererEngine.h>

#include "../Utilities/GameEngineTypes.h"

namespace GameEngine
{

	class DebugMenu
	{
		friend Utilities::Singleton<DebugMenu>;

		DebugMenu( void ){ }
		~DebugMenu( void ){ }

		typedef struct _s_text_field_
		{
			std::string text;
			D3DCOLOR colour;
			UINT32 u32LiveTime;
		} S_DEBUG_TEXT;

		typedef struct _s_button_
		{
			std::string label;
			ButtonHandler handler;
		} S_BUTTON;

		typedef struct _s_check_box_
		{
			std::string label;
			bool &value;
		} S_CHECK_BOX;

		typedef struct _s_slider_uint32_
		{
			std::string label;
			UINT32 &value;
			UINT32 minValue;
			UINT32 maxValue;
			UINT32 increment;
		} S_SLIDER_UINT32;

		typedef struct _s_slider_int32_
		{
			std::string label;
			INT32 &value;
			INT32 minValue;
			INT32 maxValue;
			INT32 increment;
		} S_SLIDER_INT32;

		typedef struct _s_slider_float_
		{
			std::string label;
			float &value;
			float minValue;
			float maxValue;
			float increment;
		} S_SLIDER_FLOAT;

		class DebugMenuNode
		{
		public:
			//DebugMenuNode( void );
			DebugMenuNode( const std::string &i_label );
			~DebugMenuNode( void );

			std::string m_label;
			std::map<Utilities::StringHash, DebugMenuNode*> *m_children;
			std::map<Utilities::StringHash, S_BUTTON> *m_buttons;
			std::map<Utilities::StringHash, S_CHECK_BOX> *m_checkBoxes;
			std::map<Utilities::StringHash, S_SLIDER_UINT32> *m_slidersUint32;
			std::map<Utilities::StringHash, S_SLIDER_INT32> *m_slidersInt32;
			std::map<Utilities::StringHash, S_SLIDER_FLOAT> *m_slidersFloat;
		};

#ifdef _DEBUG
		Utilities::StringHash _tickOnTexture;
		Utilities::StringHash _tickOffTexture;
		DebugMenuNode *_root;
		DebugMenuNode *_currNode;
		std::vector<S_DEBUG_TEXT> *_debugText;
		std::vector<RendererEngine::S_LINE_TO_DRAW> *_debugLine;
		std::vector<RendererEngine::S_SPHERE_TO_DRAW> *_debugSphere;
		UINT8 _u8CurrIndex;
		bool _bReadyToRender;

		void Draw3D( void );
		void Draw2D( void );
		DebugMenuNode* FindParent( const Utilities::StringHash &i_name );
		DebugMenuNode* FindParent( const Utilities::StringHash &i_name, const DebugMenuNode &i_node );
		DebugMenuNode* FindSelectedParent( DebugMenuNode &i_node );

	public:
		bool Initialize( void );
		void BeginUpdate( void );
		void Update( void );
		void UpdateGUI( void );
		void EndUpdate( void );
		void ShutDown( void );

		bool SetUpDebugMenuRenderer( void );

		void Log( char *i_text, const D3DXCOLOR &i_colour = Utilities::WHITE );
		void Log( char *i_text, const UINT32 &i_u32LiveTime, const D3DXCOLOR &i_colour = Utilities::WHITE );
	
		bool AddChildren( const char *i_name, const char *i_parentName );

		bool AddCheckBox( const char *i_label, bool &i_currState, const char* i_parentName = "root",
			void (*function)(const char*, bool) = NULL );

		bool AddButton( const char *i_label, ButtonHandler i_buttonHandler, const char* i_parentName = "root" );

		// Sliders
		bool AddSlider( const char *i_label, UINT32 &i_u32CurrValue, const UINT32 &i_u32MinValue,
			const UINT32 &i_u32MaxValue, const UINT32 &i_u32Increment = 1,
			const char* i_parentName = "root", void (*i_function)(const char*, UINT32) = NULL );
		bool AddSlider( const char *i_label, INT32 &i_i32CurrValue, const INT32 &i_i32MinValue,
			const INT32 &i_i32MaxValue, const INT32 &i_i32Increment = 1,
			const char* i_parentName = "root", void (*i_function)(const char*, INT32) = NULL );
		bool AddSlider( const char *i_label, float &i_currValue, const float &i_minValue,
			const float &i_maxValue, const float &i_increment = 1.0f,
			const char* i_parentName = "root", void (*i_function)(const char*, float) = NULL );

		void DrawLine( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint, const D3DXCOLOR &i_colour = Utilities::RED );
		void DrawSphere( const D3DXVECTOR3 & i_centre, const float &i_radius = Utilities::DEFAULT_DEBUG_SPHERE_RADIUS, const D3DCOLOR &i_colour = Utilities::RED );
		void DrawBox( const D3DXVECTOR3 &i_max, const float &i_size, const D3DCOLOR &i_colour = Utilities::RED );

		void DrawRectangle( const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size, const D3DCOLOR &i_colour = Utilities::RED );
#else
	public:
		bool Initialize( void ){ return SUCCESS; }
		void BeginUpdate( void ){ }
		void Update( void ){ }
		void EndUpdate( void ){ }
		void ShutDown( void ){ }

		bool SetUpDebugMenuRenderer( void ){ return SUCCESS; }

		void Log( char *i_text, const D3DXCOLOR &i_colour = Utilities::WHITE ){ }
		void Log( char *i_text, const UINT32 &i_u32LiveTime, const D3DXCOLOR &i_colour = Utilities::WHITE ){ }

		bool AddChildren( const char *i_name, const char *i_parentName ){ return SUCCESS; }

		bool AddCheckBox( const char *i_label, bool &i_currState, const char* i_parentName = "root",
			void (*function)(const char*, bool) = NULL ){ return SUCCESS; }

		bool AddButton( const char *i_label, ButtonHandler i_buttonHandler, const char* i_parentName = "root" ){ return SUCCESS; }

		// Sliders
		bool AddSlider( const char *i_label, UINT32 &i_u32CurrValue, const UINT32 &i_u32MinValue,
			const UINT32 &i_u32MaxValue, const UINT32 &i_u32Increment = 1,
			const char* i_parentName = "root", void (*i_function)(const char*, UINT32) = NULL ){ return SUCCESS; }
		bool AddSlider( const char *i_label, INT32 &i_i32CurrValue, const INT32 &i_i32MinValue,
			const INT32 &i_i32MaxValue, const INT32 &i_i32Increment = 1,
			const char* i_parentName = "root", void (*i_function)(const char*, INT32) = NULL ){ return SUCCESS; }
		bool AddSlider( const char *i_label, float &i_currValue, const float &i_minValue,
			const float &i_maxValue, const float &i_increment = 1.0f,
			const char* i_parentName = "root", void (*i_function)(const char*, float) = NULL ){ return SUCCESS; }

		void DrawLine( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint ){ }
		void DrawSphere( const D3DXVECTOR3 & i_centre, const float &i_radius, const D3DCOLOR &i_colour ){ }
		void DrawBox( const D3DXVECTOR3 &i_max, const float &i_size, const D3DCOLOR &i_colour = Utilities::RED ){ }
		void DrawRectangle( const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size, const D3DCOLOR &i_colour = Utilities::RED ){ }
#endif	// #ifdef _DEBUG
	};
}	// namespace Utilities

typedef Utilities::Singleton<GameEngine::DebugMenu> g_debugMenu;


#endif	// #ifndef _DEBUG_MENU_H_