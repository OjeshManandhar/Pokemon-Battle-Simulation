#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

int total = 0, total_moves = 0;

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
}data_move, *moves;

struct pokemon_list
{
    int id;
    char name[21];
    int major, minor_1, minor_2;
    int hp, atk, def, spd;
    int type_1, type_2;
}data, *arr;

char get_confirmation(char *details);
int check_move(char *name);
int check_type(char *name);
void get_data(struct pokemon_list *data);
void put_data(const struct pokemon_list pk);
void del_record(void *arr,int *no,const int loc);

int main()
{
    FILE *fp, *temp;
    int i, choice, flag, dummy;
    char ch;

    //Reading moves
    fp = fopen("..\\move_details.dat", "rb");
    if (fp == NULL)
    {
        perror("Can't open moves file");
        exit(EXIT_FAILURE);
    }

    while (fread(&data_move, sizeof(struct move_list), 1 ,fp) == 1)
    {
        total_moves++;
        i = data_move.id;
        moves = (struct move_list *)realloc(moves, total_moves * sizeof(struct move_list));
        moves[i].id = data_move.id;
        strcpy(moves[i].name, data_move.name);
        moves[i].damage = data_move.damage;
        moves[i].type = data_move.type;
    }

    fclose(fp);

    //Reading pokemons
    fp = fopen("..\\pokemon_details.dat", "rb+");
    if (fp == NULL)
    {
        fp = fopen("..\\pokemon_details.dat", "ab+");
        if (fp == NULL)
        {
            perror("Can't open pokemon file");
            exit(EXIT_FAILURE);
        }
    }

    while (fread(&data, sizeof(data), 1, fp) == 1)
        total++;

    while (1)
    {
        system("cls");
        printf("1. Add Pokemon Data\n"
               "2. List Pokemon Data\n"
               "3. Edit Pokemon Data\n"
               "4. Delete Pokemon Data\n"
               "5. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            fseek(fp, 0, SEEK_END);

            system("cls");
            printf("\t\t\tAdd Pokemon Data\n");
            do
            {
                printf("%-7s: %d", "ID", total + 1);
                data.id = total + 1;
                get_data(&data);

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
            printf("\t\t\tList Pokemon Data\n");

            if (total == 0)
                printf("\n\t\t\t    NO DATA\n");
            else
                while (fread(&data, sizeof(data), 1, fp) == 1)
                {
                    printf("\n%-7s: %d\n", "ID", data.id);
                    put_data(data);
                }

            printf("\nPress any key to go back to main menu....");
            getch();
            break;

        case 3:
            rewind(fp);

            system("cls");
            printf("\t\t\tEdit Pokemon Data\n");

            if (total == 0)
                printf("\n\t\t\t    NO DATA\n");
            else
            {
                temp = fopen("temp.dat", "wb+");
                if (temp == NULL)
                {
                    perror("Can't perform this operation");
                    break;
                }

                arr = (struct pokemon_list *)malloc(total * sizeof(struct pokemon_list));
                for (i = 0; i < total; i++)
                    fread(&arr[i], sizeof(arr[i]), 1, fp);

                do
                {
                    printf("\n%-7s: ", "ID");
                    scanf("%d", &dummy);

                    flag = 0;
                    for (i = 0; i < total; i++)
                        if (arr[i].id == dummy)
                        {
                            flag = 1;
                            put_data(arr[i]);

                            ch = get_confirmation("Edit this data");
                            if (ch == 'Y')
                            {
                                do
                                {
                                    printf("Enter NEW:");
                                    get_data(&arr[i]);

                                    ch = get_confirmation("Add this data");
                                }while (ch == 'N');
                                break;
                            }
                        }
                    if (flag == 0)
                        printf("No data found.\n");

                    ch = get_confirmation("Edit another data");
                }while(ch == 'Y');

                for (i = 0; i < total; i++)
                    fwrite(&arr[i], sizeof(arr[i]), 1, temp);

                free(arr);
                fclose(fp);
                fclose(temp);
                remove("..\\pokemon_details.dat");
                rename("temp.dat", "..\\pokemon_details.dat");

                fp = fopen("..\\pokemon_details.dat", "rb+");
                if (fp == NULL)
                {
                    perror("Can't complete operation.");
                    exit(EXIT_FAILURE);
                }
            }

            printf("\nPress any key to go back to main menu....");
            getch();
            break;

        case 4:
            rewind(fp);

            system("cls");
            printf("\t\t\tDelete Pokemon Data\n");

            if (total == 0)
                printf("\n\t\t\t    NO DATA\n");
            else
            {
                temp = fopen("temp.dat", "wb+");
                if (temp == NULL)
                {
                    perror("Can't perform this operation");
                    break;
                }

                arr = (struct pokemon_list *)malloc(total * sizeof(struct pokemon_list));
                for (i = 0; i < total; i++)
                    fread(&arr[i], sizeof(arr[i]), 1, fp);

                do
                {
                    printf("\n%-7s: ", "ID");
                    scanf("%d", &dummy);

                    flag = 0;
                    for (i = 0; i < total; i++)
                        if (arr[i].id == dummy)
                        {
                            flag = 1;
                            put_data(arr[i]);

                            ch = get_confirmation("Delete this data");
                            if (ch == 'Y')
                            {
                                del_record(arr, &total, i);
                                break;
                            }
                        }
                    if (flag == 0)
                        printf("No data found.\n");

                    ch = get_confirmation("Delete another data");
                }while(ch == 'Y');

                for (i = 0; i < total; i++)
                    fwrite(&arr[i], sizeof(arr[i]), 1, temp);

                free(arr);
                fclose(fp);
                fclose(temp);
                remove("..\\pokemon_details.dat");
                rename("temp.dat", "..\\pokemon_details.dat");

                fp = fopen("..\\pokemon_details.dat", "rb+");
                if (fp == NULL)
                {
                    perror("Can't complete operation.");
                    exit(EXIT_FAILURE);
                }
            }

            printf("\nPress any key to go back to main menu....");
            getch();
            break;

        case 5:
            exit(EXIT_SUCCESS);
            break;
        }
    }

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

int check_move(char *name)
{
    int i;
    for (i = 0; i < total_moves; i++)
        if (strcmpi(name, moves[i].name) == 0)
            return i;

    return -1;
}

int check_type(char *name)
{
    int i;
    for (i = 0; i < 16; i++)
        if (strcmpi(name, type_list[i]) == 0)
            return i;

    return -1;
}

void get_data(struct pokemon_list *data)
{
    int flag;
    char dummy[21];

    printf("\n%-7s: ", "Name");
    fflush(stdin);
    gets(data->name);

    flag = 0;
    do
    {
        printf("%-7s: ", "Major");
        fflush(stdin);
        gets(dummy);
        flag = check_move(dummy);
        if (flag != -1)
            data->major = flag;
    }while (flag == -1);

    flag = 0;
    do
    {
        printf("%-7s: ", "Minor 1");
        fflush(stdin);
        gets(dummy);
        flag = check_move(dummy);
        if (flag != -1)
            data->minor_1 = flag;
    }while (flag == -1);

    flag = 0;
    do
    {
        printf("%-7s: ", "Minor 2");
        fflush(stdin);
        gets(dummy);
        flag = check_move(dummy);
        if (flag != -1)
            data->minor_2 = flag;
    }while (flag == -1);

    printf("%-7s: ", "HP");
    scanf("%d", &data->hp);
    printf("%-7s: ", "ATK");
    scanf("%d", &data->atk);
    printf("%-7s: ", "DEF");
    scanf("%d", &data->def);
    printf("%-7s: ", "SPD");
    scanf("%d", &data->spd);

    flag = 0;
    do
    {
        printf("%-7s: ", "Type 1");
        fflush(stdin);
        gets(dummy);
        flag = check_type(dummy);
        if (flag != -1)
            data->type_1 = flag;
    }while (flag == -1);

    flag = 0;
    do
    {
        printf("%-7s: ", "Type 2");
        fflush(stdin);
        gets(dummy);
        flag = check_type(dummy);
        if (flag != -1)
            data->type_2 = flag;
    }while (flag == -1);
}

void put_data(const struct pokemon_list data)
{
    printf("%-7s: %s\n", "Name", data.name);

    printf("%-7s: %-20s  ", "Major", moves[data.major].name);
    printf("ID: %-2d  Damage: %-3d  Type:  %s\n", moves[data.major].id, moves[data.major].damage, type_list[moves[data.major].type]);

    printf("%-7s: %-20s  ", "Minor 1", moves[data.minor_1].name);
    printf("ID: %-2d  Damage: %-3d  Type:  %s\n", moves[data.minor_1].id, moves[data.minor_1].damage, type_list[moves[data.minor_1].type]);

    printf("%-7s: %-20s  ", "Minor 2", moves[data.minor_2].name);
    printf("ID: %-2d  Damage: %-3d  Type:  %s\n", moves[data.minor_2].id, moves[data.minor_2].damage, type_list[moves[data.minor_2].type]);

    printf("HP: %-3d \t ATK: %-3d \t DEF: %-3d \t SPD: %-3d\n", data.hp, data.atk, data.def, data.spd);

    if (data.type_1 == data.type_2)
        printf("Type: %s\n", type_list[data.type_1]);
    else
        printf("Type: %s, %s\n", type_list[data.type_1], type_list[data.type_2]);
}

void del_record(void *arr,int *no,const int loc)
{
    int i , j, n = *no, size;
    struct pokemon_list *data, *temp;

    size = sizeof(struct pokemon_list);
    data = (struct pokemon_list *)arr;
    temp = (struct pokemon_list *)malloc((n-1) * size);
    for (i = 0, j = 0; i < n; i++)
        if (i != loc)
        {
            temp[j] = data[i];
            j++;
        }
    free(data);
    n--;
    data = (struct pokemon_list *)malloc(n * size);
    for (i = 0; i < n; i++)
        data[i] = temp[i];
    free(temp);
    *no = n;
}
