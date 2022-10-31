#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define SIZE 1000

/*
Disclaimers: 
    1. I do not support letter or any other character in phone number except plus sign
    2. I support plus in query
    3. I support -l argument followed by number as Levenshtein distance
    4. My implementation of a levenshtein distance do not shows substrings
       it needs to find whole name or number within the levenshtein range 
    5. I do not support non continuous substrings
    6. I do not support -s argument
*/
char T9ALPHABET[][6] = {{"0+"},
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
int setPerson(Person *p,int num_rows, int val_len, char string_buffer[]); // Set persons name and number
char *toLower(char *a); // Convert to string lowercase
void strreplace(char *p, char old, char new, int only_first); // Replace characters in string
char charToInt(char c); // From char to index of T9ALPHABET: 2->abc...
void readFromFile(Person *people, int buffer[2]); // Handling reading from file
void error_handler(int error_code, int number_of_rows); // Printing error messages based on error code
int queryNumbers(Person p, char num[], int num_len); // Query people by numbers
int queryNameNumber(Person p, char num[]); // Query people by name
int searchContact(Person p, char num[], int num_len, int lav); // Handling which queries are used
int printQuerriedContacts(Person people[SIZE], char num[], int num_len, int rows_num, int lav); 
int levenshteinDistance(Person p, char *word1, int name); // Returns levenshtein distance
int min(int a, int b, int c); // Returns lowest number of three 
int min2(int a, int b); // Returns lowest number of two

int main(int argc, char **argv){
    Person people[SIZE]; // main array for people
    int int_buffer[2], i = 0;
    int levenshtein = 0;
    readFromFile(people, int_buffer);
    int number_of_rows = int_buffer[0], error_code = int_buffer[1];

    if (argc == 1 && !error_code){ // No query
        for (i = 0; i < (int)floor(number_of_rows / 2); i++)
            printPerson(people[i]);
        return error_code;
    }
    else if (argc > 2){ // Handle space between nubmers
        error_code = 4;
        for (int i = 0; i < argc; i++){
            if (!strcmp(argv[i], "-l") && i + 1 < argc){ // Levenshtein and its args
                error_code = 0;
                levenshtein = atoi(argv[i + 1]); // returns number from string, if its letter, returns 0
                break;
            }
        }
    }
    else if (argc == 2) // Error handling if is query number or +
        for (i = 0; i < (int)strlen(argv[1]); i++)
            if (!(argv[1][i] == '+' || isdigit(argv[1][i])))
                error_code = 3;

    if (!error_code){
        int querry_number_length = (int)strlen(argv[1]), was_there; // No need to search by name
        was_there = printQuerriedContacts(people, argv[1], querry_number_length, number_of_rows, levenshtein);
        if (!was_there && !error_code)
            printf("Not found\n");
        return error_code;
    }
    error_handler(error_code, number_of_rows);
    return -error_code;
}

void printPerson( Person p){
    for (int i = 0; i < p.name_len; i++)
        printf("%c", p.name[i]);
    printf(", ");
    for (int i = 0; i < p.tel_num_len; i++)
        printf("%c", p.tel_num[i]);
    printf("\n");
}

int setPerson(Person *p, int num_rows, int val_len, char string_buffer[]){
    if (num_rows % 2 == 0){ // Row index is even therefor value is name
        p->name_len = val_len;
        for (int i = 0; i < val_len; i++)
            p->name[i] = string_buffer[i];
    }
    else{ // Row index is odd therefor value is tel_num
        p->tel_num_len = val_len;
        for (int i = 0; i < val_len; i++)
            if(isdigit(string_buffer[i]) || string_buffer[i] == '+')
                p->tel_num[i] = string_buffer[i];
            else return 2;
    }
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
    char temp[2] = {c};
    for(int i = 0; i < 10; i++)
        if(strstr(T9ALPHABET[i], toLower(temp)) != NULL)
            return (char)(i + 48);
    return ' ';
}

int queryNumbers(Person p, char num[], int num_len){
    // Only numbers querry with plus sign
    char num_plus[num_len];
    for (int i = 0; i < num_len; i++)
        num_plus[i] = num[i];
    if (strstr(num, "0") != NULL)
        strreplace(num_plus, '0', '+', 1);
    if (strstr(p.tel_num, num_plus) != NULL || strstr(p.tel_num, num)) return 1;
    return 0;
}

int queryNameNumber(Person p, char num[]){
    int i;
    char char_buffer[p.name_len];
    for (i = 0; p.name[i] != '\0'; i++)
        char_buffer[i] = charToInt(p.name[i]);
    char_buffer[i] = '\0';
    toLower(char_buffer);
    if (strstr(char_buffer, num) != NULL) return 1;
    return 0;
}

int searchContact(Person p, char num[], int num_len, int lav){
    if (lav) return queryNumbers(p, num, num_len) || 
    queryNameNumber(p, num) || lav >= levenshteinDistance(p, num, 0);
    return queryNumbers(p, num, num_len) || queryNameNumber(p, num); // No need to calculate levenshtein distance
}

int printQuerriedContacts(Person people[SIZE], char num[], int num_len, int rows_num, int lav){
    int i, was_there = 0;
    for (i = 0; i < (int)floor(rows_num / 2); i++){
        if (searchContact(people[i], num, num_len, lav)){
            printPerson(people[i]);
            was_there = 1;
        }
    }
    return was_there;
}

void readFromFile(Person *people, int buffer[2]){
    int number_of_rows = 0, err_code = 0;
    char string_buffer[MAX], char_buffer = ' ';
    while (char_buffer != EOF || err_code){
        int j = 0;
        char_buffer = getchar();
        while (!(char_buffer == '\n' || char_buffer == EOF)){
            string_buffer[j] = char_buffer;
            char_buffer = getchar();
            j++;
        }
        if (j >= SIZE){
            err_code = 1;
            break;
        }
        if ((err_code = setPerson(&people[(int)floor(number_of_rows / 2)], number_of_rows, j, string_buffer)))
            break;
        number_of_rows++;
    }
    buffer[0] = number_of_rows, buffer[1] = err_code;
}

void error_handler(int error_code, int number_of_rows){
    switch (error_code){
        case 1:
            fprintf(stderr, "Error: Invalid line %d, too long line\n", number_of_rows + 1);
            break;
        case 2:
            fprintf(stderr, "Error: Invalid phone number on line %d\n", number_of_rows + 1);
            break;
        case 3:
            fprintf(stderr, "Error: Invalid query\n");
            break;
        case 4:
            fprintf(stderr, "Error: Too many or too little arguments\n");
            break;
    }
}

int levenshteinDistance(Person p, char *word1, int name){
    int i, j;
    char c1, c2;
    char char_buffer[name ? p.name_len : p.tel_num_len];
    if (name) // checking levenshtein distance for name and for number and returning lower
        for (i = 0; p.name[i] != '\0'; i++)
            char_buffer[i] = charToInt(p.name[i]);
    else
        for (i = 0; p.tel_num[i] != '\0'; i++)
            char_buffer[i] = charToInt(p.tel_num[i]);        
    char_buffer[i] = '\0';
    int len1 = strlen(word1);
    int len2 = strlen(char_buffer);
    int matrix[len1 + 1][len2 + 1];

    for (i = 0; i <= len1; i++) matrix[i][0] = i;
    for (i = 0; i <= len2; i++) matrix[0][i] = i;
    for (i = 1; i <= len1; i++) {
        c1 = word1[i-1];
        for (j = 1; j <= len2; j++){
            c2 = char_buffer[j-1];
            if (c1 == c2) matrix[i][j] = matrix[i - 1][j - 1];
            else matrix[i][j] = min(matrix[i-1][j] + 1, matrix[i][j-1] + 1, matrix[i-1][j-1] + 1);
        }
    }
    return name ? matrix[len1][len2] : min2(matrix[len1][len2], levenshteinDistance(p, word1, 1));
}

int min(int a, int b, int c){
    if(a < b && a < c)return a;
    if(b < a && b < c)return b;
    return c;
}

int min2(int a, int b){
    if(a < b)return a;
    return b;
}