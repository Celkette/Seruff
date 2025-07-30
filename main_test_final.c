/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test_final.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 14:56:21 by cle-rouz          #+#    #+#             */
/*   Updated: 2025/07/30 14:39:59 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/headers/minishell.h"

int main(int argc, char **argv, char **envp)
{
    
    t_meta meta;
    t_list *local_vars = NULL;
    t_list *token_list = NULL;
    t_list *new_token_list = NULL;
    
    t_env	minishell;
    minishell.env = envp;
    (void)argc;
	(void)argv;
    
    // Création d'une variable locale NAME=celine
    local_vars = add_local_var(local_vars, "NAME", "cel$FNAME");
    local_vars = add_local_var(local_vars, "FNAME", "ine $FIRST");
    local_vars = add_local_var(local_vars, "FIRST", "42");
    local_vars = add_local_var(local_vars, "NOME", "cel$FNOME");
    local_vars = add_local_var(local_vars, "FNOME", "ine$FIRST");
    local_vars = add_local_var(local_vars, "MOI", "Nico");
    local_vars = add_local_var(local_vars, "AAA", "aaa aa.txt");
    //x="ls | cat";

    //char    *str = "echo '$NAME' $hgdf <<sdf > fsd>> dsga < | fhsdkjhg";
    //char    *str = "<< $XXX";
    //char    *str = "e'\"cho \"'$MOI > '\"$NAME\"' << '|'infile <$NOME | >> outfile";
    //char    *str = "echo \"$FIRS\" moi"; // -> donne une chaine vide
    //EXPAND
    //HEREDOC
    //char *str = "<<'$ABC' $ABC $USER ";
    char *str = "1<< $\"ABC\" 2<<$\"\"ABC 3<<$A\"B\"C     4<< $'ABC' 5<<$''ABC "; //limiteur expand $ supprimé
    //char *str = "1<<\"$ABC\" <<$A\"BC\" 2<<$ABC\"\"    3<<'$ABC' 3<< $A'BC' 3<< $ABC'' 4<< $A'B'C ";//limiteur expand identique

    //char *str = "<<$XXX<<$USER <<\"$XXX\" <<$X\"\"XX <<$XXX\"\" <<$X'X'X <<$XXX''  <<$"; //limiteur expand identique
    //char *str = " <<\"$XXX\" <<$\"XX\"X  <<$''XXX <<$'XX'X <<$\"\""; //limiteur expand $ supprimé
    
    //char *str = "$abc$USER$abc";  //OK
    //char *str = "$abc$USER";      //OK
    //char *str = "$USER$abc";//OK
    //char *str = "<< $ABC<<$USER<<\"$ABC\"<<'$ABC' $ABC $USER $ABC$HOME ";
    //char *str = "<<$ABC<<$USER|12";   //KO
    //char *str = "'$USER' $'USER' \"$FIRST\"|$\"FIRST\" | $NAME \"$NAME\"|$\"NAME\"";
    //char   *str = " fdgdf $xxx";
    //char   *str = " $xxx fdgdf ";
    //char   *str = " fdgdf $xxx sqdf";
    //char    *str = "ec>$XXX >> $USER << $NAME < $NAME| '$USER' $'USER' \"$FIRST\"|$\"FIRST\"";
    //char *str = "export z$z=du_text";
    //char *str = "\"\" \"\ "\"\ "\"\ "\"\" hello";
    //char *str = "echo 'hello are you'<<$AAA";
    //char *str = "$NAME hello << $NAME > $NAME < $NAME >> $NAME";//1er expand, 2nd non expand, 3 + expand sans worsplit
    //char    *str = "echo uijhoh > $X ijh";

printf("HOME = %s\n", getenv("HOME"));
printf("MAIN STR =%s\n\n", str);
    check_lexing(str, &meta);

    token_list = tokenize_the_hell(str, " ><|");
printf("                     token_list\n");
print_token_list2(token_list);

new_token_list = expand_the_list(&minishell, token_list, &meta, local_vars);
printf("\n                    MAIN TOKEN LIST APRES EXPAND\n");
print_token_list2(new_token_list);

new_token_list = remove_list_quotes(new_token_list);
printf("\n                    MAIN TOKEN LIST APRES REMOVE_LIST_QUOTE\n");
if (!token_list)
    printf("La liste de tokens est vide (NULL)\n");
else
    print_token_list2(new_token_list);
free(local_vars);
free_token_list(new_token_list);
printf("****PARSING FINI****\n");
    return 0;
}
/*
si AAA="aaa aa.txt"
echo hellou > $AAA
=> ne wordsplit pas le nom du fichier


bash-5.1$ echo uijhoh > "" ijh  // ne trouve pas de fichier dont le nom est vide
bash: : No such file or directory

bash-5.1$ echo uijhoh > '' ijh  // ne trouve pas de fichier dont le nom est vide
bash: : No such file or directory

bash-5.1$ echo uijhoh >   ijh

bash-5.1$ echo uijhoh > "$XX"  ijh  // ne trouve pas de fichier dont le nom est vide
bash: : No such file or directory

bash-5.1$ echo uijhoh > '$XX'  ijh

bash-5.1$ echo uijhoh > '\0'  ijh

bash-5.1$ echo uijhoh > kj $X ijh

bash-5.1$ cat kj
uijhoh ijh

bash-5.1$ << $D
> ^C
bash-5.1$ << $D
> 
bash: warning: here-document at line 44 delimited by end-of-file (wanted `$D')
bash-5.1$ << ""
> hhjxc
> ""
> 
bash-5.1$ 
*/