#include "file.h"

//匿名输入函数
void Insert_password(char password[]) {
	char c;
	while ((c = _getch()) != '\r') {
		if ('\b' == c) {
			printf("\b \b");
			password = password - 1;
			*password = '\0';
		}
		else {
			putchar('*');
			*password = c;
			password = password + 1;
		}
	}
}

//登录界面，判定账号是否正确
int sign_in(User* user_table) {
	//登录，直到输入对为止
	while (true) {
		system("cls"); //清屏
		puts("Sign in Student Information Management System");
		User* p = user_table;
		char username[MaxSize_name] = { 0 };
		char password[MaxSize_name] = { 0 };
		//输入用户名和账号
		printf("\nUsername: ");
		gets(username);
again_password:
		printf("Password: ");
		Insert_password(password);
		//判断是否有这个号，有就返回权限，没有就返回-1
		while (p != NULL) {
			if (!strcmp(p->username, username)) {
				//找到了，开始检验密码
				if (!strcmp(p->password, password)) {
					print_end_info("Log in succeed.");
					return p->isAdmin;
				}
				//密码错误
				else {
					print_end_info("Error: password is wrong.");
					goto again_password;
				}
			}
			p = p->next;
		}
		print_end_info("The account could not be found.");
	}
}

//注册界面，创建非相同账号，默认为用户
void sign_out(User* user_table) {
	char username[MaxSize_name] = { 0 }, password[MaxSize_name] = { 0 }, secondword[MaxSize_name] = { 0 };
	//循环输入用户名
again_name:
	system("cls"); //清屏
	puts("Sign out of Student Information Management System");
	printf("\nUsername: ");
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
	//匿名输入密码
again_word:
	printf("Password: ");
	Insert_password(password);
	printf("\nPassword again: ");
	Insert_password(secondword);
	if (strcmp(password, secondword)) {
		puts("\nThe two entered passwords do not match, enter again.");
		goto again_word;
	}
	//链接到用户数据
	User* newNode = (User*)malloc(sizeof(User));
	if (newNode == NULL) {
		perror("calloc failed in add_studnetInfo: ");
		return;
	}
	strcpy(newNode->username, username);
	strcpy(newNode->password, password);
	newNode->next = user_table->next;
	user_table->next = newNode;
	//退出
	puts("\nCreat user succesed.press any to continue.");
	system("pause");
}

//校验参数，并打开文件，加载到内存
void Start_system(int argc, char* argv[], User* user_table, Student* student_table) {
	if (argc != 3) {
		puts("Error: Invalid Address Entered.\n");
		exit(1);
	}
	//将以只读模式文件打开
	FILE* user_film_p = fopen(argv[1], "ab+");
	if (user_film_p == NULL) {
		perror("userinfo.txt");
		exit(1);
	}
	FILE* student_film_p = fopen(argv[2], "ab+");
	if (student_film_p == NULL) {
		perror("database.txt");
		fclose("Student_film_p");
		exit(1);
	}
	//建立用户信息表
	User* p = user_table;
	do {
		User* newNode = (User*)calloc(1, sizeof(User));
		if (newNode == NULL) {
			perror("calloc failed in start_system: ");
			return;
		}
		p->next = newNode;
		p = p->next;
	} while (fread(p, sizeof(User), 1, user_film_p) != 0);
	//建立学生信息表
	Student* s = student_table;
	do {
		Student* newNode = (Student*)calloc(1, sizeof(Student));
		if (newNode == NULL) {
			perror("calloc failed in start_system: ");
			return;
		}
		s->next = newNode;
		s = s->next;
	} while (fread(s, sizeof(Student), 1, student_film_p) != 0);
	//关闭文件
	fclose(user_film_p);
	fclose(student_film_p);
}

//登录界面，返回是否为管理员
bool initial_interface(User* user_table) {
	//打印界面
	print_title();
	puts("1.sign in");
	puts("2.sign out");
	puts("3.exit");
	//判断输入
	while (true) {
		char ch = getchar();
		clear_input();
		switch (ch) {
		case '1':
			return sign_in(user_table);
		case '2':
			sign_out(user_table);
			return false;
		case '3':
			print_thank();
			exit(0);
		default:
			print_illegal_input();
			break;
		}
	}
}

//展示系统界面
void show_system(bool isAdmin, User* user_table, Student* student_table) {
	system("cls"); 
	print_title();
	if (isAdmin) {
		show_Admin_system(user_table,student_table);
	}
	else {
		show_user_system(student_table);
	}
}

//关闭系统，并把缓存内的信息写入磁盘文件，关闭文件
void close_system(int argc, char* argv[], User* user_table, Student* student_table) {
	//将以覆盖写模式打开文件,将表全部更新
	FILE* user_film_p = fopen(argv[1], "wb+");
	if (user_film_p == NULL) {
		perror("userinfo.txt");
		exit(1);
	}
	FILE* student_film_p = fopen(argv[2], "wb+");
	if (student_film_p == NULL) {
		perror("database.txt");
		fclose("Student_film_p");
		exit(1);
	}
	//将内存中数据写入文件
	User* u = user_table;
	Student* s = student_table->next;
	while (u != NULL && strlen(u->username) > 0) {
		fwrite(u, sizeof(User), 1, user_film_p);
		u = u->next;
	}
	while (s != NULL && strlen(s->name) > 0) {
		fwrite(s, sizeof(Student), 1, student_film_p);
		s = s->next;
	}
	fclose(user_film_p);
	fclose(student_film_p);
}

//打印抬头信息
void print_title(void) {
	system("cls");
	puts("*******************************************************");
	puts("******** Student Information Management Syetem ********");
	puts("****************** Build by dsy ***********************");
	puts("*******************************************************\n");
}

//清空多余的输入
void clear_input(void) {
	char temp;
	while ((temp = getchar()) != '\n');
}

//打印非法输入
void print_illegal_input(void) {
	system("cls");
	puts("Illigal argument. Press any to continue.");
	system("pause");
}

//输出最后的信息，并暂停屏幕
void print_end_info(char* c) {
	printf("\n%s . press any to continue.\n", c);
	system("pause");
}

//程序结束时打印致谢
void print_thank(void) {
	system("cls");
	print_title();
	puts("Thanks for using. See you next time.\n");
}