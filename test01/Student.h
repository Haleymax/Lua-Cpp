#pragma once

#include <iostream>
#include <string>


class Student
{
private:
    std::string m_Name;    //姓名

    unsigned m_Age;  //年龄
public:
    //构造函数
    Student(/* args */);
    
    //析构
    ~Student();

    //获取名字
    std::string get_name();

    //设置名字
    void set_name(std::string name);

    //获取年龄
    unsigned get_age();

    //设置年龄
    void set_age(unsigned age);

    //打印函数
    void print();


};