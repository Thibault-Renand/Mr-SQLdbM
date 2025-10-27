#include<stdio.h>
typedef struct{
    void*key;
    void**items;
    int count;
    int capacity;
}GROUP;
typedef struct{
    GROUP* groups;
    int count;
    int capacity;
}GROUPLIST;
int main(){}