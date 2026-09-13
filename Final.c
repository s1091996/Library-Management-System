#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#define BOOKS_SIZE 50
#define STUDENTS_SIZE 50
#define STUDENT_MAX_BORROW 3
//還書要登入 
struct Book {
	char title[100];
	char author[100];
	int year_of_publication;
	char version[100];
	char ISBN[14];
	int copy;
	struct Student* borrower;
	struct tm* borrowed_date;
	struct tm* due_date;
	/*每一本書的屬性包括「書名」(中英)、「作者」(中英)、「出版年份」、「版序」(中英)、「國際書碼」10or13碼 和「複本編碼」*/
	/*書名 作者 出版年 版序 國際書碼(10碼或13碼) 複本編碼 借閱者
		\& How to program C& \ \& Deitel& Deitel& \ 2020 5 0131971093 1 XXXX
		\ & How to program C & \ \ & Deitel & Deitel & \ 2020 5 0131971093 2 XXXX
		\ & 資訊管理 & \ \ & 陳文賢 & \ 2022 1 9574831353 1 495713391
		\ & 程式設計實務與應用with C & \ \ & 胡昭民 & \ 2021 1 9789862010723 1 495713391*/
};
struct Student {
	char id[10];
	char password[10];
	int Number_of_books_borrowed;
	struct Book* borrow_books[STUDENT_MAX_BORROW];
	/*帳號 密碼 借閱圖書之國際書碼 借閱日期…
		495713391 3391 9574831353 20240507 9789862010723 20240506*/
};
struct Book* books[BOOKS_SIZE] = { 0 };
struct Student* students[STUDENTS_SIZE] = { 0 };
int students_amount = 0;
int books_amount = 0;
struct Student* CreateAccount(char* id, char* password) {
	int i = students_amount;
	students[i] = (struct Student*)malloc(sizeof(struct Student));
	if (id != NULL) {
		strcpy(students[i]->id, id);
	}
	if (password != NULL) {
		strcpy(students[i]->password, password);
	}
	students[i]->Number_of_books_borrowed = 0;
	students[i]->borrow_books[0] = NULL;
	students[i]->borrow_books[1] = NULL;
	students[i]->borrow_books[2] = NULL;
	/*printf("(CRAC)ID:%s,Pass:%s\n", students[i]->id, students[i]->password);
	printf("(CRAC)指針:%p\n", (void*)&students[i]);*/
	students_amount++;
	return students[i];
}
struct Book* SearchBook(char* title_or_ISBN, int copy) {
	for (int i = 0; i < books_amount; i++) {
		if ((strcmp(books[i]->ISBN, title_or_ISBN) == 0 || strcmp(books[i]->title, title_or_ISBN) == 0) && books[i]->copy == copy) {
			return books[i];
		}
	}
	return NULL;
}
struct Book* SeachBookInBorrowedList(struct Student* student, char* title_or_ISBN, int copy) {
	for (int i = 0; i < STUDENT_MAX_BORROW; i++) {
		if ((strcmp(student->borrow_books[i]->ISBN, title_or_ISBN) == 0 || strcmp(student->borrow_books[i]->title, title_or_ISBN) == 0) && books[i]->copy == copy) {
			return books[i];
		}
	}
	return NULL;
}
struct Student* SearchAccount(char* ID) {
	for (int i = 0; i < students_amount; i++) {
		if (strcmp(students[i]->id, ID) == 0) {
			return students[i];
		}
	}
	return NULL;
}
struct Student* Login() {
	struct Student* student;
	char st_id[10];
	char st_password[10];

	printf("請輸入帳號:\n");
	scanf("%s", st_id);
	//getchar();
	student = SearchAccount(st_id);
	if (student == NULL) {
		printf("帳號輸入錯誤\n");
		return NULL;
	}
	printf("請輸入密碼:\n");
	scanf("%s", st_password);
	while (getchar() != '\n');
	if (strcmp(st_password, student->password) != 0) {
		printf("密碼輸入錯誤\n");
		return NULL;
	}
	printf("登入成功\n");
	return student;
}
int BorrowBook(struct Student* st, struct Book* bk, struct tm* time) {
	/*借書*/
	//printf("BR_BK指標：%p\n", (void*)st);
	if (st->Number_of_books_borrowed == 3)return 0;
	bk->borrower = st;
	bk->borrowed_date = (struct tm*)malloc(sizeof(struct tm));
	bk->due_date = (struct tm*)malloc(sizeof(struct tm));
	for (int i = 0; i < STUDENT_MAX_BORROW; i++) {
		if (st->borrow_books[i] == NULL) {
			st->borrow_books[i] = bk;
			st->Number_of_books_borrowed++;
			if (time != NULL) {
				bk->borrowed_date = time;
				bk->due_date->tm_year = bk->borrowed_date->tm_year;
				bk->due_date->tm_mon = bk->borrowed_date->tm_mon;
				bk->due_date->tm_mday = bk->borrowed_date->tm_mday + 7;
				mktime(bk->due_date);
			}
			//printf("借書成功，書名%s\n", st->borrow_books[i]->title);
			return 1;
		}
	}
	return 0;
}
void AddBookFromInput() {
	struct Book* book = (struct Book*)malloc(sizeof(struct Book));
	printf("請輸入書名\n");
	scanf("%s", book->title);
	printf("請輸入作者\n");
	getchar();
	fgets(book->author,sizeof(book->author),stdin);
	book->author[strlen(book->author) - 1] = '\0';
	printf("請輸入出版年份\n");
	scanf("%d", &book->year_of_publication);
	printf("請輸入版序\n");
	scanf("%s", book->version);
	printf("請輸入國際書碼\n");
	scanf("%s", book->ISBN);
	printf("請輸入複本編碼\n");
	scanf("%d", &book->copy);
	if (SearchBook(book->ISBN, book->copy) != NULL) {
		printf("無法新增，該複本已存在\n");
		free(book);
	}
	else {
		for (int i = 0; i < BOOKS_SIZE; i++) {
			if (books[i] == NULL) {
				books[i] = book;
				break;
			}
		}
		book->borrower = NULL;
		books_amount++;
		printf("新增成功\n");
	}
}
int has_overdue(struct Student* st) {
	time_t now = time(NULL);
	struct tm* timeinfo;
	timeinfo = localtime(&now);
	int n = timeinfo->tm_year * 10000 + timeinfo->tm_mon * 100 + timeinfo->tm_mday;
	//if (strcmp("495713005", st->id) == 0)return 1;
	for (int i = 0; i < STUDENT_MAX_BORROW; i++) {
		if (st->borrow_books[i] != NULL) {
			int x = st->borrow_books[i]->due_date->tm_year * 10000 + st->borrow_books[i]->due_date->tm_mon * 100 + st->borrow_books[i]->due_date->tm_mday;
			if (n > x) {
				return 1;
			}
		}
	}
	return 0;
}
void LoginAndBorrow() {
	struct Student* student = Login();
	if (student == NULL)return;

	char book_ISBN[14];
	int book_copy;
	struct Book* book = NULL;
	printf("請輸入書名或國際書碼\n");
	scanf("%s", book_ISBN);
	/*fgets(book_ISBN,sizeof(book_ISBN), stdin);
	book_ISBN[strlen(book_ISBN) - 1] = '\0';*/
	printf("請輸入複本編碼\n");
	scanf("%d", &book_copy);
	//search book
	for (int i = 0; i < BOOKS_SIZE && books[i] != NULL; i++) {
		book = SearchBook(book_ISBN, book_copy);
		if (book != NULL && book->borrower == NULL) {
			break;
		}
	}
	if (has_overdue(student) == 1) {
		printf("借閱失敗，借書逾期，無法再借\n");
		return;
	}
	if (book == NULL) {
		printf("借閱失敗，找不到圖書\n");
		return;
	}
	if (book->borrower != NULL) {
		printf("借閱失敗，這本書已被借閱\n");
		return;
	}
	//借書
	time_t current_time;
	struct tm* local_time;
	current_time = time(NULL);
	local_time = localtime(&current_time);
	if (BorrowBook(student, book, local_time) == 0) {
		printf("借閱失敗，借閱數已達三本\n");
	}
	else {
		printf("借書成功，還書時間%04d%02d%02d\n", book->due_date->tm_year + 1900, book->due_date->tm_mon + 1, book->due_date->tm_mday);
	}
	return;
	/*借書(指令B)
		登入帳號(帳號為學生學號，密碼預設為學號後四碼)
		輸入圖書之書名或國際書碼
		輸入複本編碼
		記錄借書日期
		顯示還書日期 //自己訂 過期未歸還
		繼續步驟2~5，直到所有欲借閱圖書都已經完成登錄作業
		登出*/
}
void ReturnBook() {
	struct student* student = Login();
	if (student == NULL) return;
	printf("請輸入書名或國際書碼\n");
	char title_or_ISBN[100];
	scanf("%s", title_or_ISBN);

	int copy;
	printf("請輸入複本編碼\n");
	scanf("%d", &copy);

	struct Book* book = SearchBook(title_or_ISBN, copy);
	if (book == NULL) {
		printf("還書失敗，找不到圖書\n");
		return;
	}
	if (book->borrower == NULL) {
		printf("沒有人借閱這本圖書\n");
		return;
	}
	//從學生端那邊斷連結

	for (int i = 0; i < STUDENT_MAX_BORROW; i++) {
		struct Student* borrower = book->borrower;
		if (borrower->borrow_books[i] != NULL && (strcmp(borrower->borrow_books[i]->title, title_or_ISBN) == 0 || strcmp(borrower->borrow_books[i]->ISBN, title_or_ISBN) == 0) && borrower->borrow_books[i]->copy == copy) {
			borrower->borrow_books[i] = NULL;
			break;
		}
	}
	book->borrower->Number_of_books_borrowed--;
	book->borrower = NULL;
	book->borrowed_date = NULL;
	book->due_date = NULL;
	printf("還書成功\n");
	/*還書(指令R)
		輸入圖書之書名或國際書碼
		輸入複本編碼*/
}
void DeleteBook() {
	char ISBN[100];
	int copy;
	struct Book* book;
	printf("請輸入國際書碼\n");
	scanf("%s", ISBN);
	printf("請輸入複本編碼\n");
	scanf("%d", &copy);
	book = SearchBook(ISBN, copy);
	if (book == NULL) {
		printf("刪除失敗，找不到此圖書\n");
		return;
	}
	if (book->borrower != NULL) {
		printf("借閱中,不可刪除\n");
		return;
	}
	char c[4];
	printf("是否將該圖書之所有資料刪除？輸入yes/no\n");
	scanf("%s", c);
	if (strcmp(c, "yes\0") == 0) {
		for (int i = 0; i < books_amount; i++) {
			if (books[i] != NULL && strcmp(books[i]->ISBN, ISBN) == 0 && books[i]->copy == copy) {
				books[i] = NULL;
				break;
			}
		}
		free(book);
		printf("刪除完成\n");
		books_amount--;
	}
	else {
		printf("刪除取消\n");
	}

	/*刪除圖書(指令D)1.輸入國際書碼2.輸入複本編碼3.系統詢問是否確定要刪除該筆資料將該圖書之所有資料刪除*/
}
void PrintBookInfor(struct Book* book) {
	printf("書名：%s ", book->title);
	printf("作者：%s ", book->author);
	printf("出版年份：%d ", book->year_of_publication);
	printf("版序：%s ", book->version);
	printf("國際書碼:%s ", book->ISBN);
	printf("複本編碼:%d ", book->copy);
	if (book->borrower == NULL) {
		printf("可借閱\n");
	}
	else {
		printf("已有人借閱\n");
	}
	return;
}
void PrintAllBooks() {
	int count = 0;
	while (books[count] != NULL && count < BOOKS_SIZE) {
		PrintBookInfor(books[count]);
		count++;
	}
}
void PrintStudentInfor(struct Student* student) {
	printf("ID：%s\n", student->id);
	//printf("密碼：%s\n", student->password);
	for (int i = 0; i < STUDENT_MAX_BORROW; i++) {
		struct Book* bk = student->borrow_books[i];
		if (bk != NULL) {
			printf("借閱書籍ISBN：%s ", bk->ISBN);
			printf("借書日期：%04d%02d%02d ", bk->borrowed_date->tm_year + 1900, bk->borrowed_date->tm_mon + 1, bk->borrowed_date->tm_mday);
			printf("還書期限：%04d%02d%02d\n", bk->due_date->tm_year + 1900, bk->due_date->tm_mon + 1, bk->due_date->tm_mday);
		}
	}
}
void PrintAllStudents() {
	int count = 0;
	while (students[count] != NULL && count < students_amount) {
		PrintStudentInfor(students[count]);
		printf("\n");
		count++;
	}
}
void LoginAndPrint() {
	struct Student* student = Login();
	if (student != NULL) {
		PrintStudentInfor(student);
	}
}
void SearchBookAndPrint() {
	char str[100];
	int b = 0;
	printf("請輸入書名或是作者或是國際書碼\n");
	scanf("%s", str);
	for (int i = 0; i < BOOKS_SIZE; i++) {
		if (books[i] != NULL && (strcmp(books[i]->ISBN, str) == 0 || strcmp(books[i]->title, str) == 0 || strcmp(books[i]->author, str) == 0)) {
			PrintBookInfor(books[i]);
			b = 1;
		}
	}
	if (b == 0) {
		printf("查無此書\n");
	}
}
void AddBookFromFile() {
	FILE* fptr = fopen("Books.txt", "r");

	if (fptr == NULL) {
		printf("圖書檔案開啟失敗\n");
		return;
	}

	int count = 0;
	char line[300];

	while (fgets(line, sizeof(line), fptr) != NULL && count < BOOKS_SIZE) {
		books[count] = (struct Book*)malloc(sizeof(struct Book));
		books[count]->borrowed_date = (struct tm*)malloc(sizeof(struct tm));
		books[count]->due_date = (struct tm*)malloc(sizeof(struct tm));
		char title[100];
		char author[100];
		//name
		char* ptr = line + 2;
		char* ptr_ = strstr(ptr, "&\\");
		strncpy(books[count]->title, ptr, ptr_ - ptr);
		books[count]->title[ptr_ - ptr] = '\0';

		//author
		ptr = ptr_ + 5;
		ptr_ = strstr(ptr, "&\\");
		strncpy(books[count]->author, ptr, ptr_ - ptr);
		books[count]->author[ptr_ - ptr] = '\0';

		//year_of_publication
		books[count]->year_of_publication = atoi(strtok(ptr_ + 3, " "));

		//version
		strcpy(books[count]->version, strtok(NULL, " "));

		//ISBN
		strcpy(books[count]->ISBN, strtok(NULL, " "));

		//copy
		books[count]->copy = atoi(strtok(NULL, " "));

		books[count]->borrower = NULL;
		//誰借書啦 先幫他找帳號，沒有帳號幫他註冊一個帳號，再丟給借書function
		char* token = strtok(NULL, " ");
		if (token != NULL) {
			token[strlen(token) - 1] = '\0';
			struct Student* currst = SearchAccount(token);
			//沒帳號 幫他註冊
			if (currst == NULL) {
				currst = CreateAccount(token, NULL);
			}

			//printf("currst指針 :%p\n", (void*)currst);
			//printf("%s\n", currst->id);
			books[count]->borrower = currst;
			BorrowBook(currst, books[count], NULL);
		}
		count++;
		books_amount++;
	}
	fclose(fptr);
}
void AddStudentFromFile() {
	FILE* fptr = fopen("Students.txt", "r");
	if (fptr == NULL) {
		printf("學生檔案開啟失敗\n");
		return 1;
	}
	char line[300];
	int n = 0;

	while (fgets(line, sizeof(line), fptr) != NULL && n < STUDENTS_SIZE) {
		char* id = strtok(line, " ");
		//如果有帳號，更改密碼和新增圖書借閱日期
		struct Student* curr_account;
		if ((curr_account = SearchAccount(id)) != NULL) {
			strcpy(curr_account->password, strtok(NULL, " "));
			char* book_ISBN;
			while ((book_ISBN = strtok(NULL, " ")) != NULL) {
				for (int i = 0; i < STUDENT_MAX_BORROW; i++) {
					if (curr_account->borrow_books[i] != NULL && strcmp(curr_account->borrow_books[i]->ISBN, book_ISBN) == 0) {
						char date_str[10];
						strcpy(date_str, strtok(NULL, " "));
						int year, month, day;
						sscanf(date_str, "%4d%2d%2d", &year, &month, &day);
						//printf("(ASFF)%s\n", date_str);
						curr_account->borrow_books[i]->borrowed_date = (struct tm*)malloc(sizeof(struct tm));
						curr_account->borrow_books[i]->due_date = (struct tm*)malloc(sizeof(struct tm));
						curr_account->borrow_books[i]->borrowed_date->tm_year = year - 1900;
						curr_account->borrow_books[i]->borrowed_date->tm_mon = month - 1;
						curr_account->borrow_books[i]->borrowed_date->tm_mday = day;
						curr_account->borrow_books[i]->due_date->tm_year = year - 1900;
						curr_account->borrow_books[i]->due_date->tm_mon = month - 1;
						curr_account->borrow_books[i]->due_date->tm_mday = day + 7;
						//mktime(curr_account->borrow_books[i]->due_date);
					}
				}
			}
		}//如果沒帳號，表示圖書檔也沒有
		else {
			CreateAccount(id, strtok(NULL, " \n"));
		}
		n++;
	}
	fclose(fptr);
}
int main() {
	//圖書檔案處理
	AddBookFromFile();
	//PrintAllBooks();
	////學生資料處理
	AddStudentFromFile();
	//PrintAllStudents();
	printf("%s\n", "請輸入指令：(B)借書 (R)還書 (A)新增圖書 (D)刪除圖書 (Q)搜尋圖書 (I)搜尋個人借閱資料 (P)傾印");
	char key;
	char str[10];
	scanf("%c", &key);
	while (key != 'E') {
		switch (key) {
		case 'B':
			LoginAndBorrow();
			break;
		case 'R':
			ReturnBook();
			break;
		case'A':
			AddBookFromInput();
			break;
		case'D':
			DeleteBook();
			break;
		case'Q':
			SearchBookAndPrint();
			break;
		case'I':
			LoginAndPrint();
			break;
		case'P':
			PrintAllBooks();
			break;
		}
		printf("\n%s\n", "請重新輸入指令：(B)借書 (R)還書 (A)新增圖書 (D)刪除圖書 (Q)搜尋圖書 (I)搜尋個人借閱資料 (P)傾印");
		if (key != 'I')while (getchar() != '\n');
		scanf("%c", &key);
	}
	/*PrintAllBooks();
	PrintAllStudents();*/
	return 0;
}
