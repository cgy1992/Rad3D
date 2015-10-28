/*
*	RTTI
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath.h"

namespace Rad {

class RTTI_INFO
{
public:
    RTTI_INFO(int typeId, const char * classname, const RTTI_INFO * baseobj) { mTypeId = typeId, mName = classname, mBase = baseobj; }

	int TypeId() const { return mTypeId; }
    const char * Name() const { return mName; }
    const RTTI_INFO * Base() const { return mBase; }

protected:
	int mTypeId;
    const char * mName;
    const RTTI_INFO * mBase;
};

#define DECLARE_RTTI() \
public: \
	static const RTTI_INFO RTTI; \
	\
	virtual const RTTI_INFO * GetRTTI() const \
	{\
		return &RTTI; \
	}

#define DECLARE_RTTI_ROOT(classname) \
public: \
    static const RTTI_INFO RTTI; \
    \
    virtual const RTTI_INFO * GetRTTI() const \
    {\
        return &RTTI; \
    }\
    \
    static bool IsTypeOf(const RTTI_INFO * rtti, const classname * obj)  \
    {\
        if (!obj)                   \
            return false;           \
                                    \
        return obj->IsTypeOf(rtti); \
    }\
    \
    bool IsTypeOf(const RTTI_INFO * obj)  const\
    { \
        return GetRTTI() == obj; \
    } \
    \
    static bool IsKindOf(const RTTI_INFO * rtti, const classname * obj)  \
    {\
        if (!obj)                   \
            return false;           \
                                    \
        return obj->IsKindOf(rtti); \
    }\
    \
    bool IsKindOf(const RTTI_INFO * obj) const \
    { \
        const RTTI_INFO * rtti = GetRTTI(); \
        while (rtti)                \
        {                           \
            if (rtti == obj)        \
                return true;        \
                                    \
            rtti = rtti->Base();	\
        }                           \
        return false;               \
    } \
	\
    static classname * DynamicCast(const RTTI_INFO * rtti, classname * obj) \
    {\
        if (obj == NULL)        \
            return 0;           \
                                \
        return obj->IsKindOf(rtti) ? obj : 0; \
    }


#define ImplementRootRTTI(classname) \
	const RTTI_INFO classname::RTTI(Math::Crc32(#classname), #classname, 0);

#define ImplementRTTI(classname, baseclass) \
    const RTTI_INFO classname::RTTI(Math::Crc32(#classname), #classname, &baseclass::RTTI);

#define TYPE_OF(classname, obj) \
    classname::IsTypeOf(&classname::RTTI, obj)

#define KIND_OF(classname, obj) \
    classname::IsKindOf(&classname::RTTI, obj)

#define DYNAMIC_CAST(classname, obj) \
    ((classname*)classname::DynamicCast(&classname::RTTI, obj))

inline bool _RTTI_InheritOf(const RTTI_INFO * _base, const RTTI_INFO * _this, bool _includeBase)
{
	const RTTI_INFO * p = _includeBase ? _this : _this->Base();
	while (p != NULL)
	{
		if (p == _base)
			break;

		p = p->Base();
	}

	return p != NULL;
}

#define RTTI_InheritOf(_baseClass, _thisClass, _includeBase) _RTTI_InheritOf(&_baseClass::RTTI, &_thisClass::RTTI, _includeBase)

}