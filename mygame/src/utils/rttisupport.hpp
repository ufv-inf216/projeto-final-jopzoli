/*
*
*/

#pragma once

#include "asserts.hpp"
#include "hashsupport.hpp"
#include "memory/memory.hpp"
#include "util.hpp"

typedef hash_t typeid_t;

#pragma warning(disable: WARN_DEPRECATED_)

struct rttype_info;

namespace detail
{
template <typename, typename...> struct rttype_data_wrapper;
template <typename> struct rttype_info_proxy;
template <typename> struct rttype_info_wrapper;

template <typename _Ty, typename _BaseTy>
NODISCARD inline constexpr ptrdiff_t computeOffset( ) noexcept
{
	_Ty* dPtr = (_Ty*) (1);
	_BaseTy* bPtr = static_cast<_BaseTy*>(dPtr);

	return (intptr_t) (bPtr) - (intptr_t) (dPtr);
}

template <typename _Ty>
NODISCARD ALWAYSINLINE constexpr typeid_t generateID( ) noexcept
{
	return static_hash<_Ty>( );
}

template <typename _Ty, typename _OtherTy>
NODISCARD ALWAYSINLINE constexpr _Ty* rawCast(_OtherTy* _val) noexcept
{
	return (_Ty*) (_val);
}

template <typename _Ty, typename _OtherTy>
NODISCARD ALWAYSINLINE constexpr _Ty* rawCastAdvance(_OtherTy* _val, ptrdiff_t _cnt) noexcept
{
	return (_Ty*) (_val + _cnt);
}

template <typename _Ty, typename _OtherTy>
NODISCARD ALWAYSINLINE constexpr _Ty* reinterpretCastAdvance(_OtherTy* _val, ptrdiff_t _cnt) noexcept
{
	return reinterpret_cast<_Ty*>(_val + _cnt);
}

#pragma pack(push, 1)

template <typename...> struct rttype_data_filler { };
template <typename> struct rttype_data_proxy { };

template <typename _BaseTy>
struct rttype_data_filler<std::enable_shared_from_this<_BaseTy>>
{
	template <typename _Ty>
	typeid_t fillBase(ptrdiff_t) noexcept { }
};

template <typename _BaseTy>
struct rttype_data_filler<_BaseTy>
{
	byte_t headData[sizeof(rttype_data_proxy<_BaseTy>) - (sizeof(ptrdiff_t) + sizeof(typeid_t))];

	template <typename _Ty>
	typeid_t fillBase(ptrdiff_t _bOffset) noexcept
	{
		const rttype_data_proxy<_BaseTy>* baseData = rawCast<const rttype_data_proxy<_BaseTy>>(rttype_info_proxy<_BaseTy>::get( )->getTypeDataHead( ));
		typeid_t headSize = baseData->size;
		const byte_t* data = baseData->getData( );
		size_t nSize = baseData->size * sizeof(typeid_t);

		//std::memcpy(headData, data, nSize);
		std::copy(data, data + nSize, headData);
		size_t byteIdx = nSize;
		ptrdiff_t offset = *reinterpretCastAdvance<const ptrdiff_t>(data, byteIdx);
		while (offset != 0)
		{
			*reinterpretCastAdvance<ptrdiff_t>(headData, byteIdx) = offset + _bOffset;
			byteIdx += sizeof(ptrdiff_t);

			const typeid_t size = *reinterpretCastAdvance<const typeid_t>(data, byteIdx);
			*reinterpretCastAdvance<typeid_t>(headData, byteIdx) = size;
			nSize = size * sizeof(typeid_t);
			byteIdx += sizeof(typeid_t);

			//std::memcpy(headData + byteIdx, data + byteIdx, nSize);
			std::copy(data + byteIdx, data + byteIdx + nSize, headData + byteIdx);
			byteIdx += nSize;

			offset = *reinterpretCastAdvance<const ptrdiff_t>(data, byteIdx);
		}

		return headSize;
	}
};

template <typename _FirstTy, typename _SecondTy, typename... _RemnTy>
struct rttype_data_filler<_FirstTy, _SecondTy, _RemnTy...>
{
	template <typename _Ty>
	typeid_t fillBase(ptrdiff_t) noexcept
	{
		typeid_t headSize = firstBase.fillBase<_Ty>(computeOffset<_Ty, _FirstTy>( ));
		offset = computeOffset<_Ty, _SecondTy>( );
		size = remnBase.fillBase<_Ty>(offset);

		return headSize;
	}

	rttype_data_filler<_FirstTy> firstBase;
	ptrdiff_t offset;
	typeid_t size;
	rttype_data_filler<_SecondTy, _RemnTy...> remnBase;
};

template <typename _Ty>
struct rttype_data_wrapper<_Ty>
{
	using value_type = _Ty;

	const typeid_t size{ 1 };
	const typeid_t typeID{ generateID<_Ty>( ) };
	const ptrdiff_t endMarker{ };

	_NODISCARD constexpr const byte_t* getData( ) const noexcept
	{
		return rawCast<const byte_t>(&typeID);
	}
};

template <typename _Ty, typename... _BaseTy>
struct rttype_data_wrapper
{
	static_assert(unique_parameters<_Ty, _BaseTy...>::value, "template parameters must be unique");
	using value_type = _Ty;

	const typeid_t size;
	const typeid_t typeID{ generateID<_Ty>( ) };
	rttype_data_filler<_BaseTy...> baseData;
	const ptrdiff_t endMarker{ };

	rttype_data_wrapper( ) noexcept
		: size{ baseData.fillBase<_Ty>(0) + 1}
	{ }

	NODISCARD constexpr const byte_t* getData( ) const noexcept
	{
		return rawCast<const byte_t>(&typeID);
	}
};

template <typename _Ty>
struct rttype_info_wrapper
{
	using value_type = _Ty;

	const rttype_info info;
	const rttype_data_proxy<_Ty> data;
};

#pragma pack(pop)

template <typename _Ty>
NODISCARD inline const rttype_info* getTypeInfo( ) noexcept
{
	return rttype_info_proxy<
		std::decay_t<typename std::remove_cv<_Ty>::type>>::get( );
}

// TODO: exception
template <typename _Ty, typename _OtherTy>
NODISCARD _Ty dynamicCast(_OtherTy _val)
{
	static_assert(std::is_pointer_v<_Ty> && std::is_pointer_v<_OtherTy>,
		"rtti casting only accepted between pointers");
	using value_type = std::remove_pointer_t<_Ty>;
	using other_type = std::remove_pointer_t<_OtherTy>;
	if constexpr (std::is_base_of_v<value_type, other_type>)
		return static_cast<_Ty>(_val);
	else if (_val)
		return reinterpret_cast<_Ty>(_val->rttiCast2(getTypeInfo<value_type>( )->getTypeID( )));
	else
		return nullptr;
}

#define _RTTI_DECL_TYPEDATA_(...)															\
template <>																					\
struct detail::rttype_data_proxy<ARG_1(__VA_ARGS__)> : rttype_data_wrapper<__VA_ARGS__> { };

#define _RTTI_DECL_TYPEINFO_(x)											\
template <>																\
struct detail::rttype_info_proxy<x>										\
{																		\
	NODISCARD ALWAYSINLINE static const rttype_info* get( ) noexcept	\
	{																	\
		static rttype_info_wrapper<x> inst = {							\
			rttype_info{ typeid(x) },									\
			rttype_data_proxy<x>{ }										\
		};																\
		return &inst.info;												\
	}																	\
};
}

struct rttype_info
{
	const std::type_info* const ltypeid;

	constexpr explicit rttype_info(const std::type_info& _val) noexcept : ltypeid{ &_val } { }

	rttype_info(const rttype_info&) = delete;
	rttype_info(rttype_info&&) = delete;
	rttype_info& operator=(const rttype_info&) = delete;
	rttype_info& operator=(rttype_info&&) = delete;

	intptr_t cast2(intptr_t _bPtr, typeid_t _bTypeID) const;

	DEPRECATEDY("do not call this function direcly") NODISCARD ALWAYSINLINE
		const byte_t* getTypeDataHead( ) const noexcept
	{
		return detail::rawCastAdvance<const byte_t>(this, 1);
	}

	DEPRECATEDY("do not call this function direcly") NODISCARD ALWAYSINLINE
		typeid_t getTypeID( ) const noexcept
	{
		return *detail::rawCastAdvance<typeid_t>(getTypeDataHead( ), sizeof(typeid_t));
	}

	NODISCARD inline const char* name( ) const noexcept
	{
		return ltypeid->name( );
	}

	NODISCARD inline bool operator==(const rttype_info& _rhs) const noexcept
	{
		// return *ltypeid == *_rhs.ltypeid;
		return getTypeID( ) == _rhs.getTypeID( );
	}

	NODISCARD inline bool operator!=(const rttype_info& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
};

template <typename _Ty, typename _OtherTy>
NODISCARD ALWAYSINLINE _Ty rtti_dynamic_cast(_OtherTy _val)
{
	return detail::dynamicCast<_Ty>(_val);
}

template <typename _Ty>
NODISCARD ALWAYSINLINE const rttype_info& rtti_type_info( ) noexcept
{
	static_assert(ExplicitlyHashSpecialized<_Ty>, "can't generate an valid id for given type");

	return *detail::getTypeInfo<_Ty>( );
}

template <typename _Ty>
NODISCARD ALWAYSINLINE const rttype_info& rtti_type_info(const _Ty&) noexcept
{
	return rtti_type_info<_Ty>( );
}

template <typename _Ty>
NODISCARD ALWAYSINLINE typeid_t rtti_typeid( ) noexcept
{
	// NOTE: sanity check
	_WARNING(detail::getTypeInfo<_Ty>( )->getTypeID( ) == detail::generateID<_Ty>( ),
		"wrong typeid generated for given type might be result of inapropriate object declaration");

	return detail::getTypeInfo<_Ty>( )->getTypeID( );
}

template <typename _Ty>
NODISCARD ALWAYSINLINE typeid_t rtti_typeid(const _Ty&) noexcept
{
	return rtti_typeid<_Ty>( );
}

#define RTTI_REGISTER_TYPE(...) _RTTI_DECL_TYPEDATA_(__VA_ARGS__) _RTTI_DECL_TYPEINFO_(ARG_1(__VA_ARGS__))

#define RTTI_POLYMORPHIC_IMPL( )													\
	virtual intptr_t rttiCast2(typeid_t _bTypeID) const {							\
		using self_type = typename std::remove_pointer_t<decltype(this)>;			\
		return rtti_type_info<self_type>( ).cast2((intptr_t) (this), _bTypeID);		\
	}

#pragma warning(default: WARN_DEPRECATED_)
