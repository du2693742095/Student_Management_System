#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <conio.h>
#include <string.h>

#define MaxSize_name 20
#define MaxScore 100

typedef struct User_s {
	char username[MaxSize_name];
	char password[MaxSize_name];
	bool isAdmin;
	struct User_s* next;
} User;

typedef struct Student_s{
	int id;
	char name[MaxSize_name];
	int chinese;
	int math;
	int english;
	struct Student_s* next;
} Student;

//file.c
void Start_system(int argc, char* argv[],User* user_table, Student* student_table);//初始化，加载表
bool initial_interface(User* user_table);	//登录界面
void show_system(bool isAdmin, User* user_table, Student* student_table);//按照权限展示界面
void close_system(int argc, char* argv[], User* user_table, Student* student_table);//关闭系统
void print_title(void);	//打印标题
void Insert_password(char password[]); //匿名输入
void clear_input(void);	//清除其余的输入
void print_illegal_input(void);//打印非法输入
void print_end_info(char* c);//打印函数最后的信息，同时暂停
void print_thank(void);	//打印结束语

//user.c
void show_user_system(const Student* student_table);//用户账号只用传入学生信息即可
void find_name(const Student* student_table);
void find_id(const Student* student_table);
void print_studentInfo(const Student *s); //输出学生信息

//admin.c
void show_Admin_system(User* user_table, Student* student_table);//管理员账户可以删改账号，所以传入二级指针
void find_all(const Student *student_table);
void add_studentInfo(Student* student_table);
void update_studentInfo(Student* student_table);
void delete_student(Student* student_table);
void add_user(User* user_table);
void update_user(User* user_table);
void search_user(const User* user_table);
void delete_user(User* user_table);