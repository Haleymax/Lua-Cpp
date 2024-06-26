#include "Student.h"

Student::Student() : m_Name("Empty") , m_Age(0){
    std::cout << "学生类创建" << std::endl;
}

Student::~Student(){
    std::cout << "学生类被销毁" << std::endl;
}

std::string Student::get_name(){
    return this->m_Name;
}

void Student::set_name(std::string name){
    this->m_Name = name;
}

 unsigned Student::get_age(){
    return this->m_Age;
 }

void Student::set_age(unsigned age){
    this->m_Age = age;
}

void Student::print(){
    std::cout << "name : " << m_Name << "age : " << m_Age << std::endl;
    
}

