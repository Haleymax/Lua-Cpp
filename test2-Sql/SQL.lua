local mysql = require("mysql_lib")

local conn = mysql.connect("127.0.0.1", "root", "209020357", "test1")

if conn then
    print("Connected to MySQL database!")

    -- 插入数据
    local insert_result = conn:insert("mytable", "column1, column2", "'value1', 'value2'")
    print(insert_result)

    -- 查询数据
    local select_result = conn:select("mytable", "*")
    if select_result then
        print("Query results:")
        for i, row in ipairs(select_result) do
            print(string.format("Row %d:", i))
            for field, value in pairs(row) do
                print(string.format("  %s: %s", field, value))
            end
        end
    else
        print("Query failed:", mysql.last_error())
    end

       -- 更新数据
       local update_result = conn:update("mytable", "column1 = 'new_value'", "column2 = 'condition'")
       print(update_result)
   
       -- 删除数据
       local delete_result = conn:delete("mytable", "column1 = 'value'")
       print(delete_result)
   
       -- 关闭数据库连接
       conn = nil
   else
       print("Connection failed:", mysql.last_error())
   end
   