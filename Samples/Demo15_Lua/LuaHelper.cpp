#include "LuaHelper.h"

namespace Rad {

	lua_State * LuaHelper::L_S = NULL;

	LuaHelper::LuaHelper()
	{
		d_assert (L_S == NULL);
	
		L_S = luaL_newstate();
		luaL_openlibs(L_S);
	}

	LuaHelper::~LuaHelper()
	{
		d_assert (L_S != NULL);

		lua_close(L_S);
		L_S = NULL;
	}

}

