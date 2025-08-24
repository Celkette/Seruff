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
mkdir -p output
OUT_MINISHELL_CMDS="output/fd1_minishell_cmds" #/AJOUT
OUT_MINISHELL_TMP="output/fd1_minishell_tmp" #AJOUT
cmp_to_bash_posix() {
	timeout 2s ./Minishell >$OUT_MINISHELL 2>&1 0>&1 <<EOF
$(for arg in "$@"; do
		echo "$arg"
	done)
EOF
	bash --posix -i >$OUT_BASH 2>&1 <<EOF
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
	sed -i -e 's/bash-5.1//' $OUT_BASH
	sed -i -e 's/bash-5.2//' $OUT_BASH
	sed -i -e 's/bash-5.0//' $OUT_BASH
	sed -i -e 's/bash://' $OUT_BASH
	sed -i -e 's/bash//' $OUT_BASH
    sed -i -e 's/$ //' $OUT_BASH

	## Affiche la sortie de chaque shell
	#echo -e "--- Bash output ---"
	#cat $OUT_BASH
	#echo -e "\n--- Minishell output ---"
	#cat $OUT_MINISHELL

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
	#check_fd_leak "$@"
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
	printf "\n$BLUE$1$WHITE"
	printf '\033[65`%s' "DIFF"
	printf '\033[76`%s' "MEM_LEAK"
	printf '\033[89`%s\n' "FDS_LEAK"
	cmp_to_bash_posix "$@"
	check_mem_leak "$@"
	#check_fd_leak "$@"
	
	if [ "$ERROR" == "1" ]; then
		ERROR=0
		printf "$RED\n====================================\n||		DIFF  		  ||\n====================================$WHITE\n"
		RESULT_MINISHELL=$(tail -n 1 "$OUT_MINISHELL")
		RESULT_BASH=$(tail -n 1 "$OUT_BASH")
# On retire toutes les lignes "exit" et celles correspondant aux commandes tapées
grep -v -x "exit" "$OUT_BASH" > "${OUT_BASH}_filtered"
grep -v -x "exit" "$OUT_MINISHELL" > "${OUT_MINISHELL}_filtered"

for arg in "$@"; do
    grep -v -x "$arg" "${OUT_BASH}_filtered" > "${OUT_BASH}_filtered2"
    mv "${OUT_BASH}_filtered2" "${OUT_BASH}_filtered"
    grep -v -x "$arg" "${OUT_MINISHELL}_filtered" > "${OUT_MINISHELL}_filtered2"
    mv "${OUT_MINISHELL}_filtered2" "${OUT_MINISHELL}_filtered"
done

RESULT_MINISHELL=$(tail -n 1 "${OUT_MINISHELL}_filtered")
RESULT_BASH=$(tail -n 1 "${OUT_BASH}_filtered")
		if [ "$RESULT_MINISHELL" = "$RESULT_BASH" ]; then
			printf "$GREEN%s$WHITE\n" "OK"
		else
			printf "$RED%s$WHITE\n" "KO"
			echo "--- Minishell output ---"
			echo "$RESULT_MINISHELL"
			echo "--- Bash output ---"
			echo "$RESULT_BASH"
		fi
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	if [ "$LEAK" == "1" ]; then
		LEAK=0
		printf "$RED\n====================================\n||		LEAK  		  ||\n====================================$WHITE\n"
		cat valgrind_log/memoire.log | sed 's/==[0-9]\{1,\}==//' | grep -e "by [0-9]x" -e bytes -e "Conditional" -e "Leak" -e "Invalid read" -e "Invalid write" -e "Mismatched" -e "LEAK SUMMARY:" -e "HEAP SUMMA"
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	#if [ "$LEAK_FD" == "1" ]; then
	#	LEAK_FD=0
	#	printf "$RED\n====================================\n||		LEAK_FD		  ||\n====================================$WHITE\n"
	#	cat valgrind_log/fd.log
	#	printf "$RED====================================\n||		END_FD 		  ||\n====================================$WHITE\n"
	#fi

	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output
}

make_a_test_ONE() {
	printf "\n$BLUE$1$WHITE"
	printf '\033[65`%s' "DIFF"
	printf '\033[76`%s' "MEM_LEAK"
	printf '\033[89`%s\n' "FDS_LEAK"
	cmp_to_bash_posix "$@"
	check_mem_leak "$@"
	#check_fd_leak "$@"
	if [ "$ERROR" == "1" ]; then
		ERROR=0
		printf "$RED\n====================================\n||		DIFF  		  ||\n====================================$WHITE\n"

		#diff $OUT_BASH $OUT_MINISHELL
		diff <(sort $OUT_BASH) <(sort $OUT_MINISHELL)
		echo "--- Minishell output (sorted) ---"
		sort $OUT_MINISHELL
		echo "--- Bash output (cat -A) ---"
		sort $OUT_BASH
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	if [ "$LEAK" == "1" ]; then
		LEAK=0
		printf "$RED\n====================================\n||		LEAK  		  ||\n====================================$WHITE\n"
		cat valgrind_log/memoire.log | sed 's/==[0-9]\{1,\}==//' | grep -e "by [0-9]x" -e bytes -e "Conditional" -e "Leak" -e "Invalid read" -e "Invalid write" -e "Mismatched" -e "LEAK SUMMARY:" -e "HEAP SUMMA"
		printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	fi
	#if [ "$LEAK_FD" == "1" ]; then
	#	LEAK_FD=0
	#	printf "$RED\n====================================\n||		LEAK_FD		  ||\n====================================$WHITE\n"
	#	cat valgrind_log/fd.log
	#	printf "$RED====================================\n||		END_FD 		  ||\n====================================$WHITE\n"
	#fi

	rm valgrind_log output typescript -rf
	mkdir valgrind_log
	mkdir output
}

make_a_test_multi() {
	# $1 doit être une chaîne contenant plusieurs lignes de commandes, séparées par \n
	printf "\n$BLUE$1$WHITE"
	printf '\033[65`%s' "DIFF"
	printf '\033[76`%s' "MEM_LEAK"
	printf '\033[89`%s\n' "FDS_LEAK"
	#echo $'export\tHOLA=bonjo' | hexdump -C
	#echo "Chaîne envoyée à Minishell (hex):"
	#echo "$1" | hexdump -C
	cmp_to_bash_posix "$1" #"$1" contient toute la chaîne multi-lignes (avec des vrais retours à la ligne)
	#PENSER  A ECRIRE make_a_test_multi $'xxxxx'($ indispensable)
	check_mem_leak "$1" # memory leak
	#check_fd_leak "$1" # fd leak
	#if [ "$ERROR" == "1" ]; then
	#	ERROR=0
	#	printf "$RED\n====================================\n||		DIFF  		  ||\n====================================$WHITE\n"
	#	diff <(sort $OUT_BASH) <(sort $OUT_MINISHELL)
	#	echo "--- Minishell output (sorted) ---"
	#	sort $OUT_MINISHELL
	#	echo "--- Bash output (sorted) ---"
	#	sort $OUT_BASH
	#	printf "$RED====================================\n||		END 		  ||\n====================================$WHITE\n"
	#	# Affichage des sorties (optionnel)
	#	echo -e "\n--- Minishell output ---"
	#	cat $OUT_MINISHELL
	#	echo -e "--- Bash output ---"
	#	cat $OUT_BASH
	#	# Ajoute ici pour voir les différences invisibles
	#	#echo -e "\n--- DIFF (caractères invisibles) ---"
	#	#diff -u <(cat -A $OUT_BASH) <(cat -A $OUT_MINISHELL)
	#fi
	# On retire toutes les lignes "exit" et celles correspondant aux commandes tapées
grep -v -x "exit" "$OUT_BASH" > "${OUT_BASH}_filtered"
grep -v -x "exit" "$OUT_MINISHELL" > "${OUT_MINISHELL}_filtered"

for arg in "$@"; do
    grep -v -x "$arg" "${OUT_BASH}_filtered" > "${OUT_BASH}_filtered2"
    mv "${OUT_BASH}_filtered2" "${OUT_BASH}_filtered"
    grep -v -x "$arg" "${OUT_MINISHELL}_filtered" > "${OUT_MINISHELL}_filtered2"
    mv "${OUT_MINISHELL}_filtered2" "${OUT_MINISHELL}_filtered"
done

RESULT_MINISHELL=$(tail -n 1 "${OUT_MINISHELL}_filtered")
RESULT_BASH=$(tail -n 1 "${OUT_BASH}_filtered")
		if [ "$RESULT_MINISHELL" = "$RESULT_BASH" ]; then
			printf "$GREEN%s$WHITE\n" "OK"
		else
			printf "$RED%s$WHITE\n" "KO"
			echo "--- Minishell output ---"
			echo "$RESULT_MINISHELL"
			echo "--- Bash output ---"
			echo "$RESULT_BASH"
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

clear
OLDPWD=$(pwd)
export OLDPWD
clear


##make_a_test 'echo $USER'    #recoit  echo $USER
##make_a_test "echo \$USER"   #recoit  echo $USER
##	make_a_test 'echo \$USER'  #recoit  echo \$USER
##	make_a_test "echo $USER"    #recoit  echo celine


#printf "TEST CHAINES VIDES, QUOTES, IMBRICATIONS, TABULATIONS, SAUTS DE LIGNE, ESPACES\n" Chaînes vides
#make_a_test ""
#make_a_test ''
#make_a_test "   "        # uniquement des espaces
#printf "# Chaînes vides avec des quotes"
#make_a_test '""'  #recoit  ""		DIFF
#make_a_test "''"  #recoit  ''		DIFF
#make_a_test "' '" #recoit  ' '
#make_a_test "\" \"" #recoit  " "

#printf "# Quotes imbriquées"
#make_a_test "'\"\"'"	 #recoit  '""'
#make_a_test "\"''\""	 #recoit  "''"
#make_a_test "'\"'\"'\""	 #recoit  '"'"'"'
#make_a_test "\"'\"\"'\"" #recoit  "'""'"

#printf "# Chaînes avec tabulation\n"
#	make_a_test $'\t' # recoit  \t
#	make_a_test $'	' # recoit  Tab
#	make_a_test $"	" # recoit  Tab
#	make_a_test $"\t" # recoit  Tab
#make_a_test $"echo '\t'" # recoit  Tab
#make_a_test "'\t'" # recoit  '\t'
#make_a_test '"\t"' # recoit  "\t"
#	make_a_test $' \t ' # recoit space tab space
#	make_a_test $'\t \t' # recoit tab space tab

#printf "# Chaînes avec saut de ligne\n" # SI entre "" n'echappe pas
#make_a_test $"echo '\n'" # sera littéralement echo \n
#make_a_test $"'\n\'" # littéralement "\n"
#make_a_test '"\n"' # littéralement "\n"
#make_a_test $'\n' #vrai retour à la ligne (caractère newline)			==> ne passe pas sans herdoc
#make_a_test $'echo \n' # echo vrai retour à la ligne
#make_a_test $''\''\n'\''' # 'vrai retour'									=> MISS QUOTE
#	make_a_test 'echo -e "\n"' # echo vrai retour à la ligne
#make_a_test $' \n '      # space vrai retour space						==> ne passe pas sans herdoc
#make_a_test $'echo '\''\n'\'\'''\'' '    # space 'vrai retour' space			==> ok sauf avec testeur miss quote
#make_a_test $'echo '\''\n'\' \''\n'\''' # 'vrai retour' space 'vrai retour'	==> ok sauf avec testeur miss quote

#printf "# Quotes imbriquées avec tabulation et saut de ligne\n"
# #LES TESTER EN DIRECT, NE MARCHE PAS AVEC LE TESTEUR avec ctr+v Tab et ctrl+v ctrl+j
#	make_a_test $'"	 \'\n "'				# recoit "Tab space '\n "
#	make_a_test $'\' 	"\n \''					# recoit 'space Tab"\"n'
#	make_a_test $'"\n"	\' '					# recoit "\n"Tab\'space
#	make_a_test $'\'\'""	\n  \'\'""   '		# recoit -''""Tab\nspace space''""space space space


#printf "# Espaces + quotes imbriquées\n"
#make_a_test "' \" \" '"
#make_a_test "\" ' ' \""
#make_a_test "   \"''\"   "
#make_a_test "   '\"\"'   "
#	make_a_test " ' ' \" \" "
#printf "# Mélanges extrêmes\n"

#printf "\nTEST QUOTE: GESTION DES ERREURS DE SYNTAXE\n"
#make_a_test "echo ''"                                    # quotes simples vides, doit passer
#make_a_test 'echo ""'                                  # quotes doubles vides, doit passer
#make_a_test "echo '\"'\"==> DIFF OK"                       # quote simple ouverte, quote double ouverte, quote simple fermée, quote double fermée (doit passer)
#make_a_test "echo '\"\"==> DIFF OK"                        # quote simple ouverte, deux quotes doubles ouvertes, simple non fermée
#make_a_test '"'\'\''==> DIFF OK' # "''"
#make_a_test "echo 'unclosed simple debut ==> DIFF OK"      # quote simple non fermée (erreur attendue)
#make_a_test "echo unclosed simple fin ==> DIFF' OK"
#make_a_test 'echo "unclosed double debut ==> DIFF OK'      # quote double non fermée (erreur attendue)
#make_a_test 'echo unclosed double fin ==> "DIFF OK'
#make_a_test "echo 'text \"unclosed simple inside double\" ==> DIFF OK" #
#make_a_test 'echo "text '\''unclosed simple inside simple'\'' ==> DIFF OK' #
#make_a_test "echo text \"unclosed double\" inside double\" ==> DIFF OK " #
#make_a_test 'echo "text '\''unclosed double\" inside simple'\'' ==> DIFF OK' #

#printf "\nTEST QUOTES SEULES\n"
#make_a_test "echo 'simple quote'"           # simple quotes, affichage littéral
#make_a_test "echo \"double quote\""         # double quotes, affichage littéral
#make_a_test "echo 'simple' \"double\""      # mélange simple et double quotes
#make_a_test "echo \"double 'nested simple' quote\""  # simple quote à l'intérieur de double quotes
#make_a_test "echo 'simple \"nested double\" quote'"  # double quote à l'intérieur de simple quotes
#make_a_test "echo ''"                       # quotes simples vides
#make_a_test "echo \"\""                     # quotes doubles vides
#make_a_test "echo '   '"                    # espaces dans quotes simples
#make_a_test "echo \"   \""                  # espaces dans quotes doubles
#make_a_test "echo 'text with spaces'"       # texte avec espaces dans quotes simples
#make_a_test "echo \"text with spaces\""     # texte avec espaces dans quotes doubles
#make_a_test "echo 'text \"with\" double inside simple'"   # double quotes dans simple
#make_a_test "echo \"text 'with' simple inside double\""   # simple quotes dans double
#make_a_test "echo 'multiple '' simple quotes'"            # quotes simples consécutives
#make_a_test "echo \"multiple \"\" double quotes\""         # quotes doubles consécutives
#make_a_test "echo \"one\"'two'\"three\"'four'"            # alternance quote/simple/double
#make_a_test "echo \"text '   ' with spaces\""    # simple quote à l'intérieur de double avec espaces

#printf "\nTEST QUOTES + TABULATIONS\n"
#make_a_test "echo  '\tsimple\t'"                    # quotes simples avec tabulations autour et dedans
#make_a_test "echo \"\tdouble\t\""                   # quotes doubles avec tabulations autour et dedans
#make_a_test "echo '\tsimple' \"\tdouble\""          # chaque quote contient des tabulations à gauche
#make_a_test "echo 'simple\t' \"double\t\""          # chaque quote contient des tabulations à droite
#make_a_test "echo  '\t'   \"\t\""                   # quotes vides contenant uniquement des tabulations
#make_a_test "echo '\tsimple\t'\"\tdouble\t\""       # quotes collées, chacune avec tabulations
#make_a_test "echo 'sim\tple' \"dou\tble\""          # tabulation à l’intérieur du texte dans chaque quote
#make_a_test "echo   'simple'    \"double\""         # tabulation hors des quotes, avant/entre/après
#make_a_test "echo    '\ts\t'    \"\td\t\""          # plusieurs tabulations hors et dans les quotes
#make_a_test "echo '\t' '\t'"                        # deux quotes simples contenant des tabulations
#make_a_test "echo \"\t\" \"\t\""                    # deux quotes doubles contenant des tabulations
#make_a_test "echo '\t'\"\t\""                       # quotes de types différents collées, chacune avec tabulations
#make_a_test "echo 'text'   \"with\"   'many'   \"tabs\"" # alternance quote/texte/tabulations
#make_a_test "echo '\ttext\t'\"\twith\t\""           # chaque quote contient tabulations et texte
#make_a_test "echo 'text \"\t\" with tabs'"          # double quote à l'intérieur de simple avec tabulations
#make_a_test "echo \"text '\t' with tabs\""          # simple quote à l'intérieur de double avec tabulations

#printf "\nTEST QUOTES + SAUTS DE LIGNE\n"
#make_a_test "echo  '\nsimple\n'"                    # quotes simples avec sauts de ligne autour et dedans
#make_a_test "echo \"\ndouble\n\""                   # quotes doubles avec sauts de ligne autour et dedans
#make_a_test "echo '\nsimple' \"\ndouble\""          # chaque quote contient des sauts de ligne à gauche
#make_a_test "echo 'simple\n' \"double\n\""          # chaque quote contient des sauts de ligne à droite
#make_a_test "echo  '\n'   \"\n\""                   # quotes vides contenant uniquement des sauts de ligne
#make_a_test "echo '\nsimple\n'\"\ndouble\n\""       # quotes collées, chacune avec sauts de ligne
#make_a_test "echo 'sim\nple' \"dou\nble\""          # saut de ligne à l’intérieur du texte dans chaque quote
#make_a_test "echo   'simple'    \"double\""         # saut de ligne hors des quotes, avant/entre/après
#make_a_test "echo    '\ns\n'    \"\nd\n\""          # plusieurs sauts de ligne hors et dans les quotes
#make_a_test "echo '\n' '\n'"                        # deux quotes simples contenant des sauts de ligne
#make_a_test "echo \"\n\" \"\n\""                    # deux quotes doubles contenant des sauts de ligne
#make_a_test "echo '\n'\"\n\""                       # quotes de types différents collées, chacune avec saut de ligne
#make_a_test "echo 'text'   \"with\"   'many'   \"lines\"" # alternance quote/texte/sauts de ligne
#make_a_test "echo '\ntext\n'\"\nwith\n\""           # chaque quote contient saut de ligne et texte
#make_a_test "echo 'text \"\n\" with newlines'"      # double quote à l'intérieur de simple avec sauts de ligne
#make_a_test "echo \"text '\n' with newlines\""      # simple quote à l'intérieur de double avec sauts de ligne

#printf "\nTEST QUOTES + MELANGE ESPACES / TAB / LIGNE\n"
#make_a_test "echo  ' \tsimple\n '"
#make_a_test "echo \" \tdouble\n \""
#make_a_test "echo '\tsimple \n' \"\tdouble \n\""
#make_a_test "echo 'sim ple\t\n' \"dou ble\t\n\""
#make_a_test "echo  ' \t\n '   \" \t\n \""
#make_a_test "echo ' \tsimple\t\n '\"\tdouble\t\n \""
#make_a_test "echo 'sim\t ple\n' \"dou\t ble\n\""
#make_a_test "echo   'simple\n'    \"double\t\""
#make_a_test "echo    '\ts\t\n'    \"\td\t\n\""
#make_a_test "echo '\t\n' ' \t\n '"
#make_a_test "echo \"\t\n\" \" \t\n \""
#make_a_test "echo ' \t\n '\"\t\n \""
#make_a_test "echo 'text\t\n'   \"with \t\n\"   'many \t\n'   \"spaces\t\n\""
#make_a_test "echo ' \t\n text \t\n '\"\t\n with \t\n \""
#make_a_test "echo 'text \"\t\n \" with spaces'"    # double quote à l'intérieur de simple avec tab/saut/espaces
#make_a_test "echo \"text '\t\n' with spaces\""    # simple quote à l'intérieur de double avec tab/saut/espaces

#printf "\nTEST PIPE: GESTION DES ERREURS DE SYNTAXE\n"
#make_a_test "|"                            # pipe seul, aucune commande
#make_a_test "| echo hello"                 # pipe en début de ligne
#make_a_test "echo 'hello ==> DIFF OK' |"                 # pipe en fin de ligne
#make_a_test "| |"                          # deux pipes consécutifs, aucune commande
#make_a_test "echo | | cat"                 # deux pipes consécutifs au milieu
#make_a_test "echo '==> DIFF OK' || cat"                  # double pipe, non supporté (erreur attendue)
#make_a_test "echo '==> DIFF OK' ||"                # double pipe en fin de ligne, non supporté
#make_a_test "echo '==> DIFF OK' ||| cat"                 # triple pipe, non supporté (erreur attendue)
#make_a_test "echo | | | cat"               # plusieurs pipes consécutifs, aucune commande entre
#make_a_test "echo '==> DIFF OK' |"                       # commande avant pipe, rien après
#make_a_test "| cat"                        # rien avant pipe, commande après
#make_a_test "echo |      | cat"            # pipes avec espaces entre, aucune commande entre
#make_a_test "echo | | |"                   # plusieurs pipes, aucune commande entre ni après

#printf "\nTEST PIPE\n"
#make_a_test "echo hello | cat -e"                  # pipe simple entre deux commandes
#make_a_test "echo hello | cat | cat"            # pipeline avec trois commandes
#make_a_test "echo a | echo b | cat"             # pipeline avec plusieurs commandes
#make_a_test "echo   hello   |   cat"            # pipe avec espaces superflus
#make_a_test "echo | cat"                        # pipe avec commande vide à gauche
#make_a_test "cat | echo"                        # pipe avec commande vide à droite
#make_a_test "echo | cat | cat | cat"            # pipeline de plusieurs commandes
#make_a_test "echo | cat | echo | cat"           # alternance de commandes dans pipeline
#make_a_test "echo hello|cat"                    # pipe sans espace autour
#make_a_test "echo hello |cat"                   # pipe sans espace à droite
#make_a_test "echo hello| cat"                   # pipe sans espace à gauche
#make_a_test "echo   hello   |cat   |   cat"     # pipeline avec espaces superflus

#printf "\nTEST PIPE + QUOTE\n"
#make_a_test "echo hello|'world'"					 # simple quote après pipe
#make_a_test 'echo hello|"world"'            		 # double quote après pipe
#make_a_test "echo 'hello world' | cat"              # simple quotes avant pipe
#make_a_test "echo \"hello world\" | cat"            # double quotes avant pipe
#make_a_test "echo 'hello' | echo \"world\""         # simple quote à gauche, double à droite
#make_a_test "echo \"hello\" | echo 'world'"         # double quote à gauche, simple à droite
#make_a_test "echo '  ' | cat"                       # quotes simples avec espaces, puis pipe
#make_a_test "echo \"  \" | cat"                     # quotes doubles avec espaces, puis pipe
#make_a_test "echo 'foo' | cat | echo 'bar'"         # pipeline avec quotes aux extrémités
#make_a_test "echo \"foo\" | cat | echo \"bar\""     # pipeline avec quotes doubles aux extrémités
#make_a_test "echo 'a' | echo 'b' | echo 'c'"        # pipeline avec quotes simples partout
#make_a_test "echo \"a\" | echo \"b\" | echo \"c\""  # pipeline avec quotes doubles partout
#make_a_test "echo 'pipe | in quotes'"               # pipe dans quotes simples (doit être pris comme texte)
#make_a_test 'echo "pipe | in quotes"'            # pipe dans quotes doubles (doit être pris comme texte)
#make_a_test "echo 'hello' | cat | cat"              # quotes simples et pipeline long
#make_a_test 'echo "hello" | cat | cat'            # quotes doubles et pipeline long
#make_a_test 'echo '\''|'\'' | cat'                        # caractère pipe dans les quotes simples, puis pipe réel
#make_a_test "echo \"|\" | cat"                      # caractère pipe dans les quotes doubles, puis pipe réel
#make_a_test "echo 'hello'|cat"                      # simple quote sans espace avant pipe
#make_a_test "echo \"hello\"|cat"                    # double quote sans espace avant pipe

#printf "\nTEST REDIRECTION: GESTION DES ERREURS DE SYNTAXE\n"
#make_a_test ">"                    # redirection sans cible (erreur)
#	make_a_test "<"                    # redirection sans source (erreur)
#make_a_test ">>"                   # append sans cible (erreur)
#	make_a_test "<<"                   # heredoc sans délimiteur (erreur)
#make_a_test "echo >"               # redirection de sortie sans cible (erreur)
#	make_a_test "echo <"               # redirection d’entrée sans source (erreur)
#make_a_test "echo >>"              # append sans cible (erreur)
#	make_a_test "echo <<"              # heredoc sans délimiteur (erreur)
#make_a_test "echo > >"             # deux redirections de sortie consécutives sans cible (erreur)
#	make_a_test "echo < <"             # deux redirections d’entrée consécutives sans source (erreur)
#make_a_test "echo >>>"             # redirection inconnue (erreur)
#	make_a_test "echo <<<"             # redirection inconnue (erreur)
#make_a_test "echo > >>"            # redirection mélange sans cible (erreur)
#	make_a_test "echo < <<"            # redirection mélange sans source (erreur)
#make_a_test "echo > < file"        # mélange incohérent de redirections (erreur)
#	make_a_test "echo < > file"        # mélange incohérent de redirections (erreur)				==> DIFF
#make_a_test "echo > file >"        # redirection valide suivie d’une incomplète (erreur)
#	make_a_test "echo < file <"        # idem entrée (erreur)
#make_a_test "echo > file >>"       # append incomplète après une redirection valide (erreur)
#	make_a_test "echo < file <<"       # heredoc incomplète après une redirection valide (erreur)
#make_a_test "echo >"               # redirection de sortie sans cible (erreur)
#	make_a_test "echo <"               # redirection d’entrée sans source (erreur)

#printf "TEST REDIRECTION\n"
#make_a_test "echo test > test_file/new_outfile1"						# redirection de sortie vers un nouveau fichier
#make_a_test "cat < test_file/infile1"									# redirection d’entrée depuis un fichier existant
#make_a_test "echo test >> test_file/outfile1"							# append (ajout) dans un fichier existant
#make_a_test "echo test > test_file/outfile1 > test_file/new_outfile2"	# redirections multiples, la dernière prend effet (nouveau fichier créé)
#make_a_test "cat < test_file/infile1 < test_file/infile2"				# redirections multiples d’entrée, la dernière prend effet (lecture dans infile2)
#make_a_test "echo test > test_file/new_outfile2 | cat"					# redirection vers nouveau fichier + pipe
#make_a_test "echo test >> test_file/outfile2 | cat"						# append dans fichier existant + pipe
#make_a_test "echo test > test_file/outfile2 >> test_file/new_outfile3"  # redirection puis append, la dernière prend effet (nouveau fichier créé)
#make_a_test "echo test > test_file/new_outfile3 < test_file/infile2"	# sortie vers nouveau fichier et entrée depuis fichier existant
#make_a_test "cat < test_file/infile2 > test_file/new_outfile4"			# entrée depuis fichier existant et sortie vers nouveau fichier
#make_a_test "cat < test_file/infile2 >> test_file/outfile1"				# entrée depuis fichier existant et append dans fichier existant
#	make_a_test "cat << EOF | cat"										# heredoc + pipe											=> BLOQUE LE TESTEUR
#	make_a_test "cat << EOF"											# heredoc avec délimiteur (EOF)								=> BLOQUE LE TESTEUR
#make_a_test "cat < test_file/infile1 | grep Nevada"	
#				 redirection d’entrée existante + pipe						=> MEM_LEAK DE GREP
#printf "TEST REDIRECTION: fichier inexistant\n"
#make_a_test "cat < test_file/inexistent-in"					# => erreur ok
#make_a_test "cat < test_file/inexistent-in > test_file/new_outfile1"	# entrée inexistante + sortie à créer
#make_a_test "cat < test_file/inexistent-in | grep test"					# entrée inexistante + pipe
#make_a_test "echo test >> test_file/inexistent-out"						# append dans un fichier qui n’existe pas (doit être créé)
#make_a_test "echo test > test_file/inexistent-out"							# redirection de sortie vers nouveau fichier (doit être créé)

#	printf "TEST EXPAND SEUL\n"
#make_a_test 'echo $USER'						# expansion de la variable USER
#make_a_test 'echo $HOME'						# expansion de la variable HOME
#make_a_test 'echo $PATH'						# expansion de la variable PATH
#make_a_test 'echo $?'						# expansion du dernier code de retour
#make_a_test 'echo $0'							# expansion du nom du shell/script
#make_a_test 'echo $NONEXISTENT'

#printf "TEST EXPAND + QUOTE\n"
#make_a_test 'echo "$USER"' 		# echo "$USER"      			# expansion dans double quotes
#make_a_test 'echo '\''$USER'\''' 	# echo '$USER'      		# pas d’expansion dans single quotes
#make_a_test 'echo '\''"$USER"'\''' #echo '"$USER"'
#make_a_test 'echo "'\''$USER'\''"' # echo "'$USER'"
#	make_a_test 'echo "Salut $USER !"' # echo "Salut $USER !" 		# expansion combinée dans double quotes
#make_a_test 'echo '\''$HOME $PATH'\''' # echo '$HOME $PATH'		# pas d’expansion dans single quotes
#make_a_test 'echo "$NONEXISTENT"'
#make_a_test  'echo "$U"SER'		# echo "$U"SER
#make_a_test  'echo $U"S"ER'		# echo $U"S"ER
#make_a_test  'echo $USER""'		# echo $USER""
#make_a_test  'echo "$"USER'		# echo "$"USER
#make_a_test  'echo $"US"ER'		# echo $"US"ER
#make_a_test  'echo $"U""SER"'	# echo $"U""SER"
#make_a_test 'echo '\''$U'\''SER'		# echo '$U'SER
#make_a_test 'echo $U'\''S'\''ER'		# echo $U'S'ER
#make_a_test 'echo $USER'\'''\'			# echo $USER''
#make_a_test 'echo '\''$'\''USER'		# echo '$'USER
#make_a_test 'echo $'\''US'\''ER'		# echo $'US'ER
#make_a_test 'echo $'\''U'\'''\''SER'\''' # echo $'U''SER'

#printf "TEST MULTI EXPAND\n"
#make_a_test 'echo "$"USER $"USER" "$USER" $U"SER" $U"SE"R $USER""'
#make_a_test 'echo $'\''USER'\'' '\''$USER'\'' $U'\''SER'\'' $U'\''SE'\''R '

#printf "TEST EXPAND +CMD\n"
#make_a_test "echo \"Mon user est \$USER\" > test_file/new_file-out1"   # expansion + redirection
#make_a_test "echo \"\$USER\" | wc -c"                                 # expansion + pipe
#make_a_test "echo \"\$USER\" | grep .u."                              # expansion + pipe + grep
#make_a_test "echo '\$USER' | wc -c"                                   # pas d’expansion + pipe
#make_a_test "echo \$USER \$HOME > test_file/new_file-out2"            # multiple expansions + redirection
#make_a_test "echo \"\$USER\" > test_file/new_file-out3 | cat"         # expansion + redirection + pipe (la redirection prend effet avant le pipe)
#make_a_test "echo \"\$USER\" >> test_file/new_file-out4"              # expansion + append
#make_a_test "echo \"\$USER\" > test_file/new_file-out5 < test_file/file-in1"   # expansion + redirection out + redirection in

#make_a_test 'echo lol'
#make_a_test_multi $'lol=celine\necho $lol'

#	printf "TEST EXPAND AVEC VAR DECLAREE"
#make_a_test_multi $'NAME=celine\necho $NAME'                       # variable simple
#make_a_test_multi $'NAME=celine\nSURNAME=dupond\necho $NAME $SURNAME'  # plusieurs variables
#make_a_test_multi $'NUMB=42\necho $NUMB'                           # variable numérique
#make_a_test_multi $'TEXT="bonjour"\necho $TEXT'                    # variable avec quotes
#make_a_test_multi $'NAME="celine"\necho $NAME'                   # variable avec double quotes
#make_a_test_multi $'EMPTY=\necho $EMPTY'                           # variable vide

#printf "TEST EXPAND AVEC VAR DECLAREE DANS VAR"
#	make_a_test_multi $'A=hello\nB="$A world"\necho $B'
#make_a_test_multi $'NAME=celine$NUMB\nNUMB=42\necho $NAME'
#make_a_test_multi $'NAME=celine$NUMB NUMB=42\necho $NAME'
#	make_a_test_multi $'X=yo\nY="$X man"\nZ=$Y!\necho $Z'

#printf "TEST DÉCLARATION VARIABLE ET APPEL\n"
#make_a_test_multi $'echo'
#make_a_test_multi $'echo $lo' # appel var inexistante

#make_a_test_multi $'lo=""' # declaration chaine vide
#make_a_test_multi $'lo=' # declaration de rien
#make_a_test_multi $'lo=""\necho $lo' # declaration et appel chaine vide
#make_a_test_multi $'lo=\necho $lo' # declaration et appel de rien
#make_a_test_multi $'lo=1\necho $lo' # declaration et appel 1 var

#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3' # declaration de 3 var separé
#make_a_test_multi $'lo1=1 lo2=2 lo3=3' # declaration de 3 var en une ligne

#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo' # declaration de 3 var et appel var inexistante
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo' # declaration de 3 var et appel var inexistante

#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo1' # declaration de 3 var sur plusieurs lignes et appel de lo1
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo2' # declaration de 3 var sur plusieurs lignes et appel de lo2
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo3' # declaration de 3 var sur plusieurs lignes et appel de lo3
#make_a_test_multi $'lo1=1 lo2=2 lo3=3 echo $lo1' # declaration de 3 var sur plusieurs lignes et appel de lo1
#make_a_test_multi $'lo1=1 lo2=2 lo3=3 echo $lo2' # declaration de 3 var sur plusieurs lignes et appel de lo2
#make_a_test_multi $'lo1=1 lo2=2 lo3=3 echo $lo3' # declaration de 3 var sur plusieurs lignes et appel de lo3
#make_a_test_multi $'lo1=1 lo2=2\nlo3=3\necho $lo1' # declaration de 3 var sur plusieurs lignes et appel de lo1
#make_a_test_multi $'lo1=1 lo2=2\nlo3=3\necho $lo2' # declaration de 3 var sur plusieurs lignes et appel de lo2
#make_a_test_multi $'lo1=1 lo2=2\nlo3=3\necho $lo3' # declaration de 3 var sur plusieurs lignes et appel de lo3
#make_a_test_multi $'lo1=1\nlo2=2 lo3=3\necho $lo1' # declaration de 3 var sur plusieurs lignes et appel de lo1
#make_a_test_multi $'lo1=1\nlo2=2 lo3=3\necho $lo2' # declaration de 3 var sur plusieurs lignes et appel de lo2
#make_a_test_multi $'lo1=1\nlo2=2 lo3=3\necho $lo3' # declaration de 3 var sur plusieurs lignes et appel de lo3

#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo1$lo2' # declaration de 3 var sur plusieurs lignes et appel de lo1 lo2
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo1$lo3' # declaration de 3 var sur plusieurs lignes et appel de lo1 lo3
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo2$lo3' # declaration de 3 var sur plusieurs lignes et appel de lo2 lo3
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo1$lo2' # declaration de 3 var une lignes et appel de lo1 lo2
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo1$lo3' # declaration de 3 var une lignes et appel de lo1 lo3
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo2$lo3' # declaration de 3 var une lignes et appel de lo2 lo3
#make_a_test_multi $'lo1=1 lo2=2\nlo3=3\necho $lo1$lo2' # declaration de 3 var sur plusieurs lignes et appel de lo1 lo2
#make_a_test_multi $'lo1=1 lo2=2\nlo3=3\necho $lo1$lo3' # declaration de 3 var sur plusieurs lignes et appel de lo1 lo3
#make_a_test_multi $'lo1=1 lo2=2\nlo3=3\necho $lo2$lo3' # declaration de 3 var sur plusieurs lignes et appel de lo2 lo3
#make_a_test_multi $'lo1=1\nlo2=2 lo3=3\necho $lo1$lo2' # declaration de 3 var sur plusieurs lignes et appel de lo1 lo2
#make_a_test_multi $'lo1=1\nlo2=2 lo3=3\necho $lo1$lo3' # declaration de 3 var sur plusieurs lignes et appel de lo1 lo3
#make_a_test_multi $'lo1=1\nlo2=2 lo3=3\necho $lo2$lo3' # declaration de 3 var sur plusieurs lignes et appel de lo2 lo3

#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo1$lo2$lo3' # declaration de 3 var sur plusieurs lignes et appel de 3 var ordre
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo1$lo2$lo3' # declaration de 3 var sur une ligne et appel de 3 var ordre
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo3$lo2$lo1' # declaration de 3 var sur plusieurs lignes et appel de 3 var desordre 1
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo1' # declaration de 3 var sur une ligne et appel de 3 var desordre 1
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo2$lo3$lo1' # declaration de 3 var sur plusieurs lignes et appel de 3 var desordre 2
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo2$lo3$lo1' # declaration de 3 var sur une ligne et appel de 3 var desordre 2
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo3$lo1$lo2' # declaration de 3 var sur plusieurs lignes et appel de 3 var desordre 3
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo1$lo2' # declaration de 3 var sur une ligne et appel de 3 var desordre 3
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo2$lo1$lo3' # declaration de 3 var sur plusieurs lignes et appel de 3 var desordre 4
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo2$lo1$lo3' # declaration de 3 var sur une ligne et appel de 3 var desordre 4

#make_a_test_multi $'lo1=1\necho $lo\nlo=2\necho $lo' # modif lo1
#make_a_test_multi $'lo1=1 lo2=2\necho $lo1\nlo1=2\necho $lo1' # declaration de 2 var et modif lo1
#make_a_test_multi $'lo1=1\nlo2=2\necho $lo1\nlo1=2\necho $lo1' # declaration de 2 var et modif lo1
#make_a_test_multi $'lo1=1 lo2=2\necho $lo2\nlo2=3\necho $lo2' # declaration de 2 var et modif lo2
#make_a_test_multi $'lo1=1\nlo2=2\necho $lo2\nlo2=3\necho $lo2' # declaration de 2 var et modif lo2

#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo1=0\necho $lo3$lo2$lo$lo1' # modif lo1
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo2=0\necho $lo3$lo2$lo$lo1' # modif lo2
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo3=0\necho $lo3$lo2$lo$lo1' # modif lo3

#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo1=0 lo2=5\necho $lo3$lo2$lo$lo1' # modif lo1 et lo2
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo1=0 lo3=5\necho $lo3$lo2$lo$lo1' # modif lo1 et lo3
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo2=0 lo3=5\necho $lo3$lo2$lo$lo1' # modif lo2 et lo3

#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo1=0\nlo2=5\necho $lo3$lo2$lo$lo1' # modif lo1 et lo2
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo1=0\nlo3=5\necho $lo3$lo2$lo$lo1' # modif lo1 et lo3
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo3$lo2$lo$lo1\nlo2=0\nlo3=5\necho $lo3$lo2$lo$lo1' # modif lo2 et lo3

#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo1$lo2$lo3\nlo1=11 lo2=22 lo3=33\necho $lo1$lo2$lo3\nlo1=111 lo2=222 lo3=333\necho $lo1$lo2$lo3' # modif successive
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo1$lo2$lo3\nlo1=11\nlo2=22\nlo3=33\necho $lo1$lo2$lo3\nlo1=111\nlo2=222\nlo3=333\necho $lo1$lo2$lo3' # modif successive
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\nlo1=11 lo2=22 lo3=33\nlo1=111 lo2=222 lo3=333\necho $lo1$lo2$lo3' # modif successive
#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\nlo1=11\nlo2=22\nlo3=33\nlo1=111\nlo2=222\nlo3=333\necho $lo1$lo2$lo3' # modif successive

#make_a_test_multi $'lo1=1\nlo2=2\nlo3=3\necho $lo1$lo2$lo3\nlo4=4\necho $lo4' # declar 3+1 var sur plusieurs lignes et appel de 3+1 var ordre
#make_a_test_multi $'lo1=1 lo2=2 lo3=3\necho $lo1$lo2$lo3\nlo4=4\necho $lo4' # declaration de 3+1 var sur une ligne et appel de 3+1 var ordre

##variable tres longue
#make_a_test_multi $'looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo=1'
#make_a_test_multi $'looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo=1\necho $looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo'
#valeur var tres longue
#make_a_test_multi $'lol=ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo'
#make_a_test_multi $'lol=ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\necho $lol'

#pas d'enregistrement des variables
#make_a_test 'lo1=1 lo2=2 lo3=3 echo hello$lo1$lo2$lo3 | cat -e'
#make_a_test 'lo1=1 lo2=2 echo hello$lo1$lo2 | lo3=3 cat -e'
#make_a_test 'lo1=1 echo hello$lo1 | lo3=3 cat -e | lo2=2 cat -e'
#make_a_test 'echo hello$lo1$lo2 | lo3=3 cat -e | lo1=1 lo2=2 cat -e'

#make_a_test 'cat -e lo1'
#make_a_test 'echo $lo1$lo2$lo3 | cat -e lo2'
#make_a_test 'ls | pwd | lo1=1 echo $lo1'
#make_a_test 'echo $lo1 | cat -e | lo2=2 cat -e'
#make_a_test '$lo' 
#make_a_test 'echo $lo1 | lo2=2 cat'

#printf "TEST suppression declaration variable si necessaire\n"
#make_a_test_multi $'lo1=you echo $lo1\necho hello$lo1'
#make_a_test_multi $'lo1=1 lo2=2 echo $lo1$lo2\necho $lo1$lo2'
#make_a_test_multi $'lo1=1 lo2=2 echo $lo1$lo2 lo1=1 lo2=2\necho $lo1$lo2'
#make_a_test_multi $'lo1=1 lo2=2 echo $lo1$lo2 lo3=3\necho $lo1$lo2$lo3'
#	make_a_test_multi $'lo1=1 export lo2=2 lo3=3 echo $lo1$lo2$lo3\necho hello$lo1$lo2$lo3'

#make_a_test_multi $'export lo1=1\necho hello$lo1'
#make_a_test_multi $'export lo1=1\necho hello$lo1$lo2'
#make_a_test_multi $'export lo1=1 lo2=2 lo3=3\necho hello$lo1$lo2$lo3'
#	make_a_test_multi $'export lo1=1 lo2=2 lo3=3 echo $lo1$lo2$lo3\necho hello$lo1$lo2$lo3'
#	make_a_test_multi $'lo1=1 export lo2=2 lo3=3\necho hello$lo1$lo2$lo3' # DIFF OK bash tmp_list
#make_a_test_multi $'export lo2=2 lo3=3 | echo $lo2$lo3\necho hello$lo1$lo2$lo3'
#make_a_test_multi $'export VAR1=hello\nenv | grep VAR1'
#make_a_test_multi $'export VAR2=world\nprintenv VAR2'
#make_a_test_multi $'export VAR3=42\nbash -c "echo $VAR3"'
#make_a_test_multi $'export VAR4=test\nexport VAR5=ok\nenv | grep VAR4\nenv | grep VAR5'
#make_a_test_multi $'export VAR6=final\nexport VAR7=last\nexport VAR8=done\nprintenv VAR6\nprintenv VAR7\nprintenv VAR8'
#	make_a_test_multi $'export VAR9=finish\nexport VAR10=theend\necho $VAR9$VAR10'
#make_a_test_multi $'export VAR11=hello VAR12=world\nenv | grep VAR11\nenv | grep VAR12'
#make_a_test_multi $'export VAR13=foo\nunset VAR13\nprintenv VAR13'
#make_a_test_multi $'export VAR14=bar\nexport VAR15=baz\nunset VAR14\nenv | grep VAR15\nenv | grep VAR14'
#	make_a_test_multi $'export VAR16=abc\nexport VAR17=def\nexport VAR18=ghi\nbash -c "echo $VAR16$VAR17$VAR18"'

 
#PARTIE NICO
#make_a_test_multi $'lol=hello\necho $lol'
#make_a_test_multi $'lol=hello\necho $lol\nunset lol'
#make_a_test_multi $'lol=hello\necho $lol\nunset lol\necho $lol'
#make_a_test_multi $'export lol=hello\necho $lol\nunset lol\necho $lol'


#Liste tmp de var non géré
#make_a_test_multi $'lo3=3 env | grep lo3 # Affiche lo3=3
#lo3=3 printenv lo3 # Affiche 3
#lo3=3 sh -c 'echo $lo3' # Affiche 3
#lo3=3 perl -e 'print $ENV{"lo3"}' # Affiche 3

#make_a_test "pwd"
#make_a_test "cd .."
#	make_a_test "cd -"
#make_a_test "cd ~"
#make_a_test "cd"

#fd_limited_test "ls | cat"
#fd_limited_test "ls | cat | cat -e"
#fd_limited_test "ls | cat | cat -e | cat -e | cat -e"
#fd_limited_test "cat < test_file/litle -e"
#fd_limited_test "cat < test_file/medium -e"
#fd_limited_test "cat < test_file/bible.txt -e"

#make_a_test "| echo hello | cat -e"
#make_a_test "|echo hello | cat -e"
#make_a_test "echo 'hello ==> DIFF OK' | cat -e |||" #erreur pour "echo hello | cat -e |"
#make_a_test "echo 'hello ==> DIFF OK' | cat -e|||" #erreur pour "echo hello | cat -e|"
#make_a_test ""
#make_a_test "echo hello | cat"
#make_a_test "echo hello| cat -e"
#make_a_test "echo hello |cat -e"
#make_a_test "echo hello|cat -e"

##printf "\nTEST Lexing \n"
##printf '\n\033[15`%s\n' "Basic lexing"
#make_a_test ""
#make_a_test "
#"
#make_a_test " "
#		make_a_test "	" #   ==> DIFF
#make_a_test ":"
#make_a_test "!"
#	make_a_test "-"
#make_a_test "&& ==> DIFF OK"
#make_a_test "&&&&& ==> DIFF OK"
#make_a_test "&&&&&&&&&&&&&&&& ==> DIFF OK"

##printf '\n\033[15`%s\n' "Pipe lexing"
#make_a_test "|"
#make_a_test "| hola"
#make_a_test "| | |"
#make_a_test "||"
#make_a_test "|||||"
#make_a_test "\"\"echo 'hola\"\"'''' que\"\"'' tal'\"\"''"

#printf '\n\033[15`%s\n' "Redirection"
#make_a_test ">"
#make_a_test "<"
#make_a_test ">"
#make_a_test "> >"
#make_a_test "> >>"
#make_a_test "> >>>"
#make_a_test "> >>>>"
#make_a_test "> >>>>>"
#make_a_test ">"
#make_a_test ">>"
#make_a_test ">>>"
#make_a_test "> "
#make_a_test ">	 "
#make_a_test ">>>>>>>>>>>>>>>"
#make_a_test "> > > >"
#make_a_test ">> >> >> >>"
#make_a_test "< < < <"
#	make_a_test "<< << << <<"
	#make_a_test "echo < > file"        # mélange incohérent de redirections (erreur)
	#make_a_test "<<<< << << <<"
#make_a_test ">>> >"
	#make_a_test "<<< <"
#touch proute
#chmod 000 proute
#make_a_test ">proute"
#	make_a_test "<proute"
#chmod 777 proute
#rm proute
#touch proute
#chmod +r proute
#make_a_test ">proute"
#	make_a_test "<proute"
#chmod 777 proute
#rm proute
#touch proute
#chmod +x proute
#chmod -r proute
#make_a_test ">proute"
#make_a_test "<proute"
#chmod 777 proute
#rm proute
#touch proute
#chmod +w proute
#chmod -x proute
#make_a_test ">proute"
#make_a_test "<proute"
#chmod 777 proute
#rm proute
#printf "\n"

#printf "\nTEST BUILT-IN\n"
#printf '\033[15`%s\n' "Test cd built-in"
#make_a_test "cd"
#make_a_test "cd .."
#make_a_test "cd /tmp"
#make_a_test "cd dsfa"
#	make_a_test "cd -"
#	make_a_test "cd -~"
#	make_a_test "cd -/~"
#	make_a_test "cd -/tmp"
#make_a_test "cd ~"
#	make_a_test "cd ~/Downloads"
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

#printf '\n\033[15`%s\n' "Test echo built-in"
#make_a_test "echo Hola"
#make_a_test "echoHola"
#make_a_test "echo-nHola"
#make_a_test "echo -n Hola"
#make_a_test "echo \"-n\" Hola"
#make_a_test "echo -nHola"
#make_a_test "echo Hola -n"
#make_a_test "echo Hola Quel tal"
#	make_a_test "echo		Hole" # echo Tab Tab Hole
#	make_a_test "echo\t\tHole"
#make_a_test "echo Hola		que	tal"
#	make_a_test "echo	\n	hola"							//diff + mem leak
#	make_a_test "echo\t\n\thola"
#	make_a_test "echo	\"\n\"	hola"							//diff + mem leak
#	make_a_test "echo\t\"\n\"\thola"
#make_a_test "echo \"\t\t\t\t\" | cat -e"
#	make_a_test "echo\t\t\t\t\\t\t\t\t\t| cat -e"
#make_a_test "\"\"''echo hola\"\"'''' que\"\"'' tal\"\"''"
#make_a_test "echo -n -n"
#make_a_test "echo -n -n Hola que"
#make_a_test "echo -n"
#make_a_test "echo -nnnnnnnnnn"
#make_a_test "echo -n -nnn -nnnnn"
#make_a_test "echo -n-nnn -nnn"
#make_a_test "echo --------n"
#make_a_test "echo -nnn ---------n"
#make_a_test "echo -nnn ------------nnn--------n"
#make_a_test "echo -nnn ----------- -nnn--------n"
#make_a_test 'echo $?'
#make_a_test 'sleep 1 |echo $?'
#make_a_test 'echo $:$= | cat -e'   # ==> LEAK
#make_a_test 'echo " $ " | cat -e'
#make_a_test "echo ' $ ' | cat -e"
#make_a_test 'echo $HOME' #
#make_a_test 'echo \$HOME diffOK' #
#make_a_test 'echo term is [$TERM]' #
#make_a_test 'echo term is [$TERM' #
#make_a_test 'echo term is [$TERM4]' #
#make_a_test 'echo $UID' #
#	make_a_test 'echo $PWD' #
#make_a_test 'echo $HOME9' #
#make_a_test 'echo $HOME%' # ==> INVALID
#make_a_test 'echo $UID$HOME' #
#make_a_test 'echo le path de mon HOME est $HOME'
#make_a_test 'echo $"HO""ME"'	 #						//diff
#make_a_test 'echo $"US""ER"'		# echo $"US"ER
#make_a_test 'echo "$"HOME'
#make_a_test 'echo "$"HOLA'

#printf '\nTEST variable supprimé ou non par le parsing\n'
#	make_a_test_multi $'VAR1=1 export VAR2=2\n echo $VAR1 $VAR2'	# ==> DIFF OK
#	make_a_test_multi $'VAR3=3 VAR4=5 export VAR5=4\n echo $VAR3 $VAR4 $VAR5'
#	make_a_test_multi $'VAR6=7 export VAR7=6 VAR8=9\n echo $VAR6 $VAR7 $VAR8'
#	make_a_test_multi $'VAR9=11 VAR10=13 export VAR11=8 VAR12=15\n echo $VAR9 $VAR10 $VAR11 $VAR12'
#make_a_test_multi $'VAR13=17 export VAR14=10 | VAR15=19\n echo $VAR13 $VAR14 $VAR15'
#make_a_test_multi $'VAR16=21 export VAR17=12 | VAR18=23 VAR19=25\n echo $VAR16 $VAR17 $VAR18 $VAR19'
#make_a_test_multi $'VAR20=27 export VAR21=14 VAR22=35| VAR23=29 export VAR24=31 VAR25=33\n echo $VAR20 $VAR21 $VAR22 $VAR23 $VAR24 $VAR25'
	#make_a_test_multi $'VAR26=37 export VAR27=16 | VAR28=41 | export VAR29=18 | VAR30=39 cat VAR31=43\n echo $VAR26 $VAR27 $VAR28 $VAR29 $VAR30 $VAR31'
#make_a_test_multi $'VAR32=45 VAR33=47 export VAR34=20 VAR35=49| export VAR36=51 | VAR37=53 export VAR38=22 VAR39=55\n echo $VAR32 $VAR33 $VAR34 $VAR35 $VAR36 $VAR37 $VAR38 $VAR39'

printf '\n\033[15`%s\n' "Test export built-in"
#make_a_test_multi $'export HOLA=bonjour\n echo $HOLA'
	make_a_test_multi $'export		HOLA=bonjour\n echo $HOLA' # ==>TAB ne passe pas
	make_a_test 'export'  ==> DIFF toujour 2* PWD
	make_a_test_multi $'export Hola\nexport'
	make_a_test 'export Hola9hey\nexport'
	make_a_test 'export $DONTEXIST'
make_a_test 'export | grep "HOME"'
make_a_test 'export ""'
make_a_test 'export ='
make_a_test 'export %'
	make_a_test 'export $?'
make_a_test 'export ?=2'
make_a_test 'export 9HOLA='
make_a_test_multi $'export HOLA9=bonjour\n echo $HOLA9'
make_a_test_multi $'export _HOLA=bonjour\n echo $_HOLA'
make_a_test_multi $'export ___HOLA=bonjour\n echo $___HOLA'
make_a_test_multi $'export _HO_LA_=bonjour\n echo $_HO_LA'
make_a_test 'export HOL@=bonjour'
make_a_test 'export HOL~A=bonjour'
	make_a_test 'export -HOLA=bonjour'
	make_a_test 'export --HOLA=bonjour'
make_a_test 'export HOLA-=bonjour'
make_a_test 'export HO-LA=bonjour'
make_a_test 'export HOL.A=bonjour'
make_a_test 'export HOL\\\$A=bonjourdiffOK'
make_a_test 'export HO\\\\LA=bonjourdiffOK'
make_a_test 'export HOL}A=bonjour'
make_a_test 'export HOL{A=bonjour'
make_a_test 'export HO*LA=bonjour'
make_a_test 'export HO#LA=bonjour'
make_a_test 'export HO@LA=bonjour'
	make_a_test 'export HO!LA=bonjour'
make_a_test_multi $'export HO$?LA=bonjour'
make_a_test 'export +HOLA=bonjour'
make_a_test 'export HOL+A=bonjour'
make_a_test_multi $'export HOLA+=bonjourdiffOK' # ==> DIFF car concatene valeur de la variable
make_a_test_multi $'export HOLA=bonjour\nexport HOLA+=2bonjour2diffOK' # ==> IDEM
make_a_test_multi $'exportHOLA=bonjour'
	make_a_test 'export HOLA =bonjour\n echo $HOLA'
make_a_test 'export HOLA = bonjour\n echo $HOLA'
	make_a_test_multi $'export HOLA=bon jour'
	make_a_test_multi $'export HOLA= bonjour'
make_a_test_multi $'export HOLA=bonsoir\nexport HOLA=bonretour\nexport HOLA=bonjour\n echo $HOLA'
make_a_test_multi $'export HOLA=$HOME\n echo $HOLA'
make_a_test_multi $'export HOLA=bonjour$HOME\n echo $HOLA'
make_a_test_multi $'export HOLA=$HOMEbonjour\n echo $HOLA'
make_a_test_multi $'export HOLA=bon$jour\n echo $HOLA'
make_a_test_multi $'export HOLA=bon\\jour\n echo $HOLA diffOK'
make_a_test_multi $'export HOLA=bon\\\\jour\n echo $HOLA diffOK'
make_a_test 'export HOLA=bon(jour diffOK'
make_a_test 'export HOLA=bon()jour diffOK'
make_a_test 'export HOLA=bon&jour diffOK'
make_a_test_multi $'export HOLA=bon@jour\n echo $HOLA'
make_a_test_multi $'export HOLA=bon;jour\n echo $HOLA diffOK'
make_a_test 'export HOLA=bon!jour\n echo $HOLA diffOK'
make_a_test_multi $'export HOLA=bon""jour""\n echo $HOLA'
make_a_test_multi $'export HOLA$USER=bonjour\n echo $HOLA'
make_a_test_multi $'export HOLA=bonjour=casse-toi\necho $HOLA'
make_a_test_multi $'export ""HOLA=bonjour""=casse-toi\necho $HOLA'
make_a_test_multi $'export HOLA=bonjour\nexport BYE=casse-toi\necho $HOLA $BYE'
make_a_test_multi $'export HOLA=bonjour BYE=casse-toi\necho $HOLA $BYE'
make_a_test_multi $'export A=a B=b C=c\necho $A $B $C'
make_a_test_multi $'export $HOLA=bonjour\n echo $HOLA'
make_a_test_multi $'export HOLA="bonjour      "\necho $HOLA | cat -e'
	make_a_test_multi $'export HOLA="   -n bonjour   "\necho $HOLA'
make_a_test_multi $'export HOLA="bonjour   "/\necho $HOLA'
	make_a_test_multi $'export HOLA=\'""\'\necho "" $HOLA "" | cat -e'
make_a_test_multi $'export HOLA=at\n echo $HOLA Makefile'
make_a_test_multi $'export """" HOLA=bonjour'
	make_a_test_multi $'export HOLA="cat Makefile | grep NAME"\necho $HOLA'
make_a_test_multi $'export HOLA=hey \necho $HOLA$HOLA$HOLA=hey$HOLA'
make_a_test_multi $'export HOLA="  bonjour  hey  "\necho $HOLA | cat -e'
make_a_test_multi $'export HOLA="  bonjour  hey  "\necho """"""$HOLA"""""" | cat -e'
make_a_test_multi $'export HOLA="  bonjour  hey  "\necho wesh""$HOLA"" | cat -e'
make_a_test_multi $'export HOLA="  bonjour  hey  "\necho wesh""""$HOLA.'
make_a_test_multi $'export HOLA="  bonjour  hey  "\necho wesh$""""HOLA.'
make_a_test_multi $'export HOLA="  bonjour  hey  "\necho wesh$""HOLA HOLA"."'

	make_a_test_multi $'export HOLA=bonjour\nexport HOLA=\' hola et $HOLA\'\necho $HOLA' ==> BLOQUE TESTEUR
make_a_test_multi $'export HOLA=bonjour\nexport HOLA=" hola et $HOLA"$HOLA\necho $HOLA'
make_a_test_multi $'export HOLA="ls        -l    - a"\necho $HOLA'
	make_a_test_multi $'export HOLA="s -la"\nl$HOLA'
make_a_test_multi $'export HOLA="s -la"\nl"$HOLA"'
make_a_test_multi $'export HOLA="s -la"\nl\'$HOLA\''
make_a_test_multi $'export HOLA="l"\n$HOLAs'
make_a_test_multi $'export HOLA="l"\n""$HOLA""s'
make_a_test_multi $'export HOL=A=bonjour\necho $HOLA'
make_a_test_multi $'export HOLA="l"\n\'$HOLA\'s'
make_a_test_multi $'export HOL=A=""\necho $HOL'
make_a_test_multi $'export TE+S=T\necho $TE+S'
make_a_test 'export ""=""'
make_a_test "export ''=''"
make_a_test "export \"=\"=\"=\""
make_a_test "export '='='='"
make_a_test_multi $'export HOLA=p\nexport BYE=w\n$HOLA""BYE""d'
make_a_test_multi $'export HOLA=p\nexport BYE=w\n""$HOLA""\'$BYE\'d'
make_a_test_multi $'export HOLA=p\nexport BYE=w\n""$HOLA""""$BYE""d'
make_a_test_multi $'export HOLA=p\nexport BYE=w\n$""HOLA""$""BYE""d'
make_a_test_multi $'export HOLA=p\nexport BYE=w\n$\'HOLA\'$\'BYE\'d'
make_a_test_multi $'export HOLA=-n\n""echo $HOLA"" hey'
make_a_test_multi $'export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8\necho ""$A\'$B""\'$C""$D\'$E\'""$F\'""\'$G\'$H""\''
make_a_test_multi $'export HOLA=bonjour\nenv\nunset HOLA\necho $HOLA'
make_a_test_multi $'export HOLA=bonjour\nenv\nunset HOLA\nunset HOLA\necho $HOLA'

##********************************************************************

#printf "\n"
#printf '\n\033[15`%s\n' "Exec cmd"
#	make_a_test ">cat test_file/litle"	==> MANQUE les :
#make_a_test "cat test_file/medium"
#make_a_test "cat test_file/big"
#make_a_test "cat test_file/litle -e"
#make_a_test "cat -e test_file/medium"
#make_a_test "cat test_file/big -e"
#make_a_test "cat -e test_file/bible.txt"
#make_a_test "cat -e test_file/bible.txt <test_file/litle"
#make_a_test "cat test_file/bible.txt <test_file/litle -e"
#make_a_test "cat <test_file/litle -e test_file/bible.txt "
#make_a_test "cat <test_file/litle -e test_file/litle <test_file/medium"
#make_a_test "ls test_file/"

#printf '\n\033[15`%s\n' "Exec pipeline"
##make_a_test "cat test_file/litle | cat -e"
##make_a_test "cat test_file/litle | lol"
##make_a_test "lol | mdr | ptdr"
##make_a_test "ls | mdr | ptdr"
##make_a_test "ls | sleep 1 | ptdr"
##make_a_test "mdr | sleep 1 | ls"
##make_a_test "ls|cat"
##make_a_test "ls|cat|cat-e"
#	make_a_test "ls|cat|cat -e"
##make_a_test "cat test_file/litle -e | cat -e"
##make_a_test "cat test_file/litle -e | ls test_file/"
##make_a_test "cat test_file/litle -e <test_file/bible.txt | ls test_file/"
#	make_a_test "cat test_file/litle -e <fake_redir <test_file/bible.txt | ls test_file/"
#	make_a_test "cat test_file/litle -e <test_file/bible.txt <fake_redir | ls test_file/"
##make_a_test "cat test_file/litle >/dev/null| ls test_file/"
##make_a_test "cat test_file/litle >/dev/full "

#printf "\n"
#printf '\n\033[15`%s\n' "Exec cmd"
#	make_a_test ">cat test_file/litle" ==> MANQUE les :
##make_a_test "cat test_file/medium"
##make_a_test "cat test_file/big"
##make_a_test "cat test_file/litle -e"
##make_a_test "cat -e test_file/medium"
##make_a_test "cat test_file/big -e"
##make_a_test "cat -e test_file/bible.txt"
##make_a_test "cat -e test_file/bible.txt <test_file/litle"
##make_a_test "cat test_file/bible.txt <test_file/litle -e"
##make_a_test "cat <test_file/litle -e test_file/bible.txt "
##make_a_test "cat <test_file/litle -e test_file/litle <test_file/medium"
##make_a_test "ls test_file/"

##printf '\n\033[15`%s\n' "Exec pipeline"
##make_a_test "cat test_file/litle | cat -e"
##make_a_test "cat test_file/litle -e | cat -e"
##make_a_test "cat test_file/litle -e | ls test_file/"
##make_a_test "cat test_file/litle -e <test_file/bible.txt | ls test_file/"
#make_a_test "cat test_file/litle -e <fake_redir <test_file/bible.txt | ls test_file/"
#make_a_test "cat test_file/litle -e <test_file/bible.txt <fake_redir | ls test_file/"
##make_a_test "cat test_file/litle >/dev/null| ls test_file/"
##make_a_test "cat test_file/litle >/dev/full "

#printf "\n"
#rm Minishell valgrind_log output typescript -rf
#exit

#text=$(cat test_file/litle)
#n=3000
#while [ $n -le 3800 ]; do
#	(
#		ulimit -v "$n"
#		./Minishell <<EOF
#		 $text
#EOF
#	)
#	((n = n + 10))
#done
#exit
#(
#	ulimit -n 18
#	make_a_test_nodiff "ls | cat -e | cat -e"
#)
#(
#	ulimit -n 18
#	make_a_test_nodiff "ls | cat <test_file/litle <test_file/bible.txt | cat <test_file/litle.txt >/dev/stdout 1>/dev/stdout"
#)
#(
#	ulimit -n 18
#	make_a_test_nodiff "ls | cat | cat"
#)



