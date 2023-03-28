#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
struct person
{
       char name[30];
       char importer[30];
       int volume;
};

void add (char*);   /* добавить запись в файл */
void edit (char*);  /* редактировать запись */
void show (char*);  /* просмотреть содержимое файла */
void search1 (char*);    /* найти самого старшего работника среди мужчин */
void search2 (char*);  /*вывести список молодых специалистов до 28 лет с высшим образованием*/
void del (char*);       /* удалить запись из файла */
void printing (FILE *, int);

int main (int argc, char* argv[])
{
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    char filename[80]; /*определение имени файла*/
    if (argc > 1)
        strcpy (filename, argv[1]);
    else
    {
        puts("Введите имя файла для обработки");
        gets(filename);
    }
    int menu;
    do
    {
       system ("CLS");
       puts ("1. Добавление записи");
       puts ("2. Редактирование записи");
       puts ("3. Просмотр записей");
       puts ("4. Удаление записи");
       puts ("5. Вывод страны и объема по товару");
       puts ("6. Выход");
       scanf ("%d%*c", &menu);
       switch (menu)
       {
              case 1 : add (filename); break;
              case 2 : edit (filename); break;
              case 3 : show (filename); break;
              case 4 : del (filename); break;
              case 5 : search1 (filename); break;
       }
    }
    while (menu!=6);
    return 0;
}

/*заполнение одной записи (ввод данных о клиенте)*/
struct person input_client (void)
{
     struct person employee;
     int temp;
     puts ("Наименование");
     gets (employee.name);
     puts("Страна-импортер");
     gets( employee.importer);
     puts("Объем");
     scanf("%d", &temp);
     employee.volume = temp;
     getchar();
     return employee;
}

/* функция добавления записи в файл */
void add (char* file)
{
     FILE * f;
     struct person employee;
     system ("CLS");
     if ((f=fopen (file, "ab"))==NULL)
     {
          perror ("Ошибка открытия файла");
          getchar();
          return;
     }
     employee = input_client();
     fwrite (&employee, sizeof (employee), 1, f);
     fclose (f);
}

/* функция вывода на экран одной записи */
void print1 (struct person data)
{
    printf ("Наименование : %s\nСтрана-импортер : %s\nОбъем : %d",
            data.name, data.importer, data.volume);
}

/* функция редактирования записи */
void edit (char* file)
{
     FILE * f;
     struct person employee;
     char name[30],yes;
     system ("CLS");
     if ((f=fopen (file, "rb+"))==NULL)
     {
          perror ("Ошибка открытия файла"); getchar();
          return;
     }
     printf ("Наименование: "); gets (name);
     while (fread(&employee,sizeof(employee), 1, f))
        if (!strcmp(employee.name,name))
        {
             print1 (employee);
             puts ("\nРедактировать? (y/n)");
             do
             {
                  yes = getchar(); getchar();
             }
             while (yes!='y' && yes!='Y' && yes!='n' && yes!='N');
             if (yes=='y' || yes=='Y')
             {
                  employee = input_client();
                  fseek (f,-sizeof(employee),SEEK_CUR);
                  fwrite (&employee, sizeof (employee), 1, f);
                  fseek (f,0,SEEK_CUR);
             }
        }
     puts ("Поиск завершен"); getchar();
     fclose(f);
}

/* функция печати содержимого файла */
void show (char* file)
{
     FILE * f;
     int menu, menu2;
     struct person employee;
     int k=0;
     system ("CLS");
     if ((f=fopen (file, "rb"))==NULL)
     {
          perror ("Ошибка открытия файла");
          getchar();
          return;
     }
    puts ("|  N |             Наименование              |      Страна-Импортер      |   Объем   |");
    puts ("--------------------------------------------------------------------------------------");
    while(fread(&employee, sizeof(employee), 1, f)==1)
     {
                printf ("|%3d | %-37.30s |%-26s |%-8d   |\n",
                ++k, employee.name, employee.importer, employee.volume);
                puts ("--------------------------------------------------------------------------------------");
                }
    fclose(f);
    getchar(); getchar();
}


/* функция удаления записи из файла */
void del (char* file)
{

    FILE *f1, *f2;
    unsigned int n, n_file, i = 0;
    struct person employee;
    system ("CLS");
    if ((f1=fopen (file, "rb"))==NULL)
    {
        perror ("Ошибка открытия файла"); getchar();
        return;
    }
    fseek (f1, 0, SEEK_END);
    n_file = ftell(f1) / sizeof (employee);
    if (n_file == 0)
    {
        puts ("Файл пуст"); getchar();
        return;
    }
    puts ("Номер удаляемой записи?");
    scanf ("%u", &n);
    if ( n > n_file )
    {
        puts ("Некорректный номер");
        getchar();getchar();
        fclose (f1);
        return;
    }
    rewind (f1);
    if ((f2=fopen ("temp.dat", "wb"))==NULL)
    {
        perror ("Ошибка открытия файла");
        getchar();
        return;
    }
    while ( fread(&employee, sizeof(employee), 1, f1)==1 )
    {
        i++;
        if ( i != n )
            fwrite(&employee, sizeof(employee), 1, f2);
    }
    fclose (f1);
    fclose (f2);
    remove (file);
    rename ("temp.dat", file);
    puts ("Запись удалена");
    getchar();getchar();
}


/* функция поиска по товару */
void search1 (char* file)
{
     FILE * f;
     struct person employee, required;
     char req[30];
     int max_age = 3000, age;
     system ("CLS");
     if ((f=fopen (file, "rb"))==NULL)
     {
          perror ("Ошибка открытия файла"); getchar();
          return;
     }
     puts ("Введите искомый товар : ");
     gets(req);
     while (fread(&employee, sizeof(employee), 1, f)==1)
        if (!strcmp(employee.name,req))
        {
            printf("\n");
            print1 (employee);
            printf("\n");
        }
     fclose(f);
     getchar();getchar();
}
