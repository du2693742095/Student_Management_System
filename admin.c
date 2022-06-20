#include "file.h"

//打印用户信息
void print_userInfo(const User* u) {
	printf("username             password             isAdmin\n");
	if (u->isAdmin) {
		printf("%-20s %-20s True\n", u->username, u->password);
	}
	else {
		printf("%-20s %-20s False\n\n", u->username, u->password);
	}
}

//展示搜索列表选项
void show_search_system(Student* student_table) {
	while (true) {
		//打印查找表
		system("cls");
		print_title();
		puts("1. find all students");
		puts("2. Search by name");
		puts("3. Search by ID");
		puts("4. back to superior menu");
		//选项选择
		char c = getchar();
		clear_input();
		switch (c) {
		case '1':
			find_all(student_table);
			break;
		case '2':
			find_name(student_table);
			break;
		case '3':
			find_id(student_table);
			break;
		case '4':
			return;
		default:
			print_illegal_input();
			break;
		}
	}
}

//展示选项
void print_options_admin() {
	puts("1. Search student information");
	puts("2. add student information");
	puts("3. update student information");
	puts("4. delete student information");
	puts("5. add user account");
	puts("6. update user account");
	puts("7. delete user account");
	puts("8. search user account");
	puts("9. exit");
}

//主调函数
void show_Admin_system(User* user_table, Student* student_table) {
	while (true) {
		//打印界面
		print_title();
		print_options_admin();
		//处理选项
		char c = getchar();
		clear_input();
		switch (c)
		{
		case '1':
			show_search_system(student_table);
			break;
		case '2':
			add_studentInfo(student_table);
			break;
		case '3':
			update_studentInfo(student_table);
			break;
		case '4':
			delete_student(student_table);
			break;
		case '5':
			add_user(user_table);
			break;
		case '6':
			update_user(user_table);
			break;
		case '7':
			delete_user(user_table);
			break;
		case '8':
			search_user(user_table);
			break;
		case '9':
			print_thank();
			return;
		default:
			print_illegal_input();
			break;
		}
	}
}

//查看所有学生
void find_all(const Student* student_table) {
	print_title();
	Student* s = student_table->next;
	printf("ID   name                 Chinese  Math  English\n");
	while (strlen(s->name) > 0 && s != NULL) {
		printf("%03d  %-20s %-7d  %-4d  %-7d\n", s->id, s->name, s->chinese, s->math, s->english);
		s = s->next;
	}
	system("pause");
}

//增加学生
void add_studentInfo(Student* student_table) {
	print_title(); //刷新页面
	Student* s = student_table->next;
	Student* newNode = (Student*)malloc(sizeof(Student));
	if (newNode == NULL) {
		perror("calloc failed in add_studnetInfo: ");
		return;
	}
	//提示输入格式
	puts("please input student infomation which you want to add.");
	puts("fomat is: ID Student_name Chinese_score Math_Score English_score.");
	puts("for example: 1 Tom 99 100 80");
	//读入学生信息(错误检查）
again:
	memset(newNode, 0, sizeof(Student));
	scanf("%d%s%d%d%d", &newNode->id, newNode->name, &newNode->chinese, &newNode->math, &newNode->english);
	clear_input();
	//检查输入错误
	if (newNode->id > 999) {
		puts("ID exceeds Lenth. Try angin.");
		goto again;
	}
	if (strlen(newNode->name) > MaxSize_name) {
		puts("name exceeds length. Try again.");
		goto again;
	}
	if (newNode->chinese > MaxScore || newNode->math > MaxScore || newNode->english > MaxScore) {
		puts("Score is error. Try angin.");
		goto again;
	}
	s = student_table->next;
	//查重
	while (s != NULL) {
		if (!strcmp(newNode->name, s->name)) {
			puts("This student's information is already exist. Tetry again.");
			goto again;
		}
		s = s->next;
	}
	//无错，则链接
	newNode->next = student_table->next;
	student_table->next = newNode;
	print_end_info("Add succeed.");
}

//更新学生信息(用姓名更新）
void update_studentInfo(Student* student_table) {
	print_title();
	puts("which student's information do you want to update?");
	char name[MaxSize_name] = { 0 };
	gets(name);
	//查找学生
	Student* s = student_table;
	while (s != NULL) {
		//如果找到了
		if (!strcmp(name, s->name)) {
			puts("find succeed.");
			print_studentInfo(s);
			puts("please input new score. Formate is: Chinese Math English.");
			while (true) {
				scanf("%d%d%d", &s->chinese, &s->math, &s->english);
				clear_input();
				if (s->chinese > 100 || s->math > 100 || s->english > 100) {
					puts("Error Score. Try it again.");
					continue;
				}
				else {
					print_end_info("update succeed.");
					return;
				}	
			}
		}
		s = s->next;
	}
	//没有找到
	print_end_info("Not found");
}

//删除学生
void delete_student(Student* student_table) {
	print_title();
	puts("which student's information do you want to delete?");
	char name[MaxSize_name] = { 0 };
	gets(name);
	//查找学生
	Student* s = student_table->next;
	Student* pre = student_table;
	while (s != NULL) {
		if (!strcmp(name, s->name)) {
			print_studentInfo(s);
			pre->next = s->next;
			free(s);
			puts("Delete succedd. Press any to continue.");
			system("pause");
			return;
		}
		pre = s;
		s = s->next;
	}
	//没有找到
	print_end_info("Not found.");
}

//增加用户，可以设置是否为管理员
void add_user(User* user_table) {
	print_title(); //刷新页面
	char username[MaxSize_name] = { 0 }, password[MaxSize_name] = { 0 }, secondword[MaxSize_name] = { 0 };
	bool isAdmin;
	puts("please input user infomation which you want to add.");
	//输入名字（判重）
again_name:
	system("cls"); //清屏
	puts("Sign out of Student Information Management System");
	printf("\nUsername: ");
	memset(username, 0, sizeof(username));
	gets(username);
	//判断是否有重复
	User* p = user_table;
	while (p != NULL) {
		if (!strcmp(p->username, username)) {
			puts("\nDuplicate name, please enter again.");
			system("pause");
			goto again_name;
		}
		p = p->next;
	}
	//输入密码（判错）
again_word:
	memset(password, 0, sizeof(password));
	memset(secondword, 0, sizeof(secondword));
	printf("Password: ");
	Insert_password(password);
	printf("\nPassword again: ");
	Insert_password(secondword);
	if (strcmp(password, secondword)) {
		puts("\nThe two entered passwords do not match, enter again.");
		goto again_word;
	}
	//输入权限
	printf("\nIs this user administrator ?");
	while (true) {
		puts("Y/N ? ");
		char c = getchar();
		//char backspace = getchar();
		//实现退格
		if (c == "\b")	{
			printf("\b \b");
			c = getchar();
			continue;
			}
		if (c  == 'Y') {
			isAdmin = true;
			break;
		}
		if(c == 'N'){
			isAdmin = false;
			break;
		}
		print_end_info("Illegal Input.");
		//clear_input();
	}
	
	//链接到用户数据
	User* newNode = (User*)malloc(sizeof(User));
	if (newNode == NULL) {
		perror("calloc failed in add_studnetInfo: ");
		return;
	}
	strcpy(newNode->username, username);
	strcpy(newNode->password, password);
	newNode->isAdmin = isAdmin;
	newNode->next = user_table->next;
	user_table->next = newNode;
	//退出
	print_end_info("Creat user succesed.");
}

//修改用户信息
void update_user(User* user_table) {
	print_title();
	puts("which user's information do you want to update?");
	char name[MaxSize_name] = { 0 };
	gets(name);
	//查找用户
	User* u = user_table;
	while (u != NULL) {
		//如果找到了
		if (!strcmp(name, u->username)) {
			puts("find succeed.");
			print_userInfo(u);
			puts("which do you want to change ? password or isAdmin ?\n");
			while (true) {
				puts("p/i ? ");
				char c = getchar();
				clear_input();
				//修改管理员信息
				if (c == 'i') {
					printf("Is %s administrator ?\n", u->username);
					while (true) {
						puts("Y/N ? ");
						char ch = getchar();
						clear_input();
						if (ch == 'Y') {
							u->isAdmin = true;
							print_end_info("Change succeed.");
							return;
						}
						if (ch == 'N') {
							u->isAdmin = false;
							print_end_info("Change succeed.");
							return;
						}
					}
				}
				//修改密码
				if (c == 'p') {
					printf("Input password: ");
					gets(u->password);
					print_end_info("password change succeed");
					return;
				}
			}
		}
		u = u->next;
	}
	//没有找到
	print_end_info("Not found.");
}

//查找用户
void search_user(const User* user_table) {
	User* u = user_table;
	char name[MaxSize_name];
	print_title();
	puts("input user name: ");
	gets(name);
	while (u != NULL) {
		if (!strcmp(u->username, name)) {
			print_userInfo(u);
			print_end_info("search succeed.");
			return;
		}
		u = u->next;
	}
	print_end_info("Not found.");
}

//删除用户
void delete_user(User* user_table) {
	User* u = user_table;
	User* pre = NULL;
	char name[MaxSize_name];
	print_title();
	puts("input user name you want to delete: ");
	gets(name);
	while (u != NULL) {
		if (!strcmp(u->username, name)) {
			print_userInfo(u);
			//如果pre是NULL，那就是说要删除第一个admin账号，不允许。
			if (pre == NULL) {
				puts("Error: You can not delete Administrator.");
				return;
			}
			User* temp = u;
			pre->next = temp->next;
			free(temp);
			print_end_info("Delete succeed.");
			return;
		}
		pre = u;
		u = u->next;
	}
	print_end_info("Not found.");
}
