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
			opcode_mnemonic(command);
			valid=1;
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
			printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\n");	// 안내문 출력
			history(trim(cmd));
			valid = 1;
		}
	}
	if (strstr(cmd, "d") || strstr(cmd, "dir")) {
		if (strcmp(trim(cmd), "d") == 0 || strcmp(trim(cmd), "dir") == 0) {
			struct dirent* d;
			DIR * dir = opendir(".");	// 현재 디렉토리 오픈
			if(dir==NULL) printf("empty\n");	// 디렉토리에 파일이 없을 경우
			while((d=readdir(dir))){
				printf("%s", d->d_name);
				if(d->d_type==DT_DIR) printf("/");	// 아이템이 디렉토리일 경우
				else if((d->d_type==S_IXUSR)||(d->d_type==S_IWRITE)||(d->d_type==S_IXGRP)||(d->d_type==S_IXOTH)) printf("*");	// 아이템이 실행 파일일 경우
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
		while (his->next != NULL) {	// 히스토리 링크드리스트의 끝까지 반복
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
void dump_print(char* cmd, int type) {
	unsigned long start, end, e, temp = addr+1;
	char* com = (char*)malloc(sizeof(char) * 100);
	strcpy(com, trim(cmd));	// 타입 5일 경우, com 값
	if (type == 0) {
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

	if (type == 1) { // type 0과 유사
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
