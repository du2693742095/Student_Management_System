#include "file.h"

int main(int argc, char* argv[]) {
	//创建头节点
	User user_table = { "admin","123456",true };//初始化一个管理员账户
	Student student_table = { 0 };
	 
	Start_system(argc, argv,&user_table,&student_table);//打开表
	bool isAdmin = initial_interface(&user_table);//登录，并判定权限
	show_system(isAdmin, &user_table, &student_table);//打开界面
	close_system(argc, argv, &user_table, &student_table);//关闭系统，并将文件再次写入

	return 0;
}