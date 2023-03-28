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

void add (char*);   /* �������� ������ � ���� */
void edit (char*);  /* ������������� ������ */
void show (char*);  /* ����������� ���������� ����� */
void search1 (char*);    /* ����� ������ �������� ��������� ����� ������ */
void search2 (char*);  /*������� ������ ������� ������������ �� 28 ��� � ������ ������������*/
void del (char*);       /* ������� ������ �� ����� */
void printing (FILE *, int);

int main (int argc, char* argv[])
{
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    char filename[80]; /*����������� ����� �����*/
    if (argc > 1)
        strcpy (filename, argv[1]);
    else
    {
        puts("������� ��� ����� ��� ���������");
        gets(filename);
    }
    int menu;
    do
    {
       system ("CLS");
       puts ("1. ���������� ������");
       puts ("2. �������������� ������");
       puts ("3. �������� �������");
       puts ("4. �������� ������");
       puts ("5. ����� ������ � ������ �� ������");
       puts ("6. �����");
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

/*���������� ����� ������ (���� ������ � �������)*/
struct person input_client (void)
{
     struct person employee;
     int temp;
     puts ("������������");
     gets (employee.name);
     puts("������-��������");
     gets( employee.importer);
     puts("�����");
     scanf("%d", &temp);
     employee.volume = temp;
     getchar();
     return employee;
}

/* ������� ���������� ������ � ���� */
void add (char* file)
{
     FILE * f;
     struct person employee;
     system ("CLS");
     if ((f=fopen (file, "ab"))==NULL)
     {
          perror ("������ �������� �����");
          getchar();
          return;
     }
     employee = input_client();
     fwrite (&employee, sizeof (employee), 1, f);
     fclose (f);
}

/* ������� ������ �� ����� ����� ������ */
void print1 (struct person data)
{
    printf ("������������ : %s\n������-�������� : %s\n����� : %d",
            data.name, data.importer, data.volume);
}

/* ������� �������������� ������ */
void edit (char* file)
{
     FILE * f;
     struct person employee;
     char name[30],yes;
     system ("CLS");
     if ((f=fopen (file, "rb+"))==NULL)
     {
          perror ("������ �������� �����"); getchar();
          return;
     }
     printf ("������������: "); gets (name);
     while (fread(&employee,sizeof(employee), 1, f))
        if (!strcmp(employee.name,name))
        {
             print1 (employee);
             puts ("\n�������������? (y/n)");
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
     puts ("����� ��������"); getchar();
     fclose(f);
}

/* ������� ������ ����������� ����� */
void show (char* file)
{
     FILE * f;
     int menu, menu2;
     struct person employee;
     int k=0;
     system ("CLS");
     if ((f=fopen (file, "rb"))==NULL)
     {
          perror ("������ �������� �����");
          getchar();
          return;
     }
    puts ("|  N |             ������������              |      ������-��������      |   �����   |");
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


/* ������� �������� ������ �� ����� */
void del (char* file)
{

    FILE *f1, *f2;
    unsigned int n, n_file, i = 0;
    struct person employee;
    system ("CLS");
    if ((f1=fopen (file, "rb"))==NULL)
    {
        perror ("������ �������� �����"); getchar();
        return;
    }
    fseek (f1, 0, SEEK_END);
    n_file = ftell(f1) / sizeof (employee);
    if (n_file == 0)
    {
        puts ("���� ����"); getchar();
        return;
    }
    puts ("����� ��������� ������?");
    scanf ("%u", &n);
    if ( n > n_file )
    {
        puts ("������������ �����");
        getchar();getchar();
        fclose (f1);
        return;
    }
    rewind (f1);
    if ((f2=fopen ("temp.dat", "wb"))==NULL)
    {
        perror ("������ �������� �����");
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
    puts ("������ �������");
    getchar();getchar();
}


/* ������� ������ �� ������ */
void search1 (char* file)
{
     FILE * f;
     struct person employee, required;
     char req[30];
     int max_age = 3000, age;
     system ("CLS");
     if ((f=fopen (file, "rb"))==NULL)
     {
          perror ("������ �������� �����"); getchar();
          return;
     }
     puts ("������� ������� ����� : ");
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
