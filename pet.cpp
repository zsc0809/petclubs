#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mysql.h> 
#define LEN 50

typedef struct pet
{
	char petname[LEN];
	char petkind[LEN];
}Pet;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

char menu(void);
void addpet(MYSQL conn);
void droppet(MYSQL conn);
void showpets(MYSQL conn);
void findpet(MYSQL conn);
void uppercase(char * str);
void petcount(MYSQL conn);
char * s_gets(char * st, int n);
int my_insert(MYSQL *conn, Pet * pt);
int my_delete(MYSQL *conn, Pet * pt);
int my_find(MYSQL conn, Pet * pt);


int main(void) 
{	
	MYSQL conn;
	//MYSQL_FIELD *fd;
	MYSQL_RES *res_ptr = nullptr;
	//MYSQL_ROW sqlrow;
	//Pet pets;
	char choice;

	mysql_init(&conn);//初始化句柄 

	if (!mysql_real_connect(&conn, "localhost", "root", "", "petclubs", 0, NULL, 0))
	{
		printf("failed to connect to database: %s\n", mysql_error(&conn));
	}
	else
	{
		printf("connected.\n");
	}

	while((choice = menu()) != 'q')
	{
		switch(choice)
		{
			case 'a': addpet(conn);
				break;
			case 'd': droppet(conn);
				break;
			case 'f': findpet(conn);
				break;
			case 's': showpets(conn);
				break;
			case 'n': petcount(conn);
				break;
			default: puts("switching error!");	
		}
	}
	mysql_free_result(res_ptr);
	mysql_close(&conn);
	getchar();
	puts("Bye!");
	


	return 0;
}

char menu(void)
{
	char ch;
	
	printf("pls enter a, d, f, s, n or q for the option.\n");
	puts("a. add pets;              d. drop pets.");
	puts("f. search pets;           s. show pets.");
	puts("n. the number of pets;    q. quit.");
	
	while((ch = getchar()) != EOF)
	{
		while(getchar() != '\n')
			continue;
		ch = tolower(ch);
		if(strchr("adfsnq", ch) == NULL)
			puts("pls enter a, d, f, s, n or q.");
		else
			break;
	}
	if(ch == EOF)
		ch = 'q';
	
	return ch;
}

void addpet(MYSQL conn)
{
	Pet temp;
	
	puts("pls enter the pet name.");
	s_gets(temp.petname, LEN);
	puts("pls enter the pet kind.");
	s_gets(temp.petkind, LEN);
	uppercase(temp.petname);
	uppercase(temp.petkind);
	my_insert(&conn, &temp);
}

void droppet(MYSQL conn)
{
	Pet item;
	
	puts("enter the pet name.");
	s_gets(item.petname, LEN);
	puts("enter the pet kind.");
	s_gets(item.petkind, LEN);
	uppercase(item.petname);
	uppercase(item.petkind);
	printf("%s, kind:%s,", item.petname, item.petkind);
	my_delete(&conn, &item);
			
}

void showpets(MYSQL conn)
{
	MYSQL_RES *temp = NULL;
	MYSQL_ROW row;
	char sql[100] = "";

	sprintf_s(sql, 100, "select * from pets");//执行查询语句，这里是查询所有，用strcpy也可以
	if ((mysql_query(&conn, sql)))
		printf("query failed.\n");
	else
	{
		temp = mysql_store_result(&conn);
		while (row = mysql_fetch_row(temp))
		{
			for (int i = 0; i < mysql_num_fields(temp); i++)
			{
				printf("%10s ", row[i]);
			}
			puts("\n");
		}
	}
			
	mysql_free_result(temp);
}

void findpet(MYSQL conn)
{
	Pet item;
	
	puts("enter the pet name.");
	s_gets(item.petname, LEN);
	puts("enter the pet kind.");
	s_gets(item.petkind, LEN);
	uppercase(item.petname);
	uppercase(item.petkind);
	printf("%s the %s :", item.petname, item.petkind);
	my_find(conn, &item);
	
}

void petcount(MYSQL conn)
{
	char sql[100] = "";
	int rows;
	MYSQL_RES *temp;

	sprintf_s(sql, 100, "select * from pets");

	if (mysql_query(&conn, sql) != 0)
	{
		printf("error:%s\n", mysql_error(&conn));
		getchar();
	}
	temp = mysql_store_result(&conn);

	rows = mysql_num_rows(temp);
	printf("%d pets in the club.\n", rows);
}

void uppercase(char * str)
{
	while(*str)
	{
		*str = toupper(*str);
		str++;		
	}

}	

char * s_gets(char * st, int n)
{
	char * ret_val;
	char * find;
	
	ret_val = fgets(st, n, stdin);
	if(ret_val)
	{
		find = strchr(st, '\n');
		if(find)
			* find = '\0';
		else 
			while(getchar() != '\n')
				continue;
	}
	
	return ret_val;
}

int my_insert(MYSQL * conn, Pet * item)
{
	char sql[100] = "";  //

	sprintf_s(sql, 100, "INSERT INTO pets(pet_name, pet_kind) VALUES('%s', '%s')"
		, item->petname, item->petkind);
	int ret = mysql_query(conn, sql);
	if (ret != 0)
	{
		printf("error:%s\n", mysql_error(conn));
		getchar();
	}
	//my_ulonglong affected_row = mysql_affected_rows(conn);
	//printf("%d rows affected.\n", (int)affected_row);
	return 0;
}

int my_delete(MYSQL *conn, Pet * item)
{
	char sql[100] = "";  //

	sprintf_s(sql, 100, "DELETE FROM pets WHERE pet_name = '%s' AND pet_kind = '%s'"
		, item->petname, item->petkind);
	printf("%s\n", sql);
	int ret = mysql_query(conn, sql);
	if (ret != 0)
	{
		printf("error:%s\n", mysql_error(conn));
		getchar();
	}
	//my_ulonglong affected_row = mysql_affected_rows(conn);
	//printf("%d rows affected.\n", (int)affected_row);
	return 0;
}

int my_find(MYSQL conn, Pet * item)
{
	char sql[100] = "";  //
	MYSQL_RES * temp;
	MYSQL_ROW row;

	sprintf_s(sql, 100, "SELECT pet_id,pet_name,pet_kind FROM pets WHERE pet_name = '%s'"
		, item->petname);
	int ret = mysql_query(&conn, sql);
	if (ret != 0)
	{
		printf("error:%s\n", mysql_error(&conn));
		getchar();
	}
	temp = mysql_store_result(&conn);
	while ((row = mysql_fetch_row(temp))) {	//依次取出记录
		for (int i = 0; i < mysql_num_fields(temp); i++)
			printf("%s\t", row[i]); 				//输出
		printf("\n");
	}
	if (mysql_errno(&conn)) {
		fprintf(stderr, "Retrive error:s\n", mysql_error(&conn));
	}
	//my_ulonglong affected_row = mysql_affected_rows(conn);
	//printf("%d rows affected.\n", (int)affected_row);
	return 0;
}
