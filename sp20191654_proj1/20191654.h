#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

/* history linkedlist�� node */
typedef struct node {
	struct node* next;
	char data[100];
}node;

/* hash table linkedlist�� node */
typedef struct hash {
	struct hash* next;
	char key[10];
	char format[10];
	int value;
}hash;

/* ���� */
node* head;
hash* table[20];	// �ؽ����̺� ũ�⸸ŭ ����
node* temp;
node* walk;
unsigned long addr = -1;	// ����޸� dump �ε��� �� ���� ����
int type = -1;	// dump�� Ÿ�� ���� ����
unsigned char* mmr;	// ����޸� �迭

/* ���ǵǴ� ��� */
#define SIZE 16*65536	// �ִ� ����޸� ������

/* �Լ� ���� */
void history(char* command);
void commander(char* command);
void opcode_mnemonic(char* command);
void dump(char* command);
void dump_print(char* command, int type);
void edit(char* command);
void fill(char* command);
void reset();
int hash_function(char* op);
unsigned long hex_to_dec(char* dmp);
char* rtrim(char* s);
char* ltrim(char* s);
char* trim(char* s);
int cvalid(char* s);
