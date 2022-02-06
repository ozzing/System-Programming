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
	CSADDR = PROGADDR;
	hash* htemp;	// ������ ��ġ ���
	hash* hwalk = (hash*)malloc(sizeof(hash));	// ���� ��ġ ���
	head = (node*)malloc(sizeof(node));
	head->next = NULL;
	temp = head;
	walk = (node*)malloc(sizeof(node));
	hdsym = (sym*)malloc(sizeof(sym));
	hdsym->next = NULL;
	hdbp = (bps*)malloc(sizeof(bps));
	hdbp->next = NULL;
	hdes = (es*)malloc(sizeof(es));
	hdes->next = NULL;
	hdrf = (rf*)malloc(sizeof(rf));
	hdrf->next = NULL;
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
		int valid = 0;
		printf("sicsim> ");
		fgets(command, 100, stdin);
		for (int i = 0; i < strlen(command); i++) {	// ���� �����̽��� ���Ҵ�
			if (command[i] == '\t') command[i] = ' ';
		}
		if (strstr(command, "opcode")) {	// ���ڿ� �ȿ� Ư�� ���ڿ��� �ִ��� �˻�
			if (strncmp(trim(command), "opcode ", 7) == 0 || strncmp(trim(command), "opcodelist", 10) == 0) {
				opcode_mnemonic(command);
				valid = 1;
			}
		}
		if (strstr(command, "dump") || strstr(command, "du")) {
			if (strncmp(trim(command), "dump", 4) == 0 || strncmp(trim(command), "du", 2) == 0) { // trim �Լ��� ���� �� �� ���� ����
				dump(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "edit") || strstr(command, "e")) {
			if (strncmp(trim(command), "edit", 4) == 0 || strncmp(trim(command), "e", 1) == 0) {
				edit(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "fill") || strstr(command, "f")) {
			if (strncmp(trim(command), "fill", 4) == 0 || strncmp(trim(command), "f", 1) == 0) {
				fill(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "reset")) {
			if (strcmp(trim(command), "reset") == 0) {
				reset();
				history(trim(command));	// command�� �յ� ���� ���� -> ���� �� �����丮 �Է�
				valid = 1;
			}
		}
		if (strstr(command, "hi") || strstr(command, "history")) {	// hi[story]�� ��쿡�� ���� �����丮 ��ũ�帮��Ʈ�� �Է� �� command ����
			if (strcmp(trim(command), "hi") == 0 || strcmp(trim(command), "history") == 0) {
				history(trim(command));
				commander(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "type")) {
			if (strncmp(trim(command), "type", 4) == 0) {
				type(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "symbol")) {
			if (strcmp(trim(command), "symbol") == 0) {
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
		if (strstr(command, "progaddr")) {
			if (strncmp(trim(command), "progaddr ", 9) == 0) {
				progaddr(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "loader")) {
			if (strncmp(trim(command), "loader ", 7) == 0) {
				loader(trim(command));
				valid = 1;
			}
		}
		if (strstr(command, "bp")) {
			if (strncmp(trim(command), "bp", 2) == 0) {
				bp(trim(command));
				valid = 1;
			}
		}
		if (strcmp(trim(command), "run") == 0) {
			run(trim(command));
			valid = 1;
		}
		if (valid == 0) {	// ���� command�� �ƴ� ���
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
			printf("\t\t%d\t%s\n", ++i, his->data);
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
	char* check;
	if ((strncmp(trim(cmd), "opcode ", 7) == 0)) {
		char* mne = (char*)malloc(sizeof(char) * 20);	// mnemonic ���ڿ�
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
				return;
			}
		}
		char opcd[20] = "opcode ";
		history(strcat(opcd, mne)); // �����丮 ����
		free(find);
	}
	else if (strcmp(trim(cmd), "opcodelist") == 0) {
		check = strtok(cmd, " ");
		if (check) {
			printf("wrong command\n");
			return;
		}
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
	unsigned long start, end, e, temp = addr + 1;
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
		if (start >= temp && start <= e) printf("%02lX ", (unsigned long)mmr[start]);	// ���� temp, e�� command���� �Է� ���� ��ȿ ��� ����
		else printf("   ");
		start += 1;
		if (start % 16 == 0) {	// 16��, �� �� �� ����� �Ϸ�� ���
			printf("; ");
			for (int j = start - 16; j < start; j++) {	// ���� �޸��� 16���� ���� ���ڷ� ���
				if (mmr[j] < 32 || mmr[j]>126 || j<temp || j > end) printf(". ");	// ��ȿ ����
				else if (mmr[j] == 32) printf("  ");	// �����̽�
				else printf("%c ", (char)mmr[j]);
			}
			printf(" \n");
		}
		if (start >= SIZE) {
			if (end > SIZE) printf("out of boundary\n");
			addr = -1;
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
			strcpy(com, trim(edt));	// e[dit] ����
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
					if (start > end) {
						printf("address error\n");
						return;
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
		if ((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102) || c == 32) {}	// 16���� ��ȿ��
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
			printf("%s\n", s);
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
					printf("Error occured in line %d\n", line + 5);
					err(asb); // err�Լ� ȣ��
					fclose(obj);
					fclose(lst);
					return 0; // ���� �߻� �� 0�� ��ȯ
				}
				if (exte == 1) { // ���� ���� format 4�� ��� loc �߰� ����
					loc++;
					exte = 0;
				}
				if (opr == NULL && strcmp(lbl, " ") != 0) { // label�� ���� ��� ���� �ڸ��� ���Ҵ�
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
					t = (char*)malloc(sizeof(char) * 69);
					int txtlen = strlen(text) / 2;
					sprintf(t, "T%06X%02X%s", st, txtlen, text);
					fprintf(obj, "%s\n", t); // obj ���� ����
					text = (char*)malloc(sizeof(char) * 100); // ���ο� T record ����
					strcpy(text, "");
					st = loc;
					reser = 0;
				}
				if ((strcmp(objcd, "") == 0) && (strcmp(mne, "BASE") != 0)) st = loc + next; // RESB, RESW�� ���� ���� ���� T record�� ���� ��ġ�� �����ؾ� �ϴ� ���
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

	t = (char*)malloc(sizeof(char) * 69);
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
	if (tmpsym == NULL || hdsym->next == NULL) { // ������ assemble�� ���ų� ��ȿ���� �ʾ� symtab�� �������� ���� ���
		printf("NO SYMBOL TABLE\n");
		return;
	}
	while (tmpsym->next != NULL) { // symtab�� ���� node�� ���� ����
		sym* nxtsym = tmpsym->next;
		while (nxtsym->next != NULL) { // symtab�� ���� node�� ���� ����
			if (strcmp(tmpsym->lbl, nxtsym->lbl) > 0) { // ���� ���� ���� ��� ���ڿ� ��, �������� ����
				char c[10];
				strcpy(c, tmpsym->lbl); // �ӽ� ����
				int i = tmpsym->loc;
				strcpy(tmpsym->lbl, nxtsym->lbl); // swap
				tmpsym->loc = nxtsym->loc;
				strcpy(nxtsym->lbl, c);
				nxtsym->loc = i;
			}
			nxtsym = nxtsym->next; // ���� ���� �̵�
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

/*
 * progaddr �Լ�
 * �Է� ���� program�� ���� �ּҸ� �����ϴ� �Լ�
 */
void progaddr(char* command) {
	char* cmd = (char*)malloc(sizeof(char) * 30);
	char* prgadd = (char*)malloc(sizeof(char) * 10);
	prgadd = strtok(command, " ");
	strcpy(cmd, prgadd); // ��ɾ� ����
	prgadd = strtok(NULL, " ");
	strcat(cmd, " ");
	strcat(cmd, trim(prgadd));
	if (prgadd == NULL) {
		printf("wrong command\n");
		return;
	}
	int check = cvalid(prgadd); // ��ȿ�� 16���� �������� Ȯ��
	if (check != 0) {
		printf("wrong address\n");
		return;
	}
	unsigned long prgaddr = hex_to_dec(prgadd);
	prgadd = strtok(NULL, " "); // �ڿ� ��ɾ �� �ִ��� Ȯ��
	if (prgadd != NULL) {
		printf("wrong command\n");
		return;
	}
	if (prgaddr > SIZE || prgaddr < 0) { // ���� ���� ������ Ȯ��
		printf("program address, out of range\n");
	}
	else {
		PROGADDR = prgaddr; // ������ ��� �ּҸ� �����ϱ� ����
		history(cmd); // �����丮 ����
	}
}

/*
 * loader �Լ�
 * �Է� ���� ���ڿ��� obj ���Ͽ� ���Ͽ� pass1, pass2�� ���� linking �۾� ���� �� ���� �޸� ���
 * pass1 : OS�κ��� ���� PROGADDR�� H, D records�� ���� CSLTH�� ���� ESTAB ����
 * pass2 : ESTAB�� ������� T, M records�� �о� ���� �޸𸮿� ��� ���� ���
 */
void loader(char* command) { // history ���� ������
	hdes = (es*)malloc(sizeof(es));
	hdes->next = NULL;

	// pass 1
	int filenum = 0;
	char* cmd = (char*)malloc(sizeof(char) * 30);
	char* ld = (char*)malloc(sizeof(char) * 10);
	char* filea = (char*)malloc(sizeof(char) * 30);
	char* fileb = (char*)malloc(sizeof(char) * 30);
	char* filec = (char*)malloc(sizeof(char) * 30);
	ld = strtok(command, " ");
	strcpy(cmd, ld);	// loader �Է�
	ld = strtok(NULL, " ");
	strcat(cmd, " ");
	strcat(cmd, trim(ld));	// ù��° ���ϸ� �Է�
	if (ld == NULL) {
		printf("wrong command\n");
		return;
	}
	filenum++;
	strcpy(filea, ld);
	ld = strtok(NULL, " ");
	if (ld != NULL) {
		strcat(cmd, " ");
		strcat(cmd, trim(ld));	// �ι�° ���ϸ� �Է�
		filenum++;
		strcpy(fileb, ld);
	}
	ld = strtok(NULL, " ");
	if (ld != NULL) {
		strcat(cmd, " ");
		strcat(cmd, trim(ld));	// ����° ���ϸ� �Է�
		filenum++;
		strcpy(filec, ld);
	}
	char* in = (char*)malloc(sizeof(char) * 100);
	char* name = (char*)malloc(sizeof(char) * 10);
	char* addrs = (char*)malloc(sizeof(char) * 60);
	char* code = (char*)malloc(sizeof(char) * 60);
	char* fname = (char*)malloc(sizeof(char) * 60);
	unsigned long lth = 0;
	for (int i = 0; i < filenum; i++) { // ���� ������ŭ
		if (i == 0) strcpy(fname, filea);	// ���ϸ� ����
		else if (i == 1) strcpy(fname, fileb);
		else if (i == 2)strcpy(fname, filec);
		else {	// ������ �Է� ���� ���� ���
			printf("error occured\n");
			return;
		}
		FILE* fin = fopen(fname, "r");
		while (feof(fin) == 0) {
			fgets(in, 100, fin);
			if (strncmp(in, "E", 1) == 0) break;	// E ���ڵ带 ������ ����
			if (strncmp(in, "H", 1) == 0) {	// H ���ڵ带 ���� ���
				sscanf(in, "%s%6s%6s", name, addrs, code);
				if ((cvalid(code) == 0)) {//&&( cvalid(addrs) == 0)) {
					lth = hex_to_dec(code);	// ���α׷� length ���ڿ� ���ڷ� ��ȯ
				}
				else {
					//error
				}
				es* hd = hdes;	// ESTAB �޾� ����
				es* nxt = (es*)malloc(sizeof(es));	// ���� ���
				nxt->next = NULL;
				if (hd->next == NULL) {	// ESTAB�� �� ���
					hd->next = nxt;	// �� ��� ����
					name = name + 1;	// ���ڿ����� 'H' ����
					strcpy(hd->cs, name);	// ���α׷� �̸� ����
					strcpy(hd->name, "\t");	// symbol name�� ���� ��� \t (���� ����)
					hd->ADDR = hex_to_dec(addrs) + PROGADDR;	// OS���� �־����� PROGADDR�� ���α׷� �ּҸ� ����
					CSADDR = hd->ADDR;	// Control Section ���� �ּҰ� ����
					hd->lth = lth;
				}
				else {
					while (hd->next != NULL) {
						if (strcmp(hd->name, fname) == 0) { // ESTAB�� �̹� �����ϴ� ���
							printf("duplicate external symbol\n");
							return;
						}
						hd = hd->next;
					}
					hd->next = nxt;
					name = name + 1;
					strcpy(hd->cs, name);
					strcpy(hd->name, "\t");
					hd->ADDR = CSADDR;
					hd->lth = lth;
				}
			}
			else if (strncmp(in, "D", 1) == 0) {
				es* hd = hdes;
				char* symbol = (char*)malloc(sizeof(char) * 6);
				unsigned long ADDR = -1;
				char* reuse = (char*)malloc(sizeof(char) * 60);
				in = in + 1;
				for (int i = 0; i < 6; i++) {
					es* nxt = (es*)malloc(sizeof(es));	// ������ ���
					nxt->next = NULL;
					if (strcmp(in, "\n") == 0) break;
					sscanf(in, "%6s%6lx%s", symbol, &ADDR, reuse);	// �޺κ��� �ٸ� symbol ������ ���� reuse�� �ӽ� ���� 
					in = in + 12;	// ���� symbol�� �̵�
					if (ADDR != -1 && symbol != NULL) {
						while (hd->next != NULL) {
							if (strcmp(hd->name, symbol) == 0) {	// �ߺ��� ���
								printf("duplicate external symbol %s\n", symbol);
								return;
							}
							hd = hd->next;
						}
						// ���� ��忡 ����
						hd->next = nxt;
						strcpy(hd->cs, "\t");
						strcpy(hd->name, symbol);
						hd->ADDR = CSADDR + ADDR;
						hd->lth = -1;
					}
				}
			}
			else {
				// printf("do nothing\n");
			}
		}
		CSADDR += lth;	// ���� cs ���̸�ŭ �����־� ���� cs ������ ����
		fclose(fin);
	}
	history(cmd);

	ld = strtok(NULL, " "); // ����° ���ϸ� ���Ŀ��� ���ڰ� ������ ���
	if (ld != NULL) {
		printf("wrong command\n");
		return;
	}
	// ��� ���
	printf("control symbol address length\n");
	printf("section name\n");
	printf("--------------------------------\n");
	es* temp = hdes;	// ESTAB �޾ƿ���
	while (temp != NULL) {
		if (temp->lth != -1) printf("%s%s\t%04lX\t%04lX\n", temp->cs, temp->name, temp->ADDR, temp->lth);	// ���Ŀ� �°� ���
		else printf("%s%6s\t%04lX\n", temp->cs, temp->name, temp->ADDR);
		temp = temp->next;
		if (temp->next == NULL) break;
	}
	printf("--------------------------------\n");
	printf("\ttotal\tlength\t%04lX\n", CSADDR - PROGADDR);	// ���α׷� ���� ���



	// pass 2
	CSADDR = PROGADDR;
	EXECADDR = PROGADDR;
	for (int i = 0; i < filenum; i++) {
		if (i == 0) strcpy(fname, filea);
		else if (i == 1) strcpy(fname, fileb);
		else if (i == 2)strcpy(fname, filec);
		else {
			printf("error occured\n");
			return;
		}
		FILE* fin = fopen(fname, "r");
		while (feof(fin) == 0) {
			fgets(in, 100, fin);
			if (strncmp(in, "E", 1) == 0) {}
			if (strncmp(in, "H", 1) == 0) {
				hdrf = (rf*)malloc(sizeof(rf));
				sscanf(in, "%s%6s%6s", name, addrs, code);
				name = name + 1;
				hdrf->num = 1;
				strcpy(hdrf->symbol, name);	// ���α׷� �̸� ����
				rf* nxt = (rf*)malloc(sizeof(rf));
				nxt->next = NULL;
				hdrf->next = nxt;
				if ((cvalid(code) == 0)) { //&&( cvalid(addrs) == 0)) { -> ��ȿ�� �Է¸� ���´ٰ� ����, ����
					lth = hex_to_dec(code);	// ���α׷� ���� ����
				}
				else {
					//error
				}
			}
			if (strncmp(in, "R", 1) == 0) {
				rf* hd = hdrf;	// ���α׷��� reference list
				char* symbol = (char*)malloc(sizeof(char) * 6);
				unsigned long ADDR = -1;
				char* reuse = (char*)malloc(sizeof(char) * 60);
				in = in + 1;	// ���ڿ����� 'R' ����
				int j = strlen(in);
				if (j % 8 != 0) j += (j % 8);
				for (int i = 0; i < j / 8; i++) {
					rf* nxt = (rf*)malloc(sizeof(rf));
					nxt->next = NULL;
					sscanf(in, "%2lx%6s%s", &ADDR, symbol, reuse);	// �޺κ��� �ٸ� symbol ������ ���� reuse�� �ӽ� ���� 
					in = in + 8;
					if (ADDR != -1 && symbol != NULL) {
						while (hd->next != NULL) {
							if (strcmp(hd->symbol, symbol) == 0) {	// �ߺ�
								printf("duplicate refer symbol %s : %s\n", symbol, symbol);
								return;
							}
							hd = hd->next;
						}
						hd->next = nxt;
						strcpy(hd->symbol, symbol);	// rf list�� ����
						hd->num = ADDR;
					}
				}
			}
			if (strncmp(in, "T", 1) == 0) {
				unsigned long mnum = 0;
				char* obadd = (char*)malloc(sizeof(char) * 6);
				char* oblen = (char*)malloc(sizeof(char) * 2);
				char* cd = (char*)malloc(sizeof(char) * 2);
				in = in + 1;
				char* reuse = (char*)malloc(sizeof(char) * 60);
				sscanf(in, "%6s%2s%s", obadd, oblen, reuse);
				mnum = hex_to_dec(obadd) + CSADDR;	// cs�� �ּҰ��� ���Ͽ� ���α׷��� �ε�� ��ġ ã��
				while (strlen(reuse) >= 2) {
					strncpy(cd, reuse, 2);	// �޺κ��� �ٸ� symbol ������ ���� reuse�� �ӽ� ���� 
					reuse = reuse + 2;	// ��ĭ (1 byte) �� �ڸ��� �̵�
					mmr[mnum] = hex_to_dec(cd);	// ���� �޸��� �� ����
					mnum += 1;	// ���� �޸� ���� index�� �̵�
				}
			}
			else if (strncmp(in, "M", 1) == 0) {
				rf* hd = hdrf;	// ���α׷��� reference list
				rf* nxt = (rf*)malloc(sizeof(rf));
				nxt->next = NULL;
				int found = 0;
				char* check = (char*)malloc(sizeof(char) * 100);
				strcpy(check, in);
				check = check + 10;	// modification�� ���� ���� ��ȣ�� reference list�� �����ϴ� ��ȿ�� ��ȣ���� Ȯ��
				int rfnum = hex_to_dec(check);
				char* refer = (char*)malloc(sizeof(char) * 6);
				while (hd->next != NULL) {
					if (hd->num == rfnum) {
						found = 1;
						strcpy(refer, hd->symbol);	// ��ȣ�� CS �̸��� refer ���ڿ��� ����
						break;
					}
					hd = hd->next;
				}
				if (found == 1 || rfnum==1) {
					unsigned long mnum = 0;
					int value = -1;
					char* obadd = (char*)malloc(sizeof(char) * 6);
					char* oblen = (char*)malloc(sizeof(char) * 2);
					char* sign = (char*)malloc(sizeof(char));
					char* cd = (char*)malloc(sizeof(char) * 6);

					in = in + 1;	// ���ڿ����� 'M' ����
					sscanf(in, "%6s%2s%1s%2s", obadd, oblen, sign, cd);
					mnum = hex_to_dec(obadd) + CSADDR;
					int oblth = hex_to_dec(oblen);	// modification ���� ����
					if (oblth % 2 != 0) oblth += 1;	// half-byte �������� �Ǿ��ֱ� ������ byte�� ���߾� �缳��
					oblth /= 2;

					char* six = (char*)malloc(sizeof(char) * 6);
					char* temp = (char*)malloc(sizeof(char) * 2);

					es* hd = hdes;
					while (hd->next != NULL) {
						if (strcmp(hd->name, refer) == 0) {
							value = hd->ADDR;	// ESTAB���� reference�� �ּҰ� ����
						}
						hd = hd->next;
					}
					if (rfnum == 1) value = CSADDR;	// 1�� ��� ���� control section�� �ּҰ� ����
					if (strcmp(sign, "+") == 0) {	// ��ȣ�� ���� ����
						sprintf(six, "%06X", value);
						int ini = oblth;
						while (oblth > 0) {	// ���̸�ŭ ���ڸ�(1����Ʈ)�� ���
							strncpy(temp, six, 2);
							value = hex_to_dec(temp);
							six = six + 2;
							if (mmr[mnum] + value > 0xFF && oblth != ini) {
								int no = 0;
								for (int o = 1; o <= ini - oblth; o++) {
									if (mmr[mnum - o] == 0xFF) mmr[mnum - o] = 0x00; // mnum ���� ǥ�� ���� �ʰ�, ���ڸ��� �ݺ������� ������ ��ġ�� ���
									else {
										no = 1; break;
									}
								}
								if (no == 1) mmr[mnum - 1]++;	// �׷��� ���� ��� �ٷ� ���ڸ���
							}
							mmr[mnum] += value;	// ���� �ڸ��� value�� ���� ����
							oblth -= 1;	// ���� ����
							mnum++;	// ���� �޸� �ּҷ� �̵�
						}
					}
					else if (strcmp(sign, "-") == 0) {
						sprintf(six, "%06X", value);
						int ini = oblth;
						while (oblth > 0) {
							strncpy(temp, six, 2);
							value = hex_to_dec(temp);
							six = six + 2;
							if (mmr[mnum] - value < 0 && oblth != ini) {
								int no = 0;
								for (int o = 1; o <= ini - oblth; o++) {
									if (mmr[mnum - o] == 0x00) mmr[mnum - o] = 0xFF;  // mnum ���� ǥ�� ���� �ʰ�, ���ڸ��� ������ ��ġ�� ���
									else {
										no = 1; break;
									}
								}
								if (no == 1) mmr[mnum - 1]--;	// �׷��� ���� ��� �ٷ� ���ڸ���
							}
							mmr[mnum] -= value;	// ���� �ڸ��� value�� ���� ����
							oblth -= 1;
							mnum++;
						}
					}
					mnum = hex_to_dec(obadd) + CSADDR;
				}
				else {
					// do nothing
					return;
				}
				
			}
		}
		CSADDR += lth;	// ���� CS�� �����ּҸ� �����ϱ� ���� ���� CS ���̸� ����
	}
	printf("\n");
	EXECEND = CSADDR;	// run�� ���� ���α׷� ������ ����
	RUNSTART = PROGADDR;	// run�� ���� ���α׷� ���� ���� ����
	for (int i = 0; i < 10; i++) regi[i] = 0;	// loader ��ɾ� ����� register �ʱ�ȭ
	regi[2] = EXECEND - EXECADDR;	// regi[L]�� ���α׷� ���̷� �ʱ�ȭ
//	hdbp = (bps*)malloc(sizeof(bps));
//	hdbp->next = NULL; // ���ο� loader ��� ���� bp�� reset �ؾ��� ��
}

/*
 * bp(debug) �Լ�
 * run ��ɾ ������ �� ����� break point���� �����ϴ� bp list�� ���� �� ����
 * bp - ���, bp addr - addr �� ����Ʈ�� ����, bp clear ����Ʈ �ʱ�ȭ
 */
void bp(char* command) {
	bps* hd = hdbp;
	char* cmd = (char*)malloc(sizeof(char) * 30);
	char* dbg = (char*)malloc(sizeof(char) * 10);
	dbg = strtok(command, " ");	// bp ����
	strcpy(cmd, dbg);
	if (strcmp(cmd, "bp") != 0) {	// bp�ڿ� ������� �ٸ� ���� ���� ���
		printf("wrong comand\n");
		return;
	}
	dbg = strtok(NULL, " ");
	if (dbg == NULL) { // bp �� ��
		printf("\t\tbreakpoint\n\t\t----------\n");
		while (hd->next != NULL) {
			printf("\t\t%X\n", hd->point);	// bp list ���
			hd = hd->next;
		}
	}
	else {
		strcat(cmd, " ");
		strcat(cmd, trim(dbg));
		if (cvalid(dbg) == 0) { // bp [address] �� ��
			unsigned long dbgaddr = hex_to_dec(dbg);	// address �� 16������ ��ȯ
		/*	if (EXECADDR == -1 || EXECEND == -1) {	// ������ ���� ���
				printf("no file loaded yet\n");
				return;
			}
			if (dbgaddr < EXECADDR || dbgaddr >= EXECEND) {	// ���� ���� ���� bp�� ���� ��� -> ����ó�� �ʿ� ����
				printf("debug address, out of range\n");
				return;
			}	*/
			// bp�� ����
			bps* nxt = (bps*)malloc(sizeof(bps));
			nxt->next = NULL;
			if (hd->next == NULL) {	// bp list�� ����� ���
				hd->next = nxt;
				hd->point = dbgaddr;
			}
			else {
				while (hd->next != NULL) {
					if (hd->point == dbgaddr) {	// �ߺ� ���� ���� ���
						printf("breakpoint already exist\n");
						return;
					}
					hd = hd->next;
				}
				// bp ����
				hd->next = nxt;
				hd->point = dbgaddr;
			}
			printf("\t\t[\x1b[32mok\x1b[0m] create breakpoint %s\n", trim(dbg));
		}
		else {
			if (strcmp(trim(dbg), "clear") == 0) { // bp clear �� ��
				printf("\t\t[\x1b[32mok\x1b[0m] clear all breakpoints\n");
				hdbp = (bps*)malloc(sizeof(bps));	// bp list �ʱ�ȭ
				hdbp->next = NULL;
			}
			else {
				printf("wrong command\n");
				return;
			}
		}
		dbg = strtok(NULL, " ");	// �̿ܿ� �ٸ� ���� �߰��� �Էµ� ���
		if (dbg != NULL) {
			printf("wrong command\n");
			return;
		}
	}
	history(cmd);
}

/*
 * run �Լ�
 * ���� �޸𸮿� load �� ���α׷��� �����ϰ�, ���������� ���� ���
 * object code �ؼ� ��, opcode ���� �ش��ϴ� ����� ����
 */
void run(char* command) {
	int A = 0, X = 1, L = 2, PC = 8, SW = 9, B = 3, S = 4, T = 5, F = 6;	// �������� ��ȣ ����
	regi[F]=0;
	unsigned long opni = 0, xbpe = 0, code = 0, rgst = 0;
	char* obj = (char*)malloc(sizeof(char) * 5);
	char* mne = (char*)malloc(sizeof(char) * 6);
	int pc = RUNSTART;	// pc�� ���� ������� ���� pc ������ �ʱ�ȭ
	regi[PC] = pc;	// ���� pc ���� register�� ����
	unsigned long ta = 0;	// target address �ʱ�ȭ
	RUNEND = EXECEND;	// ���α׷� �� ����
	int rd = 0;	// rd �ʱ�ȭ
	while (pc < RUNEND) {
		int op = 0, n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
		int reg1 = 7, reg2 = 7;
		// opcode(ni) ���ϱ�
		opni = (unsigned long)mmr[pc++];
		int ni = opni % 4;	// �� half-byte ���
		op = opni - ni;	// �� half-byte ���
		// n, i �� ����
		if (ni == 3) { n = 1; i = 1; }
		else if (ni == 2) n = 1;
		else if (ni == 1) i = 1;

		// format ���ϱ�
		hash* htemp;
		int fm = 0;
		for (int index = 0; index < 20; index++) {
			htemp = table[index];
			while (htemp->next != NULL) { // ��ġ�ϴ� opcode�� �ִ��� Ȯ��
				if (htemp->value == op) {
					fm = atoi(htemp->format);
					strcpy(mne, htemp->key);
					break;
				}
				htemp = htemp->next;
			}
			if (fm != 0) break;	// ��ġ�ϴ� opcode�� �߰ߵ� ���
		}

		// reg ���ϱ�
		if (fm == 2) {
			// �Է� ���� �������� ���� �� half-byte�� �� half-byte�� ���� reg1, reg2�� �Ҵ�
			rgst = (unsigned long)mmr[pc++];
			reg1 = rgst / 16;
			reg2 = rgst % 16;
		}

		// ���� format ���� ��� ����

		if (fm == 1) pc++;	// format1 �ΰ�� pc ���� ���
		else if (fm == 2) {
			// format 2�� ���, register��� ��� ����

			// ADDR r1, r2 
			if (op == 0x90) regi[reg2] += regi[reg1];
			// CLEAR r1
			// ---------------------------required----------------------------
			else if (op == 0xB4) regi[reg1] = 0;
			// COMPR r1, r2
			// ---------------------------required----------------------------
			else if (op == 0xA0) {
				// ���Ͽ� regi[SW] �� ��
				if (regi[reg1] == regi[reg2]) regi[SW] = 0;
				else if (regi[reg1] > regi[reg2]) regi[SW] = 1;
				else regi[SW] = -1;
				if (rd == 1) {	// ������ RD ������� ���� CC�� =�� �µǾ�� �ϴ� ���
					regi[SW] = 0;
					rd = 0;	// �ʱ�ȭ
				}
			}
			// DIVR r1, r2
			else if (op == 0x9C) regi[reg2] -= regi[reg1];
			// MULR r1, r2
			else if (op == 0x98) regi[reg2] *= regi[reg1];
			// RMO r1, r2
			else if (op == 0xAC) regi[reg2] = regi[reg1];
			// SHIFTL r1, n
			else if (op == 0xA4) {}
			// SHIFTR r1, n
			else if (op == 0xA8) {}
			// SUBR r1, r2
			else if (op == 0x94) {}
			// SVC n
			else if (op == 0xB0) {}
			// TIXR r1
			// ---------------------------required----------------------------
			else if (op == 0xB8) { // regi[X] �� ���� �� �Է� ���� ��ȣ�� �ش��ϴ� register�� �� ��, ��� CC ������Ʈ
				regi[X]++;
				if (regi[X] == regi[reg1]) regi[SW] = 0;
				else if (regi[X] > regi[reg1]) regi[SW] = 1;
				else regi[SW] = -1;
			}
		}
		else if (fm == 3) {
			// xbpe ���ϱ�
			char* pre = (char*)malloc(sizeof(char) * 10);
			xbpe = (unsigned long)mmr[pc++]; // opcode ���� ���� �޸� ��
			unsigned long ori = xbpe;
			code = xbpe % 16; // xbpe�� �ƴ� disp �Ǵ� address�� half-byte
			sprintf(obj, "%lX", code);
			xbpe /= 16;	// xbpe half-byte �и�
			// x,b,p,e �� ���
			e = xbpe % 2;
			xbpe /= 2;
			p = xbpe % 2;
			xbpe /= 2;
			b = xbpe % 2;
			xbpe /= 2;
			x = xbpe % 2;
			code = (unsigned long)mmr[pc++];	// disp �Ǵ� address �� ���� �޸� ��
			strcpy(pre, obj);
			sprintf(obj, "%s%02lX", pre, code);	// object code ���ڿ��� ����
			if (e == 1) {	// extended�� ���
				code = (unsigned long)mmr[pc++];	// address ���� �޸� ��
				strcpy(pre, obj);
				sprintf(obj, "%s%02lX", pre, code);	// object code�� ���ڿ��� ����
				fm = 4;	// format �缳��
			}
			if (n == 0 && i == 1) {	// immediate�� ���
				if (e != 1) sprintf(obj, "%01lX%02lX", ori % 16, code);
				else {
					strcpy(pre, obj);
					sprintf(obj, "%01lX%s", ori % 16, pre);
				}
			}
			if (x == 1) { // indexed
				ta = regi[X];
			}
			// if inside the range
			if (b == 1 && p == 0) { // base
				ta = regi[B] + hex_to_dec(obj);	// base + disp
				sprintf(obj, "%05lX", ta);
			}
			else if (b == 0 && p == 1) { // pc
				ta = pc + hex_to_dec(obj);	// pc + disp
				if (e != 1) {
					int check = (ori % 16) * 256 + code;
					if (check > 0x800) {	// 0x800~0x7FF ���� ���� ���
						ta -= 0x1000;
					}
				}
				sprintf(obj, "%05lX", ta);
			}
			else {
				ta = hex_to_dec(obj);
			}

			ta = ta & 0xFFFFF;

			unsigned long mm = ta;	// mmr index
			unsigned long ind = ta;	// indirect mmr index
			if (n == 1 && i == 1) { // simple 
				mm = ta;
				char* load = (char*)malloc(sizeof(char) * 8);
				sprintf(load, "%02X%02X%02X", mmr[ta], mmr[ta + 1], mmr[ta + 2]);	// �޸��� 3 ����Ʈ ���� �������� ����
				ind = hex_to_dec(load);
			}
			else if (n == 1 && i == 0) { // indirect
				char* load = (char*)malloc(sizeof(char) * 8);
				sprintf(load, "%02X%02X%02X", mmr[ta], mmr[ta + 1], mmr[ta + 2]);	// indirect target adress ��
				ind = hex_to_dec(load);
				sprintf(load, "%02X%02X%02X", mmr[ind], mmr[ind + 1], mmr[ind + 2]);	// target address ��
				mm = hex_to_dec(load);
			}
			else if (n == 0 && i == 1) { // immediate
				mm = hex_to_dec(obj);	// ���� �״�� ���
			}

			// ���� format 3/4�� opcode �� ��� ����
			// copy.obj�� �ʿ��� ��ɸ� �����Ͽ���

			// ADD m
			if (op == 0x18) {}
			// ADDF m
			else if (op == 0x58) {}
			// AND m
			else if (op == 0x40) {}
			// COMP m
			// ---------------------------required----------------------------
			else if (op == 0x28) {
				if (regi[A] == mm) regi[SW] = 0;
				else if (regi[A] > mm) regi[SW] = 1;
				else regi[SW] = -1;
			}
			// COMPF m
			else if (op == 0x88) {}
			// DIV m
			else if (op == 0x24) {}
			// DIVF m
			else if (op == 0x64) {}
			// J m
			// ---------------------------required----------------------------
			else if (op == 0x3C) {
				if (n == 1 && i == 0) {	// indirect�� ���
					pc = ind;
				}
				else pc = mm;
			}
			// JEQ m
			// ---------------------------required----------------------------
			else if (op == 0x30) {
				if (regi[SW] == 0) pc = mm;
			}
			// JGT m
			else if (op == 0x34) {}
			// JLT m
			// ---------------------------required----------------------------
			else if (op == 0x38) {
				if (regi[SW] == -1) pc = mm;
			}
			// JSUB m
			// ---------------------------required----------------------------
			else if (op == 0x48) {
				regi[L] = pc;
				pc = mm;
			}
			// LDA m
			// ---------------------------required----------------------------
			else if (op == 0x00) {
				if (n == 1 && i == 1) regi[A] = ind;
				else regi[A] = mm;

			}
			// LDB m
			// ---------------------------required----------------------------
			else if (op == 0x68) {
				if (n == 1 && i == 1) regi[B] = ind;
				else regi[B] = mm;
			}
			// LDCH m
			// ---------------------------required----------------------------
			else if (op == 0x50) {
				regi[A] = (unsigned long)mmr[mm + 2];
			}
			// LDF m
			else if (op == 0x70) {}
			// LDL m
			else if (op == 0x08) {}
			// LDS m
			else if (op == 0x6C) {}
			// LDT m
			// ---------------------------required----------------------------
			else if (op == 0x74) {
				if (n == 1 && i == 1) regi[T] = ind;
				else regi[T] = mm;
			}
			// LDX m
			else if (op == 0x04) {}
			// LPS m
			else if (op == 0xD0) {}
			// MUL m
			else if (op == 0x20) {}
			// MULF m
			else if (op == 0x60) {}
			// OR m
			else if (op == 0x44) {}
			// RD m
			// ---------------------------required----------------------------
			else if (op == 0xD8) {
				rd = 1;
			}
			// RSUB
			// ---------------------------required----------------------------
			else if (op == 0x4C) {
				pc = regi[L];
			}
			// SSK m
			else if (op == 0xEC) {}
			// STA m
			// ---------------------------required----------------------------
			else if (op == 0x0C) {
				// ���� �޸� 3���� byte�� register �� ������ ���� ���� �ڸ��� ������ ������ ����
				mmr[mm] = regi[A] / 65536;
				mmr[mm + 1] = regi[A] / 256;
				mmr[mm + 2] = regi[A] % 256;
			}
			// STB m
			else if (op == 0x78) {}
			// STCH m
			// ---------------------------required----------------------------
			else if (op == 0x54) {
				mmr[mm] = regi[A] % 256;
			}
			// STF m
			else if (op == 0x80) {}
			// STI m
			else if (op == 0xD4) {}
			// STL m
			// ---------------------------required----------------------------
			else if (op == 0x14) {
				mmr[mm] = regi[L] / 65536;
				mmr[mm + 1] = regi[L] / 256;
				mmr[mm + 2] = regi[L] % 256;
			}
			// STS m
			else if (op == 0x7C) {}
			// STSW m
			else if (op == 0xE8) {}
			// STT m
			else if (op == 0x84) {}
			// STX m
			// ---------------------------required----------------------------
			else if (op == 0x10) {
				mmr[mm] = regi[X] / 65536;
				mmr[mm + 1] = regi[X] / 256;
				mmr[mm + 2] = regi[X] % 256;
			}
			// SUB m
			else if (op == 0x1C) {}
			// SUBF m
			else if (op == 0x5C) {}
			// TD m
			// ---------------------------required----------------------------
			else if (op == 0xE0) {
				regi[SW] = -1;
			}
			// TIX m
			else if (op == 0x2C) {}
			// WD m
			// ---------------------------required----------------------------
			else if (op == 0xDC) {
			}
		}
		else {
			if (strcmp(mne, "BASE") == 0) {

			}
			if (strcmp(mne, "BYTE") == 0) {

			}
			if (strcmp(mne, "RESW") == 0) {
				pc += 3;
			}
			if (strcmp(mne, "RESB") == 0) {
				pc += 1;
			}

		}
		regi[PC] = pc;	// ���� pc ��(������ ���� + 1) register�� �Է�
		if (break_point(pc) != -1) break;	// pc�� breakpoint�� ������ ���
	}
	regi[PC] = pc;	// ������ pc ���� register�� �Է�
	// ���Ŀ� �°� ���
	printf("A : %06lX  X : %06lX\n", regi[A], regi[X]);
	printf("L : %06lX PC : %06lX\n", regi[L], regi[PC]);
	printf("B : %06lX  S : %06lX\n", regi[B], regi[S]);
	printf("T : %06lX\n", regi[T]);
	if (pc == EXECEND) {	// pc�� ���α׷��� ���� ������ ���
		pc = PROGADDR;
		for (int i = 0; i < 10; i++) regi[i] = 0;
		regi[L] = EXECEND - EXECADDR;
		printf("\t\tEnd Program\n");
	}	
	else printf("\t\tStop at checkpoint[%X]\n", break_point(pc));	// pc�� breakpoint�� ������ ���α׷� ������ ���� ���
	RUNSTART = pc;	// ���� ���� �ּ� ����
	history(command);
}

/*
 * break_point �Լ�
 * �Է� ���� bp linked list�� ���� ��ġ�ϸ�
 * bp list�� �ش� �� ����, �ƴ� ��� -1 ����
 */
int break_point(unsigned long pc) {
	bps* hd = hdbp;		// bp linked list �޾ƿ���
	while (hd->next != NULL) {
		if (hd->point == pc) { // ��ġ�� ���
			return hd->point;
		}
		hd = hd->next; // ���� ���� �̵�
	}
	return -1; // ��ġ���� ���� ���
}
