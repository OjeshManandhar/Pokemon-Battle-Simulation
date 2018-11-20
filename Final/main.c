#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>

#define HIT 1
#define NOT 0

int total_pokemon = 0, total_moves = 0;

char *types[] = {
                        "Normal", "Fighting", "Flying", "Poison", "Ground",
                        "Rock", "Bug", "Ghost", "Steel", "Fire",
                        "Water", "Grass", "Electric", "Psychic", "Ice",
                        "Dragon"
                    };

struct move_list
{
    int id;
    char name[21];
    int dmg;
    int type;
}temp_move, *move;

struct pokemon_list
{
    int id;
    char name[21];
    int major_move, minor_move_1, minor_move_2;
    int hp, atk, def, spd;
    int type_1, type_2;
}temp_pokemon, *pokemon;

struct user_pokemon
{
    int id;
    int hp;
    int atk;
    int def;
    int spd;
    int charge;
}poke[2];

int getkey();
void poke_user_stat(int, int, int);
void display_stats(int, int);
void swap(int *, int *);
int atk_power(int, int, int);
int attack_type_modifier(int, int, int , int);
int check_critical(int);
void check_effectiveness(int, int);

int main()
{
    FILE *fp;
    int ch, i, poke_num, flag;
    int level, attacker, defender, attack_no;
    int move_damage, atk_def_damage;
    int speed_diff, random_no;
    char choice, poke_name[21];

    //Rading moves
    fp = fopen("..\\move_details.dat", "rb");
    if (fp == NULL)
    {
        perror("Can't open moves file");
        exit(EXIT_FAILURE);
    }
    while (fread(&temp_move, sizeof(struct move_list), 1 ,fp) == 1)
    {
        total_moves++;
        i = temp_move.id;
        move = (struct move_list *)realloc(move, total_moves * sizeof(struct move_list));
        move[i].id = temp_move.id;
        strcpy(move[i].name, temp_move.name);
        move[i].dmg = temp_move.dmg;
        move[i].type = temp_move.type;
    }
    fclose(fp);

    //Reading pokiemons
    fp = fopen("..\\pokemon_details.dat", "rb");
    if (fp == NULL)
    {
        perror("Can't open moves file");
        exit(EXIT_FAILURE);
    }
    while (fread(&temp_pokemon, sizeof(struct pokemon_list), 1 ,fp) == 1)
    {
        total_pokemon++;
        i = total_pokemon - 1;
        pokemon = (struct pokemon_list *)realloc(pokemon, total_pokemon * sizeof(struct pokemon_list));
        pokemon[i].id = temp_pokemon.id;
        strcpy(pokemon[i].name, temp_pokemon.name);
        pokemon[i].major_move = temp_pokemon.major_move;
        pokemon[i].minor_move_1 = temp_pokemon.minor_move_1;
        pokemon[i].minor_move_2 = temp_pokemon.minor_move_2;
        pokemon[i].hp = temp_pokemon.hp;
        pokemon[i].atk = temp_pokemon.atk;
        pokemon[i].def = temp_pokemon.def;
        pokemon[i].spd = temp_pokemon.spd;
        pokemon[i].type_1 = temp_pokemon.type_1;
        pokemon[i].type_2 = temp_pokemon.type_2;
    }
    fclose(fp);

    //Displaying the list of pokemons
    while (1)
    {
        system("cls");
        puts("Choose pokemons from the following list:\n");
        for (i = 0; i < total_pokemon; i++)
        {
            printf("%-26s", pokemon[i].name);
            if ((i + 1) % 3 != 0)
                printf(" ");
        }
        puts("\n");

        //Getting user pokemons
        for(poke_num = 0; poke_num < 2; poke_num++)
            do
            {
                do
                {
                    flag = 0;
                    if(poke_num == 0)
                        printf("Enter the name of the FIRST pokemon: ");
                    else
                        printf("Enter the name of the SECOND pokemon: ");
                    fflush(stdin);
                    gets(poke_name);
                    for (i = 0; i < total_pokemon; i++)
                        if (strcmpi(poke_name, pokemon[i].name) == 0)
                        {
                            printf("Enter %s`s level: ",pokemon[i].name);
                            scanf("%d",&level);
                            poke_user_stat(poke_num, i, level);
                            flag = 1;
                            break;
                        }
                }while (flag == 0);

                display_stats(poke[poke_num].id, poke_num);
                printf("Use this pokemon [Y/N]: ");
                fseek(stdin, 0, 0);
                scanf("%c", &choice);
                //Changing entered character to UPPERCASE
                if (choice >= 'a' && choice <= 'z')
                    choice -= 32;
                printf("\n");
            }while (choice == 'N');

        //Finding out which user will go first
        if (pokemon[poke[0].id].spd >=  pokemon[poke[1].id].spd)
        {
            attacker = 0;
            defender = 1;
        }
        else
        {
            attacker = 1;
            defender = 0;
        }

        puts("\nPress any key to begin battle...");
        getch();
        system("cls");
        printf("%s has %d HP \t\t ", pokemon[poke[0].id].name, poke[0].hp);
        printf("%s has %d HP\n", pokemon[poke[1].id].name, poke[1].hp);
        puts("\n\t\t\tBATTLE BEGINS\n");

        //BATTLE Stimulation
        while (1)
        {
            if (poke[attacker].charge >= 3)
            {
                printf("Which attack will %s use???\n",pokemon[poke[attacker].id].name);
                printf("1. %-23s ", move[pokemon[poke[attacker].id].minor_move_1].name);
                printf("2. %-23s ", move[pokemon[poke[attacker].id].minor_move_2].name);
                printf("3. %s\n", move[pokemon[poke[attacker].id].major_move].name);
                do
                {
                    printf("Attack Number: ");
                    scanf("%d",&attack_no);
                }while ((attack_no < 1) || (attack_no > 3));
            }
            else
            {
                printf("Which attack will %s use ???\n",pokemon[poke[attacker].id].name);
                printf("1. %-36s ", move[pokemon[poke[attacker].id].minor_move_1].name);
                printf("2. %s\n", move[pokemon[poke[attacker].id].minor_move_2].name);
                do
                {
                    printf("Attack Number: ");
                    scanf("%d",&attack_no);
                }while ((attack_no < 1) || (attack_no > 2));
            }

            //to select a random number and the difference between attackers and defenders speed
            srand(time(0));
            speed_diff=(0.1 * (poke[defender].spd - poke[attacker].spd));
            random_no = rand()%20;

            if(attack_no==1)
            {
                poke[attacker].charge++;
                printf("%s uses %s\n", pokemon[poke[attacker].id].name, move[pokemon[poke[attacker].id].minor_move_1].name);
                if(speed_diff == random_no)
                    printf("%s dodged\n",pokemon[poke[defender].id].name);
                else
                {
                    atk_def_damage = atk_power(move[pokemon[poke[attacker].id].minor_move_1].dmg, poke[attacker].atk, poke[defender].def);
                    move_damage = attack_type_modifier(atk_def_damage, move[pokemon[poke[attacker].id].minor_move_1].type,pokemon[poke[defender].id].type_1,pokemon[poke[defender].id].type_2);
                    srand(time(0));
                    if(check_critical(rand()%100) == HIT)
                        {
                            poke[defender].hp -= (move_damage * 1.5);
                            printf("Critical Hit!!!\n");
                        }
                    else
                        poke[defender].hp -= move_damage;
                    check_effectiveness(move_damage, atk_def_damage);
                }
            }
            else if(attack_no==2)
            {
                poke[attacker].charge++;
                printf("%s uses %s\n", pokemon[poke[attacker].id].name,  move[pokemon[poke[attacker].id].minor_move_2].name);
                if (speed_diff == random_no)
                    printf("%s dodged\n",pokemon[poke[defender].id].name);
                else
                {
                    atk_def_damage = atk_power(move[pokemon[poke[attacker].id].minor_move_2].dmg, poke[attacker].atk, poke[defender].def);
                    move_damage = attack_type_modifier( atk_def_damage, move[pokemon[poke[attacker].id].minor_move_2].type,pokemon[poke[defender].id].type_1,pokemon[poke[defender].id].type_2);
                    srand(time(0));
                    if(check_critical(rand()%100) == HIT)
                    {
                        poke[defender].hp -= (move_damage * 1.5);
                        printf("Critical Hit!!!\n");
                    }
                    else
                        poke[defender].hp -= move_damage;
                    check_effectiveness(move_damage, atk_def_damage);
                }
            }
            else if(attack_no==3)
            {
                poke[attacker].charge -= 3;
                printf("%s uses %s\n", pokemon[poke[attacker].id].name,  move[pokemon[poke[attacker].id].major_move].name);
                if(speed_diff == random_no)
                    printf("%s dodged\n",pokemon[poke[defender].id].name);
                else
                {
                    atk_def_damage = atk_power(move[pokemon[poke[attacker].id].major_move].dmg, poke[attacker].atk, poke[defender].def);
                    move_damage = attack_type_modifier(atk_def_damage, move[pokemon[poke[attacker].id].major_move].type,pokemon[poke[defender].id].type_1,pokemon[poke[defender].id].type_2);
                    srand(time(0));
                    if(check_critical(rand()%100) == HIT)
                    {
                        poke[defender].hp -= (move_damage * 1.5);
                        printf("Critical Hit!!!\n");
                    }
                    else
                        poke[defender].hp -= move_damage;
                    check_effectiveness(move_damage,atk_def_damage);
                }
            }

            //Cheching HP of users
            if (poke[defender].hp <= 0)
            {
                poke[defender].hp = 0;
                printf("%s has %d HP \t\t", pokemon[poke[0].id].name, poke[0].hp);
                printf("%s has %d HP\n\n", pokemon[poke[1].id].name, poke[1].hp);
                break;
            }
                printf("%s has %d HP \t\t", pokemon[poke[0].id].name, poke[0].hp);
                printf("%s has %d HP\n\n", pokemon[poke[1].id].name, poke[1].hp);

            swap(&attacker, &defender);
            getch();
        }

        printf("\n%s wins.\n",pokemon[poke[attacker].id].name);

        printf("\n\nPress any key to continue OR ESC to exit....");
        ch = getkey();
        if (ch == 27)
        {
            printf("\nProgram Terminated.....\nTHANK YOU FOR PLAYING");
            break;
        }
    }

    return 0;
}

//To get scan codes of ESC key
int getkey()
{
    int ch;
    ch=getch();
    if (ch==224)
    {
        ch=getch();
        return ch;
    }
    return ch;
}

//To calculate user's stats
void poke_user_stat(int j, int i, int level)
{
    poke[j].id = i;
    poke[j].hp = 15 + (level/100.0) * pokemon[i].hp;
    if(poke[j].hp>pokemon[i].hp)
        poke[j].hp=pokemon[i].hp;
    poke[j].atk = 8 + (level/100.0) * pokemon[i].atk;
    if(poke[j].atk>pokemon[i].atk)
        poke[j].atk=pokemon[i].atk;
    poke[j].def = 5 + (level/100.0) * pokemon[i].def;
    if(poke[j].def>pokemon[i].def)
        poke[j].def=pokemon[i].def;
    poke[j].spd = 8 + (level/100.0) * pokemon[i].spd;
    if(poke[j].spd>pokemon[i].spd)
        poke[j].spd=pokemon[i].spd;
    poke[j].charge = 0;
}

//To print user's stats
void display_stats(int i, int j)
{
    printf("Major Move = %-20s     Damage = %d\n", move[pokemon[i].major_move].name, move[pokemon[i].major_move].dmg);
    printf("Minor Move (1) = %-20s Damage = %d\n", move[pokemon[i].minor_move_1].name, move[pokemon[i].minor_move_1].dmg);
    printf("Minor Move (2) = %-20s Damage = %d\n", move[pokemon[i].minor_move_2].name, move[pokemon[i].minor_move_2].dmg);
    printf("HP = %-3d \t ATK = %-3d \t DEF = %-3d \t SPD = %-3d\n", poke[j].hp, poke[j].atk, poke[j].def, poke[j].spd);
    if(pokemon[i].type_2 == pokemon[i].type_1)
        printf("Type : %s\n", types[pokemon[i].type_1]);
    else
        printf("Type : %s, %s\n", types[pokemon[i].type_1], types[pokemon[i].type_2]);
}

//To change the attackers turn
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//to calculate damage done top the defender after computations of the stats
int atk_power(int atk_damage, int attacker_atk, int defender_def)
{
    int atk_def_damage;

    atk_def_damage = atk_damage + 0.1*(attacker_atk - defender_def);
    if(atk_def_damage<2)
        return 2;
    else
        return atk_def_damage;
}

//To calculate the damage according to type of pokemon and the attack
int attack_type_modifier(int move_damage, int move_type, int defender_type_1, int defender_type_2)
{
    float dmg_multiplying_table[16][16]={
                                        /*1Normal*/ {1, 1, 1, 1, 1, 0.5, 1, 0, 0.5, 1, 1, 1, 1, 1, 1, 1},
                                        /*2Fight*/  {2, 1, 0.5, 0.5, 1, 2, 0.5, 0, 2, 1, 1, 1, 1 ,0.5, 2, 1},
                                        /*3Flying*/ {1, 2, 1, 1, 1, 0.5, 2, 1, 0.5, 1, 1, 2, 0.5, 1, 1, 1},
                                        /*4Poison*/ {1, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 0, 1, 1, 2, 1, 1, 1, 1},
                                        /*5Ground*/ {1, 1, 0, 2, 1, 2, 0.5, 1, 2, 2, 1, 0.5, 2, 1, 1, 1},
                                        /*6Rock*/   {1, 0.5, 2, 1, 0.5, 1, 2, 1, 0.5, 2, 1, 1, 1, 1, 2, 1},
                                        /*7Bug*/    {1, 0.5, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 0.5, 1, 2, 1, 2, 1, 1},
                                        /*8Ghost*/  {0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1},
                                        /*9Steel*/  {1, 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 1, 2, 1},
                                        /*10Fire*/  {1, 1, 1, 1, 1, 0.5 , 2, 1, 2, 0.5, 0.5, 2, 1, 1, 2, 0.5},
                                        /*11Water*/ {1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 0.5},
                                        /*12Grass*/ {1, 1, 0.5, 0.5, 2, 2, 0.5, 1, 0.5, 0.5, 2, 0.5, 1, 1, 1, 0.5},
                                        /*13Electr*/{1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 0.5},
                                        /*14Psychc*/{1, 2, 1, 2, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 0.5, 1, 1},
                                        /*15Ice*/   {1, 1, 2, 1, 2, 1, 1, 1, 0.5, 0.5, 0.5, 2, 1, 1,0.5, 2},
                                        /*16Dragon*/{1, 1, 1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 1, 1, 2}
                                        };
	move_damage *= dmg_multiplying_table[move_type][defender_type_1] ;
	if(defender_type_2 != defender_type_1)
        move_damage *= dmg_multiplying_table[move_type][defender_type_2 ];
	return (move_damage);
}

//To check probability for critical
int check_critical(int random)
{
    int i;
    for(i = 11; i < 100; i += 11)
        if(random == i)
            return HIT;
    return NOT;
}

//To print super effective Not so Effective or Not Effective!!!
void check_effectiveness(int move_damage, int initial_move_damage)
{
    if(move_damage>initial_move_damage)
        printf("Super Effective!!!\n");
    else if (move_damage<initial_move_damage)
        printf("Not so Effective!!!\n");
    else if (move_damage==0)
        printf("Not Effective!!!\n");
}
