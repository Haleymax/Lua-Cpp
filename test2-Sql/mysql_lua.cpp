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
    MYSQL ** userdata = (MYSQL **)lua_newuserdata(L , sizeof(MYSQL *));   //使用lua_newuserdata创建一个lua环境中的数据变量将其指针指向userdata用于存储C中的数据结构
    *userdata = conn;   //连接获取到的连接数据放入userdata

    //设置元表
    luaL_getmetatable(L, "mysql.connection");
    lua_setmetatable(L,-2);
    
    return 1; //返回一个值 ， 表示成功创建了 MySQL 连接并将其压入栈顶
}

//MySQL 查询数据函数
int lua_mysql_select(lua_State *L){
    MYSQL * conn = *(MYSQL**)luaL_checkudata(L , 1 ,"mysql.connection");
    const char * table = lua_tostring(L , 2);
    const char * columns = lua_tostring(L , 3);

    std::string query = "SELECT";
    query += columns;
    query += " FROM ";
    query += table;

    if (mysql_query(conn , query.c_str()) != 0)
    {
        lua_pushstring(L , mysql_error(conn));
        lua_error(L); 
    }
    
    MYSQL_RES* result = mysql_store_result(conn);
    if (result == NULL)
    {
        lua_pushnil(L);
    }else{
        int num_fields = mysql_num_fields(result);

        lua_newtable(L);    //创建一个新表,这个新的表放置于栈顶

        MYSQL_ROW row;
        int i = 1;
        while ((row = mysql_fetch_row(result)))
        {
            lua_newtable(L);
            for (size_t j = 0; j < num_fields; j++)
            {
                lua_pushstring(L , mysql_fetch_field_direct(result , j)->name);
                lua_pushstring(L , row[j]);
                lua_settable(L , -3);   //将字段名和键值对存入对应的表中（-3表示的就是刚刚创建的表，调用这个函数后会将-1 ， -2 索引的健值放入表中并弹出栈）
            }

            lua_rawseti(L , -2 , i);  //栈顶这个表表示的是一行数据，将这一行数据返回插入今栈顶下面一个的表中 i 表示第几行作为健
            i++;
            
        }
    mysql_free_result(result);    
    }

    return 1;
    
}

//MySQL 更新数据库
int lua_mysql_updata(lua_State *L){
    MYSQL *conn = *(MYSQL**)luaL_checkudata(L , 1 , "mysql.connection");
    const char * table = lua_tostring(L , 2);
    const char * set_clause = lua_tostring(L , 3);
    const char * where_clause = lua_tostring(L , 4);

    std::string query = "UPDATE ";
    query += table;
    query += " SET ";
    query += set_clause;
    query += " WHERE ";
    query += where_clause;

    if (mysql_query(conn , query.c_str()) != 0)
    {
        lua_pushstring(L , mysql_error(conn));
        lua_error(L);
    }
    
    lua_pushstring(L , "更新成功");
    return 1;
}

//MySQL 删除数据函数
int lua_mysql_delete(lua_State *L){
    MYSQL *conn = *(MYSQL**)luaL_checkudata(L , 1 , "mysql.connection");
    const char * table = lua_tostring(L , 2);
    const char * where_clause = lua_tostring(L , 3);

    std::string query = "DELETE FROM ";
    query += table;
    query += " WHERE ";
    query += where_clause;

    if (mysql_query(conn , query.c_str()) != 0)
    {
        lua_pushstring(L , mysql_error(conn));
        lua_error(L);
    }

    lua_pushstring(L , "删除成功");
    return 1;
    
}

//注册MySQL函数供lua使用
static const luaL_Reg mysql_lib[] = {
    {"connect", lua_mysql_connect},
    {"insert", lua_mysql_insert},
    {"select", lua_mysql_select},
    {"update", lua_mysql_update},
    {"delete", lua_mysql_delete},
    {NULL, NULL}
};


static int mysql_connection_gc(lua_State *L) {  
    MYSQL* conn = *(MYSQL**)luaL_checkudata(L, 1, "mysql.connection");  
    mysql_close(conn);  
    return 0;  
}  

// 打开 MySQL Lua 库的函数实现
int luaopen_mysql_lib(lua_State *L) {
    luaL_newmetatable(L, "mysql.connection");  // 创建元表用于 MySQL 连接

    // 设置元表的 __gc 方法，用于在 Lua 对象被回收时关闭 MySQL 连接
   lua_pushstring(L, "__gc");  
    lua_pushcfunction(L, mysql_connection_gc);  
    lua_settable(L, -3); 

    luaL_newlib(L, mysql_lib);  // 创建包含所有 MySQL 相关函数的 Lua 库
    return 1;  // 返回这个库供 Lua 使用
}


int main() {
    // 创建 Lua 环境
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);  // 打开标准 Lua 库

    // 加载 MySQL Lua 库
    luaopen_mysql_lib(L);
    lua_setglobal(L, "mysql");  // 将 MySQL 库注册为全局变量 mysql

    // 加载并执行 Lua 脚本
    const char* script_filename = "test.lua";  // Lua 脚本文件名
    int result = luaL_dofile(L, script_filename);

    if (result != LUA_OK) {
        std::cerr << "Error executing Lua script: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);  // 弹出错误信息
    }

    // 关闭 Lua 环境
    lua_close(L);

    return 0;
}