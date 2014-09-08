/**
 ****************************************************************************************************
 * \file		Component.h
 * \brief		Component class header
 ****************************************************************************************************
*/

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>

namespace GameEngine
{
	class Component
	{
	public:
		const std::string *m_name;
		const std::string *m_type;

		Component( const std::string &i_name, const std::string &i_type );
		virtual ~Component() {}

	//protected:
		//Utilities::Pointer::SmartPtr<Entity>	_entity;
	};
}	// namespace GameEngine

#endif	// #ifndef _COMPONENT_H_