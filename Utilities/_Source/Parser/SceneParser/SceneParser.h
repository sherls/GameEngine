/**
 ****************************************************************************************************
 * \file		SceneParser.h
 * \brief		Scene parser class declaration
 ****************************************************************************************************
*/

#ifndef _SCENEPARSER_H_
#define _SCENEPARSER_H_

#include "../../UtilitiesTypes.h"

namespace Utilities
{
	class SceneParser
	{
		// Constructor
		SceneParser( void );

		void LoadSceneData( const char* i_fileName );

	public:
		typedef struct _s_entity_
		{
			D3DXVECTOR3 position;
			std::string file;
			float orientation;
		} S_ENTITY;

		D3DXVECTOR3 m_lightPos;
		D3DXVECTOR3 m_cameraPos;
		D3DXVECTOR3 m_cameraLook;
		D3DXVECTOR3 m_cameraUp;
		D3DXVECTOR3 m_directionalLightDir;
		D3DXVECTOR3 m_lightLook;
		D3DCOLOR m_lightColour;
		D3DCOLOR m_ambientLight;
		D3DCOLOR m_directionalLightColour;
		D3DCOLOR m_directionalAmbientLight;
		UINT32 m_u32TotalEntity;
		std::string m_environmentMap;
		float m_cameraAngle;
		float m_cameraAspect;
		float m_cameraNear;
		float m_cameraFar;
		float m_lightIntensity;
		float m_lightAttenuator;
		float m_lightRadius;
		float m_directionalLightIntensity;
		float m_lightNear;
		float m_lightFar;
		float m_lightWidth;
		float m_lightHeight;

		S_ENTITY *m_entity;

		// Constructor
		inline SceneParser( const char *i_filename );

		// Destructor
		inline ~SceneParser( void );
	};
}

#include "SceneParser.inl"

#endif	// #ifndef _SCENEPARSER_H_