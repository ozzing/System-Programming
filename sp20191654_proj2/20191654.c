/* 포함되는 파일 */
#include "20191654.h"

/*
 *  메인 함수
 * 1. 데이터를 입력 받아 hash table 구축
 * 2. while문을 통해 command를 입력 받고 알맞은 함수 호출
 */
int main() {
	mmr = (unsigned char*)malloc(sizeof(unsigned char) * (SIZE));
	for (int i = 0; i < 20; i++) {	// hash table 초기화
		table[i] = (hash*)malloc(sizeof(hash));
		table[i]->next = NULL;
	}
	hash* htemp;	// 저장할 위치 노드
	hash* hwalk = (hash*)malloc(sizeof(hash));	// 다음 위치 노드
	head = (node*)malloc(sizeof(node));
	head->next = NULL;
	temp = head;
	walk = (node*)malloc(sizeof(node));
	hdsym = (sym*)malloc(sizeof(sym));
	hdsym->next = NULL;
	char* command = (char*)malloc(sizeof(char) * 100);
	FILE* fin = fopen("opcode.txt", "r");	// 파일 입력
	while (1) {
		int num;
		char op[10], fm[10];
		fscanf(fin, "%x %s %s", &num, op, fm);	// 16진수 입력
		if (feof(fin) != 0) break;	// 파일에 끝에 도달할 경우
		int index = hash_function(op);	// hash 함수로 얻은 hash 값을 index로
		htemp = table[index];
		while (1) {
			if (htemp->next == NULL) {	// 링크드리스트의 끝으로 이동, 값 입력
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
	while (1) {	// command 입력 받고 알맞은 함수 호출
		int valid=0;
		printf("sicsim> ");
		fgets(command, 100, stdin);
		for (int i = 0; i < strlen(command); i++) {	// 탭을 스페이스로 재할당
			if (command[i] == '\t') command[i] = ' ';
		}
		if (strstr(command, "opcode")) {	// 문자열 안에 특정 문자열이 있는지 검사
			if (strncmp(trim(command), "opcode ", 7) == 0 || strncmp(trim(command), "opcodelist", 10) == 0) {
				opcode_mnemonic(command);
				valid=1;
			}
		}
		if (strstr(command, "dump") || strstr(command, "du")) {
			if (strncmp(trim(command), "dump", 4) == 0 || strncmp(trim(command), "du", 2) == 0) { // trim 함수를 통해 앞 뒤 공백 제거
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
				history(trim(command));	// command의 앞뒤 공백 제거 -> 정제 후 히스토리 입력
				valid=1;
			}
			else
				printf("wrong command\n");
		}
		if (strstr(command, "hi") || strstr(command, "history")) {	// hi[story]의 경우에만 먼저 히스토리 링크드리스트에 입력 후 command 실행
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
				history(trim(command)); // symbol을 입력한 모든 경우가 history에 저장
				valid = 1;
			}
		}
		if (strstr(command, "assemble")) {
			if (strncmp(trim(command), "assemble", 8) == 0) {
				assemble(trim(command));
				valid = 1;
			}
		}
		if(valid==0){	// 위의 command가 아닌 경우
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
 *  command 함수
 * 다른 함수를 호출하지 않는 command들을 처리하는 함수
 */
void commander(char* cmd) {
	int valid = 0;

	if (strstr(cmd, "h") || strstr(cmd, "help")) {
		if (strcmp(trim(cmd), "h") == 0 || strcmp(trim(cmd), "help") == 0) {
			printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\nassemble filename\ntype filename\nsymbol\n");	// 안내문 출력
			history(trim(cmd));
			valid = 1;
		}
	}
	if (strstr(cmd, "d") || strstr(cmd, "dir")) {
		if (strcmp(trim(cmd), "d") == 0 || strcmp(trim(cmd), "dir") == 0) {
			struct dirent* d;
			struct stat st;
			DIR * dir = opendir(".");	// 현재 디렉토리 오픈
			if(dir==NULL) printf("empty\n");	// 디렉토리에 파일이 없을 경우
			while((d=readdir(dir))){
				printf("%s", d->d_name);
				stat(d->d_name, &st);
				if(d->d_type==DT_DIR) printf("/");	// 아이템이 디렉토리일 경우
				else if(S_IEXEC & st.st_mode) printf("*");	// 아이템이 실행 파일일 경우
				printf("\n");
			}
			history(trim(cmd));
			valid=1;
		}
	}
	if (strstr(cmd, "q") || strstr(cmd, "quit")) {
		if (strcmp(trim(cmd), "q") == 0 || strcmp(trim(cmd), "quit") == 0) {
			exit(0);	// 시스템 종료
		}
	}
	if (strcmp(trim(cmd), "hi") == 0 || strcmp(trim(cmd), "history") == 0) {
		node* his = head;	// 히스토리 링크드리스트의 시작점
		int i = 0;
		while (his->next != NULL) {	// 히스토리 링크드리스트의 끝까지 반복
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
 *  history 함수
 * 파라미터로 받은 command를 차례로 링크드리스트에 저장
 */
void history(char* cmd) {
	strcpy(temp->data, cmd);
	walk = (node*)malloc(sizeof(node));	// 다음 노드
	temp->next = walk;
	walk->next = NULL;
	temp = temp->next;
}

/*
 *  opcode 함수
 * opcode mnemonic, opcodelist 명령어를 처리하는 함수
 * 공백, 무효 문자를 고려하여 유효한 명령어인지 판단 및 실행
 */
void opcode_mnemonic(char* cmd) {
	if ((strncmp(trim(cmd), "opcode ", 7) == 0)) {
		char* mne = (char*)malloc(sizeof(char) * 20);	// mnemonic 문자열
		char* check;	
		strcpy(mne, trim(cmd));
		mne = strtok(mne, " ");	// 공백을 기준으로 분리
		mne = strtok(NULL, " ");
		check = strtok(NULL, " ");	// 입력 받은 mnemonic이 한 개인지 확인하기 위한 문자열
		if (check) {
			printf("wrong command\n");
			return;
		}
		mne = trim(mne);	// 앞 뒤 공백 제거
		int index = hash_function(mne);	// hash 함수 호출로 index 할당
		hash* find = (hash*)malloc(sizeof(hash));
		find = table[index];
		while (1) {
			if (strcmp(find->key, mne) == 0) {	// index에 입력받은 key 값이 있는지 확인
				printf("opcode is %X\n", find->value);
				break;
			}
			else find = find->next; // 없을 경우 index의 다음 노드로 이동
			if (find->next == NULL) {	// 마지막 노드까지 없을 경우
				printf("opcode for %s doesn't exist\n", mne);
				break;
			}
		}
		char opcd[20] = "opcode ";
		history(strcat(opcd, mne)); // 히스토리 정제
		free(find);
	}
	else if (strcmp(trim(cmd), "opcodelist") == 0) {
		hash* hwalk = (hash*)malloc(sizeof(hash));
		for (int i = 0; i < 20; i++) {
			hwalk = table[i];	// table index의 리스트 첫 노드
			printf("%d : ", i);
			while (hwalk->next != NULL) {	// index의 마지막 노드까지
				printf("[%s,%X]", hwalk->key, hwalk->value);
				hwalk = hwalk->next;
				if (hwalk->next != NULL) printf(" -> ");	// 마지막 노드에는 출력하지 않음
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
 *  dump 함수
 * 공백 및 comma 체크 후 comma 개수에 따라 타입을 구별하여 dump_print 함수 호출
 */
void dump(char* cmd) {
	if (strcmp(cmd, "dump") == 0 || strcmp(cmd, "du") == 0) {
		dump_print(cmd, 5);	// start와 end 값이 없는 경우 타입을 5로 지정
		addr += 160;
	}
	else if (strncmp(cmd, "dump ", 5) == 0 || strncmp(cmd, "du ", 3) == 0) {
		int comma = 0;
		char* ptr = strchr(cmd, ',');
		while (ptr != NULL) {	// cmd의 마지막 문자까지
			if (ptr) comma += 1;	// comma 개수 카운트
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
 *  dump_print 함수
 * 파라미터로 타입을 받아 공백 및 유효 문자 확인, 유효할 경우 command 내용대로 출력
 */
void dump_print(char* cmd, int tp) {
	unsigned long start, end, e, temp = addr+1;
	char* com = (char*)malloc(sizeof(char) * 100);
	strcpy(com, trim(cmd));	// 타입 5일 경우, com 값
	if (tp == 0) {
		char* dmp = (char*)malloc(sizeof(char) * 20);
		strcpy(dmp, cmd);
		dmp = strtok(dmp, " ");
		strcpy(com, trim(dmp));	// du[mp] 저장
		dmp = strtok(NULL, " ");
		int vld = cvalid(dmp);	// 무효한 문자가 있는지 확인
		if (vld != 0) {
			printf("wrong command\n");
			return;
		}
		strcat(com, " ");
		strcat(com, trim(dmp));	// start 값 저장
		temp = hex_to_dec(dmp);
		dmp = strtok(NULL, " ");	// start 외 다른 값이 입력 되었는지 확인
		if (dmp) {
			printf("wrong command\n");
			return;
		}

		if (temp >= SIZE || temp < 0) {	// 범위 확인
			printf("start, out of boundary\n");
			return;
		}

	}
	// 범위 지정
	e = temp + 160;
	start = temp - (temp % 16);
	end = temp + 160 + 16 - (temp % 16);
	if (temp % 16 == 0) end -= 16;

	if (tp == 1) { // tp 0과 유사
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
			strcat(com, trim(dmp = strtok(dmp, ",")));	// start 값 저장
			char* ch = (char*)malloc(sizeof(char) * 20);
			strcpy(ch, trim(dmp));
			for (int i = 0; i < strlen(ch); i++) {	// 숫자 사이 공백 확인
				if (ch[i] == ' ') {
					printf("wrong command\n");
					return;
				}
			}
			start = hex_to_dec(trim(dmp));
			strcat(com, ", ");
			strcat(com, trim(dmp = strtok(NULL, ",")));	// end 값 저장
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
	while (start < end) {	// start부터 end까지 출력
		if (start % 16 == 0) printf("%05lX ", start);
		if (start >= temp && start < temp + 160 && start <= e) printf("%02lX ", (unsigned long)mmr[start]);	// 변수 temp, e는 command에서 입력 받은 유효 출력 범위
		else printf("   ");
		start += 1;
		if (start % 16 == 0) {	// 16개, 즉 한 줄 출력이 완료된 경우
			printf("; ");
			for (int j = start - 16; j < start; j++) {	// 가상 메모리의 16진수 값을 문자로 출력
				if (mmr[j] < 32 || mmr[j]>126 || j<temp || j>temp + 159 || j > end) printf(". ");	// 무효 숫자
				else if (mmr[j] == 32) printf("  ");	// 스페이스
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
 *  edit 함수
 * 형식이 유효한지 확인 후, command 내용대로 가상메모리 값 변경
 */
void edit(char* cmd) {
	char* com = (char*)malloc(sizeof(char) * 100);
	if (strncmp(cmd, "edit ", 5) == 0 || strncmp(cmd, "e ", 2) == 0) {
		int comma = 0;
		char* ptr = strchr(cmd, ',');
		while (ptr != NULL) {	// comma 개수 확인
			if (ptr) comma += 1;
			ptr = strchr(ptr + 1, ',');
		}
		if (comma == 1) {
			unsigned long address, value;
			char* edt = (char*)malloc(sizeof(char) * 20);
			strcpy(edt, cmd);
			edt = strtok(edt, " ");
			strcpy(com,trim(edt));	// e[dit] 저장
			edt = strtok(NULL, "\0");
			int vld = cvalid(edt);	// 무효 문자가 있는지 확인
			if (vld != 1) {
				printf("wrong command\n");
				return;
			}
			if (strtok(NULL, " ") == NULL) {
				strcat(com, " ");
				strcat(com, trim(edt = strtok(edt, ",")));	// address 저장
				char* ch = (char*)malloc(sizeof(char) * 20);
				strcpy(ch, trim(edt));
				for (int i = 0; i < strlen(ch); i++) {	// 숫자 사이 공백 확인
					if (ch[i] == ' ') {
						printf("wrong command\n");
						return;
					}
				}
				address = hex_to_dec(trim(edt));
				strcat(com, ", ");
				strcat(com, trim(edt = strtok(NULL, ",")));	// value 저장
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
			mmr[address] = value;	// 메모리의 address 인덱스에 value 값 저장
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
 *  fill 함수
 * 형식이 유효한지 확인 후, command 내용대로 임의의 범위만큼 가상메모리 값 변경
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
			strcpy(com, trim(fll));	// f[ill] 저장
			fll = strtok(NULL, "\0");
			int vld = cvalid(fll);
			if (vld != 2) {
				printf("wrong command\n");
				return;
			}
			if (strtok(NULL, " ") == NULL) {
				strcat(com, " ");
				strcat(com, trim(fll = strtok(fll, ",")));	// start 저장
				char* ch = (char*)malloc(sizeof(char) * 20);
				strcpy(ch, trim(fll));
				for (int i = 0; i < strlen(ch); i++) {	// 숫자 사이 공백 확인
					if (ch[i] == ' ') {
						printf("wrong command\n");
						return;
					}
				}
				start = hex_to_dec(trim(fll));
				fll = strtok(NULL, "\0");
				if (fll) {
					strcat(com, ", ");
					strcat(com, trim(fll = strtok(fll, ",")));	// end 저장
					ch = (char*)malloc(sizeof(char) * 20);
					strcpy(ch, trim(fll));
					for (int i = 0; i < strlen(ch); i++) {	// 숫자 사이 공백 확인
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
			strcat(com, trim(fll = strtok(NULL, "\0")));	// value 저장
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
			for (int i = start; i <= end; i++) {	// start에서 end 범위까지의 메모리 값 value로 재할당
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
 *  reset 함수
 * 가상 메모리의 모든 값을 0으로 초기화
 */
void reset() {
	for (int i = 0; i < SIZE; i++) {
		mmr[i] = 0;
	}
}

/*
 *  hash_function 함수
 * 입력 받은 mnemonic의 각 문자별 아스키 코드 값을 더한 후 20으로 나눈 값을 hash 값으로 반환
 */
int hash_function(char* op) {
	char num = 0;
	for (int i = 0; i < strlen(op); i++) {
		num += *(op + i);
	}
	if (num < 0) {	// 합계가 음수 값일 경우 양수 값으로 변경
		num = num * -1;
	}
	return num % 20;
}

/*
 *  hex_to_dec 함수
 * 입력 받은 문자열이 나타내는 16진수 값을 10진수로 변환하여 반환
 */
unsigned long hex_to_dec(char* dmp) {
	int res = 0;
	res = strtoul(dmp, NULL, 16);
	return res;
}

/*
 *  rtrim 함수
 * 입력 받은 문자열의 뒷쪽 공백 제거
 */
char* rtrim(char* s) {
	char* end;
	end = s + strlen(s) - 1;	// 문자열 마지막 문자 위치
	while (1) {
		if (!*end) break;	// 공백이 없는 경우
		if (isspace(*end))	// 있는 경우 마지막 문자 위치 재할당
			end--;
		else {
			*(end + 1) = '\0';
			break;
		}
	}
	return strncpy(s, s, strlen(end));	// 재할당한 위치까지의 부분 문자열 반환
}

/*
 *  ltrim 함수
 * 입력 받은 문자열의 앞쪽 공백 제거
 */
char* ltrim(char* s) {
	char* start;
	start = s;	// 문자열 처음 문자 위치
	while (1) {
		if (!*start) break;	// 공백 없는 경우
		if (isspace(*start))	// 있는 경우 시작 문자 위치 재할당
			start++;
		else {
			s = start;	// 재할당한 시작 위치부터
			break;
		}
	}
	return s;
}

/*
 *  trim 함수
 * ltrim, rtrim 함수 호출을 통해 입력 받은 문자열의 앞뒤 공백 제거
 */
char* trim(char* s) {
	if (s != NULL)
		return ltrim(rtrim(s));
	else	// 문자열이 없을 경우
		return s;
}

/*
 *  cvalid 함수
 * 입력 받은 문자열의 문자들이 유효한 값인지 확인 (16진수), comma 개수를 반환
 */
int cvalid(char* s) {
	int cvld = 0;
	char c;
	for (int i = 0; i < strlen(s); i++) {
		c = *(s + i);
		if ((c >= 48 && c <= 57) ||( c >= 65 && c <= 70) ||( c >= 97 && c <= 102) || c == 32) {}	// 16진수 유효값
		else if (c == 44) {	// comma 개수
			cvld++;
		}
		else {	// 무효 문자 발견시
			cvld = -1;
			break;
		}
	}
	return cvld;
}

/*
 *  type 함수
 * 입력 받은 파일명이 일치하는 파일이 있으면 내용 출력
 */
void type(char* cmd) {
	char* com = (char*)malloc(sizeof(char) * 100);
	char* typ = (char*)malloc(sizeof(char) * 20);
	strcpy(typ, cmd);
	typ = strtok(typ, " ");
	strcpy(com, trim(typ));	// type 저장
	typ = strtok(NULL, " ");
	if (typ == NULL) { // 파일명을 입력 받지 못 한 경우
		printf("wrong command\n");
		return;
	}
	strcat(com, " ");
	strcat(com, trim(typ));	// filename 저장
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
 *  assemble 함수
 * 입력 받은 파일이 존재할 경우 pass함수 호출
 */
void assemble(char* cmd) {
	char* com = (char*)malloc(sizeof(char) * 100);
	char* asb = (char*)malloc(sizeof(char) * 20);
	strcpy(asb, cmd);
	asb = strtok(asb, " ");
	strcpy(com, trim(asb));	// assemble 저장
	asb = strtok(NULL, " ");
	if (asb == NULL) { // 파일명을 입력 받지 못 한 경우
		printf("wrong command\n");
		return;
	}
	strcat(com, " ");
	strcat(com, trim(asb));	// filename 저장

	struct dirent* d;
	DIR* dir = opendir("."); // 현재 디렉토리
	if (dir == NULL) printf("empty\n"); // 디렉토리가 비었을 경우
	int exist = 0;
	int tf = 0;
	while ((d = readdir(dir))) {
		if (strcmp(d->d_name, asb) == 0) { // 문자열과 일치하는 파일명이 있을 경우
			exist = 1;
			char* temp = (char*)malloc(sizeof(char) * 20);
			strcpy(temp, asb);
			temp = strtok(temp, ".");
			strcpy(asb, temp); // 파일명 저장
			temp = strtok(NULL, "."); // 파일의 확장자 저장
			if (strcmp(temp, "asm")==0) { // 확장자가 asm과 일치할 경우
				tf=pass(asb); // pass를 호출, 성공 여부를 리턴 받음
				break;
			}
			else {
				printf("%s is not a asm type file\n", d->d_name); // asm 파일이 아닐 경우
				return;
			}
		}
	}
	if (exist == 0) { // 문자열과 일치하는 파일이 디렉토리에 존재하지 않을 경우
		printf("No such file\n");
		return;
	}
	asb = strtok(NULL, " ");
	asb = strtok(NULL, " ");
	if (asb) { // 확장자 뒤에 잘못된 추가 입력이 있을 경우
		printf("wrong command\n");
		return;
	}
	if(tf==1) history(com); // pass함수 결과 assemble을 성공한 경우
	return;
}

/*
 *  pass 함수
 * 입력 받은 문자열의 asm 파일에 대하여 pass1, pass2를 통해 obj, lst 파일을 생성
 * pass1 : forward referencing을 막기 위해 각 label과 그 주소를 저장하는 SYMTAB 생성
 * pass2 : SYMTAB을 기반으로 opcode를 계산, 그 결과에 맞게 obj, lst 파일을 생성
 * assemble 성공 여부에 따라 성공시 1, 실패시 0 리턴
 */
int pass(char* asb) {

	// pass 1 시작
	char* in = (char*)malloc(sizeof(char) * 30);
	char* save = (char*)malloc(sizeof(char) * 30);
	char* lbl = (char*)malloc(sizeof(char) * 10);
	char* mne = (char*)malloc(sizeof(char) * 10);
	char* opr = (char*)malloc(sizeof(char) * 10);

	FILE* fin = fopen(strcat(asb, ".asm"), "r");
	FILE* op = fopen("opcode.txt", "r");
	FILE* md = fopen("intermediate.txt", "w");

	fgets(in, 100, fin); // asm 파일의 한줄씩 입력
	strcpy(save, in);
	lbl = strtok(in, " "); // label
	mne = strtok(NULL, " "); // mnemonic opcode
	opr = strtok(NULL, "\n"); // operand

	int stadd = 0;
	int loc = 0;
	int length = 0;
	int check = 0;
	if (strcmp(mne, "START") == 0) { // START가 있을 경우
		stadd = atoi(trim(opr));
		loc = stadd; // START의 현 주소 loc으로 지정
		fprintf(md, "%s", save);
		scsym = (sym*)malloc(sizeof(sym));
		scsym->next = NULL;
	}
	else { // 없을 경우 임의로 초기화
		loc = 0;
		scsym = (sym*)malloc(sizeof(sym));
		scsym->next = NULL;
	}

	while (fgets(in, 100, fin) && feof(fin) == 0) { // 각 줄을 입력 받고, 파일이 끝이 나지 않은 경우
		sym* tmpsym = scsym; // second symbol table, 즉 이번 assemble을 통해 생성 중인 SYMTAB의 주소를 받아 옴
		int symfound = 0;
		check += 5;
		strcpy(save, in);
		lbl = strtok(in, " ");
		mne = strtok(NULL, " ");
		opr = strtok(NULL, "\n");
		if (mne == NULL) mne = " ";
		if (strstr(mne, ",")) { // comma가 있을 경우 다시 operand를 합쳐줌
			strcat(mne, opr);
			opr = NULL;
		}
		if (strcmp(lbl, "END") != 0) { // END가 아닐 동안
			if (strncmp(lbl, ".", 1) != 0) { // 주석이 아니면
				if (opr != NULL) {
					while (tmpsym->next != NULL) { // SYMTAB 탐색
						if (strcmp(mne, tmpsym->lbl) == 0) {
							symfound = 1;
						}
						tmpsym = tmpsym->next;
					}
					if (symfound == 1) { // 중복 될 경우
						printf("duplicate symbol error in line %d\n", check);
						return 0;
					}
					else { // 아닐 경우 새로 SYMTAB에 삽입
						sym* nxtsym = (sym*)malloc(sizeof(sym));
						nxtsym->next = NULL;
						tmpsym->next = nxtsym;
						strcpy(tmpsym->lbl, lbl);
						tmpsym->loc = loc;
					}
				}
				else { // opr이 없을 때
					if (strcmp(mne, " ") != 0) { // mne는 있다면
						opr = (char*)malloc(sizeof(char) * 10);
						strcpy(opr, mne); // 알맞은 자리로 이동
						strcpy(mne, lbl);
						lbl = NULL; // 라벨 값 삭제
						opr = strtok(opr, "\n");
					}
				}
				int opfound = 0;
				if (strncmp(mne, "+", 1) == 0) { // extended 인 경우
					mne = strtok(mne, "+"); // 제외하고 문자열 재입력
					loc++; // loc 증가 (format 4)
				}
				int index = hash_function(mne);
				hash* htemp;
				htemp = table[index];
				while (htemp->next != NULL) {
					if (strcmp(htemp->key, mne) == 0) { // 존재하는 opcode일 경우
						if (strlen(htemp->format) > 1) loc += 3; // format 3/4의 경우
						else loc += atoi(htemp->format); // 아닐 경우 format만큼 loc 증가
						opfound = 1;
						break;
					}
					htemp = htemp->next;
				}
				if (opfound == 1) {
				}
				else if (strcmp(mne, "WORD") == 0) { // 이하 존재하지 않는 mnemonic opcode일 경우
					loc += 3;
				}
				else if (strcmp(mne, "RESW") == 0) { // word*3 만큼 증가
					loc += 3 * atoi(opr);
				}
				else if (strcmp(mne, "RESB") == 0) { // operand만큼 증가
					loc += atoi(opr);
				}
				else if (strcmp(mne, "BYTE") == 0) {
					if (strncmp(trim(opr), "X", 1) == 0) {
						loc = loc + ((strlen(trim(opr)) - 3) / 2); // 문자 2개가 1 바이트이므로 길이의 반 증가
					}
					if (strncmp(trim(opr), "C", 1) == 0) {
						loc = loc + strlen(trim(opr)) - 3; // 문자 길이만큼 증가
					}
				}
				else if (strcmp(mne, "BASE") == 0) { // BASE는 저장으로 LOC 증가 없음
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
	length = loc - stadd;	// 전체 asm의 길이 저장
	fclose(md);


	// pass 2
	int line = 5;
	md = fopen("intermediate.txt", "r");
	asb = strtok(asb, ".");
	FILE* obj = fopen(strcat(asb, ".obj"), "w"); // 생성할 obj 파일 오픈
	asb = strtok(asb, ".");
	FILE* lst = fopen(strcat(asb, ".lst"), "w"); // 생성할 lst 파일 오픈
	asb = strtok(asb, ".");

	fgets(in, 30, md); // intermediate 파일의 첫 줄 입력
	strcpy(save, in); // 출력을 위해 내용 저장
	lbl = strtok(in, " ");
	mne = strtok(NULL, " ");
	opr = strtok(NULL, "\n");
	save = trim(strtok(save, "\n"));
	if (strcmp(mne, "START") == 0) {
		loc = 0;
		fprintf(lst, "%d\t%04X\t%s\n", line, loc, save); // lst 파일 저장
	}
	char* header = (char*)malloc(sizeof(char) * 100); // obj 파일 header record
	strcpy(header, "H");
	strcat(header, lbl);
	int i = 6 - strlen(lbl);
	while (i > 0) { // 길이에 맞추어 프로그램명 여백 설정
		strcat(header, " ");
		i--;
	}
	char sloc[20];
	char slen[20];
	sprintf(sloc, "%06X", loc);
	sprintf(slen, "%06X", length);
	strcat(header, sloc);
	strcat(header, slen);
	fprintf(obj, "%s\n", header); // obj 파일 저장

	char* text = (char*)malloc(sizeof(char) * 30); // obj 파일 T record
	strcpy(text, "");
	char* t = (char*)malloc(sizeof(char) * 69); // obj 파일 T record
	char* modi = (char*)malloc(sizeof(char) * 100); // obj 파일 M record
	strcpy(modi, "");

	int st = atoi(sloc);
	int opadd = 0;
	int exte = 0;
	int next = 0;
	int base = 0;
	int reser = 0;
	char* objcd = (char*)malloc(sizeof(char) * 20); // object code 계산 결과를 저장할 문자열
	while (fgets(in, 100, md) && feof(md) == 0) {
		objcd = (char*)malloc(sizeof(char) * 20);
		while (strncmp(in, ".", 1) == 0) { // 주석일 경우
			line += 5;
			fprintf(lst, "%d\t%s", line, in); // 리스트 파일에 저장
			fgets(in, 100, md); // 다음줄 입력
		}
		loc += next; // 이전 줄의 크기만큼 증가하여 현재 loc 계산
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
		if (mne == NULL && opr == NULL && (strncmp(lbl, "RSUB", 4) == 0)) { // RSUB인 경우 (lbl, opr 존재하지 않을 경우)
			mne = (char*)malloc(sizeof(char) * 10);
			strcpy(mne, lbl);
			mne = strtok(mne, "\n");
			lbl = " ";
		}
		if (strstr(mne, ",")) { // comma가 있을 경우 문자열 합침
			strcat(mne, opr);
			opr = NULL;
		}
		if (strcmp(lbl, "END") != 0) {
			if (strncmp(lbl, ".", 1) != 0) {
				char fl = lbl[0];
				if (fl >= 48 && fl <= 57) { // label의 첫 글자가 숫자일 경우
					printf("Error occured in line %d\n", line+5);
					err(asb); // err함수 호출
					fclose(obj);
					fclose(lst);
					return 0; // 에러 발생 시 0을 반환
				}
				if (exte == 1) { // 이전 줄이 format 4인 경우 loc 추가 증가
					loc++;
					exte = 0;
				}
				if (opr == NULL && strcmp(lbl, " ")!=0) { // label이 없는 경우 원래 자리로 재할당
					opr = (char*)malloc(sizeof(char) * 10);
					strcpy(opr, mne);
					strcpy(mne, lbl);
					lbl = NULL;
					opr = strtok(opr, "\n");
				}
				int opfound = 0;
				if (strncmp(mne, "+", 1) == 0) { // extended인 경우 flag 설정
					mne = strtok(mne, "+");
					exte = 1;
				}
				int index = hash_function(mne); // 해시로 index 값 얻음
				hash* htemp;
				htemp = table[index];
				while (htemp->next != NULL) { // 일치하는 opcode가 있는지 확인
					if (strcmp(htemp->key, mne) == 0) {
						opfound = 1;
						value = htemp->value; // opcode의 value 저장, object code 값 계산을 위함
						fm = atoi(htemp->format);
						next += fm; // format만큼 다음 줄에 증가할 loc 설정
						break;
					}
					htemp = htemp->next;
				}
				if (opfound == 1) { // opcode가 OPTAB에 존재 할 경우
					opr = trim(opr);
					if (opr != NULL) {
						if (strncmp(opr, "@", 1) == 0) { // indirect
							opr = strtok(opr, "@");
							ni = 2; // n, i 값 설정
						}
						if (strncmp(opr, "#", 1) == 0) { // immediate
							opr = strtok(opr, "#");
							ni = 1;
						}
						if (strstr(opr, ",")) { // comma 여부 확인
							comma = 1;
						}
						tmpsym = scsym; // 현재 assemble 파일의 SYMTAB head에 접근
						while (tmpsym->next != NULL) {
							if (strcmp(tmpsym->lbl, opr) == 0) { // 존재할 경우
								symfound = 1;
								opadd = tmpsym->loc; // loc address 저장
								break;
							}
							tmpsym = tmpsym->next;
						}
						if (symfound == 1) { // SYMTAB에 operand가 존재할 경우
							if (opadd - loc >= -2048 && opadd - loc <= 2047) { // 값이 범위 안일 때, 
								bp = 2;
								sprintf(objcd, "%02X%X%03X", value + ni, bp, (opadd - loc - fm) & 0x00000FFF);
							}
							else if (opadd - loc < -2048) { // pc로 해결되지 않아 base일 경우
								bp = 4;
								sprintf(objcd, "%02X%X%03X", value + ni, bp, opadd - base);
							}
							else { // extended, direct addressing 의 경우
								bp = 1;
								sprintf(objcd, "%02X%X%05X", value + ni, bp, opadd);
								int md = loc + 1;
								char* mdf = (char*)malloc(sizeof(char) * 20);
								sprintf(mdf, "M%06X05\n", md); // obj 파일 M record
								strcat(modi, mdf); // M record 도합
							}
						}
						// 이하 SYMTAB에 operand가 없을 때(register 및 상수), format과 comma에 따라 세분
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
								if (opr2 == NULL) { // comma 뒤에 operand가 없는 경우
									printf("Error occured in line %d\n", line + 5);
									err(asb);
									fclose(obj);
									fclose(lst);
									return 0;
								}
								int op1 = get_reg(opr1);
								int op2 = get_reg(opr2);
								if (op1 != 7 && op2 != 7) { // 두 operand가 모두 register 인 경우
									if ((strcmp(mne, "SHIFTL") != 0) && (strcmp(mne, "SHIFTR") != 0)) { // 상수가 들어올 수 있는 명령어
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
									if (exte == 1) // extended의 경우 xbpe 비트에 값 1 셋
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
								else { // comma가 있으며 둘다 register가 아닌 경우, 예) BUFFER, X
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
						// RSUB인 경우
						opadd = 0;
						lbl = NULL;
						sprintf(objcd, "%02X%04X", value + ni, 0);
					}
				}
				else if (strcmp(mne, "WORD") == 0) { // WORD 그대로 
					next += 3;
					sprintf(objcd, "%06X", atoi(opr));
				}
				else if (strcmp(mne, "RESW") == 0) { // objcd 없음, reser 통해 줄넘김 flag
					next += 3 * atoi(opr);
					objcd = "";
					reser++;
				}
				else if (strcmp(mne, "RESB") == 0) { // objcd 없음, reser 통해 줄넘김 flag
					next += atoi(opr);
					objcd = "";
					reser++;
				}
				else if (strcmp(mne, "BYTE") == 0) { 
					if (strncmp(trim(opr), "X", 1) == 0) {
						next = next + ((strlen(trim(opr)) - 3) / 2);
						opr = strtok(opr, "'");
						opr = strtok(NULL, "'");
						sprintf(objcd, "%s", opr); // 숫자일 경우 그대로
					}
					if (strncmp(trim(opr), "C", 1) == 0) { // 문자일 경우
						next = next + strlen(trim(opr)) - 3;
						opr = strtok(opr, "'");
						opr = strtok(NULL, "'");
						char* cc = (char*)malloc(sizeof(char) * strlen(opr));
						objcd = (char*)malloc(sizeof(char) * 20);
						for (int i = 0; i < strlen(opr); i++) { // 각 문자를 16진수로 변환하여 문자열로 저장
							sprintf(cc, "%X", opr[i]);
							if (i == 0) strcpy(objcd, cc);
							else strcat(objcd, cc);
						}
					}
				}
				else if (strcmp(mne, "BASE") == 0) {
					tmpsym = scsym;

					while (tmpsym->next != NULL) { // SYMTAB 탐색
						if (strcmp(tmpsym->lbl, opr) == 0) {
							symfound = 1;
							opadd = tmpsym->loc; // SYMTAB 검색 결과 loc address 저장
							break;
						}
						tmpsym = tmpsym->next;
					}
					line += 5;
					objcd = "";
					base = opadd;
					fprintf(lst, "%d\t\t%-20s\t\t%s\n", line, save, objcd);  // lst 파일 저장
				}
				else {
					printf("Error occured in line %d\n", line + 5);
					err(asb);
					fclose(obj);
					fclose(lst);
					return 0;
				}
				if (strlen(text) + strlen(objcd) > 60 || (reser == 1 && strcmp(text, "") != 0)) { // obj 파일 T record가 30바이트가 넘었거나 RESB, RESW 명령어를 입력 받은 경우
					t = (char*)malloc(sizeof(char)*69);
					int txtlen = strlen(text) / 2;
					sprintf(t, "T%06X%02X%s", st, txtlen, text);
					fprintf(obj, "%s\n", t); // obj 파일 저장
					text = (char*)malloc(sizeof(char)*100); // 새로운 T record 시작
					strcpy(text, "");
					st = loc;
					reser = 0;
				}
				if ((strcmp(objcd, "") == 0) &&(strcmp(mne,"BASE")!=0)) st = loc + next; // RESB, RESW로 인해 새로 다음 T record의 시작 위치를 선언해야 하는 경우
				strcat(text, objcd); // 최대 길이 미만인 경우 T record에 objcd 붙임
			}
			if (strcmp(mne, "BASE") != 0) { // BASE는 따로 lst 저장
				line += 5;
				fprintf(lst, "%d\t%04X\t%-20s\t\t%s\n", line, loc, save, objcd); // lst 파일 저장
			}
		}
		else break;
	}
	line += 5;
	fprintf(lst, "%d\t\t%-20s\n", line, save); // 마지막 줄 lst 파일 저장

	t = (char*)malloc(sizeof(char)*69);
	int txtlen = strlen(text) / 2;
	sprintf(t, "T%06X%02X%s", st, txtlen, text);
	fprintf(obj, "%s\n", t); // obj 파일 마지막 T record 저장
	fprintf(obj, "%s", modi); // obj 파일 M record 저장
	fprintf(obj, "E%06X\n", atoi(sloc)); // obj 파일 E record 저장

	hdsym = scsym; // assemble 성공시 현재 SYMTAB을 hdsym으로 지정
	printf("[%s.lst], [%s.obj]\n", asb, asb); // 성공 메시지

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
 *  get_reg 함수
 * 입력 받은 레지스터가 존재할 경우 그에 맞는 레지스터 번호 리턴
 * 아닐 경우 7 리턴으로 입력 받은 문자열이 레지스터가 아님을 알림
 */
int get_reg(char* reg) { 
	if (strcmp(reg, "A") == 0) return 0; // 레지스터 번호 리턴
	else if (strcmp(reg, "X") == 0) return 1;
	else if (strcmp(reg, "L") == 0) return 2;
	else if (strcmp(reg, "PC") == 0) return 8;
	else if (strcmp(reg, "SW") == 0) return 9;
	else if (strcmp(reg, "B") == 0) return 3;
	else if (strcmp(reg, "S") == 0) return 4;
	else if (strcmp(reg, "T") == 0) return 5;
	else if (strcmp(reg, "F") == 0) return 6;
	else return 7; // 존재하는 레지스터가 아닐 경우
}

/*
 *  err 함수
 * error 발생 시 생성하고 있던 lst, obj 파일을 삭제하고 symtab을 빈 상태로 초기화
 */
void err(char* asb) {
	remove(strcat(asb, ".lst")); // lst 삭제
	strtok(asb, ".");
	remove(strcat(asb, ".obj")); // obj 삭제
	strtok(asb, ".");
	scsym = NULL; // 저장 중이던 symtab 삭제
	return;
}

/*
 *  symbol 함수
 * 마지막 assemble한 파일에서 생성된 symtab을 정렬 후 출력
 */
void symbol() {
	sym* tmpsym = hdsym; // symtab의 head에 접근
	if (tmpsym == NULL || hdsym->next==NULL) { // 마지막 assemble이 없거나 유효하지 않아 symtab이 존재하지 않을 경우
		printf("NO SYMBOL TABLE\n");
		return;
	}
	while (tmpsym->next != NULL) { // symtab에 현재 node가 있을 동안
		sym* nxtsym = tmpsym->next;
		while(nxtsym->next !=NULL){ // symtab에 다음 node가 있을 동안
			if (strcmp(tmpsym->lbl, nxtsym->lbl) > 0) { // 현재 노드와 다음 노드 문자열 비교, 오름차순 정렬
				char c[10];
				strcpy(c, tmpsym->lbl); // 임시 저장
				int i = tmpsym->loc;
				strcpy(tmpsym->lbl, nxtsym->lbl); // swap
				tmpsym->loc = nxtsym->loc;
				strcpy(nxtsym->lbl, c);
				nxtsym->loc = i;
			}
			nxtsym=nxtsym->next; // 다음 노드로 이동
		}
		tmpsym = tmpsym->next; // 다음 노드로 이동
	}
	tmpsym = hdsym; // 정렬 후 symtab의 head 노드에 다시 접근
	while (tmpsym->next != NULL) { // symtab에 현재 node가 있을 동안
		if (tmpsym->next->next == NULL) { // symtab의 다음노드가 없을 경우
			printf("\t%s\t%04X\n", tmpsym->lbl, tmpsym->loc);
			break; // 다음 노드로 이동하지 않고 중지
		}
		printf("\t%s\t%04X\n", tmpsym->lbl, tmpsym->loc); // node 출력 후 이동
		tmpsym = tmpsym->next;
	}
}
