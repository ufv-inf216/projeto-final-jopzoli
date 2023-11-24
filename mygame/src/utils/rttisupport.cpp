#include "rttisupport.hpp"

#pragma warning(disable: WARN_DEPRECATED_)

intptr_t rttype_info::cast2(intptr_t _bPtr, typeid_t _bTypeID) const
{
	constexpr size_t tidsz = sizeof(typeid_t);
	const byte_t* data = getTypeDataHead( );
	size_t byteIdx = 0;
	ptrdiff_t offset = 0;
	while (true)
	{
		typeid_t size = *detail::reinterpretCastAdvance<const typeid_t>(data, byteIdx);
		byteIdx += tidsz;

		for (typeid_t i = 0; i < size; i++, byteIdx += tidsz)
		{
			if (*detail::reinterpretCastAdvance<const typeid_t>(data, byteIdx) == _bTypeID)
				return _bPtr + offset;
		}

		offset = *detail::reinterpretCastAdvance<const ptrdiff_t>(data, byteIdx);
		if (offset == 0) return 0;
		byteIdx += sizeof(ptrdiff_t);
	}
}

#pragma warning(default: WARN_DEPRECATED_)
