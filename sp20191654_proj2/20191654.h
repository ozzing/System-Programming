#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

/* history linkedlist의 node */
typedef struct node {
	struct node* next;
	char data[100];
}node;

/* hash table linkedlist의 node */
typedef struct hash {
	struct hash* next;
	char key[10];
	char format[5];
	int value;
}hash;

/* symbol table의 node*/
typedef struct sym {
	struct sym* next;
	char lbl[10];
	int loc;
}sym;

/* 변수 */
node* head;
hash* table[20];	// 해시테이블 크기만큼 선언
node* temp;
node* walk;
unsigned long addr = -1;	// 가상메모리 dump 인덱스 값 저장 변수
int tp = -1;	// dump의 타입 구별 변수
unsigned char* mmr;	// 가상메모리 배열
sym* hdsym; // 저장되어 있을 SYMTAB
sym* scsym; // 생성중인 임시 SYMTAB

/* 정의되는 상수 */
#define SIZE 16*65536	// 최대 가상메모리 사이즈

/* 함수 원형 */
void history(char* command);
void commander(char* command);
void opcode_mnemonic(char* command);
void dump(char* command);
void dump_print(char* command, int tp);
void edit(char* command);
void fill(char* command);
void reset();
void type(char* command);
void assemble(char* command);
int pass(char* asb);
int get_reg(char* reg);
void err(char* asb);
void symbol();
int hash_function(char* op);
unsigned long hex_to_dec(char* dmp);
char* rtrim(char* s);
char* ltrim(char* s);
char* trim(char* s);
int cvalid(char* s);
