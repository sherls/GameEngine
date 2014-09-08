/**
 ****************************************************************************************************
 * \file		cCamera.cpp
 * \brief		Camera implementation
 ****************************************************************************************************
*/

// Utilities header
#include <Debug/Debug.h>

#include "../GameEngine.h"
#include "../World/World.h"
#include "../Renderer/Renderer.h"
#include "../DebugMenu/DebugMenu.h"
#include "../Utilities/IDCreator/IDCreator.h"

#include "Camera.h"
#include "CameraController.h"

bool GameEngine::Camera::m_bOnDebugCamera = false;

/**
 ****************************************************************************************************
	\fn			void Move( void )
	\brief		Move camera
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Camera::Move( void )
{
	D3DXVECTOR3 direction, rotationAxis, position;
	D3DXMATRIX rotationAxisMatrix, rotationZMatrix;

	FUNCTION_START;

	position = GetPosition();

	// Create direction vector
	D3DXVec3Normalize( &direction, &(_entity->m_vLookAt - position) );

	// Create strafe vector
	D3DXVec3Cross( &rotationAxis, &rotationAxis, &m_upVector );
	D3DXVec3Normalize( &rotationAxis, &rotationAxis );

	// Create rotation matrix
	D3DXMatrixRotationAxis( &rotationAxisMatrix, &rotationAxis, m_yawChange );
	D3DXMatrixRotationZ( &rotationZMatrix, 0.0f );

	// Rotate direction
	D3DXVec3TransformCoord( &direction, &direction, &(rotationAxisMatrix * rotationZMatrix) );
	// Rotate up vector
	D3DXVec3TransformCoord( &m_upVector, &m_upVector, &(rotationAxisMatrix * rotationZMatrix) );
	// Translate up vector
	_entity->m_vLookAt = direction + position;

	D3DXMatrixLookAtLH( &m_worldToViewMatrix, &position, &_entity->m_vLookAt, &m_upVector );

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Rotate( void )
	\brief		Rotate camera
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Camera::Rotate( void )
{
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX yawMatrix, pitchMatrix, rollMatrix;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 position = GetPosition();

	FUNCTION_START;

	D3DXVec3Cross( &rightVector, &m_upVector, &_entity->m_vLookAt );
	rightVector.y = 0.0f;
	D3DXVec3Normalize( &rightVector, &rightVector );

	D3DXMatrixRotationAxis( &pitchMatrix, &rightVector, D3DXToRadian(m_pitchChange) );
	D3DXMatrixRotationAxis( &yawMatrix, &m_upVector, D3DXToRadian(m_yawChange) );
	D3DXMatrixRotationAxis( &rollMatrix, &_entity->m_vLookAt, D3DXToRadian(m_rollChange) );

	D3DXMatrixMultiply( &rotationMatrix, &yawMatrix, &pitchMatrix );
	D3DXMatrixMultiply( &rotationMatrix, &rollMatrix, &rotationMatrix );

	D3DXVec3TransformCoord( &rightVector, &rightVector, &rotationMatrix );
	D3DXVec3TransformCoord( &m_upVector, &m_upVector, &rotationMatrix );
	D3DXVec3Cross( &_entity->m_vLookAt, &rightVector, &m_upVector );

	if( fabs(D3DXVec3Dot(&m_upVector, &rightVector)) > 0.01f )
	{
		D3DXVec3Cross( &m_upVector, &_entity->m_vLookAt, &rightVector );
	}

	D3DXVec3Normalize( &rightVector, &rightVector );
	D3DXVec3Normalize( &m_upVector, &m_upVector );
	D3DXVec3Normalize( &_entity->m_vLookAt, &_entity->m_vLookAt );

	float fView41,fView42,fView43;
	fView41 = -D3DXVec3Dot( &rightVector, &position);
	fView42 = -D3DXVec3Dot( &m_upVector, &position );
	fView43 = -D3DXVec3Dot( &_entity->m_vLookAt, &position );

	m_worldToViewMatrix = D3DXMATRIX(rightVector.x, m_upVector.x, _entity->m_vLookAt.x, 0.0f,
								rightVector.y, m_upVector.y, _entity->m_vLookAt.y, 0.0f,
								rightVector.z, m_upVector.z, _entity->m_vLookAt.z, 0.0f,
								fView41, fView42,  fView43,	1.0f);

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			cCamera( Utilities::Pointer::SmartPtr<Entity> &i_followEntity )
	\brief		cCamera default constructor
	\param		i_followEntity the entity this camera follows
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Camera::Camera( Utilities::Pointer::SmartPtr<Entity> &i_followEntity ) :
	_position( 0.0f, 0.0f, -10.f ),
	_followEntity( i_followEntity ),
	_angle( D3DXToRadian(45.0f) ),
	_aspectRatio( static_cast<float>(Renderer::GetWindowSize().width) / static_cast<float>(Renderer::GetWindowSize().height) ),
	_nearView( 0.1f ),
	m_upVector( D3DXVECTOR3_UP ),
	m_farView( 100.0f ),
	m_yawChange( 0.0f ),
	m_pitchChange( 0.0f ),
	m_rollChange( 0.0f ),
	m_backDistance( 0.0f ),
	m_upDistance( 0.0f ),
	m_bAngleChanged( false )
{
	D3DXMatrixIdentity( &m_worldToViewMatrix );

	if( m_backDistance > 0.0f )
		CreateCameraEntity( i_followEntity );

#ifdef ENABLE_FREE_CAMERA
	g_debugMenu::Get().AddCheckBox( "Free camera", m_bOnDebugCamera );
#endif	// #ifdef ENABLE_FREE_CAMERA
}

/**
 ****************************************************************************************************
	\fn			cCamera( Utilities::Pointer::SmartPtr<Entity> &i_followEntity,
					D3DXVECTOR3 &i_position, D3DXVECTOR3 &i_lookAtPoint,
					float &i_angle, float &i_aspectRatio, float &i_nearView, float &i_farView )
	\brief		cCamera constructor
	\param		i_followEntity the entity this camera follows
	\param		i_position position of camera
	\param		i_lookDirection looking direction
	\param		i_angle angle of camera
	\param		i_aspectRatio aspect ratio of camera
	\param		i_nearView near view of camera
	\param		i_farView far view of camera
	\return		NONE
 ****************************************************************************************************
*/
GameEngine::Camera::Camera( Utilities::Pointer::SmartPtr<Entity> &i_followEntity,
	D3DXVECTOR3 &i_position, D3DXVECTOR3 &i_lookDirection,
		float &i_angle, float &i_aspectRatio, float &i_nearView, float &i_farView ) :
	_position( i_position ),
	_followEntity( i_followEntity ),
	_angle( D3DXToRadian(i_angle) ),
	_aspectRatio( i_aspectRatio ),
	_nearView( i_nearView ),
	m_upVector( D3DXVECTOR3_UP ),
	m_farView( i_farView ),
	m_yawChange( 0.0f ),
	m_pitchChange( 0.0f ),
	m_rollChange( 0.0f ),
	m_backDistance( 0.0f ),
	m_upDistance( 0.0f ),
	m_bAngleChanged( false )
{
	FUNCTION_START;

	if( _aspectRatio == -1 )
		_aspectRatio = static_cast<float>(Renderer::GetWindowSize().width) / static_cast<float>(Renderer::GetWindowSize().height);
	D3DXMatrixIdentity( &m_worldToViewMatrix );

	CreateCameraEntity( i_followEntity );
	D3DXVec3Normalize( &_entity->m_vLookAt, &i_lookDirection );

#ifdef ENABLE_FREE_CAMERA
	g_debugMenu::Get().AddCheckBox( "Free camera", m_bOnDebugCamera );
#endif	// #ifdef ENABLE_FREE_CAMERA

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void Update( void )
	\brief		cCamera constructor
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Camera::Update( void )
{
	FUNCTION_START;

	if( m_bAngleChanged ||
		(!m_bAngleChanged && Utilities::Math::AreRelativelyEqual(m_yawChange, 0.0f) && Utilities::Math::AreRelativelyEqual(m_pitchChange, 0.0f))
		)
	Rotate();

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			void EndUpdate( void )
	\brief		End updating AI
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Camera::EndUpdate( void )
{
	FUNCTION_START;

// 	m_prevYaw = m_currYaw;
// 	m_prevPitch = m_currPitch;
// 	m_prevRoll = m_currRoll;
// 
// 	m_currYaw += m_yawChange;
// 	m_currPitch += m_pitchChange;
// 	m_currRoll += m_rollChange;
	m_yawChange = 0.0f;
	m_pitchChange = 0.0f;
	m_rollChange = 0.0f;

	m_bYawChanged = false;
	m_bPitchChanged = false;
	m_bRollChanged = false;
	m_bAngleChanged = false;

	FUNCTION_FINISH;
}

/**
 ****************************************************************************************************
	\fn			D3DXVECTOR3 GetPosition( void )
	\brief		Get real camera position in the world
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const D3DXVECTOR3 GameEngine::Camera::GetPosition( void ) const
{
	D3DXVECTOR3 position;

	FUNCTION_START;

	if( m_bOnDebugCamera )
	{
		position = _position;
	}
	else
	{
		position.x = _entity->m_v3Position.X();
		position.y = _entity->m_v3Position.Y();
		position.z = _entity->m_v3Position.Z();
	}

	FUNCTION_FINISH;
	return position;
}

/**
 ****************************************************************************************************
	\fn			const D3DXVECTOR3 &GetViewDirection( void ) const
	\brief		Get camera view direction
	\param		NONE
	\return		D3DXVECTOR3
	\retval		current camera direction
 ****************************************************************************************************
*/
const D3DXVECTOR3 &GameEngine::Camera::GetViewDirection( void ) const
{
	return _entity->m_vLookAt;
}

/**
 ****************************************************************************************************
	\fn			const D3DXMATRIX GetViewToProjectedTransform( void ) const
	\brief		Get camera view to projected transform
	\param		NONE
	\return		NONE
 ****************************************************************************************************
*/
const D3DXMATRIX GameEngine::Camera::GetViewToProjectedTransform( void ) const
{
	D3DXMATRIX transform_viewToProjected;

	FUNCTION_START;

	D3DXMatrixPerspectiveFovLH( &transform_viewToProjected, _angle, _aspectRatio, _nearView, m_farView );

	FUNCTION_FINISH;
	return transform_viewToProjected;
}

/**
 ****************************************************************************************************
	\fn			void CreateCameraEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
	\brief		Create camera entity
	\param		i_entity the camera entity
	\return		NONE
 ****************************************************************************************************
*/
void GameEngine::Camera::CreateCameraEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity )
{
	CameraController *controller = new CameraController();

	FUNCTION_START;

	controller->m_followEntity = i_entity;
	_entity = Entity::Create( _position, controller, "Camera" );
	_entity->m_applyPhysics = false;
	_entity->m_u8EntityID = g_IDCreator::Get().GetID( "Camera" );
	_entity->m_u32CollisionMask = g_IDCreator::Get().IDtoBitMask( g_IDCreator::Get().GetID("COL") );

	g_world::Get().AddEntity( _entity );
	g_world::Get().SetCollisionDetectionType( E_COLLISION_BY_MESH );
	g_world::Get().CreateCollisionEntity( _entity, "fakeMesh.txt" );
	g_world::Get().SetCollisionHandler( _entity, new CameraCollisionHandler() );

	FUNCTION_FINISH;
}