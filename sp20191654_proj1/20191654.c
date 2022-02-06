/* ���ԵǴ� ���� */
#include "20191654.h"

/*
 *  ���� �Լ�
 * 1. �����͸� �Է� �޾� hash table ����
 * 2. while���� ���� command�� �Է� �ް� �˸��� �Լ� ȣ��
 */
int main() {
	mmr = (unsigned char*)malloc(sizeof(unsigned char) * (SIZE));
	for (int i = 0; i < 20; i++) {	// hash table �ʱ�ȭ
		table[i] = (hash*)malloc(sizeof(hash));
		table[i]->next = NULL;
	}
	hash* htemp;	// ������ ��ġ ���
	hash* hwalk = (hash*)malloc(sizeof(hash));	// ���� ��ġ ���
	head = (node*)malloc(sizeof(node));
	head->next = NULL;
	temp = head;
	walk = (node*)malloc(sizeof(node));
	char* command = (char*)malloc(sizeof(char) * 100);
	FILE* fin = fopen("opcode.txt", "r");	// ���� �Է�
	while (1) {
		int num;
		char op[10], fm[10];
		fscanf(fin, "%x %s %s", &num, op, fm);	// 16���� �Է�
		if (feof(fin) != 0) break;	// ���Ͽ� ���� ������ ���
		int index = hash_function(op);	// hash �Լ��� ���� hash ���� index��
		htemp = table[index];
		while (1) {
			if (htemp->next == NULL) {	// ��ũ�帮��Ʈ�� ������ �̵�, �� �Է�
				strcpy(htemp->format, fm);
				strcpy(htemp->key, op);
				htemp->value = num;
				hwalk = (hash*)malloc(sizeof(hash));
				htemp->next = hwalk;
				hwalk->next = NULL;
				break;
			}
			htemp = htemp->next;
		}
	}
	reset();
	while (1) {	// command �Է� �ް� �˸��� �Լ� ȣ��
		int valid=0;
		printf("sicsim> ");
		fgets(command, 100, stdin);
		for (int i = 0; i < strlen(command); i++) {	// ���� �����̽��� ���Ҵ�
			if (command[i] == '\t') command[i] = ' ';
		}
		if (strstr(command, "opcode")) {	// ���ڿ� �ȿ� Ư�� ���ڿ��� �ִ��� �˻�
			opcode_mnemonic(command);
			valid=1;
		}
		if (strstr(command, "dump") || strstr(command, "du")) {
			if (strncmp(trim(command), "dump", 4) == 0 || strncmp(trim(command), "du", 2) == 0) { // trim �Լ��� ���� �� �� ���� ����
				dump(trim(command));
				valid=1;
			}
		}
		if (strstr(command, "edit") || strstr(command, "e")) {
			if (strncmp(trim(command), "edit", 4) == 0 || strncmp(trim(command), "e", 1) == 0) {
				edit(trim(command));
				valid=1;
			}
		}
		if (strstr(command, "fill") || strstr(command, "f")) {
			if (strncmp(trim(command), "fill", 4) == 0 || strncmp(trim(command), "f", 1) == 0) {
				fill(trim(command));
				valid=1;
			}
		}
		if (strstr(command, "reset")) {
			if (strcmp(trim(command), "reset") == 0) {
				reset();
				history(trim(command));	// command�� �յ� ���� ���� -> ���� �� �����丮 �Է�
				valid=1;
			}
			else
				printf("wrong command\n");
		}
		if (strstr(command, "hi") || strstr(command, "history")) {	// hi[story]�� ��쿡�� ���� �����丮 ��ũ�帮��Ʈ�� �Է� �� command ����
			if (strcmp(trim(command), "hi") == 0 || strcmp(trim(command), "history") == 0) {
				history(trim(command));
				commander(trim(command));
				valid=1;
			}
		}
		if(valid==0){	// ���� command�� �ƴ� ���
			commander(command);
		}
	}

	fclose(fin);
	free(command);
	free(htemp);
	free(hwalk);
	return 0;
}

/*
 *  command �Լ�
 * �ٸ� �Լ��� ȣ������ �ʴ� command���� ó���ϴ� �Լ�
 */
void commander(char* cmd) {
	int valid = 0;

	if (strstr(cmd, "h") || strstr(cmd, "help")) {
		if (strcmp(trim(cmd), "h") == 0 || strcmp(trim(cmd), "help") == 0) {
			printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\n");	// �ȳ��� ���
			history(trim(cmd));
			valid = 1;
		}
	}
	if (strstr(cmd, "d") || strstr(cmd, "dir")) {
		if (strcmp(trim(cmd), "d") == 0 || strcmp(trim(cmd), "dir") == 0) {
			struct dirent* d;
			DIR * dir = opendir(".");	// ���� ���丮 ����
			if(dir==NULL) printf("empty\n");	// ���丮�� ������ ���� ���
			while((d=readdir(dir))){
				printf("%s", d->d_name);
				if(d->d_type==DT_DIR) printf("/");	// �������� ���丮�� ���
				else if((d->d_type==S_IXUSR)||(d->d_type==S_IWRITE)||(d->d_type==S_IXGRP)||(d->d_type==S_IXOTH)) printf("*");	// �������� ���� ������ ���
				printf("\n");
			}
			history(trim(cmd));
			valid=1;
		}
	}
	if (strstr(cmd, "q") || strstr(cmd, "quit")) {
		if (strcmp(trim(cmd), "q") == 0 || strcmp(trim(cmd), "quit") == 0) {
			exit(0);	// �ý��� ����
		}
	}
	if (strcmp(trim(cmd), "hi") == 0 || strcmp(trim(cmd), "history") == 0) {
		node* his = head;	// �����丮 ��ũ�帮��Ʈ�� ������
		while (his->next != NULL) {	// �����丮 ��ũ�帮��Ʈ�� ������ �ݺ�
			printf("%s\n", his->data);
			his = his->next;
		}
		valid = 1;
	}
	if (valid == 0) {
		printf("wrong command\n");
	}
}

/*
 *  history �Լ�
 * �Ķ���ͷ� ���� command�� ���ʷ� ��ũ�帮��Ʈ�� ����
 */
void history(char* cmd) {
	strcpy(temp->data, cmd);
	walk = (node*)malloc(sizeof(node));	// ���� ���
	temp->next = walk;
	walk->next = NULL;
	temp = temp->next;
}

/*
 *  opcode �Լ�
 * opcode mnemonic, opcodelist ��ɾ ó���ϴ� �Լ�
 * ����, ��ȿ ���ڸ� ����Ͽ� ��ȿ�� ��ɾ����� �Ǵ� �� ����
 */
void opcode_mnemonic(char* cmd) {
	if ((strncmp(trim(cmd), "opcode ", 7) == 0)) {
		char* mne = (char*)malloc(sizeof(char) * 20);	// mnemonic ���ڿ�
		char* check;	
		strcpy(mne, trim(cmd));
		mne = strtok(mne, " ");	// ������ �������� �и�
		mne = strtok(NULL, " ");
		check = strtok(NULL, " ");	// �Է� ���� mnemonic�� �� ������ Ȯ���ϱ� ���� ���ڿ�
		if (check) {
			printf("wrong command\n");
			return;
		}
		mne = trim(mne);	// �� �� ���� ����
		int index = hash_function(mne);	// hash �Լ� ȣ��� index �Ҵ�
		hash* find = (hash*)malloc(sizeof(hash));
		find = table[index];
		while (1) {
			if (strcmp(find->key, mne) == 0) {	// index�� �Է¹��� key ���� �ִ��� Ȯ��
				printf("opcode is %X\n", find->value);
				break;
			}
			else find = find->next; // ���� ��� index�� ���� ���� �̵�
			if (find->next == NULL) {	// ������ ������ ���� ���
				printf("opcode for %s doesn't exist\n", mne);
				break;
			}
		}
		char opcd[20] = "opcode ";
		history(strcat(opcd, mne)); // �����丮 ����
		free(find);
	}
	else if (strcmp(trim(cmd), "opcodelist") == 0) {
		hash* hwalk = (hash*)malloc(sizeof(hash));
		for (int i = 0; i < 20; i++) {
			hwalk = table[i];	// table index�� ����Ʈ ù ���
			printf("%d : ", i);
			while (hwalk->next != NULL) {	// index�� ������ ������
				printf("[%s,%X]", hwalk->key, hwalk->value);
				hwalk = hwalk->next;
				if (hwalk->next != NULL) printf(" -> ");	// ������ ��忡�� ������� ����
			}
			printf("\n");
		}
		history(trim(cmd));
		free(hwalk);
	}
	else {
		printf("wrong command\n");
		return;
	}
}

/*
 *  dump �Լ�
 * ���� �� comma üũ �� comma ������ ���� Ÿ���� �����Ͽ� dump_print �Լ� ȣ��
 */
void dump(char* cmd) {
	if (strcmp(cmd, "dump") == 0 || strcmp(cmd, "du") == 0) {
		dump_print(cmd, 5);	// start�� end ���� ���� ��� Ÿ���� 5�� ����
		addr += 160;
	}
	else if (strncmp(cmd, "dump ", 5) == 0 || strncmp(cmd, "du ", 3) == 0) {
		int comma = 0;
		char* ptr = strchr(cmd, ',');
		while (ptr != NULL) {	// cmd�� ������ ���ڱ���
			if (ptr) comma += 1;	// comma ���� ī��Ʈ
			ptr = strchr(ptr + 1, ',');
		}
		if (comma == 0) {
			dump_print(cmd, 0);
		}
		else if (comma == 1) {
			dump_print(cmd, 1);
		}
		else {
			printf("wrong command\n");
			return;
		}
	}
	else {
		printf("wrong command\n");
		return;
	}
}

/*
 *  dump_print �Լ�
 * �Ķ���ͷ� Ÿ���� �޾� ���� �� ��ȿ ���� Ȯ��, ��ȿ�� ��� command ������ ���
 */
void dump_print(char* cmd, int type) {
	unsigned long start, end, e, temp = addr+1;
	char* com = (char*)malloc(sizeof(char) * 100);
	strcpy(com, trim(cmd));	// Ÿ�� 5�� ���, com ��
	if (type == 0) {
		char* dmp = (char*)malloc(sizeof(char) * 20);
		strcpy(dmp, cmd);
		dmp = strtok(dmp, " ");
		strcpy(com, trim(dmp));	// du[mp] ����
		dmp = strtok(NULL, " ");
		int vld = cvalid(dmp);	// ��ȿ�� ���ڰ� �ִ��� Ȯ��
		if (vld != 0) {
			printf("wrong command\n");
			return;
		}
		strcat(com, " ");
		strcat(com, trim(dmp));	// start �� ����
		temp = hex_to_dec(dmp);
		dmp = strtok(NULL, " ");	// start �� �ٸ� ���� �Է� �Ǿ����� Ȯ��
		if (dmp) {
			printf("wrong command\n");
			return;
		}

		if (temp >= SIZE || temp < 0) {	// ���� Ȯ��
			printf("start, out of boundary\n");
			return;
		}

	}
	// ���� ����
	e = temp + 160;
	start = temp - (temp % 16);
	end = temp + 160 + 16 - (temp % 16);
	if (temp % 16 == 0) end -= 16;

	if (type == 1) { // type 0�� ����
		char* dmp = (char*)malloc(sizeof(char) * 20);
		strcpy(dmp, cmd);
		dmp = strtok(dmp, " ");
		strcpy(com, trim(dmp));
		dmp = strtok(NULL, "\0");
		int vld = cvalid(dmp);
		if (vld != 1) {
			printf("wrong command\n");
			return;
		}
		if (strtok(NULL, " ") == NULL) {
			strcat(com, " ");
			strcat(com, trim(dmp = strtok(dmp, ",")));	// start �� ����
			char* ch = (char*)malloc(sizeof(char) * 20);
			strcpy(ch, trim(dmp));
			for (int i = 0; i < strlen(ch); i++) {	// ���� ���� ���� Ȯ��
				if (ch[i] == ' ') {
					printf("wrong command\n");
					return;
				}
			}
			start = hex_to_dec(trim(dmp));
			strcat(com, ", ");
			strcat(com, trim(dmp = strtok(NULL, ",")));	// end �� ����
		}
		else {
			printf("wrong command\n");
			return;
		}
		char* check = strtok(dmp, " ");
		check = strtok(NULL, " ");
		if (check) {
			printf("wrong command\n");
			return;
		}
		end = hex_to_dec(trim(dmp));
		temp = end;
		dmp = strtok(NULL, " ");
		if (dmp) {
			printf("wrong command\n");
			return;
		}
		if (start >= SIZE || start < 0) {
			printf("start, out of boundary\n");
			return;
		}
		if (end >= SIZE || end < 0) {
			printf("end, out of boundary\n");
			return;
		}
		if (start > end) {
			printf("start, bigger than end\n");
			return;
		}
		temp = start;
		e = end;
		start = start - (start % 16);
		end = end + 16 - (end % 16);
	}
	while (start < end) {	// start���� end���� ���
		if (start % 16 == 0) printf("%05lX ", start);
		if (start >= temp && start < temp + 160 && start <= e) printf("%02lX ", (unsigned long)mmr[start]);	// ���� temp, e�� command���� �Է� ���� ��ȿ ��� ����
		else printf("   ");
		start += 1;
		if (start % 16 == 0) {	// 16��, �� �� �� ����� �Ϸ�� ���
			printf("; ");
			for (int j = start - 16; j < start; j++) {	// ���� �޸��� 16���� ���� ���ڷ� ���
				if (mmr[j] < 32 || mmr[j]>126 || j<temp || j>temp + 159 || j > end) printf(". ");	// ��ȿ ����
				else if (mmr[j] == 32) printf("  ");	// �����̽�
				else printf("%c ", (char)mmr[j]);
			}
			printf(" \n");
		}
		if (start >= SIZE) {
			if (end > SIZE) printf("out of boundary\n");
			addr = 0;
			break;
		}
	}
	history(com);
	free(com);
}

/*
 *  edit �Լ�
 * ������ ��ȿ���� Ȯ�� ��, command ������ ����޸� �� ����
 */
void edit(char* cmd) {
	char* com = (char*)malloc(sizeof(char) * 100);
	if (strncmp(cmd, "edit ", 5) == 0 || strncmp(cmd, "e ", 2) == 0) {
		int comma = 0;
		char* ptr = strchr(cmd, ',');
		while (ptr != NULL) {	// comma ���� Ȯ��
			if (ptr) comma += 1;
			ptr = strchr(ptr + 1, ',');
		}
		if (comma == 1) {
			unsigned long address, value;
			char* edt = (char*)malloc(sizeof(char) * 20);
			strcpy(edt, cmd);
			edt = strtok(edt, " ");
			strcpy(com,trim(edt));	// e[dit] ����
			edt = strtok(NULL, "\0");
			int vld = cvalid(edt);	// ��ȿ ���ڰ� �ִ��� Ȯ��
			if (vld != 1) {
				printf("wrong command\n");
				return;
			}
			if (strtok(NULL, " ") == NULL) {
				strcat(com, " ");
				strcat(com, trim(edt = strtok(edt, ",")));	// address ����
				char* ch = (char*)malloc(sizeof(char) * 20);
				strcpy(ch, trim(edt));
				for (int i = 0; i < strlen(ch); i++) {	// ���� ���� ���� Ȯ��
					if (ch[i] == ' ') {
						printf("wrong command\n");
						return;
					}
				}
				address = hex_to_dec(trim(edt));
				strcat(com, ", ");
				strcat(com, trim(edt = strtok(NULL, ",")));	// value ����
			}
			else {
				printf("wrong command\n");
				return;
			}
			char* check = strtok(edt, " ");
			check = strtok(NULL, " ");
			if (check) {
				printf("wrong command\n");
				return;
			}
			value = hex_to_dec(trim(edt));
			edt = strtok(NULL, " ");
			if (edt) {
				printf("wrong command\n");
				return;
			}
			if (address >= SIZE || address < 0) {
				printf("address, out of boundary\n");
				return;
			}
			if (value > 255 || value < 0) {
				printf("value, out of boundary\n");
				return;
			}
			mmr[address] = value;	// �޸��� address �ε����� value �� ����
		}
		else {
			printf("wrong command\n");
			return;
		}
	}
	else {
		printf("wrong command\n");
		return;
	}
	history(com);
	free(com);
}

/*
 *  fill �Լ�
 * ������ ��ȿ���� Ȯ�� ��, command ������ ������ ������ŭ ����޸� �� ����
 */
void fill(char* cmd) {
	char* com = (char*)malloc(sizeof(char) * 100);
	if (strncmp(cmd, "fill ", 5) == 0 || strncmp(cmd, "f ", 2) == 0) {
		int comma = 0;
		char* ptr = strchr(cmd, ',');
		while (ptr != NULL) {
			if (ptr) comma += 1;
			ptr = strchr(ptr + 1, ',');
		}
		if (comma == 2) {
			unsigned long start, end, value;
			char* fll = (char*)malloc(sizeof(char) * 20);
			strcpy(fll, cmd);
			fll = strtok(fll, " ");
			strcpy(com, trim(fll));	// f[ill] ����
			fll = strtok(NULL, "\0");
			int vld = cvalid(fll);
			if (vld != 2) {
				printf("wrong command\n");
				return;
			}
			if (strtok(NULL, " ") == NULL) {
				strcat(com, " ");
				strcat(com, trim(fll = strtok(fll, ",")));	// start ����
				char* ch = (char*)malloc(sizeof(char) * 20);
				strcpy(ch, trim(fll));
				for (int i = 0; i < strlen(ch); i++) {	// ���� ���� ���� Ȯ��
					if (ch[i] == ' ') {
						printf("wrong command\n");
						return;
					}
				}
				start = hex_to_dec(trim(fll));
				fll = strtok(NULL, "\0");
				if (fll) {
					strcat(com, ", ");
					strcat(com, trim(fll = strtok(fll, ",")));	// end ����
					ch = (char*)malloc(sizeof(char) * 20);
					strcpy(ch, trim(fll));
					for (int i = 0; i < strlen(ch); i++) {	// ���� ���� ���� Ȯ��
						if (ch[i] == ' ') {
							printf("wrong command\n");
							return;
						}
					}
					end = hex_to_dec(trim(fll));
				}
				else {
					printf("wrong command\n");
					return;
				}
			}
			else {
				printf("wrong command\n");
				return;
			}
			strcat(com, ", ");
			strcat(com, trim(fll = strtok(NULL, "\0")));	// value ����
			value = hex_to_dec(trim(fll));
			fll = strtok(NULL, " ");
			if (fll) {
				printf("wrong command\n");
				return;
			}

			if (start < 0) {
				printf("start, out of boudnary\n");
				return;
			}
			if (end >= SIZE) {
				printf("end, out of boundary\n");
				return;
			}
			if (value > 255 || value < 0) {
				printf("value, out of boundary\n");
				return;
			}
			for (int i = start; i <= end; i++) {	// start���� end ���������� �޸� �� value�� ���Ҵ�
				mmr[i] = value;
			}
		}
		else {
			printf("wrong command\n");
			return;
		}
	}
	else {
		printf("wrong command\n");
		return;
	}
	history(com);
	free(com);
}

/*
 *  reset �Լ�
 * ���� �޸��� ��� ���� 0���� �ʱ�ȭ
 */
void reset() {
	for (int i = 0; i < SIZE; i++) {
		mmr[i] = 0;
	}
}

/*
 *  hash_function �Լ�
 * �Է� ���� mnemonic�� �� ���ں� �ƽ�Ű �ڵ� ���� ���� �� 20���� ���� ���� hash ������ ��ȯ
 */
int hash_function(char* op) {
	char num = 0;
	for (int i = 0; i < strlen(op); i++) {
		num += *(op + i);
	}
	if (num < 0) {	// �հ谡 ���� ���� ��� ��� ������ ����
		num = num * -1;
	}
	return num % 20;
}

/*
 *  hex_to_dec �Լ�
 * �Է� ���� ���ڿ��� ��Ÿ���� 16���� ���� 10������ ��ȯ�Ͽ� ��ȯ
 */
unsigned long hex_to_dec(char* dmp) {
	int res = 0;
	res = strtoul(dmp, NULL, 16);
	return res;
}

/*
 *  rtrim �Լ�
 * �Է� ���� ���ڿ��� ���� ���� ����
 */
char* rtrim(char* s) {
	char* end;
	end = s + strlen(s) - 1;	// ���ڿ� ������ ���� ��ġ
	while (1) {
		if (!*end) break;	// ������ ���� ���
		if (isspace(*end))	// �ִ� ��� ������ ���� ��ġ ���Ҵ�
			end--;
		else {
			*(end + 1) = '\0';
			break;
		}
	}
	return strncpy(s, s, strlen(end));	// ���Ҵ��� ��ġ������ �κ� ���ڿ� ��ȯ
}

/*
 *  ltrim �Լ�
 * �Է� ���� ���ڿ��� ���� ���� ����
 */
char* ltrim(char* s) {
	char* start;
	start = s;	// ���ڿ� ó�� ���� ��ġ
	while (1) {
		if (!*start) break;	// ���� ���� ���
		if (isspace(*start))	// �ִ� ��� ���� ���� ��ġ ���Ҵ�
			start++;
		else {
			s = start;	// ���Ҵ��� ���� ��ġ����
			break;
		}
	}
	return s;
}

/*
 *  trim �Լ�
 * ltrim, rtrim �Լ� ȣ���� ���� �Է� ���� ���ڿ��� �յ� ���� ����
 */
char* trim(char* s) {
	if (s != NULL)
		return ltrim(rtrim(s));
	else	// ���ڿ��� ���� ���
		return s;
}

/*
 *  cvalid �Լ�
 * �Է� ���� ���ڿ��� ���ڵ��� ��ȿ�� ������ Ȯ�� (16����), comma ������ ��ȯ
 */
int cvalid(char* s) {
	int cvld = 0;
	char c;
	for (int i = 0; i < strlen(s); i++) {
		c = *(s + i);
		if ((c >= 48 && c <= 57) ||( c >= 65 && c <= 70) ||( c >= 97 && c <= 102) || c == 32) {}	// 16���� ��ȿ��
		else if (c == 44) {	// comma ����
			cvld++;
		}
		else {	// ��ȿ ���� �߽߰�
			cvld = -1;
			break;
		}
	}
	return cvld;
}
