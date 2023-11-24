/*
* 
*/

#pragma once

#include "object/object.hpp"

class Component
	: public Object
{
	friend class Actor;
public:
	OBJIMPL( );
	Component( ) noexcept { }

	NODISCARD bool isActive( ) const noexcept
	{
		return m_active;
	}

	NODISCARD bool isCreated( ) const noexcept
	{
		return m_created;
	}

	NODISCARD bool isInitialized( ) const noexcept
	{
		return m_isInitialized;
	}

	NODISCARD bool isBeingDestroyed( ) const noexcept
	{
		return m_beingDestroyed;
	}

	// TODO: begunplay, endplay?

	virtual void initialize( )
	{ }

	// TODO
	void setActive(bool _val = true) noexcept
	{
		if (!m_active && _val)
		{
			m_active = true;
			// activate operation...
		}
		if (m_active && !_val)
		{
			m_active = false;
			// deactivate operation...
		}
	}

	virtual void tick(float _deltaTime)
	{ }

protected:
	bool registered;

	void setOwner(Object* _owner) noexcept
	{
		thisOwner = _owner;
	}

private:
	bool m_active;
	bool m_created;
	bool m_isInitialized;
	bool m_beingDestroyed;
};
OBJDECL(Component, Object);
