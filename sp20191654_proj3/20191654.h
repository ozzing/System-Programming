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
	char format[5];
	int value;
}hash;

/* symbol table�� node*/
typedef struct sym {
	struct sym* next;
	char lbl[10];
	int loc;
}sym;

typedef struct bps {
	struct bps* next;
	int point;
}bps;

typedef struct es {
	struct es* next;
	char cs[10];
	char name[10];
	unsigned long ADDR;
	unsigned long lth;
}es;

typedef struct rf {
	struct rf* next;
	char symbol[6];
	int num;
}rf;

/* ���� */
node* head;
hash* table[20];	// �ؽ����̺� ũ�⸸ŭ ����
node* temp;
node* walk;
unsigned long addr = -1;	// ����޸� dump �ε��� �� ���� ����
int tp = -1;	// dump�� Ÿ�� ���� ����
unsigned char* mmr;	// ����޸� �迭
sym* hdsym; // ����Ǿ� ���� SYMTAB
sym* scsym; // �������� �ӽ� SYMTAB
bps* hdbp; // ����� bp list
es* hdes; // loader ���� ������ ESTAB
rf* hdrf; // reference list 
unsigned long PROGADDR = 0;
unsigned long CSADDR;
unsigned long EXECADDR=-1;
unsigned long EXECEND=-1;
unsigned long RUNSTART;
unsigned long RUNEND;
unsigned long regi[10]; // �������� �� ����


/* ���ǵǴ� ��� */
#define SIZE 16*65536	// �ִ� ����޸� ������

/* �Լ� ���� */
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
void progaddr(char* command);
void loader(char* command);
void bp(char* command);
void run(char* command);
int get_reg(char* reg);
void err(char* asb);
void symbol();
int hash_function(char* op);
unsigned long hex_to_dec(char* dmp);
char* rtrim(char* s);
char* ltrim(char* s);
char* trim(char* s);
int cvalid(char* s);
int break_point(unsigned long pc);