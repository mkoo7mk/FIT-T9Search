#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define SIZE 1000
// gcc -std=c99 -Wall -Wextra -Werror t9search.c -o t9search
// ./t9search 111 <seznam.txt

char T9ALPHABET[][5] = {{"0+"},
                        {"1"},
                        {"2abc"},
                        {"3def"},
                        {"4ghi"},
                        {"5jkl"},
                        {"6mno"},
                        {"7pqrs"},
                        {"8tuv"},
                        {"9wxyz"}};

typedef struct{
    char name[MAX];
    int name_len;
    char tel_num[15];
    int tel_num_len;

} Person;

void printPerson(Person p);
int setPerson(Person p,int num_rows, int val_len, char string_buffer[]);
char *toLower(char *a);
void strreplace(char *p, char old, char new, int only_first);
char charToInt(char c);
void readFromFile(int *int_buffer);
int error_handler(int error_code, int number_of_rows);
int queryNumbers(Person p, char num[], int num_len);
int queryNameNumber(Person p, char num[], int num_len);
int searchContact(Person p, char num[], int num_len);
int printQuerriedContacts(Person people[SIZE], char num[], int num_len, int rows_num);
int printQuerriedContact(Person p, char num[], int num_len){
    int was_there;
    if ((was_there = searchContact(p, num, num_len)))
        printPerson(p);
    return was_there;
}

int main(){
    int int_buffer[2];
    readFromFile(int_buffer);
    return error_handler(int_buffer[0], int_buffer[1]);
}

void printPerson( Person p){
    for (int i = 0; i < p.name_len; i++)
        printf("%c", p.name[i]);
    printf(", ");
    for (int i = 0; i < p.tel_num_len; i++)
        printf("%c", p.tel_num[i]);
    printf("\n");
}

int setPerson(Person p, int num_rows, int val_len, char string_buffer[]){
    if (num_rows % 2 == 0){ // Row index is even therefor value is name
        p.name_len = val_len;
        for (int i = 0; i < val_len; i++)
            p.name[i] = string_buffer[i];
    }
    else{ // Row index is odd therefor value is tel_num
        p.tel_num_len = val_len;
        for (int i = 0; i < val_len; i++){
            if(isdigit(string_buffer[i]) || string_buffer[i] == '+')
                p.tel_num[i] = string_buffer[i];
            else return 2;
        }
    }
    printf("%d\n", p.tel_num_len);
    return 0;
}

char *toLower(char *a){
    for (char *p = a; *p; p++)
        *p = tolower(*p);
    return a;
}

void strreplace(char *p, char old, char new, int only_first){
    for (int i = 0; i < (int)strlen(p); i++){
        if (p[i] == old){
            p[i] = new;
            if (only_first)
                break;
        }
    }
}

char charToInt(char c){
    char temp[] = {c};
    for (int i = 0; i < 10; i++)
        if (strstr(T9ALPHABET[i], toLower(temp)))
            return i + '0';
    return ' ';
}

int queryNumbers(Person p, char num[], int num_len){
    // Only numbers querry with plus sign
    int i, j, was_there;
    char num_plus[num_len];
    for (i = 0; i < num_len; i++)
        num_plus[i] = num[i];
    if (strstr(num, "0") != NULL)
        strreplace(num_plus, '0', '+', 1);
    for (i = 0; i <= p.tel_num_len - num_len; i++){
        was_there = 0;
        for (j = 0; j < num_len; j++){
            if (p.tel_num[i + j] == num[j] || p.tel_num[i + j] == num_plus[j]){
                was_there = 1;
            }
            else{
                was_there = 0;
                break;
            }
        }
        if (was_there)
            return 1;
    }
    return 0;
}

int queryNameNumber(Person p, char num[], int num_len){
    int i, j, was_there;
    char char_buffer[p.name_len];
    for (i = 0; i < p.name_len; i++)
        char_buffer[i] = charToInt(p.name[i]);
    i = 0;
    toLower(char_buffer);
    for (i = 0; i <= p.name_len - num_len; i++){
        was_there = 0;
        for (j = 0; j < num_len; j++){
            if (char_buffer[i + j] == num[j])
                was_there = 1;
            else{
                was_there = 0;
                break;
            }
        }
        if (was_there)
            return 1;
    }
    return 0;
}

int searchContact(Person p, char num[], int num_len){
    return queryNumbers(p, num, num_len) || queryNameNumber(p, num, num_len);
}

int printQuerriedContacts(Person people[SIZE], char num[], int num_len, int rows_num){
    int i, was_there = 0;
    for (i = 0; i < (int)floor(rows_num / 2); i++){
        if (searchContact(people[i], num, num_len)){
            printPerson(people[i]);
            was_there = 1;
        }
    }
    return was_there;
}

void readFromFile(int *int_buffer){
    int number_of_rows = 0, err_code = 0;
    char string_buffer[MAX], char_buffer = ' ';
    Person temp_person;
    while (char_buffer != EOF || err_code){
        int j = 0;
        while (!(char_buffer = getchar() == '\n' || char_buffer == EOF)){
            string_buffer[j] = char_buffer;
            j++;
        }
        if (j >= MAX){
            int_buffer[0] = 1, int_buffer[1] = number_of_rows;
            return;
        }
        if ((int_buffer[0] = setPerson(temp_person, number_of_rows, j, string_buffer))){
            int_buffer[1] = number_of_rows;
            return;
        }
        number_of_rows++;
        // printQuerriedContact(temp_person, num, num_len);
    }
}

int error_handler(int error_code, int number_of_rows){
    switch (error_code){
        case 1:
            printf("Error: Invalid line %d, too long line\n", number_of_rows + 1);
            break;
        case 2:
            printf("Error: Invalid phone number on line %d\n", number_of_rows + 1);
            break;
        case 3:
            printf("Error: Invalid query\n");
            break;
        case 4:
            printf("Error: Too many arguments\n");
            break;
    }
    return error_code;
}