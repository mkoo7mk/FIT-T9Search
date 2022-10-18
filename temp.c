#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define SIZE 100
#define NUM_ROWS 104
#define s 3
// gcc -std=c99 -Wall -Wextra -Werror t9search.c -o t9search
// ./t9search 111 <seznam.txt

char a[][4] = { {'+'},
                {""},
                {"abc"},
                {"def"},
                {"ghi"},
                {"jkl"},
                {"mno"},
                {"pqrs"},
                {"tuv"},
                {"wxyz"}};

typedef struct Person{
    char name[MAX];
    int name_len;
    char tel_num[15];
    int tel_num_len;

} Person;

void printPerson(const Person p){
    int i;
    for(i = 0; i < p.name_len; i++) printf("%c", p.name[i]);
    printf(", ");
    for(i = 0; i < p.tel_num_len; i++) printf("%c", p.tel_num[i]);
    printf("\n");
}

void setPerson(Person *p, int num_rows, int val_len, char string_buffer[]){
    if (num_rows % 2 == 0){ // Row index is even therefor value is name
        p->name_len = val_len;
        for(int i = 0; i < val_len; i++) p->name[i] = string_buffer[i];
    }
    else{ // Row index is odd therefor value is tel_num
        p->tel_num_len = val_len;
        for(int i = 0; i < val_len; i++) p->tel_num[i] = string_buffer[i];                
    }
}

void print2dArray(const int rows, const int columns, const char array[rows][columns]){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++)printf("%c", array[i][j]);
        printf("\n");
    }
}

char *toLower(char* a) {
  for(char *p=a; *p; p++) *p=tolower(*p);
  return a;
}

int power(const int x, const int y){
    int ans = 1;
    for(int i=1; i<=y; i++) ans = ans*x;
    return ans;
}

int isInSet(const char p[], const char a){
    for(int i = 0; i < (int)strlen(p); i++) if(p[i] == a){return 1;}
    return 0;
}

int isIntInSet(const int p[], const int a, const int p_len){
    for(int i = 0; i < p_len; i++) if(p[i] == a){return 1;}
    return 0;
}
void strreplace(char *p, const char old, const char new, const int only_first){
    for(int i = 0; i < (int)strlen(p); i++){
        if(p[i] == old){
            p[i] = new;
            if(only_first)break;
        }
    }
}

char charToInt(char c){
    char temp[] = {c};
    for (int i = 0; i < 10; i++){
        if(strstr(a[i], toLower(temp))){
            return i + '0';
        }
    }
    return ' ';
}

void make_array(const int rows, const int columns, char (*arr)[rows][columns], const char value){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++)(*arr)[i][j] = value;
    }
}

int get_length(const int num_len, char **argv){
    int j = 1;
    for (int i = 0; i < num_len; i++) j *= (int)strlen(a[(int)argv[1][i]- 48]);
    return j;
}

int querryNumbers(const Person p, const char num[], const int num_len){
    // Only numbers querry with plus sign
    int i, j, was_there;
    char num_plus[num_len];
    for(i = 0; i < num_len; i++) num_plus[i] = num[i];
    if(strstr(num, "0") != NULL) strreplace(num_plus, '0', '+',  1);
    for(i = 0; i <= p.tel_num_len - num_len; i++){
        was_there = 0;
        for(j = 0; j < num_len; j++){
            if(p.tel_num[i+j] == num[j] || p.tel_num[i+j] == num_plus[j]){
                was_there = 1;
            }
            else{
                was_there = 0;
                break;
            }
        }
        if(was_there) return 1;
    }
    return 0;
}

int queryNameNumber(const Person p, const char num[], const int num_len){
    int i, j, was_there;
    char char_buffer[p.name_len];
    for(i = 0; i < p.name_len; i++){
        char_buffer[i] = charToInt(p.name[i]);
    }
    i = 0;
    toLower(char_buffer);
    for(i = 0; i <= p.name_len - num_len; i++){
        was_there = 0;
        for(j = 0; j < num_len; j++){
            if(char_buffer[i+j] == num[j]){
                was_there = 1;
            }
            else{
                was_there = 0;
                break;
            }
        }
        if(was_there) return 1;
    }
    return 0;
}
int querryNames(Person p, const int num_len, const int rows, const char array[rows][num_len]){
    int i, j;
    char t[num_len];
    for(i = 0; i < rows ; i++){
        for(j = 0; j < num_len; j++)t[j] = array[i][j];
        t[j] = '\0';
        if(strstr(toLower(p.name), t) != NULL) return 1;
    }
    return 0;
}

int searchContact(const Person p, const char num[], const int num_len, const int k, const char needle[k][num_len], const int both){
    if(both)return querryNumbers(p, num, num_len) || querryNames(p, num_len, k, needle);
    return querryNumbers(p, num, num_len);
}

int printQuerriedContacts(const Person people[SIZE], const char num[], const int num_len, const int k, const char needle[k][num_len], const int both){
    int i, was_there = 0;
    for(i = 0; i < (int)floor(NUM_ROWS/2); i++){
        if(searchContact(people[i], num, num_len, k, needle, both)){
            printPerson(people[i]);
            was_there = 1;
        }
    }
    return was_there;
}

int main(int argc, char **argv){
    
    char char_buffer;
    char string_buffer[MAX];
    Person people[SIZE];
    int number_of_rows = 0, i = 0, j = 0, permutations_array_len = 0, was_there = 0, error = 0;

    while (number_of_rows < NUM_ROWS){
        j = 0;
        while ((char_buffer = getchar()) != '\n')
        {
            string_buffer[j] = char_buffer;
            j++;
        }
        if(j >= SIZE){
            printf("Error: Too long row on line %d \n", number_of_rows);
            error = 1;
            break;
        }
        setPerson(&people[(int)floor(number_of_rows/2)], number_of_rows, j, string_buffer);
        number_of_rows++;
    }
    int querry_number_length = (int)strlen(argv[1]);
    if (argc == 1 && !error){ // No query
        for(i = 0; i < (int)floor(NUM_ROWS/2); i++)printPerson(people[i]);
        return 0;
    }
    else if (argc == 2){ // Error handling if is query number or +
        for(i = 0; i < querry_number_length; i++){
            if(!(argv[1][i] == '+' || isdigit(argv[1][i]))){
                error = 1;
            }
        }
    }
    if (argc == 2 && !error){
        if(querry_number_length == 1 || strstr(argv[1], "1") != NULL || strstr(argv[1], "0") != NULL){
            // If there is no need to search by name
            char temp[1][querry_number_length];
            was_there = printQuerriedContacts(people, argv[1], querry_number_length, 1, temp, 0);
        }

        else if(0){
            // Array region
            char out[querry_number_length][s];
            make_array(querry_number_length, s, &out, '0'); // Make me a 2d array of 0s
            // Make me a 2d sorted array of used chars 
            for(i = 0; i < querry_number_length; i++){
                j = 0;
                while(j < (int)strlen(a[(int)(argv[1][i] - 48)])){
                    out[i][j] = a[(int)argv[1][i]- 48][j];
                    j++;
                }
            }
            permutations_array_len = get_length(querry_number_length, argv);
            char all_permutations_str[permutations_array_len + querry_number_length][querry_number_length];
            for (i = 0; i < permutations_array_len; i++){
                for(j = 0; j < querry_number_length; j++){
                    all_permutations_str[i][j] = out[j][((int)floor(i/(int)(power(3, j))))%3];
                }
            }

            int int_buffer[querry_number_length];
            int temporary_calculation = 0, k = 0, m;
            for(j = 0; j < querry_number_length; j++){
                if((int)strlen(a[(int)(argv[1][i] - 48)])){
                    int_buffer[temporary_calculation] = j;
                    temporary_calculation++;
                }
            }
            if (temporary_calculation){ // TODO toto treba dorobit
            // Alebo spravit z pismen cisla a hladat podla toho
                for (i = power(3, querry_number_length); i < permutations_array_len; i++){
                    for(j = 0; j < temporary_calculation; j++){
                        if(isIntInSet(int_buffer, j, temporary_calculation)){
                            for(m = 0; m < querry_number_length; m++){
                                if(j == m) all_permutations_str[i][j] = a[(int)(argv[1][i] - 48)][3];
                                else all_permutations_str[i][j] = out[j][((int)floor(k/(int)(power(3, j))))%3];
                            }
                        }
                        k++;
                    }
                }
            }
            print2dArray(permutations_array_len + querry_number_length, querry_number_length, all_permutations_str);
            // End of array region
            was_there = printQuerriedContacts(people, argv[1], querry_number_length, permutations_array_len, all_permutations_str, 1);
        }
        else{
            for(i = 0; i < (int)floor(NUM_ROWS/2); i++){
                if(queryNameNumber(people[i], argv[1], querry_number_length)){
                    printPerson(people[i]);
                    was_there = 1;
                }
            }
        }
        if (!was_there && !error){
            printf("Not found\n");
        }
        return 0;
    }
    else printf("Invalid query\n");
    return -1;
}