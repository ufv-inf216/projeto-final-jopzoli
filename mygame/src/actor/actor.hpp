/*
*
*/

#pragma once

#include "component/component.hpp"
#include "component/scenecomponent.hpp"
#include "object/object.hpp"
#include "utils/logger.hpp"
#include <map>

class Actor
	: public Object
{
public:
	OBJIMPL( );

	using map_type = std::pmr::map<std::type_index, object_ptr<Component>>;

	enum Status
	{
		Inactive,
		Active,
	};

	Actor( ) noexcept;

	NODISCARD const map_type& components( ) const noexcept
	{
		return m_components;
	}

	NODISCARD object_ptr<SceneComponent> rootComponent( ) noexcept
	{
		return thisRootComponent;
	}
	
	NODISCARD object_ptr<const SceneComponent> rootComponent( ) const noexcept
	{
		return thisRootComponent;
	}

	NODISCARD Status status( ) const noexcept
	{
		return m_status;
	}

	template <typename _Ty>
	NODISCARD _Ty* getComponent( )
	{
		static_assert(std::is_base_of_v<Component, _Ty>, "given type is not data Component");
		auto it = m_components.find(typeid(_Ty));
		if (it != m_components.end( ))
			return rtti_dynamic_cast<_Ty*>(it->second.get( ));

		_WARNING(it != m_components.end( ), "trying to get unexistent component");

		return nullptr;
	}

	template <typename _Ty>
	NODISCARD object_ptr<const _Ty> getComponent( ) const
	{
		static_assert(std::is_base_of_v<Component, _Ty>, "given type is not data Component");
		auto it = m_components.find(typeid(_Ty));
		if (it != m_components.end( ))
			return rtti_dynamic_cast<_Ty*>(it->second.get( ));

		_WARNING(it != m_components.end( ), "trying to get unexistent component");

		return nullptr;
	}

	void attachRoot(const object_ptr<SceneComponent>& _component) noexcept
	{
		thisRootComponent = _component;
	}

	void detachRoot( ) noexcept
	{
		thisRootComponent = nullptr;
	}

	void addComponent(const object_ptr<Component>& _component);

	template <typename _IterTy>
	void addComponents(_IterTy _first, _IterTy _last)
	{
		for (auto i : std::ranges::subrange(_first, _last))
			addComponent(i);
	}

	void removeComponent(const not_null<Component>& _component) noexcept;

	template <typename _Ty>
		requires std::derived_from<_Ty, Component>
	void removeComponent( ) noexcept
	{
		auto it = m_components.find(typeid(_Ty));
		if (it != m_components.end( ))
		{
			it->second->setOwner(nullptr);
			m_components.erase(it);
		}
	}

	template <typename _IterTy>
	void removeComponents(_IterTy _first, _IterTy _last)
	{
		for (auto i : std::ranges::subrange(_first, _last))
			removeComponent(i);
	}

	void removeAllComponents( ) noexcept
	{
		m_components.clear( );
	}

	void setOwner(Object* _owner) noexcept
	{
		thisOwner = _owner;
	}

	void activate( ) noexcept
	{
		m_status = Active;
		onActivate( );
	}

	void inactivate( ) noexcept
	{
		m_status = Inactive;
		onInactivate( );
	}

	virtual void onActivate( )
	{ }

	virtual void onInactivate( )
	{ }

	void tick(float _deltaTime);

protected:
	object_ptr<SceneComponent> thisRootComponent;

	virtual void tickActor(float _deltaTime)
	{ }

private:
	map_type m_components;
	Status m_status;

	void _tickRootComponent(float _deltaTime);
	void _tickComponents(float _deltaTime);
};
OBJDECL(Actor, Object);
