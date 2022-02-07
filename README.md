<h1> System Programming 🔩 </h1>

<h3> Project 1 </h3>
SIC/XE머신 구현을 위한 전 단계로서 어셈블러, 링커, 로더들을 실행하게 될 셸(shell)과 컴파일을 통해서 만들어진 object코드가 적재되고 실행될 메모리공간, mnemonic (ADD, COMP, FLOAT, etc …)을 opcode값으로 변환하는 OPCODE 테이블과 관련 명령어들을 구현하는 프로그램.<br>
<h4> 주요 개발 내용 </h4>
①	셸 (sicsim>)<br>
②	셸 관련 명령어들 (help, dir, quit, history)<br>
③	메모리공간 (1MB의 메모리를 할당해서 사용)<br>
④	메모리공간 관련 명령어들 (dump, edit, fill, reset)<br>
⑤	opcode 테이블 (HashTable로 만들어야 함)<br>
⑥	opcode 관련 명령어들 (opcode, opcodelist)<br>
<br>

<hr>

<h3> Project 2 </h3>
Project 1에서 구현한 셀(shell)에 assemble 기능을 추가하는 프로그램. SIC/XE의 assembly program source 파일을 입력 받아서 object파일을 생성하고, 어셈블리 과정 중 생
성된 symbol table과 결과물인 object 파일을 볼 수 있는 기능을 제공.<br>
<h4> 주요 개발 내용 </h4>
① Shell 관련 명령어들 (help, type)<br>
② assembler (assemble)<br>
③ assemble 관련 명령어 (symbol)<br>
<br>

<hr>

<h3> Project 3 </h3>
프로젝트 1, 2 에서 구현한 셀(shell)에 linking과 loading 기능을 추가하는 프로그램.
프로젝트 2 에서 구현된 assemble 명령을 통해서 생성된 object 파일을 link시켜 메모리에 올리는 일을 수행.<br>
<h4> 주요 개발 내용 </h4>
① 주소 지정 명령어 (progaddr)<br>
② Linking Loader (loader)<br>
③ 프로그램 실행 (run)<br>
④ debug 명령어 (bp)<br>
<br>

<hr>

<h3> Project 4 </h3>
System-level Process Control, Process Signalling, Interprocess Communication and Running Processes and Jobs 
in the Background in Linux Shell<br>
<h4> 주요 개발 내용 </h4>
①	cd, ls, mkdir, rmdir, touch, cat, echo, exit 명령어<br>
②	파이프 커맨드 <i>ex) ls -al | grep filename, cat filename | grep -v "abc" | sort -r </i><br>
③ jobs, bg, fg, kill 등 백그라운드 프로세스 <i>ex) ls -al | grep filename &</i> <br>
<br>

<hr>

<h3> Project 5 </h3>
동시 주식 서버. 여러 클라이언트의 동시 접속 및 서비스를 위한 주식 서버를 구축. <br>
<h4> 주요 개발 내용 </h4>
①	Event-driven Approach <br>
②	Thread-based Approach <br>
③	이진 트리 자료구조 사용 <br>
