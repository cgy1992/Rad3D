/*
*	Property
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMath2.h"
#include "MString.h"
#include "MUString.h"
#include "MFixedString.h"
#include "MFile.h"

namespace Rad {

	struct M_ENTRY EnumDesc
	{
		const char * name;
		int id;
		int val;

		EnumDesc(const char * n, int i, int v) : name(n), id(i), val(v) {}
	};

	struct M_ENTRY IEnum
	{
	public:
		IEnum() {}
		virtual ~IEnum() {}

		virtual int 
			GetEnumCount() const { return 0; }
		virtual const EnumDesc * 
			GetEnum(int i) const { return NULL; }
		virtual const EnumDesc * 
			GetEnumByValue(int val) const { return NULL; }
		virtual const EnumDesc * 
			GetEnum(const char * name) const { return NULL; }
	};


#define DF_ENUM_BEGIN(classname) const \
	classname classname::msInstance; \
	const EnumDesc classname::msEnums[] = {

#define DF_ENUM_END() EnumDesc("", 0, -1) };

#define DF_ENUM(e) EnumDesc(#e, Math::Crc32(#e), e),
#define DF_ENUM_EX(s, e) EnumDesc(s, Math::Crc32(s), e),


#define DECLARE_ENUM(classname)										\
protected:                                                          \
	static const EnumDesc msEnums[];								\
	static const classname msInstance;								\
																	\
public:																\
	static const classname * Instance()								\
	{																\
		return &msInstance;											\
	}																\
																	\
	virtual int GetEnumCount() const 								\
	{																\
		int count = 0;												\
																	\
		while (msEnums[count].val != -1)							\
			++count;												\
																	\
		return count;												\
	}																\
																	\
	virtual const EnumDesc * GetEnum(int i) const					\
	{																\
		return &msEnums[i];											\
	}																\
																	\
	virtual const EnumDesc * GetEnumByValue(int v) const			\
	{																\
		int count = GetEnumCount();									\
		for (int i = 0; i < count; ++i)								\
		{															\
			if (msEnums[i].val == v)								\
				return &msEnums[i];									\
		}															\
																	\
		return NULL;												\
	}																\
																	\
	virtual const EnumDesc * GetEnum(const char * name) const		\
	{																\
		int id = Math::Crc32(name), i = 0;							\
		while (msEnums[i].val != -1)								\
		{															\
			if (msEnums[i].id == id)								\
				return &msEnums[i];									\
																	\
			++i;													\
		}															\
																	\
		return NULL;												\
	}

#define M_ENUMERATION(classname) \
	enum_t value; \
	classname() {} \
	classname(enum_t e) : value(e) {} \
	classname & operator =(enum_t e) { value = e; return *this; } \
	operator enum_t() const { return value; } \
	bool operator ==(enum_t e) const { return value == e; }	\
	bool operator !=(enum_t e) const { return value != e; }

	//
	enum PropertyType 
	{
		PT_Bool			= 0x00,

		PT_Int			= 0x10,
		PT_Int2,
		PT_Int3,
		PT_Int4,
		PT_IntX,

		PT_Float		= 0x20,
		PT_Float2,
		PT_Float3,
		PT_Float4,
		PT_FloatX,

		PT_FixedString	= 0x30,
		PT_String,
		PT_UString,

		PT_UserData		= 0x7F,

		PT_UNKNOWN		= 0xFF,
	};

#define PROPERTY_MAX_SIZE 2048

	//
	enum PropertyFlag
	{
		PF_NONE,

		PF_Primary	= 0x01,
		//PF_Unique	= 0x02,
		//PF_Increse = 0x04,
		PF_Stricmp = 0x08,
		PF_Unsigned = 0x80,
	};

	//
	struct IObject;

	struct M_ENTRY Property
	{
		int id;
		const char * name;
		const char * group;
		const char * displayName;
		const char * editorName;

		word type;
		word flag;
		int size;
		int offset;
		
		const IEnum * enumObj;

		Property(
			int _id,
			const char * _name,
			const char * _group, 
			const char * _displayName,
			const char * _editorName, 
			PropertyType _type, 
			PropertyFlag _flag,
			int _offset,
			int _size,
			const IEnum * _enum)
			: id(_id)
			, name(_name)
			, group(_group)
			, displayName(_displayName)
			, editorName(_editorName)
			, type(_type)
			, flag(_flag)
			, offset(_offset)
			, size(_size)
			, enumObj(_enum)
		{
		}

		template <class T>
		const T & AsT(const void * data) const
		{
			d_assert (sizeof(T) == size);

			return *(const T *)data;
		}

		template <class T>
		void AsT(T & v, const void * data) const
		{
			memcpy(&v, data, size);
		}

		bool AsBool(const void * data) const
		{
			d_assert (type == PT_Bool);
			return *(const bool *)data;
		}

		int AsInt(const void * data) const
		{
			d_assert (type == PT_Int && size <= sizeof(int));

			int v = 0;

			switch (size)
			{
			case 4:
				v = *(int32 *)data;
				break;

			case 2:
				v = *(int16 *)data;
				break;

			case 1:
				v = *(int8 *)data;
				break;
			}

			return v;
		}

		Int2 AsInt2(const void * data) const
		{
			d_assert (type == PT_Int2);
			return *(const Int2 *)data;
		}

		Int3 AsInt3(const void * data) const
		{
			d_assert (type == PT_Int3);
			return *(const Int3 *)data;
		}

		Int4 AsInt4(const void * data) const
		{
			d_assert (type == PT_Int4);
			return *(const Int4 *)data;
		}

		const int * AsIntX(const void * data) const
		{
			d_assert (type == PT_IntX);
			return (const int *)data;
		}

		float AsFloat(const void * data) const
		{
			d_assert (type == PT_Float && size == sizeof(float));
			return *(const float *)data;
		}

		double AsDouble(const void * data) const
		{
			d_assert (type == PT_Float && size == sizeof(double));
			return *(const double *)data;
		}

		Float2 AsFloat2(const void * data) const
		{
			d_assert (type == PT_Float2);
			return *(const Float2 *)data;
		}

		Float3 AsFloat3(const void * data) const
		{
			d_assert (type == PT_Float3);
			return *(const Float3 *)data;
		}

		Float4 AsFloat4(const void * data) const
		{
			d_assert (type == PT_Float4);
			return *(const Float4 *)data;
		}

		const float * AsFloatX(const void * data) const
		{
			d_assert (type == PT_FloatX);
			return (const float *)data;
		}

		const char * AsFixedString(const void * data) const
		{
			d_assert (type == PT_FixedString);
			return (const char *)data;
		}

		const char * AsString(const void * data) const
		{
			d_assert (type == PT_String);
			return ((String*)data)->c_str();
		}

		const uchar_t * AsUString(const void * data) const
		{
			d_assert (type == PT_UString);
			return ((UString*)data)->c_str();
		}

	};


#define DF_PROPERTY_BEGIN(classname) \
	const Property classname::msPropertys[] = {

#define DF_PROPERTY_END() \
	Property(0, "", "", "", "", PT_UNKNOWN, PF_NONE, -1, 0, NULL) };


#define DF_PROPERTY(classname, varname, group, displayName, type) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		"",													\
		type,												\
		PF_NONE,											\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		NULL),

#define DF_PROPERTY_F(classname, varname, group, displayName, type, flag) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		"",													\
		type,												\
		flag,												\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		NULL),

#define DF_PROPERTY_EX(classname, varname, group, displayName, editorName, type) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		editorName,											\
		type,												\
		PF_NONE,											\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		NULL),

#define DF_PROPERTY_EX_F(classname, varname, group, displayName, editorName, type, flag) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		editorName,											\
		type,												\
		flag,												\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		NULL),

#define DF_PROPERTY_ENUM(classname, varname, group, displayName, enumClass) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		"",													\
		PT_Int,												\
		PF_NONE,											\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		enumClass::Instance()),

#define DF_PROPERTY_ENUM_F(classname, varname, group, displayName, enumClass, flag) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		"",													\
		PT_Int,												\
		flag,												\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		enumClass::Instance()),

#define DF_PROPERTY_ENUM_EX(classname, varname, group, displayName, editorName, enumClass) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		editorName,											\
		PT_Int,												\
		PF_NONE,											\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		enumClass::Instance()),

#define DF_PROPERTY_ENUM_EX_F(classname, varname, group, displayName, editorName, enumClass, flag) \
	Property(												\
		Math::Crc32(#varname),								\
		#varname,											\
		group,												\
		displayName,										\
		editorName,											\
		type,												\
		flag,												\
		OFFSET_OF_CLASS(classname, varname),				\
		SIZE_OF_CLASS(classname, varname),					\
		enumClass::Instance()),

//
#define DECLARE_PROPERTY(superClass)								\
protected:                                                          \
    static const Property msPropertys[];                            \
                                                                    \
public:									                            \
    virtual const Property * GetProperty(const char * name) const	\
    {                                                               \
		int crc32 = Math::Crc32(name);								\
		const Property * p = NULL;									\
		for (int i = 0; msPropertys[i].type != PT_UNKNOWN; ++i)		\
		{															\
			if (msPropertys[i].id == crc32)							\
			{														\
				p = &msPropertys[i];								\
				break;												\
			}														\
		}															\
																	\
		if (p == NULL)												\
			p = superClass::GetPropertyById(crc32);					\
																	\
		d_assert (p == NULL || strcmp(p->name, name) == 0);			\
																	\
		return p;													\
    }                                                               \
	\
	virtual const Property * GetPropertyById(int id) const			\
    {                                                               \
		for (int i = 0; msPropertys[i].type != PT_UNKNOWN; ++i)		\
		{															\
			if (msPropertys[i].id == id)							\
			{														\
				return &msPropertys[i];								\
			}														\
		}															\
																	\
		return superClass::GetPropertyById(id);						\
    }                                                               \
    \
    virtual const Property * GetProperty(int index) const			\
    {                                                               \
		int superSize = superClass::GetPropertySize();				\
		int mySize = GetPropertySizeLocal();						\
		if (index < superSize)										\
			return superClass::GetProperty(index);					\
																	\
		d_assert(index - superSize < mySize);						\
        return &msPropertys[index - superSize];                     \
    }                                                               \
    \
    virtual int	GetPropertySize() const								\
	{																\
		int superSize = superClass::GetPropertySize();				\
		int mySize = GetPropertySizeLocal();						\
																	\
		return mySize + superSize;									\
    }                                                               \
	\
	int	GetPropertySizeLocal() const								\
	{                                                               \
		int size = 0;												\
		while (msPropertys[size].type != PT_UNKNOWN)				\
			++size;													\
		return size;												\
	}

#define PROPERTY_CHECK_CLASS()										\
	d_assert(this == (IObject *)this && "Inherit IObject first")

}
