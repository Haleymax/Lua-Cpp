#include "iostream"

extern "C"
{
#include "../lua-5.4.6/src/lua.h"
#include "../lua-5.4.6/src/lauxlib.h"
#include "../lua-5.4.6/src/lualib.h"
}

#include "Student.h"
#include "StudentRegFunc.h"

static const luaL_Reg lua_reg_libs[] = {
    {"base" , luaopen_base },
    {"Student" , luaopen_student_libs},
    {NULL , NULL}
};

int main(int argc , char *argv[]){
    if (lua_State *L = luaL_newstate())
    {
        //注册让lua使用的库
        const luaL_Reg *lua_reg = lua_reg_libs;
        for ( ; lua_reg->func ; ++lua_reg)
        {
            luaL_requiref(L,lua_reg->name , lua_reg->func , 1);    //取到函数名和具体的函数方法
            lua_pop(L,1);
        }

        //加载脚本，如果出错则打印错误
        if (luaL_dofile(L,"lua.lua"))
        {
            std::cout << lua_tostring(L,-1) << std::endl;
        }

        lua_close(L);  
    }else {
        std::cout << "lua_newstate error ! " << std::endl;
    }

    
    return 0;
    
}

//g++ -std=c++11 -o main main.cpp Student.cpp StudentRegFunc.cpp -I../lua-5.4.6/src -L../lua-5.4.6/src -llua -ldl
//编译语句