#pragma once

#include <iostream>
#include <string>

using namespace std;

class Student
{
private:
    string m_Name;    //姓名

    unsigned m_Age;  //年龄
public:
    //构造函数
    Student(/* args */);
    
    //析构
    ~Student();

    //获取名字
    string get_name();

    //设置名字
    void set_name(string name);

    //获取年龄
    unsigned get_age();

    //设置年龄
    void set_age(unsigned age);

    //打印函数
    void print();


};
