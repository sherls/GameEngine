#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_

namespace GameEngine
{
	class Entity;
	class EntityController;
}

class CameraController : public GameEngine::EntityController
{
public:
	void BeginUpdate( GameEngine::Entity &i_entity ) {}
	void Update( GameEngine::Entity &i_entity );
	void EndUpdate( GameEngine::Entity &i_entity ) {}
};

#endif	// #ifndef _CAMERA_CONTROLLER_H_