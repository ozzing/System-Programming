<h1> System Programming ๐ฉ </h1>

<h3> Project 1 </h3>
SIC/XE๋จธ์  ๊ตฌํ์ ์ํ ์  ๋จ๊ณ๋ก์ ์ด์๋ธ๋ฌ, ๋ง์ปค, ๋ก๋๋ค์ ์คํํ๊ฒ ๋  ์ธ(shell)๊ณผ ์ปดํ์ผ์ ํตํด์ ๋ง๋ค์ด์ง object์ฝ๋๊ฐ ์ ์ฌ๋๊ณ  ์คํ๋  ๋ฉ๋ชจ๋ฆฌ๊ณต๊ฐ, mnemonic (ADD, COMP, FLOAT, etc โฆ)์ opcode๊ฐ์ผ๋ก ๋ณํํ๋ OPCODE ํ์ด๋ธ๊ณผ ๊ด๋ จ ๋ช๋ น์ด๋ค์ ๊ตฌํํ๋ ํ๋ก๊ทธ๋จ.<br>
<h4> ์ฃผ์ ๊ฐ๋ฐ ๋ด์ฉ </h4>
โ 	์ธ (sicsim>)<br>
โก	์ธ ๊ด๋ จ ๋ช๋ น์ด๋ค (help, dir, quit, history)<br>
โข	๋ฉ๋ชจ๋ฆฌ๊ณต๊ฐ (1MB์ ๋ฉ๋ชจ๋ฆฌ๋ฅผ ํ ๋นํด์ ์ฌ์ฉ)<br>
โฃ	๋ฉ๋ชจ๋ฆฌ๊ณต๊ฐ ๊ด๋ จ ๋ช๋ น์ด๋ค (dump, edit, fill, reset)<br>
โค	opcode ํ์ด๋ธ (HashTable๋ก ๋ง๋ค์ด์ผ ํจ)<br>
โฅ	opcode ๊ด๋ จ ๋ช๋ น์ด๋ค (opcode, opcodelist)<br>
<br>

<hr>

<h3> Project 2 </h3>
Project 1์์ ๊ตฌํํ ์(shell)์ assemble ๊ธฐ๋ฅ์ ์ถ๊ฐํ๋ ํ๋ก๊ทธ๋จ. SIC/XE์ assembly program source ํ์ผ์ ์๋ ฅ ๋ฐ์์ objectํ์ผ์ ์์ฑํ๊ณ , ์ด์๋ธ๋ฆฌ ๊ณผ์  ์ค ์
์ฑ๋ symbol table๊ณผ ๊ฒฐ๊ณผ๋ฌผ์ธ object ํ์ผ์ ๋ณผ ์ ์๋ ๊ธฐ๋ฅ์ ์ ๊ณต.<br>
<h4> ์ฃผ์ ๊ฐ๋ฐ ๋ด์ฉ </h4>
โ  Shell ๊ด๋ จ ๋ช๋ น์ด๋ค (help, type)<br>
โก assembler (assemble)<br>
โข assemble ๊ด๋ จ ๋ช๋ น์ด (symbol)<br>
<br>

<hr>

<h3> Project 3 </h3>
ํ๋ก์ ํธ 1, 2 ์์ ๊ตฌํํ ์(shell)์ linking๊ณผ loading ๊ธฐ๋ฅ์ ์ถ๊ฐํ๋ ํ๋ก๊ทธ๋จ.
ํ๋ก์ ํธ 2 ์์ ๊ตฌํ๋ assemble ๋ช๋ น์ ํตํด์ ์์ฑ๋ object ํ์ผ์ link์์ผ ๋ฉ๋ชจ๋ฆฌ์ ์ฌ๋ฆฌ๋ ์ผ์ ์ํ.<br>
<h4> ์ฃผ์ ๊ฐ๋ฐ ๋ด์ฉ </h4>
โ  ์ฃผ์ ์ง์  ๋ช๋ น์ด (progaddr)<br>
โก Linking Loader (loader)<br>
โข ํ๋ก๊ทธ๋จ ์คํ (run)<br>
โฃ debug ๋ช๋ น์ด (bp)<br>
<br>

<hr>

<h3> Project 4 </h3>
System-level Process Control, Process Signalling, Interprocess Communication and Running Processes and Jobs 
in the Background in Linux Shell<br>
<h4> ์ฃผ์ ๊ฐ๋ฐ ๋ด์ฉ </h4>
โ 	cd, ls, mkdir, rmdir, touch, cat, echo, exit ๋ช๋ น์ด<br>
โก	ํ์ดํ ์ปค๋งจ๋ <i>ex) ls -al | grep filename, cat filename | grep -v "abc" | sort -r </i><br>
โข jobs, bg, fg, kill ๋ฑ ๋ฐฑ๊ทธ๋ผ์ด๋ ํ๋ก์ธ์ค <i>ex) ls -al | grep filename &</i> <br>
<br>

<hr>

<h3> Project 5 </h3>
๋์ ์ฃผ์ ์๋ฒ. ์ฌ๋ฌ ํด๋ผ์ด์ธํธ์ ๋์ ์ ์ ๋ฐ ์๋น์ค๋ฅผ ์ํ ์ฃผ์ ์๋ฒ๋ฅผ ๊ตฌ์ถ. <br>
<h4> ์ฃผ์ ๊ฐ๋ฐ ๋ด์ฉ </h4>
โ 	Event-driven Approach <br>
โก	Thread-based Approach <br>
โข	์ด์ง ํธ๋ฆฌ ์๋ฃ๊ตฌ์กฐ ์ฌ์ฉ <br>
