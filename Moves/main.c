#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

char *type_list[] = {
                        "Normal", "Fighting", "Flying", "Poison", "Ground",
                        "Rock", "Bug", "Ghost", "Steel", "Fire",
                        "Water", "Grass", "Electric", "Psychic", "Ice",
                        "Dragon"
                    };

struct move_list
{
    int id;
    char name[21];
    int damage;
    int type;
}data, *arr;

void del_record(void *arr,int *no,const int loc);
char get_confirmation(char *details);

int main()
{
    FILE *fp, *temp;
    int i, j, total = 0, choice, flag, temp_flag, temp_id;
    char ch, type[10];

    fp = fopen("..\\move_details.dat", "rb+");
    if (fp == NULL)
    {
        fp = fopen("..\\move_details.dat", "ab+");
        {
            perror("Can't open file");
            exit(EXIT_FAILURE);
        }
    }

    while (fread(&data, sizeof(data), 1, fp) == 1)
        total++;

    while (1)
    {
        system("cls");
        printf("1. Add Move Data\n"
               "2. List Move Data\n"
               "3. Edit Move Data\n"
               "4. Delete Move Data\n"
               "5. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            fseek(fp, 0, SEEK_END);

            system("cls");
            printf("\t\tAdd Move Data\n");
            do
            {
                printf("\n%-6s: %d\n", "ID", total);
                data.id = total;
                printf("%-6s: ", "Name");
                fflush(stdin);
                gets(data.name);
                printf("%-6s: ", "Damage");
                scanf("%d", &data.damage);
                flag = 0;
                do
                {
                    printf("%-6s: ", "Type");
                    fflush(stdin);
                    gets(type);
                    for (i = 0; i < 16; i++)
                        if (strcmpi(type, type_list[i]) == 0)
                        {
                            flag = 1;
                            data.type = i;
                            break;
                        }
                }while (flag == 0);

                ch = get_confirmation("Add this data");
                if (ch == 'Y')
                {
                    fwrite(&data, sizeof(data), 1, fp);
                    total++;
                }

                ch = get_confirmation("Add another data");
            }while (ch == 'Y');

            printf("\nPress any key to go back to main menu....");
            getch();
            break;

        case 2:
            rewind(fp);

            system("cls");
            printf("\t\tList Move Data\n\n");

            if (total == 0)
                printf("\t\t   NO DATA\n");
            else
            {
                printf("Id  %-20s  Damage  Type\n", "Name");
                while (fread(&data, sizeof(data), 1, fp) == 1)
                    printf("%-2u  %-20s  %-6u  %s\n", data.id, data.name, data.damage, type_list[data.type]);
            }

            printf("\nPress any key to go back to main menu....");
            getch();
            break;

        case 3:
            rewind(fp);

            temp = fopen("temp.dat", "wb+");
            if (temp == NULL)
            {
                perror("Can't perform this operation");
                break;
            }

            system("cls");
            printf("\t\tEdit Move Data\n");

            if (total == 0)
                printf("\n\t\t   NO DATA\n");
            else
            {
                arr = (struct move_list *)malloc(total * sizeof(struct move_list));
                for (i = 0; i < total; i++)
                    fread(&arr[i], sizeof(arr[i]), 1, fp);

                do
                {
                    printf("\nEnter ID: ");
                    scanf("%d", &temp_id);

                    flag = 0;
                    for (i = 0; i < total; i++)
                        if (arr[i].id == temp_id)
                        {
                            flag = 1;
                            printf("%-6s: %d\n", "ID", arr[i].id);
                            printf("%-6s: %s\n", "Name", arr[i].name);
                            printf("Damage: %d\n", arr[i].damage);
                            printf("%-6s: %s\n", "Type", type_list[arr[i].type]);

                            ch = get_confirmation("Edit this data");
                            if (ch == 'Y')
                            {
                                do
                                {
                                    printf("New %-6s: %d\n", "ID", arr[i].id);
                                    printf("New %-6s: ", "Name");
                                    fflush(stdin);
                                    gets(arr[i].name);
                                    printf("New %-6s: ", "Damage");
                                    scanf("%d", &arr[i].damage);
                                    temp_flag = 0;
                                    do
                                    {
                                        printf("New %-6s: ", "Type");
                                        fflush(stdin);
                                        gets(type);
                                        for (j = 0; j < 16; j++)
                                            if (strcmpi(type, type_list[j]) == 0)
                                            {
                                                temp_flag = 1;
                                                arr[i].type = j;
                                                break;
                                            }
                                    }while (temp_flag == 0);

                                    ch = get_confirmation("Add this data");
                                }while (ch == 'N');
                                break;
                            }
                        }
                    if (flag == 0)
                        printf("No data found.\n");

                    ch = get_confirmation("Edit another data");
                }while(ch == 'Y');
            }

            for (i = 0; i < total; i++)
                fwrite(&arr[i], sizeof(arr[i]), 1, temp);

            free(arr);
            fclose(fp);
            fclose(temp);
            remove("..\\move_details.dat");
            rename("temp.dat", "..\\move_details.dat");

            fp = fopen("..\\move_details.dat", "rb+");
            if (fp == NULL)
            {
                perror("Can;t complete operation.");
                exit(EXIT_FAILURE);
            }

            printf("\nPress any key to go back to main menu....");
            getch();
            break;

        case 4:
            rewind(fp);

            temp = fopen("temp.dat", "wb+");
            if (temp == NULL)
            {
                perror("Can't perform this operation");
                break;
            }

            system("cls");
            printf("\t\tDelete Move Data\n");

            if (total == 0)
                printf("\n\t\t   NO DATA\n");
            else
            {
                arr = (struct move_list *)malloc(total * sizeof(struct move_list));
                for (i = 0; i < total; i++)
                    fread(&arr[i], sizeof(arr[i]), 1, fp);

                do
                {
                    printf("\nEnter ID: ");
                    scanf("%d", &temp_id);

                    flag = 0;
                    for (i = 0; i < total; i++)
                    {
                        if (arr[i].id == temp_id)
                        {
                            flag = 1;
                            printf("%-6s: %d\n", "ID", arr[i].id);
                            printf("%-6s: %s\n", "Name", arr[i].name);
                            printf("Damage: %d\n", arr[i].damage);
                            printf("%-6s: %s\n", "Type", type_list[arr[i].type]);

                            ch = get_confirmation("Delete this data");
                            if (ch == 'Y')
                            {
                                del_record(arr,&total,i);
                                break;
                            }
                        }
                    }
                    if (flag == 0)
                        printf("No data found.\n");

                    ch = get_confirmation("Delete another data");
                }while(ch == 'Y');
            }

            for (i = 0; i < total; i++)
                fwrite(&arr[i], sizeof(arr[i]), 1, temp);

            free(arr);
            fclose(fp);
            fclose(temp);
            remove("..\\move_details.dat");
            rename("temp.dat", "..\\move_details.dat");

            fp = fopen("..\\move_details.dat", "rb+");
            if (fp == NULL)
            {
                perror("Can;t complete operation.");
                exit(EXIT_FAILURE);
            }


            printf("\nPress any key to go back to main menu....");
            getch();
            break;

        case 5:
            exit(EXIT_SUCCESS);
            break;
        }
    }

    fclose(fp);

    return 0;
}

char get_confirmation(char *details)
{
    char ch;

    printf("\n");
    do
    {
        printf("%s [Y/N]: ", details);
        fflush(stdin);
        scanf("%c", &ch);
        if (ch >= 'a' && ch <= 'z')
            ch -= 32;
    }while (ch != 'Y' && ch != 'N');

    return ch;
}

void del_record(void *arr,int *no,const int loc)
{
    int i , j, n = *no, size;
    struct move_list *data, *temp;

    size = sizeof(struct move_list);
    data = (struct move_list *)arr;
    temp = (struct move_list *)malloc((n-1) * size);
    for (i = 0, j = 0; i < n; i++)
        if (i != loc)
        {
            temp[j] = data[i];
            temp[j].id = j;
            j++;
        }
    free(data);
    n--;
    data = (struct move_list *)malloc(n * size);
    for (i = 0; i < n; i++)
        data[i] = temp[i];
    free(temp);
    *no = n;
}
