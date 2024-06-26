local student_obj = Student.create()
student_obj:set_name("Haley")
student_obj:print()


print("age : " ..student_obj:get_age())

--使用内部的__tostring函数进行打印
print(student_obj)

student_obj = nil