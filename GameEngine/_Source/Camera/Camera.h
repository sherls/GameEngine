/**
 ****************************************************************************************************
 * \file		Camera.h
 * \brief		The camera control
 ****************************************************************************************************
*/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../World/Entity.h"
#include "../Utilities/GameEngineTypes.h"

namespace GameEngine
{
	class Camera
	{
		Utilities::Pointer::SmartPtr<Entity>	_followEntity;
		Utilities::Pointer::SmartPtr<Entity>	_entity;
		float		_angle;
		float		_aspectRatio;
		float		_nearView;

		void Move( void );
		void Rotate( void );
		void CreateCameraEntity( Utilities::Pointer::SmartPtr<Entity> &i_entity );

	public:
		static bool						m_bOnDebugCamera;
		D3DXMATRIX						m_worldToViewMatrix;
		D3DXVECTOR3						_position;
		D3DXVECTOR3						m_upVector;
		float									m_farView;
		float									m_yawChange;
		float									m_pitchChange;
		float									m_rollChange;
		float									m_backDistance;
		float									m_upDistance;
		bool									m_bAngleChanged : 1;
		bool									m_bYawChanged : 1;
		bool									m_bPitchChanged : 1;
		bool									m_bRollChanged : 1;

		Camera( Utilities::Pointer::SmartPtr<Entity> &i_followEntity );

		Camera( Utilities::Pointer::SmartPtr<Entity> &i_followEntity, D3DXVECTOR3 &i_position, D3DXVECTOR3 &i_lookDirection,
			float &i_angle, float &i_aspectRatio, float &i_nearView, float &i_farView );

		void Update( void );
		void EndUpdate( void );
		const D3DXVECTOR3 GetPosition( void ) const;
		const D3DXMATRIX GetViewToProjectedTransform( void ) const;
		const D3DXVECTOR3 &GetViewDirection( void ) const;
	};
}

#endif	// #ifndef _C_CAMERA_H_