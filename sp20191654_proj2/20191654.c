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
	hdsym = (sym*)malloc(sizeof(sym));
	hdsym->next = NULL;
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
			if (strncmp(trim(command), "opcode ", 7) == 0 || strncmp(trim(command), "opcodelist", 10) == 0) {
				opcode_mnemonic(command);
				valid=1;
			}
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
		if (strstr(command, "type")) {
			if (strncmp(trim(command), "type", 4) == 0) {
				type(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "symbol")) {
			if (strcmp(trim(command), "symbol") == 0 ) {
				symbol();
				history(trim(command)); // symbol�� �Է��� ��� ��찡 history�� ����
				valid = 1;
			}
		}
		if (strstr(command, "assemble")) {
			if (strncmp(trim(command), "assemble", 8) == 0) {
				assemble(trim(command));
				valid = 1;
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
			printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\nassemble filename\ntype filename\nsymbol\n");	// �ȳ��� ���
			history(trim(cmd));
			valid = 1;
		}
	}
	if (strstr(cmd, "d") || strstr(cmd, "dir")) {
		if (strcmp(trim(cmd), "d") == 0 || strcmp(trim(cmd), "dir") == 0) {
			struct dirent* d;
			struct stat st;
			DIR * dir = opendir(".");	// ���� ���丮 ����
			if(dir==NULL) printf("empty\n");	// ���丮�� ������ ���� ���
			while((d=readdir(dir))){
				printf("%s", d->d_name);
				stat(d->d_name, &st);
				if(d->d_type==DT_DIR) printf("/");	// �������� ���丮�� ���
				else if(S_IEXEC & st.st_mode) printf("*");	// �������� ���� ������ ���
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
		int i = 0;
		while (his->next != NULL) {	// �����丮 ��ũ�帮��Ʈ�� ������ �ݺ�
			printf("%d\t%s\n", ++i, his->data);
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
void dump_print(char* cmd, int tp) {
	unsigned long start, end, e, temp = addr+1;
	char* com = (char*)malloc(sizeof(char) * 100);
	strcpy(com, trim(cmd));	// Ÿ�� 5�� ���, com ��
	if (tp == 0) {
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

	if (tp == 1) { // tp 0�� ����
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

/*
 *  type �Լ�
 * �Է� ���� ���ϸ��� ��ġ�ϴ� ������ ������ ���� ���
 */
void type(char* cmd) {
	char* com = (char*)malloc(sizeof(char) * 100);
	char* typ = (char*)malloc(sizeof(char) * 20);
	strcpy(typ, cmd);
	typ = strtok(typ, " ");
	strcpy(com, trim(typ));	// type ����
	typ = strtok(NULL, " ");
	if (typ == NULL) { // ���ϸ��� �Է� ���� �� �� ���
		printf("wrong command\n");
		return;
	}
	strcat(com, " ");
	strcat(com, trim(typ));	// filename ����
	struct dirent* d;
	DIR* dir = opendir(".");
	if (dir == NULL) printf("empty\n");
	int exist=0;
	while ((d = readdir(dir))) {
		if (strcmp(d->d_name, typ) == 0) {
			exist=1;
			if(d->d_type==DT_DIR){
				printf("%s is a directory\n", d->d_name);
				return;
			}
			FILE * fin = fopen(typ, "r");
			char* s=(char*)malloc(sizeof(char)*100);
			while(fgets(s, 100, fin) && feof(fin)==0){
				printf("%s", s);
			}			
		}
	}
	if (exist==0) {
		printf("No such file\n");
		return;
	}
	typ = strtok(NULL, " ");	
	if (typ) {
		printf("wrong command\n");
		return;
	}

	history(com);
	return;
}

/*
 *  assemble �Լ�
 * �Է� ���� ������ ������ ��� pass�Լ� ȣ��
 */
void assemble(char* cmd) {
	char* com = (char*)malloc(sizeof(char) * 100);
	char* asb = (char*)malloc(sizeof(char) * 20);
	strcpy(asb, cmd);
	asb = strtok(asb, " ");
	strcpy(com, trim(asb));	// assemble ����
	asb = strtok(NULL, " ");
	if (asb == NULL) { // ���ϸ��� �Է� ���� �� �� ���
		printf("wrong command\n");
		return;
	}
	strcat(com, " ");
	strcat(com, trim(asb));	// filename ����

	struct dirent* d;
	DIR* dir = opendir("."); // ���� ���丮
	if (dir == NULL) printf("empty\n"); // ���丮�� ����� ���
	int exist = 0;
	int tf = 0;
	while ((d = readdir(dir))) {
		if (strcmp(d->d_name, asb) == 0) { // ���ڿ��� ��ġ�ϴ� ���ϸ��� ���� ���
			exist = 1;
			char* temp = (char*)malloc(sizeof(char) * 20);
			strcpy(temp, asb);
			temp = strtok(temp, ".");
			strcpy(asb, temp); // ���ϸ� ����
			temp = strtok(NULL, "."); // ������ Ȯ���� ����
			if (strcmp(temp, "asm")==0) { // Ȯ���ڰ� asm�� ��ġ�� ���
				tf=pass(asb); // pass�� ȣ��, ���� ���θ� ���� ����
				break;
			}
			else {
				printf("%s is not a asm type file\n", d->d_name); // asm ������ �ƴ� ���
				return;
			}
		}
	}
	if (exist == 0) { // ���ڿ��� ��ġ�ϴ� ������ ���丮�� �������� ���� ���
		printf("No such file\n");
		return;
	}
	asb = strtok(NULL, " ");
	asb = strtok(NULL, " ");
	if (asb) { // Ȯ���� �ڿ� �߸��� �߰� �Է��� ���� ���
		printf("wrong command\n");
		return;
	}
	if(tf==1) history(com); // pass�Լ� ��� assemble�� ������ ���
	return;
}

/*
 *  pass �Լ�
 * �Է� ���� ���ڿ��� asm ���Ͽ� ���Ͽ� pass1, pass2�� ���� obj, lst ������ ����
 * pass1 : forward referencing�� ���� ���� �� label�� �� �ּҸ� �����ϴ� SYMTAB ����
 * pass2 : SYMTAB�� ������� opcode�� ���, �� ����� �°� obj, lst ������ ����
 * assemble ���� ���ο� ���� ������ 1, ���н� 0 ����
 */
int pass(char* asb) {

	// pass 1 ����
	char* in = (char*)malloc(sizeof(char) * 30);
	char* save = (char*)malloc(sizeof(char) * 30);
	char* lbl = (char*)malloc(sizeof(char) * 10);
	char* mne = (char*)malloc(sizeof(char) * 10);
	char* opr = (char*)malloc(sizeof(char) * 10);

	FILE* fin = fopen(strcat(asb, ".asm"), "r");
	FILE* op = fopen("opcode.txt", "r");
	FILE* md = fopen("intermediate.txt", "w");

	fgets(in, 100, fin); // asm ������ ���پ� �Է�
	strcpy(save, in);
	lbl = strtok(in, " "); // label
	mne = strtok(NULL, " "); // mnemonic opcode
	opr = strtok(NULL, "\n"); // operand

	int stadd = 0;
	int loc = 0;
	int length = 0;
	int check = 0;
	if (strcmp(mne, "START") == 0) { // START�� ���� ���
		stadd = atoi(trim(opr));
		loc = stadd; // START�� �� �ּ� loc���� ����
		fprintf(md, "%s", save);
		scsym = (sym*)malloc(sizeof(sym));
		scsym->next = NULL;
	}
	else { // ���� ��� ���Ƿ� �ʱ�ȭ
		loc = 0;
		scsym = (sym*)malloc(sizeof(sym));
		scsym->next = NULL;
	}

	while (fgets(in, 100, fin) && feof(fin) == 0) { // �� ���� �Է� �ް�, ������ ���� ���� ���� ���
		sym* tmpsym = scsym; // second symbol table, �� �̹� assemble�� ���� ���� ���� SYMTAB�� �ּҸ� �޾� ��
		int symfound = 0;
		check += 5;
		strcpy(save, in);
		lbl = strtok(in, " ");
		mne = strtok(NULL, " ");
		opr = strtok(NULL, "\n");
		if (mne == NULL) mne = " ";
		if (strstr(mne, ",")) { // comma�� ���� ��� �ٽ� operand�� ������
			strcat(mne, opr);
			opr = NULL;
		}
		if (strcmp(lbl, "END") != 0) { // END�� �ƴ� ����
			if (strncmp(lbl, ".", 1) != 0) { // �ּ��� �ƴϸ�
				if (opr != NULL) {
					while (tmpsym->next != NULL) { // SYMTAB Ž��
						if (strcmp(mne, tmpsym->lbl) == 0) {
							symfound = 1;
						}
						tmpsym = tmpsym->next;
					}
					if (symfound == 1) { // �ߺ� �� ���
						printf("duplicate symbol error in line %d\n", check);
						return 0;
					}
					else { // �ƴ� ��� ���� SYMTAB�� ����
						sym* nxtsym = (sym*)malloc(sizeof(sym));
						nxtsym->next = NULL;
						tmpsym->next = nxtsym;
						strcpy(tmpsym->lbl, lbl);
						tmpsym->loc = loc;
					}
				}
				else { // opr�� ���� ��
					if (strcmp(mne, " ") != 0) { // mne�� �ִٸ�
						opr = (char*)malloc(sizeof(char) * 10);
						strcpy(opr, mne); // �˸��� �ڸ��� �̵�
						strcpy(mne, lbl);
						lbl = NULL; // �� �� ����
						opr = strtok(opr, "\n");
					}
				}
				int opfound = 0;
				if (strncmp(mne, "+", 1) == 0) { // extended �� ���
					mne = strtok(mne, "+"); // �����ϰ� ���ڿ� ���Է�
					loc++; // loc ���� (format 4)
				}
				int index = hash_function(mne);
				hash* htemp;
				htemp = table[index];
				while (htemp->next != NULL) {
					if (strcmp(htemp->key, mne) == 0) { // �����ϴ� opcode�� ���
						if (strlen(htemp->format) > 1) loc += 3; // format 3/4�� ���
						else loc += atoi(htemp->format); // �ƴ� ��� format��ŭ loc ����
						opfound = 1;
						break;
					}
					htemp = htemp->next;
				}
				if (opfound == 1) {
				}
				else if (strcmp(mne, "WORD") == 0) { // ���� �������� �ʴ� mnemonic opcode�� ���
					loc += 3;
				}
				else if (strcmp(mne, "RESW") == 0) { // word*3 ��ŭ ����
					loc += 3 * atoi(opr);
				}
				else if (strcmp(mne, "RESB") == 0) { // operand��ŭ ����
					loc += atoi(opr);
				}
				else if (strcmp(mne, "BYTE") == 0) {
					if (strncmp(trim(opr), "X", 1) == 0) {
						loc = loc + ((strlen(trim(opr)) - 3) / 2); // ���� 2���� 1 ����Ʈ�̹Ƿ� ������ �� ����
					}
					if (strncmp(trim(opr), "C", 1) == 0) {
						loc = loc + strlen(trim(opr)) - 3; // ���� ���̸�ŭ ����
					}
				}
				else if (strcmp(mne, "BASE") == 0) { // BASE�� �������� LOC ���� ����
				}
				else if (strncmp(lbl, "RSUB", 4) == 0) {
					loc += 3;
				}
				else {
					printf("invalid operation code in line %d\n", check);
					return 0;
				}
			}
			fprintf(md, "%s", save);
		}
		else break;
	}
	fprintf(md, "%s", save);
	length = loc - stadd;	// ��ü asm�� ���� ����
	fclose(md);


	// pass 2
	int line = 5;
	md = fopen("intermediate.txt", "r");
	asb = strtok(asb, ".");
	FILE* obj = fopen(strcat(asb, ".obj"), "w"); // ������ obj ���� ����
	asb = strtok(asb, ".");
	FILE* lst = fopen(strcat(asb, ".lst"), "w"); // ������ lst ���� ����
	asb = strtok(asb, ".");

	fgets(in, 30, md); // intermediate ������ ù �� �Է�
	strcpy(save, in); // ����� ���� ���� ����
	lbl = strtok(in, " ");
	mne = strtok(NULL, " ");
	opr = strtok(NULL, "\n");
	save = trim(strtok(save, "\n"));
	if (strcmp(mne, "START") == 0) {
		loc = 0;
		fprintf(lst, "%d\t%04X\t%s\n", line, loc, save); // lst ���� ����
	}
	char* header = (char*)malloc(sizeof(char) * 100); // obj ���� header record
	strcpy(header, "H");
	strcat(header, lbl);
	int i = 6 - strlen(lbl);
	while (i > 0) { // ���̿� ���߾� ���α׷��� ���� ����
		strcat(header, " ");
		i--;
	}
	char sloc[20];
	char slen[20];
	sprintf(sloc, "%06X", loc);
	sprintf(slen, "%06X", length);
	strcat(header, sloc);
	strcat(header, slen);
	fprintf(obj, "%s\n", header); // obj ���� ����

	char* text = (char*)malloc(sizeof(char) * 30); // obj ���� T record
	strcpy(text, "");
	char* t = (char*)malloc(sizeof(char) * 69); // obj ���� T record
	char* modi = (char*)malloc(sizeof(char) * 100); // obj ���� M record
	strcpy(modi, "");

	int st = atoi(sloc);
	int opadd = 0;
	int exte = 0;
	int next = 0;
	int base = 0;
	int reser = 0;
	char* objcd = (char*)malloc(sizeof(char) * 20); // object code ��� ����� ������ ���ڿ�
	while (fgets(in, 100, md) && feof(md) == 0) {
		objcd = (char*)malloc(sizeof(char) * 20);
		while (strncmp(in, ".", 1) == 0) { // �ּ��� ���
			line += 5;
			fprintf(lst, "%d\t%s", line, in); // ����Ʈ ���Ͽ� ����
			fgets(in, 100, md); // ������ �Է�
		}
		loc += next; // ���� ���� ũ�⸸ŭ �����Ͽ� ���� loc ���
		next = 0;
		int fm = 0;
		int ni = 3;
		int value = 0;
		int bp = 2;
		int comma = 0;
		int symfound = 0;
		opadd = 0;
		sym* tmpsym = scsym;
		strcpy(save, in);
		lbl = strtok(in, " ");
		mne = strtok(NULL, " ");
		opr = strtok(NULL, "\n");
		save = strtok(save, "\n");
		if (mne == NULL && opr == NULL && (strncmp(lbl, "RSUB", 4) == 0)) { // RSUB�� ��� (lbl, opr �������� ���� ���)
			mne = (char*)malloc(sizeof(char) * 10);
			strcpy(mne, lbl);
			mne = strtok(mne, "\n");
			lbl = " ";
		}
		if (strstr(mne, ",")) { // comma�� ���� ��� ���ڿ� ��ħ
			strcat(mne, opr);
			opr = NULL;
		}
		if (strcmp(lbl, "END") != 0) {
			if (strncmp(lbl, ".", 1) != 0) {
				char fl = lbl[0];
				if (fl >= 48 && fl <= 57) { // label�� ù ���ڰ� ������ ���
					printf("Error occured in line %d\n", line+5);
					err(asb); // err�Լ� ȣ��
					fclose(obj);
					fclose(lst);
					return 0; // ���� �߻� �� 0�� ��ȯ
				}
				if (exte == 1) { // ���� ���� format 4�� ��� loc �߰� ����
					loc++;
					exte = 0;
				}
				if (opr == NULL && strcmp(lbl, " ")!=0) { // label�� ���� ��� ���� �ڸ��� ���Ҵ�
					opr = (char*)malloc(sizeof(char) * 10);
					strcpy(opr, mne);
					strcpy(mne, lbl);
					lbl = NULL;
					opr = strtok(opr, "\n");
				}
				int opfound = 0;
				if (strncmp(mne, "+", 1) == 0) { // extended�� ��� flag ����
					mne = strtok(mne, "+");
					exte = 1;
				}
				int index = hash_function(mne); // �ؽ÷� index �� ����
				hash* htemp;
				htemp = table[index];
				while (htemp->next != NULL) { // ��ġ�ϴ� opcode�� �ִ��� Ȯ��
					if (strcmp(htemp->key, mne) == 0) {
						opfound = 1;
						value = htemp->value; // opcode�� value ����, object code �� ����� ����
						fm = atoi(htemp->format);
						next += fm; // format��ŭ ���� �ٿ� ������ loc ����
						break;
					}
					htemp = htemp->next;
				}
				if (opfound == 1) { // opcode�� OPTAB�� ���� �� ���
					opr = trim(opr);
					if (opr != NULL) {
						if (strncmp(opr, "@", 1) == 0) { // indirect
							opr = strtok(opr, "@");
							ni = 2; // n, i �� ����
						}
						if (strncmp(opr, "#", 1) == 0) { // immediate
							opr = strtok(opr, "#");
							ni = 1;
						}
						if (strstr(opr, ",")) { // comma ���� Ȯ��
							comma = 1;
						}
						tmpsym = scsym; // ���� assemble ������ SYMTAB head�� ����
						while (tmpsym->next != NULL) {
							if (strcmp(tmpsym->lbl, opr) == 0) { // ������ ���
								symfound = 1;
								opadd = tmpsym->loc; // loc address ����
								break;
							}
							tmpsym = tmpsym->next;
						}
						if (symfound == 1) { // SYMTAB�� operand�� ������ ���
							if (opadd - loc >= -2048 && opadd - loc <= 2047) { // ���� ���� ���� ��, 
								bp = 2;
								sprintf(objcd, "%02X%X%03X", value + ni, bp, (opadd - loc - fm) & 0x00000FFF);
							}
							else if (opadd - loc < -2048) { // pc�� �ذ���� �ʾ� base�� ���
								bp = 4;
								sprintf(objcd, "%02X%X%03X", value + ni, bp, opadd - base);
							}
							else { // extended, direct addressing �� ���
								bp = 1;
								sprintf(objcd, "%02X%X%05X", value + ni, bp, opadd);
								int md = loc + 1;
								char* mdf = (char*)malloc(sizeof(char) * 20);
								sprintf(mdf, "M%06X05\n", md); // obj ���� M record
								strcat(modi, mdf); // M record ����
							}
						}
						// ���� SYMTAB�� operand�� ���� ��(register �� ���), format�� comma�� ���� ����
						else if (fm == 1) { 
							sprintf(objcd, "%2X", value);
						}
						else if (fm == 2) {
							if (comma == 0) {
								int opadd = get_reg(opr);
								sprintf(objcd, "%02X%X0", value, opadd);
							}
							else {
								char* opr1 = strtok(opr, ",");
								char* opr2 = strtok(NULL, ",");
								if (opr2 == NULL) { // comma �ڿ� operand�� ���� ���
									printf("Error occured in line %d\n", line + 5);
									err(asb);
									fclose(obj);
									fclose(lst);
									return 0;
								}
								int op1 = get_reg(opr1);
								int op2 = get_reg(opr2);
								if (op1 != 7 && op2 != 7) { // �� operand�� ��� register �� ���
									if ((strcmp(mne, "SHIFTL") != 0) && (strcmp(mne, "SHIFTR") != 0)) { // ����� ���� �� �ִ� ��ɾ�
										sprintf(objcd, "%02X%X%X", value, op1, op2);
									}
									else {
										printf("Error occured in line %d\n", line + 5);
										err(asb);
										fclose(obj);
										fclose(lst);
										return 0;
									}
								}
								else if (op1 != 7 && op2 == 7) {
									if ((strcmp(mne, "SHIFTL") == 0) && (strcmp(mne, "SHIFTR") == 0)) {
										sprintf(objcd, "%02X%X%X", value, op1, atoi(opr2));
									}
									else {
										printf("Error occured in line %d\n", line + 5);
										err(asb);
										fclose(obj);
										fclose(lst);
										return 0;
									}
								}
								else {
									printf("Error occured in line %d\n", line + 5);
									err(asb);
									fclose(obj);
									fclose(lst);
									return 0;
								}
							}
						}
						else if (fm == 3) {
							if (comma == 0) {
								if (ni == 1) {
									if (exte == 1) // extended�� ��� xbpe ��Ʈ�� �� 1 ��
										sprintf(objcd, "%02X1%05X", value + ni, atoi(opr));
									else
										sprintf(objcd, "%02X%04X", value + ni, atoi(opr));
								}
								else if (ni == 2) { 
									if (exte == 1)
										sprintf(objcd, "%02X%X%05X", value + ni, bp, opadd);
									else 
										sprintf(objcd, "%02X%X%05X", value + ni, bp, opadd);
								}
								else {
									printf("Error occured in line %d\n", line + 5);
									err(asb);
									fclose(obj);
									fclose(lst);
									return 0;
								}
							}
							else if (comma == 1) {
								char* opr1 = strtok(opr, ",");
								char* opr2 = strtok(NULL, ",");
								if (opr2 == NULL) {
									printf("Error occured in line %d\n", line + 5);
									err(asb);
									fclose(obj);
									fclose(lst);
									return 0;
								}
								int op1 = get_reg(opr1);
								int op2 = get_reg(opr2);
								if (op1 != 7 && op2 != 7) {
									printf("Error occured in line %d\n", line + 5);
									err(asb);
									fclose(obj);
									fclose(lst);
									return 0;
								}
								else { // comma�� ������ �Ѵ� register�� �ƴ� ���, ��) BUFFER, X
									tmpsym = scsym;
									while (tmpsym->next != NULL) {
										if (strcmp(tmpsym->lbl, opr) == 0) {
											symfound = 1;
											opadd = tmpsym->loc;
											break;
										}
										tmpsym = tmpsym->next;
									}
									bp = 4;
									if (op2 == 1) bp += 8;
									else {}
									sprintf(objcd, "%02X%X%03X", value + ni, bp, (opadd - base) & 0x00000FFF);
								}
							}
							else {
								printf("Error occured in line %d\n", line + 5);
								err(asb);
								fclose(obj);
								fclose(lst);
								return 0;
							}
						}
						else {
							opadd = 0;
							printf("undefined symbol in line %d\n", line + 5);
							err(asb);
							fclose(obj);
							fclose(lst);
							return 0;
						}
					}
					else {
						// RSUB�� ���
						opadd = 0;
						lbl = NULL;
						sprintf(objcd, "%02X%04X", value + ni, 0);
					}
				}
				else if (strcmp(mne, "WORD") == 0) { // WORD �״�� 
					next += 3;
					sprintf(objcd, "%06X", atoi(opr));
				}
				else if (strcmp(mne, "RESW") == 0) { // objcd ����, reser ���� �ٳѱ� flag
					next += 3 * atoi(opr);
					objcd = "";
					reser++;
				}
				else if (strcmp(mne, "RESB") == 0) { // objcd ����, reser ���� �ٳѱ� flag
					next += atoi(opr);
					objcd = "";
					reser++;
				}
				else if (strcmp(mne, "BYTE") == 0) { 
					if (strncmp(trim(opr), "X", 1) == 0) {
						next = next + ((strlen(trim(opr)) - 3) / 2);
						opr = strtok(opr, "'");
						opr = strtok(NULL, "'");
						sprintf(objcd, "%s", opr); // ������ ��� �״��
					}
					if (strncmp(trim(opr), "C", 1) == 0) { // ������ ���
						next = next + strlen(trim(opr)) - 3;
						opr = strtok(opr, "'");
						opr = strtok(NULL, "'");
						char* cc = (char*)malloc(sizeof(char) * strlen(opr));
						objcd = (char*)malloc(sizeof(char) * 20);
						for (int i = 0; i < strlen(opr); i++) { // �� ���ڸ� 16������ ��ȯ�Ͽ� ���ڿ��� ����
							sprintf(cc, "%X", opr[i]);
							if (i == 0) strcpy(objcd, cc);
							else strcat(objcd, cc);
						}
					}
				}
				else if (strcmp(mne, "BASE") == 0) {
					tmpsym = scsym;

					while (tmpsym->next != NULL) { // SYMTAB Ž��
						if (strcmp(tmpsym->lbl, opr) == 0) {
							symfound = 1;
							opadd = tmpsym->loc; // SYMTAB �˻� ��� loc address ����
							break;
						}
						tmpsym = tmpsym->next;
					}
					line += 5;
					objcd = "";
					base = opadd;
					fprintf(lst, "%d\t\t%-20s\t\t%s\n", line, save, objcd);  // lst ���� ����
				}
				else {
					printf("Error occured in line %d\n", line + 5);
					err(asb);
					fclose(obj);
					fclose(lst);
					return 0;
				}
				if (strlen(text) + strlen(objcd) > 60 || (reser == 1 && strcmp(text, "") != 0)) { // obj ���� T record�� 30����Ʈ�� �Ѿ��ų� RESB, RESW ��ɾ �Է� ���� ���
					t = (char*)malloc(sizeof(char)*69);
					int txtlen = strlen(text) / 2;
					sprintf(t, "T%06X%02X%s", st, txtlen, text);
					fprintf(obj, "%s\n", t); // obj ���� ����
					text = (char*)malloc(sizeof(char)*100); // ���ο� T record ����
					strcpy(text, "");
					st = loc;
					reser = 0;
				}
				if ((strcmp(objcd, "") == 0) &&(strcmp(mne,"BASE")!=0)) st = loc + next; // RESB, RESW�� ���� ���� ���� T record�� ���� ��ġ�� �����ؾ� �ϴ� ���
				strcat(text, objcd); // �ִ� ���� �̸��� ��� T record�� objcd ����
			}
			if (strcmp(mne, "BASE") != 0) { // BASE�� ���� lst ����
				line += 5;
				fprintf(lst, "%d\t%04X\t%-20s\t\t%s\n", line, loc, save, objcd); // lst ���� ����
			}
		}
		else break;
	}
	line += 5;
	fprintf(lst, "%d\t\t%-20s\n", line, save); // ������ �� lst ���� ����

	t = (char*)malloc(sizeof(char)*69);
	int txtlen = strlen(text) / 2;
	sprintf(t, "T%06X%02X%s", st, txtlen, text);
	fprintf(obj, "%s\n", t); // obj ���� ������ T record ����
	fprintf(obj, "%s", modi); // obj ���� M record ����
	fprintf(obj, "E%06X\n", atoi(sloc)); // obj ���� E record ����

	hdsym = scsym; // assemble ������ ���� SYMTAB�� hdsym���� ����
	printf("[%s.lst], [%s.obj]\n", asb, asb); // ���� �޽���

	// fclose, free
	fclose(fin);
	fclose(op);
	fclose(md);
	fclose(obj);
	fclose(lst);

	free(header);
	free(text);
	free(t);
	free(modi);
	free(objcd);
	return 1;
}

/*
 *  get_reg �Լ�
 * �Է� ���� �������Ͱ� ������ ��� �׿� �´� �������� ��ȣ ����
 * �ƴ� ��� 7 �������� �Է� ���� ���ڿ��� �������Ͱ� �ƴ��� �˸�
 */
int get_reg(char* reg) { 
	if (strcmp(reg, "A") == 0) return 0; // �������� ��ȣ ����
	else if (strcmp(reg, "X") == 0) return 1;
	else if (strcmp(reg, "L") == 0) return 2;
	else if (strcmp(reg, "PC") == 0) return 8;
	else if (strcmp(reg, "SW") == 0) return 9;
	else if (strcmp(reg, "B") == 0) return 3;
	else if (strcmp(reg, "S") == 0) return 4;
	else if (strcmp(reg, "T") == 0) return 5;
	else if (strcmp(reg, "F") == 0) return 6;
	else return 7; // �����ϴ� �������Ͱ� �ƴ� ���
}

/*
 *  err �Լ�
 * error �߻� �� �����ϰ� �ִ� lst, obj ������ �����ϰ� symtab�� �� ���·� �ʱ�ȭ
 */
void err(char* asb) {
	remove(strcat(asb, ".lst")); // lst ����
	strtok(asb, ".");
	remove(strcat(asb, ".obj")); // obj ����
	strtok(asb, ".");
	scsym = NULL; // ���� ���̴� symtab ����
	return;
}

/*
 *  symbol �Լ�
 * ������ assemble�� ���Ͽ��� ������ symtab�� ���� �� ���
 */
void symbol() {
	sym* tmpsym = hdsym; // symtab�� head�� ����
	if (tmpsym == NULL || hdsym->next==NULL) { // ������ assemble�� ���ų� ��ȿ���� �ʾ� symtab�� �������� ���� ���
		printf("NO SYMBOL TABLE\n");
		return;
	}
	while (tmpsym->next != NULL) { // symtab�� ���� node�� ���� ����
		sym* nxtsym = tmpsym->next;
		while(nxtsym->next !=NULL){ // symtab�� ���� node�� ���� ����
			if (strcmp(tmpsym->lbl, nxtsym->lbl) > 0) { // ���� ���� ���� ��� ���ڿ� ��, �������� ����
				char c[10];
				strcpy(c, tmpsym->lbl); // �ӽ� ����
				int i = tmpsym->loc;
				strcpy(tmpsym->lbl, nxtsym->lbl); // swap
				tmpsym->loc = nxtsym->loc;
				strcpy(nxtsym->lbl, c);
				nxtsym->loc = i;
			}
			nxtsym=nxtsym->next; // ���� ���� �̵�
		}
		tmpsym = tmpsym->next; // ���� ���� �̵�
	}
	tmpsym = hdsym; // ���� �� symtab�� head ��忡 �ٽ� ����
	while (tmpsym->next != NULL) { // symtab�� ���� node�� ���� ����
		if (tmpsym->next->next == NULL) { // symtab�� ������尡 ���� ���
			printf("\t%s\t%04X\n", tmpsym->lbl, tmpsym->loc);
			break; // ���� ���� �̵����� �ʰ� ����
		}
		printf("\t%s\t%04X\n", tmpsym->lbl, tmpsym->loc); // node ��� �� �̵�
		tmpsym = tmpsym->next;
	}
}
