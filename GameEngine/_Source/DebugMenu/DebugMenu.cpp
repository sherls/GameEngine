/**
 ****************************************************************************************************
 * \file		DebugMenu.cpp
 * \brief		Debug menu class implementation
 ****************************************************************************************************
*/

#ifdef _DEBUG

// Renderer engine header
#include <RendererEngine.h>

#include "DebugMenu.h"
#include "../GameEngine.h"
#include "../World/World.h"
#include "../Camera/Camera.h"
#include "../GameEngineDefault.h"
#include "../Renderer/Renderer.h"
#include "../UserInput/UserInput.h"
#include "../Light/PointLight/PointLight.h"
#include "../Light/DirectionalLight/DirectionalLight.h"

/****************************************************************************************************
			Global variables/enumeration
****************************************************************************************************/
namespace GameEngine
{
	typedef enum _e_menu_type_
	{
		E_MENU_TYPE_CHILDREN = 0,
		E_MENU_TYPE_SLIDER,
		E_MENU_TYPE_CHECK_BOX,
		E_MENU_TYPE_BUTTON,
		E_MENU_TYPE_TEXT
	} E_MENU_TYPE;
}

/****************************************************************************************************
			Public function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize DebugMenu
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::Initialize( void )
{
	FUNCTION_START;

	_bReadyToRender = false;
	_debugText = new std::vector<S_DEBUG_TEXT>();
	_debugLine = new std::vector<RendererEngine::S_LINE_TO_DRAW>();
	_debugSphere = new std::vector<RendererEngine::S_SPHERE_TO_DRAW>();
	_root = new DebugMenuNode( "root" );
	_currNode = _root;
	_u8CurrIndex = 0;

	_tickOnTexture = Utilities::StringHash( "tick_ON.dds" );
	_tickOffTexture = Utilities::StringHash( "tick_OFF.dds" );

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void BeginUpdate( void )
	\brief		Begin DebugMenu update
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::BeginUpdate( void )
{
	FUNCTION_START;

	if( !_bReadyToRender )
	{
		if( !RendererEngine::BeginScene(Utilities::TRANSPARANT) )
		{
			FUNCTION_FINISH;
			return;
		}
		_bReadyToRender = true;
	}

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		Update DebugMenu
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::Update( void )
{
	FUNCTION_START;

	Draw3D();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void UpdateGUI( void )
	\brief		Handle user input and update DebugMenu
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::UpdateGUI( void )
{
	FUNCTION_START;

	if( !IsDebugMenuActivated() )
	{
		FUNCTION_FINISH;
		return;
	}

	UINT32 u32TotalChildren = _currNode->m_children->size();
	UINT32 u32TotalSliders = _currNode->m_slidersUint32->size() + _currNode->m_slidersInt32->size() + _currNode->m_slidersFloat->size();
	UINT32 u32TotalCheckBoxes = _currNode->m_checkBoxes->size();
	UINT32 u32TotalButtons = _currNode->m_buttons->size();
	UINT32 u32TotalSlidersUint32 = _currNode->m_slidersUint32->size();
	UINT32 u32TotalSlidersInt32 = _currNode->m_slidersInt32->size();
	UINT32 u32TotalSlidersFloat = _currNode->m_slidersFloat->size();
	E_MENU_TYPE currMenuType = E_MENU_TYPE_TEXT;

	UINT32 u32TotalItems = u32TotalChildren + u32TotalSliders + u32TotalCheckBoxes + u32TotalButtons;

	if( u32TotalItems > 0 )
	{
		if( UserInput::IsKeyUp(VK_UP) )
			_u8CurrIndex = (_u8CurrIndex + u32TotalItems - 1) % u32TotalItems;
		else if( UserInput::IsKeyUp(VK_DOWN) )
			_u8CurrIndex = (_u8CurrIndex + 1) % u32TotalItems;
		else if( UserInput::IsKeyUp(VK_BACK) )
		{
			if( _currNode->m_label != "root" )
			{
				_currNode = _root;
				_u8CurrIndex = 0;
			}
		}
		else
		{
			bool isUint32Slider = false;
			bool isInt32Slider = false;
			bool isFloatSlider = false;
			UINT32 u32Ctr = _u8CurrIndex;

			// Get the type
			if( _u8CurrIndex < u32TotalChildren )
				currMenuType = E_MENU_TYPE_CHILDREN;
			else if( _u8CurrIndex < (u32TotalChildren + u32TotalButtons) )
			{
				u32Ctr = _u8CurrIndex - u32TotalChildren;
				currMenuType = E_MENU_TYPE_BUTTON;
			}
			else if( _u8CurrIndex < (u32TotalChildren + u32TotalButtons + u32TotalCheckBoxes) )
			{
				u32Ctr = _u8CurrIndex - u32TotalChildren - u32TotalButtons;
				currMenuType = E_MENU_TYPE_CHECK_BOX;
			}
			else if( _u8CurrIndex < u32TotalItems )
			{
				u32Ctr = _u8CurrIndex - u32TotalChildren - u32TotalButtons - u32TotalCheckBoxes;
				currMenuType = E_MENU_TYPE_SLIDER;
				UINT32 u32TotalUint32Slider = _currNode->m_slidersUint32->size();
				UINT32 u32TotalInt32Slider = _currNode->m_slidersInt32->size();
				UINT32 u32TotalFloatSlider = _currNode->m_slidersFloat->size();
				if( u32Ctr < u32TotalUint32Slider )
					isUint32Slider = true;
				else if( u32Ctr < (u32TotalUint32Slider + u32TotalInt32Slider) )
				{
					u32Ctr -= u32TotalUint32Slider;
					isInt32Slider = true;
				}
				else if( u32Ctr < u32TotalSliders )
				{
					u32Ctr = u32Ctr - u32TotalUint32Slider - u32TotalInt32Slider;
					isFloatSlider = true;
				}
				else
					assert( false );
			}

			// The operation
			if( currMenuType == E_MENU_TYPE_SLIDER )
			{
				if( UserInput::IsKeyPressed(VK_RIGHT) )
				{
					if( isUint32Slider )
					{
						std::map<Utilities::StringHash, S_SLIDER_UINT32>::const_iterator iter = _currNode->m_slidersUint32->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						}
						iter->second.value += iter->second.increment;
						if( iter->second.value > iter->second.maxValue )
							iter->second.value = iter->second.maxValue;
					}
					else if( isInt32Slider )
					{
						std::map<Utilities::StringHash, S_SLIDER_INT32>::const_iterator iter = _currNode->m_slidersInt32->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						}
						iter->second.value += iter->second.increment;
						if( iter->second.value > iter->second.maxValue )
							iter->second.value = iter->second.maxValue;
					}
					else if( isFloatSlider )
					{
						std::map<Utilities::StringHash, S_SLIDER_FLOAT>::const_iterator iter = _currNode->m_slidersFloat->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						}
						iter->second.value += iter->second.increment;
						if( iter->second.value > iter->second.maxValue )
							iter->second.value = iter->second.maxValue;
					}
				}
				else if( UserInput::IsKeyPressed(VK_LEFT) )
				{
					if( isUint32Slider )
					{
						std::map<Utilities::StringHash, S_SLIDER_UINT32>::const_iterator iter = _currNode->m_slidersUint32->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						}
						if( iter->second.value > iter->second.minValue )
							iter->second.value -= iter->second.increment;
					}
					else if( isInt32Slider )
					{
						std::map<Utilities::StringHash, S_SLIDER_INT32>::const_iterator iter = _currNode->m_slidersInt32->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						}
						if( iter->second.value > iter->second.minValue )
							iter->second.value -= iter->second.increment;
					}
					else if( isFloatSlider )
					{
						std::map<Utilities::StringHash, S_SLIDER_FLOAT>::const_iterator iter = _currNode->m_slidersFloat->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						}
						if( iter->second.value > iter->second.minValue )
							iter->second.value -= iter->second.increment;
					}
				}
			}
			else
			{
				if( UserInput::IsKeyUp(VK_RIGHT) )
				{
					if( currMenuType == E_MENU_TYPE_CHILDREN )
					{
						std::map<Utilities::StringHash, DebugMenuNode*>::const_iterator iter = _currNode->m_children->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						};
						_currNode = iter->second;
						_u8CurrIndex = 0;
					}
					else if( currMenuType == E_MENU_TYPE_BUTTON )
					{
						std::map<Utilities::StringHash, S_BUTTON>::const_iterator iter = _currNode->m_buttons->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						};
						if( iter->second.handler != NULL )
							iter->second.handler();
					}
					else if( currMenuType == E_MENU_TYPE_CHECK_BOX )
					{
						std::map<Utilities::StringHash, S_CHECK_BOX>::const_iterator iter = _currNode->m_checkBoxes->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						};
						iter->second.value = !iter->second.value;
					}
				}
				else if( UserInput::IsKeyUp(VK_LEFT) )
				{
					if( currMenuType == E_MENU_TYPE_CHILDREN )
					{
						if( _currNode->m_label != "root" )
						{
							_currNode = _root;
							_u8CurrIndex = 0;
						}
					}
					else if( currMenuType == E_MENU_TYPE_CHECK_BOX )
					{
						std::map<Utilities::StringHash, S_CHECK_BOX>::const_iterator iter = _currNode->m_checkBoxes->begin();
						while( u32Ctr > 0 )
						{
							iter++;
							u32Ctr--;
						};
						iter->second.value = !iter->second.value;
					}
				}
			}
		}
	}

	Draw2D();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End DebugMenu update
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::EndUpdate( void )
{
	FUNCTION_START;

	if( _bReadyToRender )
	{
		if( !RendererEngine::EndScene() )
		{
			FUNCTION_FINISH;
			return;
		}
		_bReadyToRender = false;
	}

	_debugText->clear();
	_debugLine->clear();
	_debugSphere->clear();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void ShutDown( void )
	\brief		Shut down DebugMenu and release all memory
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::ShutDown( void )
{
	FUNCTION_START;

	if( _debugText )
	{
		_debugText->clear();
		delete _debugText;
		_debugText = NULL;
	}

	if( _debugLine )
	{
		_debugLine->clear();
		delete _debugLine;
		_debugLine = NULL;
	}

	if( _debugSphere )
	{
		_debugSphere->clear();
		delete _debugSphere;
		_debugSphere = NULL;
	}

	delete _root;
	_root = NULL;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool SetUpDebugMenuRender( void )
	\brief		Initialize DebugMenu
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::SetUpDebugMenuRenderer( void )
{
	FUNCTION_START;

	if( !RendererEngine::CreateTexture("tick_ON.dds") )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	if( !RendererEngine::CreateTexture("tick_OFF.dds") )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void Log( const std::string &i_text, const D3DXCOLOR &i_colour )
	\brief		Add log text to debug menu
	\param		i_text text to be drawn on screen
	\param		i_colour colour of the text
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::Log( char *i_text, const D3DXCOLOR &i_colour )
{
	FUNCTION_START;

	S_DEBUG_TEXT newData = { i_text, i_colour, 0xFFFFFFF };
	_debugText->push_back( newData );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Log( const std::string &i_text, const UINT32 &i_u32LiveTime, const D3DXCOLOR &i_colour )
	\brief		Initialize DebugMenu
	\brief		Add log text to debug menu
	\param		i_text text to be drawn on screen
	\param		i_u32LiveTime live time
	\param		i_colour colour of the text
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::Log( char *i_text, const UINT32 &i_u32LiveTime, const D3DXCOLOR &i_colour )
{
	FUNCTION_START;

	S_DEBUG_TEXT newData = { i_text, i_colour, i_u32LiveTime };
	_debugText->push_back( newData );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool AddChildren( const char *i_name, const char *i_parentName )
	\brief		Add the children of given parent
	\param		i_name name of the children
	\param		i_parentName name of the parent
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::AddChildren( const char *i_name, const char *i_parentName )
{
	Utilities::StringHash hashedParentName = Utilities::StringHash( i_parentName );
	Utilities::StringHash hashedName = Utilities::StringHash( i_name );
	DebugMenuNode *parent = _root;

	FUNCTION_START;

	parent = _root;

	if( hashedParentName != Utilities::StringHash("root") )
		parent = FindParent( i_parentName );

	if( parent == NULL )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	std::map<Utilities::StringHash, DebugMenuNode*>::const_iterator iter;
	iter = parent->m_children->find( hashedName );
	if( iter == parent->m_children->end() )
	{
		std::pair<Utilities::StringHash, DebugMenuNode*> insertingPair( hashedName, new DebugMenuNode(i_name) );
		parent->m_children->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool GameEngine::DebugMenu::AddCheckBox( const char *i_label, bool &i_currState, const char* i_parentName,
				void (*i_function)(const char*, bool) )
	\brief		Add check box to debug menu
	\param		i_label label of check box
	\param		i_currState current state
	\param		i_parentName the name of parent where this button will be attached
	\param		i_function the callback function
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::AddCheckBox( const char *i_label, bool &i_currState, const char* i_parentName,
	void (*i_function)(const char*, bool) )
{
	Utilities::StringHash hashedParentName = Utilities::StringHash( i_parentName );
	Utilities::StringHash hashedName = Utilities::StringHash( i_label );
	DebugMenuNode *parent = _root;

	FUNCTION_START;

	if( hashedParentName != Utilities::StringHash("root") )
	{
		parent = FindParent( hashedParentName );
		if( parent == NULL )
		{
			// Inserting parent to "root"
			AddChildren( i_parentName, "root" );
			parent = FindParent( hashedParentName );
		}
	}

	if( parent == NULL )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	std::map<Utilities::StringHash, S_CHECK_BOX>::const_iterator iter;
	iter = parent->m_checkBoxes->find( hashedName );
	if( iter == parent->m_checkBoxes->end() )
	{
		S_CHECK_BOX newData = { i_label, i_currState };
		std::pair<Utilities::StringHash, S_CHECK_BOX> insertingPair( hashedName, newData );
		parent->m_checkBoxes->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool AddButton( const char *i_name, const char* i_parentName, ButtonHandler i_buttonHandler )
	\brief		Add button to debug menu
	\param		i_label label of the button
	\param		i_parentName the name of parent where this button will be attached
	\param		i_buttonHandler the callback function
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::AddButton( const char *i_label, ButtonHandler i_buttonHandler, const char* i_parentName )
{
	Utilities::StringHash hashedParentName = Utilities::StringHash( i_parentName );
	Utilities::StringHash hashedName = Utilities::StringHash( i_label );
	DebugMenuNode *parent = _root;

	FUNCTION_START;

	if( hashedParentName != Utilities::StringHash("root") )
	{
		parent = FindParent( hashedParentName );
		if( parent == NULL )
		{
			// Inserting parent to "root"
			AddChildren( i_parentName, "root" );
			parent = FindParent( hashedParentName );
		}
	}

	if( parent == NULL )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	std::map<Utilities::StringHash, S_BUTTON>::const_iterator iter;
	iter = parent->m_buttons->find( hashedName );
	if( iter == parent->m_buttons->end() )
	{
		S_BUTTON newData = { i_label, i_buttonHandler };
		std::pair<Utilities::StringHash, S_BUTTON> insertingPair( hashedName, newData );
		parent->m_buttons->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool GameEngine::DebugMenu::AddSlider( const char *i_name, UINT32 &i_u32CurrValue, const UINT32 &i_u32MinValue,
				const UINT32 &i_u32MaxValue, const UINT32 &i_u32Increment,
				const char* i_parentName, void (*i_function)(const char*, UINT32) )
	\brief		Add UINT32-typed slider to debug menu
	\param		i_label label of the slider
	\param		i_u32CurrValue current slider value
	\param		i_u32MinValue minimum slider value
	\param		i_u32MaxValue maximum slider value
	\param		i_u32Increment increment in each step
	\param		i_parentName the name of parent where this slider will be attached
	\param		i_function the callback function
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::AddSlider( const char *i_label, UINT32 &i_u32CurrValue, const UINT32 &i_u32MinValue,
	const UINT32 &i_u32MaxValue, const UINT32 &i_u32Increment,
	const char* i_parentName, void (*i_function)(const char*, UINT32) )
{
	Utilities::StringHash hashedParentName = Utilities::StringHash( i_parentName );
	Utilities::StringHash hashedName = Utilities::StringHash( i_label );
	DebugMenuNode *parent = _root;

	FUNCTION_START;

	if( hashedParentName != Utilities::StringHash("root") )
	{
		parent = FindParent( hashedParentName );
		if( parent == NULL )
		{
			// Inserting parent to "root"
			AddChildren( i_parentName, "root" );
			parent = FindParent( hashedParentName );
		}
	}

	if( parent == NULL )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	std::map<Utilities::StringHash, S_SLIDER_UINT32>::const_iterator iter;
	iter = parent->m_slidersUint32->find( hashedName );
	if( iter == parent->m_slidersUint32->end() )
	{
		S_SLIDER_UINT32 newData = { i_label, i_u32CurrValue, i_u32MinValue, i_u32MaxValue, i_u32Increment };
		std::pair<Utilities::StringHash, S_SLIDER_UINT32> insertingPair( hashedName, newData );
		parent->m_slidersUint32->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool GameEngine::DebugMenu::AddSlider( const char *i_name, INT32 &i_i32CurrValue, const INT32 &i_i32MinValue,
				const INT32 &i_i32MaxValue, const INT32 &i_i32Increment,
				const char* i_parentName, void (*i_function)(const char*, INT32) )
	\brief		Add INT32-typed slider to debug menu
	\param		i_label label of the slider
	\param		i_i32CurrValue current slider value
	\param		i_i32MinValue minimum slider value
	\param		i_i32MaxValue maximum slider value
	\param		i_i32Increment increment in each step
	\param		i_parentName the name of parent where this slider will be attached
	\param		i_function the callback function
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::AddSlider( const char *i_label, INT32 &i_i32CurrValue, const INT32 &i_i32MinValue,
	const INT32 &i_i32MaxValue, const INT32 &i_i32Increment,
	const char* i_parentName, void (*i_function)(const char*, INT32) )
{
	Utilities::StringHash hashedParentName = Utilities::StringHash( i_parentName );
	Utilities::StringHash hashedName = Utilities::StringHash( i_label );
	DebugMenuNode *parent = _root;

	FUNCTION_START;

	if( hashedParentName != Utilities::StringHash("root") )
	{
		parent = FindParent( hashedParentName );
		if( parent == NULL )
		{
			// Inserting parent to "root"
			AddChildren( i_parentName, "root" );
			parent = FindParent( hashedParentName );
		}
	}

	if( parent == NULL )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	std::map<Utilities::StringHash, S_SLIDER_INT32>::const_iterator iter;
	iter = parent->m_slidersInt32->find( hashedName );
	if( iter == parent->m_slidersInt32->end() )
	{
		S_SLIDER_INT32 newData = { i_label, i_i32CurrValue, i_i32MinValue, i_i32MaxValue, i_i32Increment };
		std::pair<Utilities::StringHash, S_SLIDER_INT32> insertingPair( hashedName, newData );
		parent->m_slidersInt32->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			bool GameEngine::DebugMenu::AddSlider( const char *i_name, float &i_currValue, const float &i_minValue,
				const float &i_maxValue, const float &i_increment,
				const char* i_parentName, void (*i_function)(const char*, float) )
	\brief		Add float-typed slider to debug menu
	\param		i_label label of the slider
	\param		i_currValue current slider value
	\param		i_minValue minimum slider value
	\param		i_maxValue maximum slider value
	\param		i_increment increment in each step
	\param		i_parentName the name of parent where this slider will be attached
	\param		i_function the callback function
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
bool GameEngine::DebugMenu::AddSlider( const char *i_label, float &i_currValue, const float &i_minValue,
	const float &i_maxValue, const float &i_increment,
	const char* i_parentName, void (*i_function)(const char*, float) )
{
	Utilities::StringHash hashedParentName = Utilities::StringHash( i_parentName );
	Utilities::StringHash hashedName = Utilities::StringHash( i_label );
	DebugMenuNode *parent = _root;

	FUNCTION_START;

	if( hashedParentName != Utilities::StringHash("root") )
	{
		parent = FindParent( hashedParentName );
		if( parent == NULL )
		{
			// Inserting parent to "root"
			AddChildren( i_parentName, "root" );
			parent = FindParent( hashedParentName );
		}
	}

	if( parent == NULL )
	{
		FUNCTION_FINISH;
		return FAIL;
	}

	std::map<Utilities::StringHash, S_SLIDER_FLOAT>::const_iterator iter;
	iter = parent->m_slidersFloat->find( hashedName );
	if( iter == parent->m_slidersFloat->end() )
	{
		S_SLIDER_FLOAT newData = { i_label, i_currValue, i_minValue, i_maxValue, i_increment };
		std::pair<Utilities::StringHash, S_SLIDER_FLOAT> insertingPair( hashedName, newData );
		parent->m_slidersFloat->insert( insertingPair );
	}

	FUNCTION_FINISH;
	return SUCCESS;
}

/**
 ****************************************************************************************************
	\fn			void DrawLine( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint, const D3DXCOLOR &i_colour = Utilities::RED )
	\brief		Draw debug line
	\param		i_startPoint start point
	\param		i_endPoint end point
	\param		i_colour line colour
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::DrawLine( const D3DXVECTOR3 &i_startPoint, const D3DXVECTOR3 &i_endPoint, const D3DXCOLOR &i_colour )
{
	RendererEngine::S_LINE_TO_DRAW newLine;

	FUNCTION_START;

	newLine.startPosition = i_startPoint;
	newLine.endPosition = i_endPoint;
	newLine.startColour = i_colour;
	newLine.endColour = i_colour;
	_debugLine->push_back( newLine );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			bool Initialize( void )
	\brief		Initialize DebugMenu
	\param		NONE
	\return		boolean
	\retval		SUCCESS success
	\retval		FAIL otherwise
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::DrawSphere( const D3DXVECTOR3 &i_centre, const float &i_radius, const D3DCOLOR &i_colour )
{
	FUNCTION_START;

	RendererEngine::S_SPHERE_TO_DRAW newSphere = { i_centre, i_colour, i_radius };
	_debugSphere->push_back( newSphere );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void DrawBox( const D3DXVECTOR3 &i_maxDimension, const float &i_size, const D3DCOLOR &i_colour )
	\brief		Draw debug box
	\param		i_maxDimension maximum dimension
	\param		i_size size of the box
	\param		i_colour the colour of the box
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::DrawBox( const D3DXVECTOR3 &i_maxDimension, const float &i_size, const D3DCOLOR &i_colour )
{
	D3DXVECTOR3 a = i_maxDimension;
	D3DXVECTOR3 b( a.x - i_size, a.y, a.z );
	D3DXVECTOR3 c( a.x - i_size, a.y - i_size, a.z );
	D3DXVECTOR3 d( a.x, a.y - i_size, a.z );
	D3DXVECTOR3 e( a.x, a.y, a.z - i_size );
	D3DXVECTOR3 f( a.x - i_size, a.y, a.z - i_size );
	D3DXVECTOR3 g( a.x, a.y - i_size, a.z - i_size );
	D3DXVECTOR3 h( a.x - i_size, a.y - i_size, a.z - i_size);

	FUNCTION_START;

	DrawLine( a, b, i_colour );
	DrawLine( b, c, i_colour );
	DrawLine( c, d, i_colour );
	DrawLine( d, a, i_colour );

	DrawLine( a, e, i_colour );
	DrawLine( b, f, i_colour );
	DrawLine( c, h, i_colour );
	DrawLine( d, g, i_colour );

	DrawLine( e, f, i_colour );
	DrawLine( f, h, i_colour );
	DrawLine( h, g, i_colour );
	DrawLine( g, e, i_colour );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			DrawRectangle( const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size, const D3DCOLOR &i_colour )
	\brief		Draw debug rectangle
	\param		i_centre the centre of rectangle
	\param		i_size size of the box (from side to side)
	\param		i_colour the colour of the rectangle
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::DrawRectangle( const D3DXVECTOR3 &i_centre, const Utilities::S_SIZE &i_size, const D3DCOLOR &i_colour )
{
	D3DXVECTOR3 a = i_centre + D3DXVECTOR3( i_size.width/2.0f, i_size.height/2.0f, i_size.depth/2.0f );
	D3DXVECTOR3 b( a.x - i_size.width, a.y, a.z );
	D3DXVECTOR3 c( a.x - i_size.width, a.y - i_size.height, a.z );
	D3DXVECTOR3 d( a.x, a.y - i_size.height, a.z );
	D3DXVECTOR3 e( a.x, a.y, a.z - i_size.depth );
	D3DXVECTOR3 f( a.x - i_size.width, a.y, a.z - i_size.depth );
	D3DXVECTOR3 g( a.x, a.y - i_size.height, a.z - i_size.depth );
	D3DXVECTOR3 h( a.x - i_size.width, a.y - i_size.height, a.z - i_size.depth);

	FUNCTION_START;

	DrawLine( a, b, i_colour );
	DrawLine( b, c, i_colour );
	DrawLine( c, d, i_colour );
	DrawLine( d, a, i_colour );

	DrawLine( a, e, i_colour );
	DrawLine( b, f, i_colour );
	DrawLine( c, h, i_colour );
	DrawLine( d, g, i_colour );

	DrawLine( e, f, i_colour );
	DrawLine( f, h, i_colour );
	DrawLine( h, g, i_colour );
	DrawLine( g, e, i_colour );

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private class implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			DebugMenuNode( const std::string &i_label )
	\brief		Default constructor of DebugMenuNode
	\param		i_label the label of the node
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::DebugMenu::DebugMenuNode::DebugMenuNode( const std::string &i_label ) :
	m_label( i_label )
{
	FUNCTION_START;

	m_children = new std::map<Utilities::StringHash, DebugMenuNode*>;
	m_buttons = new std::map<Utilities::StringHash, S_BUTTON>;
	m_checkBoxes = new std::map<Utilities::StringHash, S_CHECK_BOX>;
	m_slidersUint32 = new std::map<Utilities::StringHash, S_SLIDER_UINT32>;
	m_slidersInt32 = new std::map<Utilities::StringHash, S_SLIDER_INT32>;
	m_slidersFloat = new std::map<Utilities::StringHash, S_SLIDER_FLOAT>;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			~DebugMenuNode( void )
	\brief		Default destructor of DebugMenuNode
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::DebugMenu::DebugMenuNode::~DebugMenuNode( void )
{
	FUNCTION_START;

	std::map<Utilities::StringHash, DebugMenuNode*>::const_iterator iter;
	for( iter = m_children->begin(); iter != m_children->end(); iter++ )
	{
		delete iter->second;
	}
	m_children->clear();
	delete m_children;
	delete m_buttons;
	delete m_checkBoxes;
	delete m_slidersUint32;
	delete m_slidersInt32;
	delete m_slidersFloat;

	FUNCTION_FINISH;
}

/****************************************************************************************************
			Private function implementation
****************************************************************************************************/
/**
 ****************************************************************************************************
	\fn			void Draw3D( void )
	\brief		Draw 3D objects of DebugMenu
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::Draw3D( void )
{
	FUNCTION_START;

	//if( !IsDebugMenuActivated() )
	//{
	//	FUNCTION_FINISH;
	//	return;
	//}

	std::vector<RendererEngine::S_ENTITY_TO_DRAW> entityToDraw;

	RendererEngine::Draw3D( entityToDraw, *_debugLine, *_debugSphere,
		g_world::Get().m_camera->m_worldToViewMatrix, g_world::Get().m_camera->GetViewToProjectedTransform(),
		g_world::Get().m_camera->GetPosition(), g_world::Get().m_camera->m_farView,
		g_world::Get().m_directionalLight->GetWorldToViewTransform(), g_world::Get().m_directionalLight->GetViewToProjectedTransform(),
		g_world::Get().m_directionalLight->m_direction, g_world::Get().m_directionalLight->m_colour,
		g_world::Get().m_directionalLight->m_intensity, g_world::Get().m_directionalLight->m_farView,
		g_world::Get().m_pointLight->m_colour, g_world::Get().m_pointLight->m_ambient, g_world::Get().m_pointLight->m_position,
		g_world::Get().m_pointLight->m_intensity, g_world::Get().m_pointLight->m_radius );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Draw2D( void )
	\brief		Draw 2D objects of DebugMenu
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::DebugMenu::Draw2D( void )
{
	FUNCTION_START;

	if( !IsDebugMenuActivated() )
	{
		FUNCTION_FINISH;
		return;
	}

	float textHeightInWorldSpace = DEBUG_MENU_TEXT_HEIGHT / Renderer::GetWindowSize().height;
	float textWidthInWorldSpace = DEBUG_MENU_TEXT_WIDTH / Renderer::GetWindowSize().width;
	float marginInWorldSpace = DEBUG_MENU_MARGIN / Renderer::GetWindowSize().width;
	float columnWidthInWorldSpace = textWidthInWorldSpace / 2;
	float firstColumnXInWorldSpace = -1.0f + textWidthInWorldSpace;
	float secondColumnXInWorldSpace = firstColumnXInWorldSpace + columnWidthInWorldSpace;
	UINT32 currYPosInPixel = 0;
	UINT32 u32TotalButtons = _currNode->m_buttons->size();
	UINT32 u32TotalCheckBoxes = _currNode->m_checkBoxes->size();
	UINT32 u32TotalSlidersUint32 = _currNode->m_slidersUint32->size();
	UINT32 u32TotalSlidersInt32 = _currNode->m_slidersInt32->size();
	UINT32 u32TotalSlidersFloat = _currNode->m_slidersFloat->size();

	UINT32 u32TotalItems = _currNode->m_children->size() + _debugText->size()
		+ u32TotalSlidersUint32 + u32TotalSlidersInt32 + u32TotalSlidersFloat + u32TotalCheckBoxes + u32TotalButtons;
	UINT32 itemCtr = 0;
	UINT32 textCtr = 0;
	UINT32 quadCtr = 0;
	std::vector<RendererEngine::S_QUAD_TO_DRAW> quadsToDraw( u32TotalCheckBoxes
		+ (u32TotalSlidersUint32 * 2) + (u32TotalSlidersInt32 * 2)+ (u32TotalSlidersFloat *2)
		+ 2); // Plus the background and the highlight
	std::vector<RendererEngine::S_TEXT_TO_DRAW> textToDraw(u32TotalItems);

	// The background
	quadsToDraw[quadCtr].position.x = firstColumnXInWorldSpace;
	quadsToDraw[quadCtr].position.y = 1.0f;
	quadsToDraw[quadCtr].size.width = textWidthInWorldSpace;
	quadsToDraw[quadCtr].size.height = (u32TotalItems * (DEBUG_MENU_TEXT_HEIGHT + DEBUG_MENU_MARGIN + DEBUG_MENU_MARGIN)
		/ Renderer::GetWindowSize().height);
	quadsToDraw[quadCtr++].colour = DEBUG_MENU_BACKGROUND_COLOUR;

	// The highlight
	quadsToDraw[quadCtr].position.x = firstColumnXInWorldSpace;
	quadsToDraw[quadCtr].position.y = (((_u8CurrIndex * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
	quadsToDraw[quadCtr].size.width = textWidthInWorldSpace;
	quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
	quadsToDraw[quadCtr++].colour = DEBUG_MENU_HIGHLIGHT_COLOUR;

	// The children
	std::map<Utilities::StringHash, DebugMenuNode*>::const_iterator childrenIter = _currNode->m_children->begin();
	for( ; childrenIter != _currNode->m_children->end(); childrenIter++ )
	{
		// The label
		textToDraw[textCtr].position.x = static_cast<float>(DEBUG_MENU_MARGIN);
		textToDraw[textCtr].position.y = static_cast<float>(currYPosInPixel);
		textToDraw[textCtr].size.x = DEBUG_MENU_TEXT_WIDTH;
		textToDraw[textCtr].size.y = DEBUG_MENU_TEXT_HEIGHT;
		textToDraw[textCtr].colour = DEBUG_MENU_FONT_COLOUR;
		textToDraw[textCtr].text = childrenIter->second->m_label;
		textToDraw[textCtr].hAlign = DT_LEFT;
		textCtr++;
		currYPosInPixel += DEBUG_MENU_TEXT_HEIGHT;
		itemCtr++;
	}

	// Button
	std::map<Utilities::StringHash, S_BUTTON>::const_iterator buttonIter = _currNode->m_buttons->begin();
	for( ; buttonIter != _currNode->m_buttons->end(); buttonIter++ )
	{
		// The label 
		textToDraw[textCtr].position.x = static_cast<float>(DEBUG_MENU_MARGIN);
		textToDraw[textCtr].position.y = static_cast<float>(currYPosInPixel);
		textToDraw[textCtr].size.x = DEBUG_MENU_TEXT_WIDTH;
		textToDraw[textCtr].size.y = DEBUG_MENU_TEXT_HEIGHT;
		textToDraw[textCtr].colour = DEBUG_MENU_FONT_COLOUR;
		textToDraw[textCtr].text = "[" + buttonIter->second.label + "]";
		textToDraw[textCtr].hAlign = DT_LEFT;
		textCtr++;
		currYPosInPixel += DEBUG_MENU_TEXT_HEIGHT;
		itemCtr++;
	}

	// Check box
	std::map<Utilities::StringHash, S_CHECK_BOX>::const_iterator checkBoxIter = _currNode->m_checkBoxes->begin();
	for ( ; checkBoxIter != _currNode->m_checkBoxes->end(); checkBoxIter++ )
	{
		// The label
		textToDraw[textCtr].position.x = static_cast<float>(DEBUG_MENU_MARGIN);
		textToDraw[textCtr].position.y = static_cast<float>(currYPosInPixel);
		textToDraw[textCtr].size.x = DEBUG_MENU_TEXT_WIDTH;
		textToDraw[textCtr].size.y = DEBUG_MENU_TEXT_HEIGHT;
		textToDraw[textCtr].colour = DEBUG_MENU_FONT_COLOUR;
		textToDraw[textCtr].text = checkBoxIter->second.label;
		textToDraw[textCtr].hAlign = DT_LEFT;
		textCtr++;
		currYPosInPixel += DEBUG_MENU_TEXT_HEIGHT;

		quadsToDraw[quadCtr].position.x = secondColumnXInWorldSpace;
		quadsToDraw[quadCtr].position.y = (((itemCtr * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
		quadsToDraw[quadCtr].size.width = textHeightInWorldSpace;
		quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
		if( checkBoxIter->second.value )
			quadsToDraw[quadCtr].texture = _tickOnTexture;
		else
			quadsToDraw[quadCtr].texture = _tickOffTexture;
		quadsToDraw[quadCtr].colour = DEBUG_MENU_SLIDER_BACKGROUND_COLOUR;
		quadCtr++;

		itemCtr++;
	}

	// The UINT32 sliders
	std::map<Utilities::StringHash, S_SLIDER_UINT32>::const_iterator slidersUint32Iter = _currNode->m_slidersUint32->begin();
	for( ; slidersUint32Iter != _currNode->m_slidersUint32->end(); slidersUint32Iter++ )
	{
		textToDraw[textCtr].position.x = static_cast<float>(DEBUG_MENU_MARGIN);
		textToDraw[textCtr].position.y = static_cast<float>(currYPosInPixel);
		textToDraw[textCtr].size.x = DEBUG_MENU_TEXT_WIDTH;
		textToDraw[textCtr].size.y = DEBUG_MENU_TEXT_HEIGHT;
		textToDraw[textCtr].colour = DEBUG_MENU_FONT_COLOUR;
		textToDraw[textCtr].text = slidersUint32Iter->second.label;
		textToDraw[textCtr].hAlign = DT_LEFT;
		textCtr++;
		currYPosInPixel += DEBUG_MENU_TEXT_HEIGHT;

		// The slider background
		quadsToDraw[quadCtr].position.x = secondColumnXInWorldSpace;
		quadsToDraw[quadCtr].position.y = (((itemCtr * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
		quadsToDraw[quadCtr].size.width = columnWidthInWorldSpace;
		quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
		quadsToDraw[quadCtr].colour = DEBUG_MENU_SLIDER_BACKGROUND_COLOUR;
		quadCtr++;

		// The slider foreground
		float percentage = static_cast<float>(slidersUint32Iter->second.value) / static_cast<float>(slidersUint32Iter->second.maxValue - slidersUint32Iter->second.minValue);
		quadsToDraw[quadCtr].size.width = percentage * columnWidthInWorldSpace;
		quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
		quadsToDraw[quadCtr].position.x = secondColumnXInWorldSpace - (columnWidthInWorldSpace - quadsToDraw[quadCtr].size.width);
		quadsToDraw[quadCtr].position.y = (((itemCtr * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
		quadsToDraw[quadCtr].colour = DEBUG_MENU_SLIDER_FOREGROUND_COLOUR;
		quadCtr++;

		itemCtr++;
	}

	// The INT32 sliders
	std::map<Utilities::StringHash, S_SLIDER_INT32>::const_iterator slidersInt32Iter = _currNode->m_slidersInt32->begin();
	for( ; slidersInt32Iter != _currNode->m_slidersInt32->end(); slidersInt32Iter++ )
	{
		textToDraw[textCtr].position.x = static_cast<float>(DEBUG_MENU_MARGIN);
		textToDraw[textCtr].position.y = static_cast<float>(currYPosInPixel);
		textToDraw[textCtr].size.x = DEBUG_MENU_TEXT_WIDTH;
		textToDraw[textCtr].size.y = DEBUG_MENU_TEXT_HEIGHT;
		textToDraw[textCtr].colour = DEBUG_MENU_FONT_COLOUR;
		textToDraw[textCtr].text = slidersInt32Iter->second.label;
		textToDraw[textCtr].hAlign = DT_LEFT;
		textCtr++;
		currYPosInPixel += DEBUG_MENU_TEXT_HEIGHT;

		// The slider background
		quadsToDraw[quadCtr].position.x = secondColumnXInWorldSpace;
		quadsToDraw[quadCtr].position.y = (((itemCtr * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
		quadsToDraw[quadCtr].size.width = columnWidthInWorldSpace;
		quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
		quadsToDraw[quadCtr].colour = DEBUG_MENU_SLIDER_BACKGROUND_COLOUR;
		quadCtr++;

		// The slider foreground
		float percentage = static_cast<float>(slidersInt32Iter->second.value) / static_cast<float>(slidersInt32Iter->second.maxValue - slidersInt32Iter->second.minValue);
		quadsToDraw[quadCtr].size.width = percentage * columnWidthInWorldSpace;
		quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
		quadsToDraw[quadCtr].position.x = secondColumnXInWorldSpace - (columnWidthInWorldSpace - quadsToDraw[quadCtr].size.width);
		quadsToDraw[quadCtr].position.y = (((itemCtr * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
		quadsToDraw[quadCtr].colour = DEBUG_MENU_SLIDER_FOREGROUND_COLOUR;
		quadCtr++;

		itemCtr++;
	}

	// The float sliders
	std::map<Utilities::StringHash, S_SLIDER_FLOAT>::const_iterator slidersFloatIter = _currNode->m_slidersFloat->begin();
	for( ; slidersFloatIter != _currNode->m_slidersFloat->end(); slidersFloatIter++ )
	{
		textToDraw[textCtr].position.x = static_cast<float>(DEBUG_MENU_MARGIN);
		textToDraw[textCtr].position.y = static_cast<float>(currYPosInPixel);
		textToDraw[textCtr].size.x = DEBUG_MENU_TEXT_WIDTH;
		textToDraw[textCtr].size.y = DEBUG_MENU_TEXT_HEIGHT;
		textToDraw[textCtr].colour = DEBUG_MENU_FONT_COLOUR;
		textToDraw[textCtr].text = slidersFloatIter->second.label;
		textToDraw[textCtr].hAlign = DT_LEFT;
		textCtr++;
		currYPosInPixel += DEBUG_MENU_TEXT_HEIGHT;

		// The slider background
		quadsToDraw[quadCtr].position.x = secondColumnXInWorldSpace;
		quadsToDraw[quadCtr].position.y = (((itemCtr * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
		quadsToDraw[quadCtr].size.width = columnWidthInWorldSpace;
		quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
		quadsToDraw[quadCtr].colour = DEBUG_MENU_SLIDER_BACKGROUND_COLOUR;
		quadCtr++;

		// The slider foreground
		float percentage = static_cast<float>(slidersFloatIter->second.value) / static_cast<float>(slidersFloatIter->second.maxValue - slidersFloatIter->second.minValue);
		quadsToDraw[quadCtr].size.width = percentage * columnWidthInWorldSpace;
		quadsToDraw[quadCtr].size.height = textHeightInWorldSpace;
		quadsToDraw[quadCtr].position.x = secondColumnXInWorldSpace - (columnWidthInWorldSpace - quadsToDraw[quadCtr].size.width);
		quadsToDraw[quadCtr].position.y = (((itemCtr * DEBUG_MENU_TEXT_HEIGHT) + DEBUG_MENU_MARGIN) / Renderer::GetWindowSize().height) * -2 + 1;
		quadsToDraw[quadCtr].colour = DEBUG_MENU_SLIDER_FOREGROUND_COLOUR;
		quadCtr++;

		itemCtr++;
	}

	// Text
	std::vector<S_DEBUG_TEXT>::const_iterator iter = _debugText->begin();
	for( ; iter != _debugText->end(); iter++)
	{
		textToDraw[textCtr].position.x = static_cast<float>(DEBUG_MENU_MARGIN);
		textToDraw[textCtr].position.y = static_cast<float>(currYPosInPixel);
		textToDraw[textCtr].size.x = DEBUG_MENU_TEXT_WIDTH;
		textToDraw[textCtr].size.y = DEBUG_MENU_TEXT_HEIGHT;
		textToDraw[textCtr].colour = DEBUG_MENU_FONT_COLOUR;
		textToDraw[textCtr].text = iter->text;
		textToDraw[textCtr].hAlign = DT_LEFT;
		textCtr++;
		currYPosInPixel += DEBUG_MENU_TEXT_HEIGHT;
		itemCtr++;
	}

	RendererEngine::Draw2D( quadsToDraw, textToDraw );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			DebugMenuNode* FindParent( const Utilities::StringHash &i_name )
	\brief		Find the child of given name from root
	\param		i_name name of the child
	\return		DebugMenuNode
	\retval		Pointer to the child of root of the given name
 ****************************************************************************************************
*/
GameEngine::DebugMenu::DebugMenuNode* GameEngine::DebugMenu::FindParent( const Utilities::StringHash &i_name )
{
	FUNCTION_START;

	std::map<Utilities::StringHash, DebugMenuNode*>::const_iterator iter = _root->m_children->find( i_name );

	if( iter == _root->m_children->end() )
	{
		DebugMenuNode *result = NULL;
		for( iter = _root->m_children->begin(); iter != _root->m_children->end(); iter++ )
		{
			result = FindParent( i_name, *(iter->second) );
			if( result != NULL )
			{
				FUNCTION_FINISH;
				return result;
			}

			FUNCTION_FINISH;
			return NULL;
		}
	}

	if( iter == _root->m_children->end() )
	{
		FUNCTION_FINISH;
		return NULL;
	}

	FUNCTION_FINISH;
	return iter->second;
}

/**
 ****************************************************************************************************
	\fn			DebugMenuNode* FindParent( const Utilities::StringHash &i_name, const DebugMenuNode &i_node )
	\brief		Find the child of given name from the node (root's child)
	\param		i_name name of the child
	\param		i_node the node of root
	\return		DebugMenuNode
	\retval		Pointer to the node of the given name (root's child --> child)
 ****************************************************************************************************
*/
GameEngine::DebugMenu::DebugMenuNode* GameEngine::DebugMenu::FindParent( const Utilities::StringHash &i_name,
	const DebugMenuNode &i_node )
{
	FUNCTION_START;

	std::map<Utilities::StringHash, DebugMenuNode*>::const_iterator iter = i_node.m_children->find( i_name );

	if( iter == i_node.m_children->end() )
	{
		DebugMenuNode* result = NULL;
		for( iter = i_node.m_children->begin(); iter != i_node.m_children->end(); iter++ )
		{
			result = FindParent( i_name, *(iter->second) );
			if( result != NULL )
			{
				FUNCTION_FINISH;
				return result;
			}

			FUNCTION_FINISH;
			return NULL;
		}
	}

	FUNCTION_FINISH;
	return iter->second;
}

/**
 ****************************************************************************************************
	\fn			DebugMenuNode* FindSelectedParent( DebugMenuNode &i_node )
	\brief		Find the child of given name from root
	\param		i_name name of the child
	\return		DebugMenuNode
	\retval		Pointer to the child of root of the given name
 ****************************************************************************************************
*/
GameEngine::DebugMenu::DebugMenuNode* GameEngine::DebugMenu::FindSelectedParent( DebugMenuNode &i_node )
{
	//DebugMenuNode* parent = _root;

	FUNCTION_START;

	//std::map<Utilities::StringHash, DebugMenuNode*>::const_iterator iter = i_node.m_children->begin();

	FUNCTION_FINISH;
	return NULL;
}
#endif	// #ifdef _DEBUG