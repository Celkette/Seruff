/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fonction_pour_debug.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:58:59 by celine            #+#    #+#             */
/*   Updated: 2025/07/30 11:06:10 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "src/headers/minishell.h"
//Print la structure du token
/*void print_token_flags(t_token *token, int i)
{
    if (!token)
    {
        printf("Token NULL\n");
        return;
    }
    printf("---- TOKEN FLAGS %d ----\n", i);
    printf("type       = %d\n", token->type);
    printf("data       = %s\n", token->data ? token->data : "(null)");
    printf("consume    = %d\n", token->consume);
    printf("quote_type = %d\n", token->quote_type);
    printf("is_quote   = %d\n", token->is_quote);
    printf("hered      = %d\n", token->hered);
    printf("redir      = %d\n", token->redir);
    printf("---------------------\n");
}*/
//@Affiche le type de token sous forme visible
void print_token_type(t_token_type type)
{
    switch(type) {
        case TOK_WORD: printf("WORD       "); break;
        case TOK_PIPE: printf("PIPE       "); break;
        case TOK_REDIR_OUT: printf("RED_OUT    "); break;
        case TOK_REDIR_IN: printf("RED_IN     "); break;
        case TOK_REDIR_APPEND: printf("RED_APPEND "); break;
        case TOK_REDIR_HEREDOC: printf("RED_HEREDOC"); break;
        case TOK_AMPERSAND: printf("AMPERSAND  "); break;
        case TOK_PARA_OPEN: printf("PARA_BEGIN "); break;
        case TOK_PARA_CLOSE: printf("PARA_END   "); break;
        default: printf("UNKNOWN    "); break;
    }
}
//Affiche le type de quote associé au token
void print_quote_type(int quote_type)
{
    switch(quote_type) {
        case 0: printf("NONE   "); break;
        case 1: printf("SIMPLE "); break;
        case 2: printf("DOUBLE "); break;
       default: printf("UNKNOWN"); break;
    }
}
//affiche la liste chainée
void print_token_list2(t_list *list)
{
     t_list *current = list;
    int i = 0;
    if(!list)
    {
        printf("  Token list is empty.\n");
        return ;
    }
    if(!list->data)
        printf("  1er Token data is empty.\n");
    else
    {
        //printf("\n=== RÉSULTATS DE LA TOKENISATION ===\n");
        while (current) 
        {
//            printf("DEBUG: current=%p, current->data=%p\n", (void*)current, (void*)current->data);
            t_token *token = (t_token *)current->data;
//            printf("DEBUG: token=%p\n", (void*)token);
            //printf(", value=[%s]", token->data ? token->data : "NULL");
            printf("Token[%d]", i);
            if (!token) 
                printf("(null token)\n");
            else
            {
               // Vérification supplémentaire
                // Si tu as un champ type connu (enum), tu peux tester sa plage
                // Par exemple, si TOK_WORD = 0 et TOK_PARA_CLOSE = 8 :
                if(token->type < 0 || token->type > 11) 
                    printf("type= [corrupted]\n");
                else {
                    printf("type= ");
                    print_token_type(token->type);
                }
                printf("  quote= ");
                print_quote_type(token->quote_type);
                if (token->data)
                    printf(",   value=  [%s]", token->data);
                else
                    printf("  value=[NULL]");
                printf("\n");
            }
            current = current->next;
            i++;
        }
    }
}
/*void print_token_list(t_list *list)
{
    int i = 0;
    if (!list) 
    {
        printf("  Token list is empty.\n");
        return;
    }
  //   while (list)
  //  {
  //      printf("  [print] list=%p, list->data=%p\n", (void*)list, (void*)list->data);
//        if (!list->data)
  //      {
//            printf("        Token[%d]: (null)\n", i);
//        }
  //      else
//        {
//            t_token *tok = (t_token *)list->data;
//            if (tok && tok->data)
//                printf("        Token[%d]: '%s'\n", i, tok->data);
//            else
//                printf("        Token[%d]: (null data)\n", i);
//        }
//        list = list->next;
//        i++;
//    }
     while (list)
    {
        //printf("  token-list existe.\n");
        if (!list->data) 
        {
            //printf("        1er Token data is empty.\n");
            printf("            Token[%d]: (null)\n", i);
        } 
        else 
        {
            //printf("        1er Token data existe.\n");
            t_token *tok = ((t_token *)list->data);
            if (tok->data)
                printf("        Token[%d]: [%s]\n", i, tok->data);
            else
                printf("            Token[%d]: (null data)\n", i);
        }
        list = list->next;
        i++;
    }
}*/
/*************************************
* 			 TEST pour 				 *
* 	find_var_name & find_value_var	 *
**************************************/

// Fonction utilitaire pour créer une variable locale (pour le test)
t_list *add_local_var(t_list *list, const char *key, const char *val) 
{
    t_var *var = malloc(sizeof(t_var));
    var->key = ft_strdup(key);
    var->val = ft_strdup(val);
    t_list *node = malloc(sizeof(t_list));
    node->data = var;
    node->next = list;
    return node;
}
//A mettre dans main.c a la racine du projet
