# 学生管理系统

### 功能：

**1. 登录界面**：
   1. 提供登录接口和注册接口
   2. 登录时查系统保存的文件，如没有该用户名或密码错误只能重试
   3. 注册只能成为用户权限，且用户名不能与已知用户重合，有匿名输入功能，要求注册时重复输入密码，

**2. 用户界面：**
   1. 提供按ID查找和按姓名查找功能，都是找到了则返回全部信息，没有找到则输出“Not Found”

**3. 管理员界面：**
   1. 提供对学生增删改查，对用户账号增删改查接口
   2. 对学生：
      1. 查：比用户界面内多了查看所有学生信息的功能
      2. 增：可以创建学生信息，比如按照规定格式输入，格式错误会报错重新输入
      3. 删：先查询删除学生信息，如果有则列出，再删除，没有则“Not Found”退出
      4. 改：可以修改学生的成绩信息，按规定输入，输入错误报错
   3. 对用户：
      1. 增：可以增加非重复的用户名，在这里面可以直接将用户注册为管理员权限
      2. 删：删除用户
      3. 改：可以修改密码和管理员权限。按提示修改哪一个。
      4. 查：查找用户
     
**4. 退出接口：**
   1. 所有的界面只有按界面提示的exit退出，输入的数据才能保存，如果没有按照提示正常退出，则操作的数据将不被写入文件。退出时输出感谢语。



### 文件：
会由系统传入用户信息和学生信息，argc和argv校验失败则报错。创建文件database.ext和userinfo.txt



# 代码：

### file.h
file.c、user.c、admin.c、main.c中的函数声明都在里面
```c
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
```



### main.c
主函数

```c
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
```



### file.c
- 实现**登录界面**，展示**登录**、**注册**、**退出**功能
- 实现登录时匿名输入（即输入数据显示成 ‘ * ’）功能
- 负责**文件处理**，将用户数据和学生的数据都读入内存

```c
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
```



### user.c
- 实现**普通用户界面**的输出
- 实现对学生信息的按ID查询和按学号查询。

```c
#include "file.h"

//展示选项
void print_options_user() {
	puts("1. Search by name");
	puts("2. Search by ID");
	puts("3. exit");
}

//展示用户的界面
void show_user_system(const Student* student_table) {
	while (true) {
		//打印界面
		print_title();
		print_options_user();
		//处理选项
		char c = getchar();
		clear_input();
		switch (c)
		{
		case '1':
			find_name(student_table);
			break;
		case '2':
			find_id(student_table);
			break;
		case '3':
			print_thank();
			return;
		default:
			print_illegal_input();
			break;
		}
	}
}

//查找名字
void find_name(const Student* student_table) {
	Student* p = student_table->next;
	char name[MaxSize_name];
	print_title();
	puts("input student name: ");
	gets(name);
	while (p != NULL) {
		if (!strcmp(p->name, name)) {
			print_studentInfo(p);
			puts("\npress any to continue.\n");
			system("pause");
			return;
		}
		p = p->next;
	}
	print_end_info("Not found");
}

//查找id
void find_id(const Student* student_table) {
	Student* p = student_table->next;
	int id;
	print_title();
	puts("input student name: ");
	scanf("%d", &id);
	clear_input();
	while (p != NULL) {
		if (id == p->id) {
			print_studentInfo(p);
			puts("\npress any to continue.\n");
			system("pause");
			return;
		}
		p = p->next;
	}
	printf("%d number is not found.\n", id);
}

void print_studentInfo(const Student* s) {
	printf("\nID   name                 Chinese  Math  English\n");
	printf("%03d  %-20s %-7d  %-4d  %-7d\n\n", s->id, s->name, s->chinese, s->math, s->english);
}
```

### admin.c
- 实现**管理员账号界面**的输出
- 实现管理员功能：对学生信息增删改查、对用户信息增删改查

```c
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
```

