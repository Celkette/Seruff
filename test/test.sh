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
VALGRIND='valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all --suppressions=valgrind.supp --log-file=valgrind_log/memoire.log'
VALGRIND2='valgrind --trace-children=yes --track-fds=yes --suppressions=valgrind.supp --log-file=valgrind_log/fd.log'
ERROR=0
LEAK=0

cmp_to_bash_posix() {
	./Minishell 1>$OUT_MINISHELL <<EOF
$@
EOF
	bash --posix -i >$OUT_BASH 2>&1 <<EOF
$@
EOF
	cp $OUT_MINISHELL $OUT_MINISHELL2
	cp $OUT_BASH $OUT_BASH2
	sed -i -e 's/minishell0-0//' $OUT_MINISHELL
	sed -i -e 's/minishell//' $OUT_MINISHELL
	sed -i -e 's/bash-5.1//' $OUT_BASH
	sed -i -e 's/bash-5.2//' $OUT_BASH
	sed -i -e 's/bash//' $OUT_BASH
	sed -i -e 's/bash://' $OUT_BASH
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
	$VALGRIND ./Minishell >/dev/null <<EOF
$@
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
	$VALGRIND2 ./Minishell >/dev/null <<EOF
$@
EOF
	if ! grep -sq 'Open file descriptor' valgrind_log/fd.log; then
		printf "$GREEN"
		printf '\033[92`%s' "OK"
		printf "$WHITE"
	else
		printf "$RED"
		printf '\033[92`%s' "KO"
		printf "$WHITE"
	fi
}

make_a_test2() {
	cmp_to_bash_posix $1
	#check_mem_leak $1
	#check_fd_leak $1
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

make_a_test() {
	printf "\n$BLUE$1$WHITE"
	printf '\033[65`%s' "DIFF"
	printf '\033[76`%s' "MEM_LEAK"
	printf '\033[89`%s\n' "FDS_LEAK"
	cmp_to_bash_posix $1
	check_mem_leak $1
	check_fd_leak $1
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
	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output
}

clear
printf "Compil minishell in parent ...\n"
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

OLDPWD=$(pwd)
export OLDPWD
clear

printf "TEST Lexing \n"
printf '\n\033[15`%s\n' "Basic lexing"
#make_a_test ""
#make_a_test "
#"
#make_a_test " "
#make_a_test "	"
#make_a_test ":"
#make_a_test "!"
#make_a_test "-"
#make_a_test "&&"
#make_a_test "&&&&&"
#make_a_test "&&&&&&&&&&&&&&&&"
printf '\n\033[15`%s\n' "Pipe lexing"
make_a_test "|"
make_a_test "| hola"
make_a_test "| | |"
make_a_test "||"
make_a_test "|||||"
make_a_test ""
make_a_test2 "\"\"\echo hola\"\"'''' que\"\"'' tal\"\"''"
printf '\n\033[15`%s\n' "Redirection lexing"
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
make_a_test "<"
make_a_test "< <"
make_a_test "< <<"
make_a_test "< <<<"
make_a_test "< <<<<"
make_a_test "< <<<<<"
make_a_test "<  "
make_a_test "<	 "
make_a_test ">>"
make_a_test "<<"
make_a_test "<>"
make_a_test "><"
make_a_test ">><"
make_a_test ">><<"
make_a_test "><<"
make_a_test "><<<"
make_a_test ">><<<"
make_a_test ">>>>>"
make_a_test ">>>>>>>>>>>>>>>"
make_a_test "<<<<<"
make_a_test "<<<<<<<<<<<<<<<<"
make_a_test "> > > >"
make_a_test ">> >> >> >>"
make_a_test ">>>> >> >> >>"
make_a_test "< < < <"
make_a_test "<< << << <<"
make_a_test "<<<< << << <<"
make_a_test ">>> >"
make_a_test "<<< >"
make_a_test ">>> >>"
make_a_test "<<< <"
printf "\n"

#rm Minishell valgrind_log output typescript -rf
#exit

#printf "\nTEST BUILT-IN\n"
#printf '\033[15`%s\n' "Test cd built-in"
#make_a_test "cd"
#make_a_test "cd .."
#make_a_test "cd /tmp"
#make_a_test "cd dsfa"
#make_a_test "cd -"
#make_a_test "cd -~"
#make_a_test "cd -/~"
#make_a_test "cd -/tmp"
#make_a_test "cd ~"
#make_a_test "cd ~/Downloads"
#make_a_test "cd ~Downloads"
#make_a_test "cd ~../../"
#make_a_test "cd ~../"
#make_a_test "cd ~/.."
#make_a_test "cd ~/../"
#make_a_test "cd /dev/tty"
#make_a_test "cd /dev/stdout"
#make_a_test "cd /dev/stdin"
#make_a_test "cd /dev/stderr"
#make_a_test "cd /dev/null"
#make_a_test "cd /dev/full"
#make_a_test "cd /dev/fd/0"
#make_a_test "cd /dev/char"
#make_a_test "cd /dev/bus"
printf "\n"
printf '\n\033[15`%s\n' "Exec cmd"
make_a_test "cat test_file/litle"
make_a_test "cat test_file/medium"
make_a_test "cat test_file/big"
make_a_test "cat test_file/bible.txt"
make_a_test "cat test_file/litle -e"
make_a_test "cat -e test_file/medium"
make_a_test "cat test_file/big -e"
make_a_test "cat -e test_file/bible.txt"
make_a_test "cat -e test_file/bible.txt <test_file/litle"
make_a_test "cat test_file/bible.txt <test_file/litle -e"
make_a_test "cat <test_file/litle -e test_file/bible.txt "
make_a_test "cat <test_file/litle -e test_file/bible.txt <test_file/medium"
make_a_test "ls test_file/"

printf '\n\033[15`%s\n' "Exec pipeline"
make_a_test "cat test_file/litle | cat -e"
make_a_test "cat test_file/litle -e | cat -e"
make_a_test "cat test_file/litle -e | ls test_file/"
make_a_test "cat test_file/litle -e <test_file/bible.txt | ls test_file/"
make_a_test "cat test_file/litle -e <fake_redir <test_file/bible.txt | ls test_file/"
make_a_test "cat test_file/litle -e <test_file/bible.txt <fake_redir | ls test_file/"
make_a_test "cat test_file/litle >/dev/stdin | ls test_file/"
make_a_test "cat test_file/litle >/dev/null| ls test_file/"
make_a_test "cat test_file/litle >/dev/full | ls test_file/"
rm Minishell valgrind_log output typescript -rf
exit

make_a_test2 "\"\"\echo hola\"\"'''' que\"\"'' tal\"\"''"
printf '\n\033[15`%s\n' "Redirection lexing"

printf "\n"
rm Minishell valgrind_log output typescript -rf

exit
