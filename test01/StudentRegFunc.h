
#pragma once


#include "Student.h"

extern "C"
{
#include "../lua-5.4.6/src/lua.h"
#include "../lua-5.4.6/src/lauxlib.h"
#include "../lua-5.4.6/src/lualib.h"
}


//定义相关的全局函数
int lua_create_new_student(lua_State *L);

//get/set函数
int lua_get_name(lua_State *L);
int lua_set_name(lua_State *L);
int lua_get_age(lua_State *L);
int lua_set_age(lua_State *L);

//打印函数
int lua_print(lua_State *L);

//注册全局函数供lua使用
static const luaL_Reg lua_reg_student_funcs[] = {
    {"create" , lua_create_new_student },
    {"get_name" , lua_get_name},
    {"set_name" , lua_set_name},
    {"get_age" , lua_get_age},
    {"set_age" , lua_set_age},
    {"print" , lua_print},
    {NULL , NULL}
};

int luaopen_student_libs(lua_State *L);