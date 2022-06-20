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