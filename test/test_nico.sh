#!/usr/bin/bash

OUT_MINISHELL="output/fd1_minishell"
OUT_BASH="output/fd1_bash"
OUT_MINISHELL2="output/fd1_minishell_print"
OUT_BASH2="output/fd1_bash_print"
WHITE="\033[0m"
RED="\033[91m"
GREEN="\033[92m"
ORANGE="\033[93m"
BLUE="\033[94m"
VALGRIND='valgrind --trace-children=yes --child-silent-after-fork=yes --leak-check=full --show-leak-kinds=all --suppressions=valgrind.supp --log-file=valgrind_log/memoire.log'
VALGRIND2='valgrind --trace-children=yes --child-silent-after-fork=yes --leak-check=no --track-fds=yes --suppressions=valgrind.supp --log-file=valgrind_log/fd.log'
ERROR=0
LEAK=0
LEAK_FD=0

cmp_to_bash_posix() {
	timeout 2s ./Minishell >$OUT_MINISHELL 2>&1 0>&1 <<EOF
$(for arg in "$@"; do
		echo "$arg"
	done)
EOF
	bash --posix >$OUT_BASH 2>&1 <<EOF
$(for arg in "$@"; do
		echo "$arg"
	done)
EOF
	sed -i -e "s/\\x1b\[?[0123456789]\{1,\}l//g" $OUT_MINISHELL
	cp $OUT_MINISHELL $OUT_MINISHELL2
	cp $OUT_BASH $OUT_BASH2
	sed -i -e 's/minishell0-0//' $OUT_MINISHELL
	sed -i -e 's/minishell://' $OUT_MINISHELL
	sed -i -e 's/minishell//' $OUT_MINISHELL
	sed -i -e 's/minishell//' $OUT_BASH
	sed -i -e 's/line [0123456789]\{1,\}: //' $OUT_BASH
	sed -i -e 's/bash-5.1//' $OUT_BASH
	sed -i -e 's/bash-5.2//' $OUT_BASH
	sed -i -e 's/bash-5.3//' $OUT_BASH
	sed -i -e 's/bash-5.0//' $OUT_BASH
	sed -i -e 's/bash://' $OUT_BASH
	sed -i -e 's/bash//' $OUT_BASH
	sed -i -e 's/\$ //' $OUT_BASH

	# Affiche la sortie de chaque shell
	#echo -e "\n--- Minishell output ---"
	#cat $OUT_MINISHELL
	#echo -e "--- Bash output ---"
	#cat $OUT_BASH

	if ! (diff $OUT_BASH $OUT_MINISHELL) >/dev/null; then
		printf "$RED"
		printf '\033[66`%s' "KO"
		printf "$WHITE"
		ERROR=1
	else
		printf "$GREEN"
		printf '\033[66`%s' "OK"
		printf "$WHITE"
	fi
}

check_mem_leak() {
	LEAK=0
	$VALGRIND ./Minishell >/dev/null 2>&1 <<EOF
$(for arg in "$@"; do
		echo "$arg"
	done)
EOF
	if grep -sq 'blocks are still reachable' valgrind_log/memoire.log; then
		printf "$ORANGE"
		printf '\033[79`%s' "KO"
		printf "$WHITE"
		LEAK=1
		return -1
	else
		if grep -sq 'blocks are definitely lost in' valgrind_log/memoire.log; then
			printf "$RED"
			printf '\033[79`%s' "KO"
			printf "$WHITE"
			LEAK=1
			return -1
		else
			if grep -sq 'blocks are indirectly lost' valgrind_log/memoire.log; then
				printf "$RED"
				printf '\033[79`%s' "KO"
				printf "$WHITE"
				LEAK=1
				return -1
			else
				if grep -sq 'blocks are possibly lost' valgrind_log/memoire.log; then
					printf "$ORANGE"
					printf '\033[79`%s' "KO"
					printf "$WHITE"
					LEAK=1
					return -1
				fi
			fi
		fi
	fi
	printf "$GREEN"
	printf '\033[79`%s' "OK"
	printf "$WHITE"
}

check_fd_leak() {
	$VALGRIND2 ./Minishell >/dev/null 2>&1 <<EOF
$(for arg in "$@"; do
		echo "$arg"
	done)
EOF
	if ! grep -sq 'Open file descriptor' valgrind_log/fd.log; then
		printf "$GREEN"
		printf '\033[92`%s' "OK"
		printf "$WHITE"
	else
		printf "$RED"
		printf '\033[92`%s' "KO"
		printf "$WHITE"
		LEAK_FD=1
	fi
}

make_a_test2() {
	cmp_to_bash_posix "$@"
	check_mem_leak "$@"
	check_fd_leak "$@"
	if [ "$ERROR" == "1" ]; then
		ERROR=0
		printf "$RED\n====================================\n||		DIFF  		  ||\n====================================$WHITE\n"
		diff $OUT_BASH2 $OUT_MINISHELL2
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	if [ "$LEAK" == "1" ]; then
		LEAK=0
		printf "$RED\n====================================\n||		LEAK  		  ||\n====================================$WHITE\n"
		cat valgrind_log/memoire.log | sed 's/==[0-9]\{1,\}==//' | grep -e "by [0-9]x" -e bytes -e "Conditional" -e "Leak" -e "Invalid read" -e "Invalid write" -e "Mismatched" -e "LEAK SUMMARY:" -e "HEAP SUMMA"
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output
}

fd_limited_test() {
	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output

	printf "\n$BLUE$1$WHITE"
	printf '\033[76`%s\n' "MEM_LEAK"
	(
		ulimit -n 17
		$VALGRIND ./Minishell <<EOF
$(for arg in "$@"; do
			echo "$arg"
		done)
EOF
	)
	(
		ulimit -n 17
		$VALGRIND2 ./Minishell <<EOF
$(for arg in "$@"; do
			echo "$arg"
		done)
EOF
	)
	if ! grep -sq 'Open file descriptor' valgrind_log/fd.log; then
		printf "$GREEN"
		printf '\033[76`%s' "OK"
		printf "$WHITE"
	else
		printf "$RED"
		printf '\033[76`%s' "KO"
		printf "$WHITE"
		LEAK_FD=1
	fi
	if [ "$LEAK" == "1" ]; then
		LEAK=0
		printf "$RED\n====================================\n||		LEAK  		  ||\n====================================$WHITE\n"
		cat valgrind_log/memoire.log | sed 's/==[0-9]\{1,\}==//' | grep -e "by [0-9]x" -e bytes -e "Conditional" -e "Leak" -e "Invalid read" -e "Invalid write" -e "Mismatched" -e "LEAK SUMMARY:" -e "HEAP SUMMA"
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output
}

make_a_test() {
	printf "\n$BLUE$*$WHITE"
	printf '\033[65`%s' "DIFF"
	printf '\033[76`%s' "MEM_LEAK"
	printf '\033[89`%s\n' "FDS_LEAK"
	cmp_to_bash_posix "$@"
	#check_mem_leak "$@"
	#check_fd_leak "$@"
	if [ "$ERROR" == "1" ]; then
		ERROR=0
		printf "$RED\n====================================\n||		DIFF  		  ||\n====================================$WHITE\n"
		diff $OUT_BASH $OUT_MINISHELL
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	if [ "$LEAK" == "1" ]; then
		LEAK=0
		printf "$RED\n====================================\n||		LEAK  		  ||\n====================================$WHITE\n"
		cat valgrind_log/memoire.log | sed 's/==[0-9]\{1,\}==//' | grep -e "by [0-9]x" -e bytes -e "Conditional" -e "Leak" -e "Invalid read" -e "Invalid write" -e "Mismatched" -e "LEAK SUMMARY:" -e "HEAP SUMMA"
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	if [ "$LEAK_FD" == "1" ]; then
		LEAK_FD=0
		printf "$RED\n====================================\n||		LEAK_FD		  ||\n====================================$WHITE\n"
		cat valgrind_log/fd.log
		printf "$RED====================================\n||		END_FD 		  ||\n====================================$WHITE\n"
	fi
	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output
}

make_a_test_nodiff() {
	printf "\n$BLUE$*$WHITE"
	printf '\033[65`%s' "DIFF"
	printf '\033[76`%s' "MEM_LEAK"
	printf '\033[89`%s\n' "FDS_LEAK"
	check_mem_leak $@
	check_fd_leak $@
	if [ "$LEAK" == "1" ]; then
		LEAK=0
		printf "$RED\n====================================\n||		LEAK  		  ||\n====================================$WHITE\n"
		cat valgrind_log/memoire.log | sed 's/==[0-9]\{1,\}==//' | grep -e "by [0-9]x" -e bytes -e "Conditional" -e "Leak" -e "Invalid read" -e "Invalid write" -e "Mismatched" -e "LEAK SUMMARY:" -e "HEAP SUMMA"
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	if [ "$LEAK_FD" == "1" ]; then
		LEAK_FD=0
		printf "$RED\n====================================\n||		LEAK_FD		  ||\n====================================$WHITE\n"
		cat valgrind_log/fd.log
		printf "$RED====================================\n||		END_FD 		  ||\n====================================$WHITE\n"
	fi
	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output
}

clear
printf "Compil minishell ...\n"
make -C ../ re >/dev/null
if [ "$?" != "0" ]; then
	exit
fi
clear
printf "Prepare for test ...\n"
rm Minishell valgrind_log output -rf
mv ../Minishell .
make -C ../ fclean >/dev/null
mkdir valgrind_log
mkdir output

clear
#OLDPWD=$(pwd)
#export OLDPWD
#clear

# _              _
#| |    _____  _(_)_ __   __ _
#| |   / _ \ \/ / | '_ \ / _` |
#| |__|  __/>  <| | | | | (_| |
#|_____\___/_/\_\_|_| |_|\__, |
#                        |___/

printf '\n%s\n' "Test lexing"
printf '\n\033[15`%s\n' "Test lexing"
if false; then
	make_a_test "
"
	make_a_test " "
	make_a_test "	"
	make_a_test ":"
	make_a_test "!"
	make_a_test "-"
	make_a_test "| ls | cat -e"
	make_a_test "|o | cat -e"
	make_a_test "hello || cat -e |||"
	make_a_test "|"
	make_a_test "| hola"
	make_a_test "| | |"
	make_a_test "||"
	make_a_test "|||||"
	make_a_test ""
	make_a_test ""
	make_a_test "
"
	make_a_test_nodiff "&&"
	make_a_test_nodiff "&&&&&"
	make_a_test_nodiff "&&&&&&&&&&&&&&&&"

	printf '\n\033[15`%s\n' "Redirection"
	make_a_test ">"
	make_a_test "<"
	make_a_test ">"
	make_a_test "> >"
	make_a_test "> >>"
	make_a_test "> >>>"
	make_a_test "> >>>>"
	make_a_test "> >>>>>"
	make_a_test ">"
	make_a_test ">>"
	make_a_test ">>>"
	make_a_test "> "
	make_a_test ">	 "
	make_a_test ">>>>>>>>>>>>>>>"
	make_a_test "> > > >"
	make_a_test ">> >> >> >>"
	make_a_test "< < < <"
	make_a_test "<< << << <<"
	make_a_test "<<<< << << <<"
	make_a_test ">>> >"
	make_a_test "<<< <"
	touch proute
	chmod 000 proute
	make_a_test ">proute"
	make_a_test "<proute"
	chmod 777 proute
	rm proute
	touch proute
	chmod +r proute
	make_a_test ">proute"
	make_a_test "<proute"
	chmod 777 proute
	rm proute
	touch proute
	chmod +x proute
	chmod -r proute
	make_a_test ">proute"
	make_a_test "<proute"
	chmod 777 proute
	rm proute
	touch proute
	chmod +w proute
	chmod -x proute
	make_a_test ">proute"
	make_a_test "<proute"
	chmod 777 proute
	rm proute
	printf "\n"

	#rm Minishell valgrind_log output typescript -rf
	#exit

	# ____        _ _ _        _
	#| __ ) _   _(_) | |_     (_)_ __
	#|  _ \| | | | | | __|____| | '_ \
	#| |_) | |_| | | | ||_____| | | | |
	#|____/ \__,_|_|_|\__|    |_|_| |_|

	printf "\nTEST BUILT-IN\n"
	printf '\033[15`%s\n' "Test cd built-in"
	make_a_test "cd" "pwd"
	make_a_test "cd .." "pwd"
	make_a_test "cd /tmp" "pwd"
	make_a_test "cd /tmp" "pwd" "cd -" "pwd" "cd -" "pwd"
	make_a_test "cd dsfa" "pwd"
	make_a_test "cd -" "pwd"
	make_a_test_nodiff "cd -~"
	make_a_test_nodiff "cd -/~"
	make_a_test_nodiff "cd -/tmp" "pwd"
	make_a_test "cd ~" "pwd" "cd -" "pwd"
	make_a_test "cd ~/Downloads" "pwd"
	make_a_test "cd ~Downloads" "pwd"
	make_a_test "cd ~../../" "pwd"
	make_a_test "cd ~../" "pwd"
	make_a_test "cd ~/.." "pwd" "cd -" "pwd"
	make_a_test "cd ~/../" "pwd"
	make_a_test "cd /dev/tty" "pwd"
	make_a_test "cd /dev/stdout" "pwd"
	make_a_test "cd /dev/stdin" "pwd"
	make_a_test "cd /dev/stderr" "pwd"
	make_a_test "cd /dev/null" "pwd"
	make_a_test "cd /dev/full" "pwd"
	make_a_test "cd /dev/fd/0" "pwd"
	make_a_test "cd /dev/char" "pwd"
	make_a_test "cd /dev/bus" "pwd"
	make_a_test "cd /dev/null" "pwd"
	make_a_test "cd //~" "pwd"
	make_a_test "cd \$HOME" "pwd"
	make_a_test "cd \$HOMEE" "pwd"

fi
printf '\n\033[15`%s\n' "Test echo built-in"
make_a_test "echo Hola"
make_a_test "echoHola"
make_a_test "echo-nHola"
make_a_test "echo -n Hola"
make_a_test "echo \"-n\" Hola"
make_a_test "echo -nHola"
make_a_test "echo Hola -n"
make_a_test "echo Hola Quel tal"
make_a_test "echo		Hole"
make_a_test "echo Hola		que	tal"
make_a_test_nodiff "echo	\n	hola"
make_a_test "echo \"				\" | cat -e"
make_a_test "echo								| cat -e"
make_a_test "echo hola\"\"'''' que\"\"'' tal\"\"''"
make_a_test "echo -n -n"
make_a_test "echo -n -n Hola que"
make_a_test "echo -n"
make_a_test "echo -nnnnnnnnnn"
make_a_test "echo -n -nnn -nnnnn"
make_a_test "echo -n-nnn -nnn"
make_a_test "echo --------n"
make_a_test "echo -nnn ---------n"
make_a_test "echo -nnn ------------nnn--------n"
make_a_test "echo -nnn ----------- -nnn--------n"
make_a_test "echo \$?"
make_a_test "sleep 1 |echo \$?"
make_a_test "echo $:$= | cat -e"
make_a_test "echo \" $ \" | cat -e"
make_a_test "echo ' $ ' | cat -e"
make_a_test 'echo $HOME'
make_a_test 'echo term is [$TERM]'
make_a_test 'echo term is [$TERM'
make_a_test 'echo term is [$TERM4]'
make_a_test 'echo $UID'
make_a_test 'echo $PWD'
make_a_test 'echo $HOME9'
make_a_test 'echo $HOME%'
make_a_test 'echo $UID$HOME'
make_a_test 'echo le path de mon HOME est $HOME'
make_a_test 'echo $"HO""ME"'
make_a_test 'echo $"HO""ME"'
make_a_test 'echo "$"HOME'
make_a_test 'echo "$"HOLA'

printf '\n\033[15`%s\n' "Test export built-in"
make_a_test "export HOLA=bonjour"
make_a_test "export HOLA=bonjour" "env | grep HOLA"
make_a_test "export Hola" "env | grep HOLA"
make_a_test "export Hola9Hey" "env | grep Hola9Hey"
make_a_test "export \$DONTEXIST | grep LOGNAME"
make_a_test "export | grep \"HOME\""
make_a_test "export \"\""
make_a_test "export ="
make_a_test "export %"
make_a_test "export \$?"
make_a_test "export ?=2"
make_a_test "export 9HOLA="
make_a_test "export HOLA9=bonjour" "env | grep HOLA9"
make_a_test "export ___HOLA=Bonjour" "env | grep __HOLA"
make_a_test "export _HO_LA_=bonjour" "env | grep _HO_LA_"
make_a_test "export HOL@=bonjour" "env | grep HOL@"
make_a_test "export -HOLA=bonjour" "env | grep -HOLA"
make_a_test "export --HOLA=bonjour" "env | grep --HOLA"
make_a_test "export HOLA-=bonjour"
make_a_test "export HO-LA=bonjour"
make_a_test "export HOL.A=bonjour"
make_a_test "export HOL}A=bonjour"
make_a_test "export HOL{A=bonjour"
make_a_test "export HOLA=bonjour" "env | grep HOLA9"
make_a_test "export HO*LA=bonjour" "env | grep HOLA9"
make_a_test "export HO#LA=bonjour" "env | grep HOLA9"
make_a_test "export HO@LA=bonjour" "env | grep HOLA9"
make_a_test "export HO\$?LA=bonjour"
make_a_test "export +HOLA=bonjour"
make_a_test "export HOL+A=bonjour"
make_a_test "export HOLA =bonjour"
make_a_test "export HOLA=bon jour" "env | grep HOLA"
make_a_test "export HOLA=bonjour" "env | grep HOLA"
make_a_test "export HOLA=bonjour" "export HOLA=bonretour" "export HOLA=bonjour2" "env | grep HOLA"
make_a_test "export HOLA=bonjour\$HOME" "env | grep HOLA"
make_a_test "export HOLA=bon\$jour" "env | grep HOLA"
make_a_test "export HOLA=bon@jour" "env | grep HOLA"
make_a_test "export HOLA=bon;jour" "env | grep HOLA"
make_a_test "export HOLA=bon!jour" "env | grep HOLA"
make_a_test "export HOLA=bon\"jour\"" "env | grep HOLA"
make_a_test "export HOLA\$USER=bonjour" "env | grep HOLA"
make_a_test "export HOLA=bonjour=casse-toi"" echo[ \$HOLA"
make_a_test "export \"HOLA=bonjour\"=casse-toi" "echo \$HOLA"
make_a_test "export HOLA=bonjour" "export BYE=casse-toi" "echo \$HOLA et \$BYE"
make_a_test "export HOLA=bonjour BYE=casse-toi" "echo \$HOLA et \$BYE"
make_a_test "export A=a B=b C=c" "echo \$A \$B \$C"
make_a_test "\$HOLA=bonjour" "env | grep \$HOLA"
make_a_test "export HOLA=\"bonjour      \"" "echo \$HOLA | cat -e"
make_a_test "export HOLA=\"   -n bonjour\"" "echo \$HOLA"
make_a_test "export HOLA=\"bonjour	\"" "echo \$HOLA"
make_a_test "export HOL>A='\"'" "echo \" \$HOLA \" | cat -e"
make_a_test "export HOLA=at c\$HOLA test_file/litle"
make_a_test "export \"\" HOLA=bonjour" "env"
make_a_test "export HOLA=\"cat Makefile | grep NAME\"" "echo \$HOLA"
make_a_test "export HOLA=hey" "echo \$HOLA\$HOLA\$HOLA=hey\$HOLA"
make_a_test "export HOLA=\"  bonjour  hey  \"" "echo \$HOLA | cat -e"
make_a_test "export HOLA=\"  bonjour  hey  \"" "echo \"\"\"\$HOLA\"\"\" | cat -e"
make_a_test "export HOLA=\"  bonjour  hey  \"" "echo wesh\"\$HOLA\" | cat -e"
make_a_test "export HOLA=\"  bonjour  hey  \"" "echo wesh\"\"\$HOLA."
make_a_test "export HOLA=\"  bonjour  hey  \"" "echo wesh$\"\"HOLA."
make_a_test "export HOLA=\"  bonjour  hey  \"" "echo wesh$\"HOLA HOLA\"."
make_a_test "export HOLA=bonjour" "export HOLA=\" hola et \$HOLA\"" "echo \$HOLA"

printf '\n\033[15`%s\n' "Test pwd built-in"
make_a_test "pwd"
make_a_test "pwd | cat -e"
make_a_test "cd /tmp" "pwd | cat -e"
make_a_test "cd ~" "pwd | cat -e"
make_a_test "cd ~" "cd /tmp" "cd -" "pwd | cat -e"
#make_a_test "export HOLA=bonjour" "export HOLA=' hola et \$HOLA'" "echo \$HOLA"
#make_a_test "export HOLA=bonjour" "export HOLA=\" hola et \$HOLA\"\$HOLA" "echo \$HOLA"
make_a_test "export HOLA=\"ls        -l    - a\"" "echo \$HOLA"
make_a_test "export HOLA=\"s -la\"" "l\$HOLA"
make_a_test "export HOLA=\"s -la\"" "l\"\$HOLA\""
make_a_test "export HOLA=\"s -la\"" "l'\$HOLA'"
make_a_test "export HOLA=\"l\"" "\$HOLAs"
make_a_test "export HOLA=\"l\"" "\"\$HOLA\"s"
make_a_test "export HOL=A=bonjour" "env"
make_a_test "export HOLA=\"l\"" "'\$HOLA's"
make_a_test "export HOL=A=\"\"" "env"
make_a_test "export TE+S=T"
make_a_test "export \"\"=\"\""
make_a_test "export \"\"=\"\""
make_a_test "export ''=''"
make_a_test "export \"=\"=\"=\""
make_a_test "export '='='='"
make_a_test "export HOLA=p" "export BYE=w" "\$HOLA\"BYE\"d"
make_a_test "export HOLA=p" "export BYE=w" "\"\$HOLA\"'\$BYE'd"
make_a_test "export HOLA=p" "export BYE=w" "\"\$HOLA\"\"\$BYE\"d"
make_a_test "export HOLA=p" "export BYE=w" "\$\"HOLA\"$\"BYE\"d"
make_a_test "export HOLA=p" "export BYE=w" "$'HOLA'$'BYE'd\""
make_a_test "export HOLA=-n" "echo $\HOLA\" hey"
make_a_test "export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8" "echo \"\$A'\$B\"'\$C\"\$D'\$E'\"\$F'\"'\$G'\$H\""

printf "\nTEST BUILT-IN\n"
printf '\n\033[15`%s\n' "Exec cmd"
make_a_test "ls /tmp"
make_a_test "ls ~"
make_a_test "ls /dev/null"
make_a_test "ls ////////"
make_a_test "sleep 1"
make_a_test "pwd"
make_a_test "pwdd"
make_a_test "ls -la"
make_a_test "ls /tmp -la"
make_a_test "ls -la /tmp"
make_a_test "exit"
make_a_test ">cat test_file/litle"
make_a_test "cat test_file/medium"
make_a_test "cat test_file/big"
make_a_test "cat test_file/litle -e"
make_a_test "cat -e test_file/medium"
make_a_test "cat test_file/big -e"
make_a_test "cat -e test_file/bible.txt"
make_a_test "cat -e test_file/bible.txt <test_file/litle"
make_a_test "cat test_file/bible.txt <test_file/litle -e"
make_a_test "cat <test_file/litle -e test_file/bible.txt "
make_a_test "cat <test_file/litle -e test_file/litle <test_file/medium"
make_a_test "ls test_file/"

printf '\n\033[15`%s\n' "Exec pipeline"
make_a_test "cat test_file/litle | cat -e"
make_a_test "cat test_file/litle | lol"
make_a_test "lol | mdr | ptdr"
make_a_test "ls | mdr | ptdr"
make_a_test "ls | sleep 1 | ptdr"
make_a_test "mdr | sleep 1 | ls"
make_a_test "ls|cat"
make_a_test "ls|cat|cat-e"
make_a_test "ls|cat|cat -e"
make_a_test "cat test_file/litle -e | cat -e"
make_a_test "cat test_file/litle -e | ls test_file/"
make_a_test "cat test_file/litle -e <test_file/bible.txt | ls test_file/"
make_a_test "cat test_file/litle -e <fake_redir <test_file/bible.txt | ls test_file/"
make_a_test "cat test_file/litle -e <test_file/bible.txt <fake_redir | ls test_file/"
make_a_test "cat test_file/litle >/dev/null| ls test_file/"
make_a_test "cat test_file/litle >/dev/full "

printf '\n\033[15`%s\n' "Exec unset"
make_a_test "unset"
make_a_test "unsset"
make_a_test "unsett"
make_a_test "uunset"
make_a_test "export HOLA=a" "echo \$HOLA" "unset \$HOLA" "echo \$HOLA"
make_a_test "export HOLA=a" "unset HOLA" "echo \$HOLA"
make_a_test "HOLA=mdr" "export HOLA" "unset HOLA" "echo \$HOLA"
make_a_test "HOLA=mdr" "export \$HOLA" "unset HOLA" "echo \$HOLA"
make_a_test "HOLA=mdr" "export \$HOLA" "unset \$HOLA" "echo \$HOLA"
make_a_test "HOLA=mdr" "echo \$HOLA | cat -e"
make_a_test "HOLA=mdr" "echo \$HOLA | cat -e | unset \$HOLA"
make_a_test "HOLA=mdr" "echo \$HOLA | cat -e | unset \$HOLA | echo \$HOLA" "echo \$HOLA"
make_a_test "HOLA=mdr" "echo \$HOLA | cat -e | unset HOLA | echo \$HOLA" "echo \$HOLA"
make_a_test "HOLA=mdr" "echo \$HOLA | cat -e | unset HOLA" "echo \$HOLA"

printf "\n"
rm valgrind_log output typescript -rf

# _   _ _ _           _ _     _            _
#| | | | (_)_ __ ___ (_) |_  | |_ ___  ___| |_
#| | | | | | '_ ` _ \| | __| | __/ _ \/ __| __|
#| |_| | | | | | | | | | |_  | ||  __/\__ \ |_
# \___/|_|_|_| |_| |_|_|\__|  \__\___||___/\__|
printf "\nTEST ulimit\n"
printf '\n\033[15`%s\n' "limited memory"
text=$(cat test_file/litle)
n=3000
while [ $n -le 4000 ]; do
	(
		ulimit -v "$n"
		./Minishell <<EOF
		 $text
EOF
	)
	((n = n + 50))
done
printf '\n\033[15`%s\n' "limited filedescriptor"

n=5
while [ $n -le 20 ]; do
	(
		ulimit -n "$n"
		./Minishell <<EOF
		ls | cat -e | cat -e
EOF
	)
	(
		ulimit -n "$n"
		./Minishell <<EOF
		ls | cat <test_file/litle <test_file/bible.txt | cat <test_file/litle.txt >/dev/stdout 1>/dev/stdout
EOF
	)
	(
		ulimit -n "$n"
		./Minishell <<EOF
		ls | cat | cat
EOF
	)
	((n = n + 1))
done
rm Minishell valgrind_log output typescript A=\" cat -rf
exit
