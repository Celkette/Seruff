/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_fun.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:05:32 by nlaporte          #+#    #+#             */
/*   Updated: 2025/07/30 13:59:37 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_FUN_H
# define MINISHELL_FUN_H

# include <dirent.h> // Pour le type DIR
# include <sys/stat.h>

//***********/
// BUILD_IN
//***********/
//cd
	//cd_utils.c
int		err_stat(const char *path);
int		check_perm(t_env *minishell, struct stat *dir);
void	chdir_error(char *path, char *err);
int		set_new_old_path(t_env *minishell, const char *path);
int		manage_back(t_env *minishell);
	//cd.c
int		err_cd(const char *path);
int		manage_tile(t_env *minishell, const char *path);
int		clearr(void *p);
int		print_error(const char *path);
int		echoo(void *p);
	//cd2.c
int		manage_tile2(t_env *minishell, char *path, char *og);
DIR		*prepare_cd(t_env *minishell, const char *path, DIR *dir);
DIR		*prepare_cd2(t_env *minishell, const char *path, char *og, DIR *dir);
//env.c
int	env(void *p);
//pwd.c
int pwd(void *p);
//***********/
// EXIT
//***********/
void	exit_minishell(t_env *minishell);
//***********/
// INIT
//***********/
void	init_env(t_env *minishell);
int		init_sig(t_sig_manage *sig_manage);
void	ft_init_meta(t_meta *meta);
//***********/
// LEXING
//***********/
//lexing_check_syntaxe.c
int		check_empty_input(char *str);
int		check_quotes(char *str, t_meta *meta);
int		check_pipe(char *str, t_meta *meta);
int		check_redir(t_list *lst);

// TREE
void	print_tree(t_tree *tree, int level, char r);
void	free_tree(t_tree *tree);
void	exec_tree(t_env *minishell, t_tree *tree, t_tree *parent, int i, int *j);
int		exec_on_tree(t_env *minishell, t_tree *node);
int		exec_one_cmd(t_env *minishell, t_tree *node);
void	exec_pipeline(t_env *minishell, t_tree *node, int last, int i);
void	exec_cmd(t_env *minishell, t_tree *node);
int		use_redir_out(t_tree *node);
int		use_redir_in(t_tree *node);
t_token	*see_token(t_list *lst);
t_token	*consume_token(t_list *lst);
t_tree	*init_node(t_token *token, int type);
t_tree	*create_word_node_tree(t_env *minishell, t_list *tokens, int *valid);
t_tree	*prepare(t_env *minishell, t_list *lst, int min_power, int *valid);
t_redir_out	*get_redir_out(t_list *head);
t_redir_in	*get_redir_in(t_list *head);



// META struct
void	ft_init_meta(t_meta *meta);
int		flag_quote(char c, t_meta *meta);
int		flag_space(char c, t_meta *meta);
int		flag_quote(char c, t_meta *meta);
int		flag_pipe(char c, t_meta *meta);
int		flag_redir(char c, t_meta *meta);
int		flag_dollar(char c, t_meta *meta);
//lexing_util.c
int ft_update_flag(char c, t_meta *meta);//ne sait pas où et si elle est utilisée
//***********/
// PARSING
//***********/
//tokenize
//tokenize_the_hell.c
t_list	*tokenize_the_hell(char *str, char *charset);
t_token *ft_creat_token(const char *str, int len);
size_t	ft_token_len(char *s, char *charset, t_meta *meta);
int		check_lexing(char *s, t_meta *meta);
//util_tokenize.c
int		is_in_charset(char c, char *charset);
int		ft_get_quote_type(const char *str);
//Expand
//expand_util.c
int		is_dollar_in(char *str, t_meta *meta);
int		is_expand_ok(char *str, t_meta *meta, int i);
char	*delete_dollar(char *str, int i);
void	update_token_struct(t_list *list);
//expand.c 
void	copy_env_var(t_env *minishell);
char	*check_which_expand(t_env *minishell, t_list *list, char *str, t_meta *meta, int *i);
char	*expand_char(t_env *minishell, t_list *list, char *str, t_meta *meta, int i);
t_list	*replace_expand(t_list *token_list, t_list *expand_list, t_list *cur_list);
//Delete quote
t_list	*remove_list_quotes(t_list *token_list);
char	*remove_str_quotes(char *str);
//Linked_list
//util_linked_list.c
t_list	*ft_create_node(void *data);
t_list	*ft_add_node(t_list *list, void *data);
//t_list	*ft_delete_node(t_list *list, void *data);

// Fonction pour lire l'entrée standard
void	read_stdin(t_env *minishell, t_meta *meta, t_list *varList);

void	free_split(char **split);
// Fonction pour signaux
void	sig_ctrl_c_catcher(int n);

// BUILT-IN
int		cd(void *p);
t_arg	*get_arg_built_in(t_env *minishell, t_tree *node);
int		cd_exec(t_env *minishell, const char *path);

// SPECIAL CHAR
void	set_var(t_env *minishell, char *s);
char	*get_var(t_env *minishell, char *name);
//void	clean_var(t_env *minishell);
		//nouvelle
//t_var	*is_var_in_list(t_list *list, char *var_name);
int		parse_var(char *s, char **key, char **val);
t_var	*creat_local_var(char *key, char *val);
void	set_local_var(t_env *minishell, char *s);
void	run_all_tests(t_env *minishell);//Util pour test set_local_var()

// EXIT MINISHELL
void	exit_minishell(t_env *minishell);

// UTILS
t_list	*tokenize_the_hell(char *str, char *charset);
//void	free_token_list(t_list *token_list);
//void	free_token(t_token *token);
void	*ft_memset(void *s, int c, size_t n);
int		ft_strlen(char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *nptr);
int		ft_atoi2(const char *nptr);
void	ft_lstadd_back(t_list **lst, t_list *neww);
void	ft_lstadd_front(t_list **lst, t_list *neww);
int		ft_lstsize(t_list *lst);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	free_split(char **split);
char	*ft_itoa(int n);
char	*ft_strdup(const char *src);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char *str, char c);
void	free_split(char **split);
char	*ft_strchr(const char *s, int c);
char	*ft_strndup(const char *s, size_t n);
int		check_perm(t_env *minishell, struct stat *dir);
void	chdir_error(char *path, char *err);
int		set_new_old_path(t_env *minishell, const char *path);
int		manage_back(t_env *minishell);
DIR		*prepare_cd2(t_env *minishell, const char *path, char *og, DIR *dir);
DIR		*prepare_cd(t_env *minishell, const char *path, DIR *dir);
int		manage_tile(t_env *minishell, const char *path);
int		manage_tile2(t_env *minishell, char *path, char *og);
int		err_stat(const char *path);
int		err_cd(const char *path);
int		print_error(const char *path);
void	ft_lstadd_front(t_list **lst, t_list *neww);
int		ft_lstsize(t_list *lst);
char	*ft_strstr(char *str, char *to_find);
char	*ft_strjoin(char const *s1, char const *s2);


// GNL
	//get_next_line_utils.c
void	clear_buffer(t_file *file);
void	clear_file(t_file *file);
void	init_file2(t_file *file, int fd);
int		ft_strlen_gnl(char *s, int endl);
char	*ft_give_malloc(int linebreak, int size);
	//get_next_line.c
char	*realloc_char(t_file *file, int linebreak);
void	*read_error(t_file *file, int freee);
char	*logic(t_file *file);
char	*next_line(t_file *file);
char	*get_next_line(int fd);


//***********/
// PARSING
//***********/
//tokenize
//tokenize_the_hell.c
//t_list	*tokenize_the_hell(char *str, char *charset);
t_token *ft_creat_token(const char *str, int len);
char 	*ft_strndup_token(const char *str, int n);
size_t	ft_token_len(char *s, char *charset, t_meta *meta);
//util_tokenize.c
int		is_in_charset(char c, char *charset);
int		ft_get_quote_type(const char *str);
t_token_type	ft_get_token_type(const char *data, int len);
//Expand
//expand_util.c
int		is_dollar_in(char *str, t_meta *meta);
int		is_expand_ok(char *str, t_meta *meta, int i);
char	*special_case(const char *name,t_env *minishell);
char	*delete_dollar(char *str, int i);
//expand_util2.c
void	update_token_struct(t_list *list);
char	*manage_hered(t_list *current, t_meta *meta);
//expand.c 
t_list	*expand_the_list(t_env *minishell, t_list *token_list, t_meta *meta, t_list *var_list);
char	*expand(t_env *minishell, t_list *var_list, char *str, t_meta *meta);
t_list	*replace_expand(t_list *token_list, t_list *expand_list, t_list *cur_list);
//Delete quote
t_list	*remove_list_quotes(t_list *token_list);
char	*remove_str_quotes(char *str);
//Linked_list
//util_linked_list.c
//t_list	*ft_create_node(void *data);
//t_list	*ft_add_node(t_list *list, void *data);
t_list	*ft_add_list_at_begin(t_list *list, t_list *add_list);//, t_list *current);
t_list	*ft_add_list_at_middle(t_list *list, t_list *add_list, t_list *current);

t_list	*ft_delete_node(t_list *node);
//free_element.c
void	free_token_struct(t_token *token);
void	free_token_node(t_list *node);//void free_token(t_token *token);
void	free_token_list(t_list *token_list);
//Local_var
//local_var_len.c
int		var_name_index_end(char *str, t_meta *meta);
int		len_manage_name(char *str);
int		len_manage_brace(char *str);
//local_var_list.c
t_var	*is_var_in_local(t_list *list, char *var_name);//t_var *is_var_in_list(t_list *list, char *var_name);
//int		parse_var(char *s, char **key, char **val);
//t_var	*creat_local_var(char *key, char *val);
void	update_local_var(t_env *minishell, char *s);
void	clean_var(t_env *minishell);
//local_var_name.c
char	*find_var_name(char *str, t_meta *meta, int i);
char	*manage_name(char *str);
char	*manage_brace(char *str);
//local_var_value.c
char	*find_value_var(t_list *list, char *var_name);
//get_env_var
int		is_var_in_env(t_env *minishell, char *var_name);
char	*var_env_value(t_env *minishell, char *var_name);

//fonction debug
t_list *add_local_var(t_list *list, const char *key, const char *val);
void print_token_list(t_list *list);
void print_token_list2(t_list *list);
void print_token_type(t_token_type type);


#endif
