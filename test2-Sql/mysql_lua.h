
#pragma once

#include <mysql/mysql.h>

extern "C"
{
#include "../lua-5.4.6/src/lua.h"
#include "../lua-5.4.6/src/lauxlib.h"
#include "../lua-5.4.6/src/lualib.h"
}

//打开MySQL Lua库的函数声明
int luaopen_mysql_lib(lua_State *L);

//MySQL 连接和操作函数
int lua_mysql_connect(lua_State *L);    //连接
int lua_mysql_insert(lua_State *L);     //插入
int lua_mysql_select(lua_State *L);     //查询
int lua_mysql_update(lua_State *L);     //修改
int lua_mysql_delete(lua_State *L);     //删除
