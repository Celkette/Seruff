/* ************************
/*   tokenize_the_hell.c  *
/* ************************/


/**************************************
*				BONUS				  * 
* 	   Attribue le type au token      *
***************************************/
/*t_token_type	ft_bonus_get_token_type(const char *data)
{
	if (ft_strncmp(data, ">>", 2) == 0)
		return (TOK_REDIR_APPEND);
	if (ft_strncmp(data, "<<", 2) == 0)
		return (TOK_REDIR_HEREDOC);
	if (ft_strncmp(data, "&&", 2) == 0)
		return (TOK_AND);
	if (ft_strncmp(data, "||", 2) == 0)
		return (TOK_OR);
	if (ft_strncmp(data, "|", 1) == 0)
		return (TOK_PIPE);
	if (ft_strncmp(data, ">", 1) == 0)
		return (TOK_REDIR_OUT);
	if (ft_strncmp(data, "<", 1) == 0)
		return (TOK_REDIR_IN);
	if (ft_strncmp(data, "$", 1) == 0)
		return (TOK_DOLLAR);
	if (ft_strncmp(data, "&", 1) == 0)
		return (TOK_AMPERSAND);
	if (ft_strncmp(data, "(", 1) == 0)
		return (TOK_PARA_BEGIN);
	if (ft_strncmp(data, ")", 1) == 0)
		return (TOK_PARA_END);
	
	return (TOK_WORD);
}
*/
/********************************
*			BONUS			    * 
*     	 Cree le token          *
*********************************/
/*t_token *bonus_create_token(const char *str, int len)
{
    t_token *new_token;

	new_token = malloc(sizeof(t_token));
//VERIFIER LIBERATION PLUS TARD DE CE MALLOC
    if (new_token == NULL)
    {
		perror("Memory allocation error for create_liste");
		exit(EXIT_FAILURE);
	}
    new_token->type = ft_bonus_get_token_type(str);
	//if (data) deja verifié dans ft_strlen et strndup
	new_token->quote_type = ft_get_quote_type(str);//verification sur le debut de str
	new_token->data = ft_strndup__token(str, len);
//DOIT ON VERIFIER SI DATA EST VIDE?
	if (new_token->data == NULL)
	{
		perror("Memory allocation error for token data");
		free(new_token);
		exit(EXIT_FAILURE);
	}
    return (new_token);
}*/
//static size_t	ft_bonus_token_len(char *s, char *charset, t_meta *meta)
/*********************************************
*					BONUS					 * 
*   compte le nombre de char du futur token  *
**********************************************/
/*static size_t	ft_bonus_token_len(char *s, char *charset, t_meta *meta)
>>>>>>> origin/celine
{//ex ft_next
	size_t	j;

	j = 0;
	if (s[j] == '\'' || s[j] == '\"')
	{
		meta->last_quote_is = s[j];
		while (s[j + 1] && s[j + 1] != meta->last_quote_is)
		   j++;
		return (j + 2); // +1 car j commence a zero +1 pour quote fermante
	}
//si caractere n'est pas dans le charset
	else if (s[j] && is_in_charset(s[j], charset) == 0)//si le caractere n'est pas dans le charset
	{
		while (s[j] && is_in_charset(s[j], charset) == 0 && is_it_quote(s[j]) == 0)//et qu'on atteint pas une quote
			j++;
		return (j);
	}
//si caractere est dans le charset
	else if (s[j] && is_in_charset(s[j], charset))//si le caractere est dans le charset 
	{
		if ((s[j] == '<' && s[j + 1] == '<') || (s[j] == '>' && s[j + 1] == '>')
			|| (s[j] == '&' && s[j + 1] == '&') || (s[j] == '|' && s[j + 1] == '|'))//si >> ou <<
	   		return (2); // Cas spécifique pour cree token [<<]
		else
			return (1);
	}
	return (0); // Si aucun des cas précédents n'est vrai, retourne 0 (fin de chaîne ou caractère non reconnu)
}
*/
