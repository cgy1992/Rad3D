#pragma once

#include "MBase.h"
#include "MResourceManager.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#ifndef LUA_OK
#define LUA_OK 0
#endif

namespace Rad {

	class LuaHelper
	{
	public:
		static lua_State * L_S;

	public:
		LuaHelper();
		~LuaHelper();
	};


	template <class T>
	inline void LuaPush(T p)
	{
		d_assert (0);
	}

	template <>
	inline void LuaPush(bool p)
	{
		lua_pushboolean(LuaHelper::L_S, p ? 1 : 0);
	}

	template <>
	inline void LuaPush(int p)
	{
		lua_pushinteger(LuaHelper::L_S, p);
	}

	template <>
	inline void LuaPush(float p)
	{
		lua_pushnumber(LuaHelper::L_S, p);
	}

	template <>
	inline void LuaPush(const char * p)
	{
		lua_pushstring(LuaHelper::L_S, p);
	}

	template <>
	inline void LuaPush(void * p)
	{
		lua_pushlightuserdata(LuaHelper::L_S, p);
	}

	inline bool LuaGetBool()
	{
		lua_State * LS = LuaHelper::L_S;

		d_assert (lua_type(LS, -1) == LUA_TBOOLEAN);

		int r = lua_toboolean(LS, -1);

		lua_pop(LS, 1);

		return r != 0;
	}

	inline int LuaGetInt()
	{
		lua_State * LS = LuaHelper::L_S;

		d_assert (lua_isnumber(LS, -1));

		int r = (int)lua_tointeger(LS, -1);

		lua_pop(LS, 1);

		return r;
	}

	inline float LuaGetFloat()
	{
		lua_State * LS = LuaHelper::L_S;

		d_assert (lua_isnumber(LS, -1));

		float r = (float)lua_tonumber(LS, -1);

		lua_pop(LS, 1);

		return r;
	}

	inline const char * LuaGetString()
	{
		lua_State * LS = LuaHelper::L_S;

		d_assert (lua_isstring(LS, -1));

		static char buff[256];

		const char * src = lua_tostring(LS, -1);

		if (src != NULL)
		{
			strcpy(buff, src);
		}
		else
		{
			buff[0] = 0;
		}

		lua_pop(LS, 1);

		return buff;
	}

	inline void * LuaGetUserData()
	{
		lua_State * LS = LuaHelper::L_S;

		d_assert (lua_isuserdata(LS, -1));

		void * r = lua_touserdata(LS, -1);

		lua_pop(LS, 1);

		return r;
	}


	inline int LuaGetGlobalInt(const char * var)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, var);

		return LuaGetInt();
	}

	inline float LuaGetGlobalFloat(const char * var)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, var);

		return LuaGetFloat();
	}

	inline const char * LuaGetGlobalString(const char * var)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, var);
	}


	inline void LuaCall(const char * func, int nresults)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, func);

		if (lua_pcall(LS, 0, nresults, 0) != LUA_OK)
		{
			const char * errStr = lua_tostring(LS, -1);

			d_assert(0);
		}
	}

	template<class P0>   
	inline void LuaCall(const char * func, int nresults, P0 p0)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, func);

		LuaPush(p0);

		if (lua_pcall(LS, 1, nresults, 0) != LUA_OK)
		{
			const char * errStr = lua_tostring(LS, -1);

			d_assert(0);
		}
	}

	template<class P0, class P1>
	inline void LuaCall(const char * func, int nresults, P0 p0, P1 p1)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, func);

		LuaPush(p0);
		LuaPush(p1);

		if (lua_pcall(LS, 2, nresults, 0) != LUA_OK)
		{
			const char * errStr = lua_tostring(LS, -1);

			d_assert(0);
		}
	}

	template<class P0, class P1, class P2>
	inline void LuaCall(const char * func, int nresults, P0 p0, P1 p1, P2 p2)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, func);

		LuaPush(p0);
		LuaPush(p1);
		LuaPush(p2);

		if (lua_pcall(LS, 3, nresults, 0) != LUA_OK)
		{
			const char * errStr = lua_tostring(LS, -1);

			d_assert(0);
		}
	}

	template<class P0, class P1, class P2, class P3>
	inline void LuaCall(const char * func, int nresults, P0 p0, P1 p1, P2 p2, P3 p3)
	{
		lua_State * LS = LuaHelper::L_S;

		lua_getglobal(LS, func);

		LuaPush(p0);
		LuaPush(p1);
		LuaPush(p2);
		LuaPush(p3);

		if (lua_pcall(LS, 4, nresults, 0) != LUA_OK)
		{
			const char * errStr = lua_tostring(LS, -1);

			d_assert(0);
		}
	}

	inline void LuaLoad(const char * filename)
	{
		DataStreamPtr stream = ResourceManager::Instance()->OpenResource(filename);
		if (stream != NULL)
		{
			if (luaL_loadbuffer(LuaHelper::L_S, (const char *)stream->GetData(), stream->Size(), filename) ||
				lua_pcall(LuaHelper::L_S, 0, 0, 0))
			{
				const char * errStr = lua_tostring(LuaHelper::L_S, -1);

				d_assert(0);
			}
		}
	}

	inline void LuaDoString(const char * s)
	{
		if (luaL_dostring(LuaHelper::L_S, s) != LUA_OK)
		{
			const char * errStr = lua_tostring(LuaHelper::L_S, -1);

			d_assert(0);
		}
	}


	inline void LuaRegLib(const char * libname, const luaL_Reg * regs)
	{
		luaL_register(LuaHelper::L_S, libname, regs);
	};

	inline void LuaRegFunction(const char * name, lua_CFunction fn)
	{
		lua_pushcfunction(LuaHelper::L_S, fn);
		lua_setglobal(LuaHelper::L_S, name);
	}

}
