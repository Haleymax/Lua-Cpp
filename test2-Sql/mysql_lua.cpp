#include "mysql_lua.h"
#include "iostream"

//MySQL 连接函数
int lua_mysql_connect(lua_State *L){
    const char * host = lua_tostring(L , 1);        //主机（栈底）
    const char * user = lua_tostring(L , 2);        //用户名
    const char * password = lua_tostring(L , 3);    //密码
    const char * database = lua_tostring(L , 4);    //数据库名

    MYSQL *conn = mysql_init(NULL);     //创建并初始化连接对象
    if (conn == NULL)
    {
        lua_pushstring(L , "初始化连接失败");
        lua_error(L);   //终端Cpp程序
    }

    if (mysql_real_connect(conn, host, user, password, database, 0, NULL, 0) == NULL) {
        lua_pushstring(L, mysql_error(conn));   //将返回的错误信息字符串返回给lua段
        lua_error(L);
    }

    //将MySQL 连接句柄作为 Lua 用户数据压栈
    MYSQL ** userdata = (MYSQL **)lua_newuserdata(L , sizeof(MYSQL *));
    *userdata = conn;

    //设置元表
    luaL_getmetatable(L, "mysql.connection");
    lua_setmetatable(L,-2);
    
    return 1; //返回一个值 ， 表示成功创建了 MySQL 连接并将其压入栈顶
}