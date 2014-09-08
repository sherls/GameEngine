/**
 ****************************************************************************************************
 * \file		BoundingBox.inl
 * \brief		The inline functions implementation of BoundingBox.h
 ****************************************************************************************************
*/

#include <math.h>

/**
 ****************************************************************************************************
	\fn			BoundingBox( void )
	\brief		Default constructor of BoundingBox class
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::TriggerBox::BoundingBox::BoundingBox( void ) :
	_origin( Math::Vector3::Zero ),
	m_halfW( 0.0f ),
	m_halfH( 0.0f ),
	m_halfD( 0.0f )	
{
}

/**
 ****************************************************************************************************
	\fn			BoundingBox( const BoundingBox &i_other )
	\brief		Copy constructor of BoundingBox class
	\param		i_other other bounding box
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::TriggerBox::BoundingBox::BoundingBox( const BoundingBox &i_other ) :
	_origin( i_other._origin ),
 	m_halfW( i_other.m_halfW ),
	m_halfH( i_other.m_halfH ),
	m_halfD( i_other.m_halfD )
{
}

/**
 ****************************************************************************************************
	\fn			BoundingBox( const Math::Vector3 &i_origin, const Utilities::S_SIZE &i_size )
	\brief		Default constructor of BoundingBox class
	\param		i_origin origin of BoundingBox
	\param		i_size size of the BoundingBox
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::TriggerBox::BoundingBox::BoundingBox( const Math::Vector3 &i_origin, const Utilities::S_SIZE &i_size ) :
	_origin( i_origin ),
	m_halfW( i_size.width/2.0f ),
	m_halfH( i_size.height/2.0f ),
	m_halfD( i_size.depth/2.0f )
{
}

/**
 ****************************************************************************************************
	\fn			BoundingBox( const Math::Vector3 &i_origin, const float &i_halfW, const float &i_halfH, const float &i_halfD )
	\brief		Default constructor of BoundingBox class
	\param		i_origin origin of BoundingBox
	\param		i_halfW half of width
	\param		i_halfH half of height
	\param		i_halfD half of depth
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::TriggerBox::BoundingBox::BoundingBox( const Math::Vector3 &i_origin, const float &i_halfW, const float &i_halfH, const float &i_halfD ) :
	_origin( i_origin ),
	m_halfW( i_halfW ),
	m_halfH( i_halfH ),
	m_halfD( i_halfD )
{
}

/**
 ****************************************************************************************************
	\fn			Math::Vector3 Center( void ) const
	\brief		Get the center of BoundingBox
	\param		NONE
	\return		Math::Vector3 the center of BoundingBox
 ****************************************************************************************************
*/
const GameEngine::Math::Vector3 &GameEngine::TriggerBox::BoundingBox::Center( void ) const
{
	return _origin;
}

/**
 ****************************************************************************************************
	\fn			S_EXTENDS GetExtends( void ) const
	\brief		Get the extend of BoundingBox
	\param		NONE
	\return		S_EXTENDS the minimum x, y, z as well as the maximum x, y, z
 ****************************************************************************************************
*/
const GameEngine::TriggerBox::BoundingBox::S_EXTENDS GameEngine::TriggerBox::BoundingBox::GetExtends( void ) const
{
	return S_EXTENDS( _origin.X() - m_halfW, _origin.X() + m_halfW, _origin.Y() - m_halfH, _origin.Y() + m_halfH, _origin.Z() - m_halfD, _origin.Z() + m_halfD );
}

/**
 ****************************************************************************************************
	\fn			void Move( const Math::Vector3 &i_move )
	\brief		Move the origin as much as given movement
	\param		i_move movemend direction
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::TriggerBox::BoundingBox::Move( const Math::Vector3 &i_move )
{
	_origin += i_move;
}

/**
 ****************************************************************************************************
	\fn			const BoundingBox Moved( const Math::Vector3 &i_move ) const
	\brief		Moved the copy of this BoundingBox and return it
	\param		i_move new origin position
	\return		NONE
 ****************************************************************************************************
*/
const GameEngine::TriggerBox::BoundingBox GameEngine::TriggerBox::BoundingBox::Moved( const Math::Vector3 &i_move ) const
{
	return BoundingBox( _origin + i_move, m_halfW, m_halfH, m_halfD );
}