#include <stdio.h>
#include <string.h>

// 올바른 방식 (char** 사용)
void sort_correct(char** str1, char** str2) {
    char* temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}

// 잘못된 방식 (char* 사용)
void sort_wrong(char* str1, char* str2) {
    char* temp = str1;
    str1 = str2;
    str2 = temp;
}

int main() {
    char *table[] = {"apple", "banana"};
    
    // 올바른 방식 테스트
    sort_correct(&table[0], &table[1]);
    printf("Correct: %s, %s\n", table[0], table[1]); // 정상적으로 교환됨
    
    // 잘못된 방식 테스트
    char *str1 = table[0];
    char *str2 = table[1];
    sort_wrong(str1, str2);
    printf("Wrong: %s, %s\n", table[0], table[1]); // 교환되지 않음
    
    return 0;
}
