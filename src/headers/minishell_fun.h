/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_fun.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-rouz <cle-rouz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:05:32 by nlaporte          #+#    #+#             */
/*   Updated: 2025/08/29 10:44:46 by cle-rouz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_FUN_H
# define MINISHELL_FUN_H

# include <dirent.h> // Pour le type DIR
# include <sys/stat.h>
/************
  BUILD_IN
*************/
//cd/cd.c
int		get_new_path(t_env *minishell);
char	*manage_tilee(t_env *minishell, const char *path);
char	*get_old_path(t_arg *arg);
char	*get_path(t_arg *arg);
int		make_chdir(t_arg *arg, char *new_path);
//cd/cd2.c
int		cd(void *p);
//cd/cd3.c
void	update_env_oldpath(t_env *minishell);
//export/export.c
void	export_part1_2(int *code, int *i, char *tmp);
char	*export_part1(int *code, char *tmp);
int		export_while(t_arg *arg, t_list *list_arg, int *code);
int		export_built_in_part2(void *p, t_arg *arg);
int		export_built_in(void *p);
//export/export2.c
t_var	*create_var(char *str, char *sep);
int		local_var_to_env_var(t_env *minishell, t_tree *node);
void	print_export_error(char *s);
//export/export3.c
t_var	*create_var_no_value(char *str);
int		export_no_arg(void *p, t_env *minishell);
//echo.c
int		echoo(void *p);
//env.c
//
void	update_last_cmd(t_env *minishell, char *s);
int		env(void *p);
//pwd.c
int		pwd(void *p);
//unset.c
int		unset_built_in(void *p);
//utils.c
t_arg	*get_arg_built_in(t_env *minishell, t_tree *node);
/************
 EXIT
 *************/
void	exit_minishell(t_env *minishell);
int		exit_minishell_built_in(void *p);
/************
 INIT
 *************/
/*init_env.c*/
void	init_env(t_env *minishell);
/*init_env2.c*/
void	load_env_user(t_env *m);
/*inti_struct_meta*/
void	ft_init_meta(t_meta *meta);
/************
 LEXING
 *************/
/*lexing_check_syntaxe.c*/
int		check_lexing(char *s, t_meta *meta);
/*lexing_flag.c */
int		flag_space(char c, t_meta *meta);
int		flag_quote(char c, t_meta *meta);
int		flag_pipe(char c, t_meta *meta);
int		flag_redir(char c, t_meta *meta);
int		flag_dollar(char c, t_meta *meta);
/*lexing_util.c */
int		ft_update_flag(char c, t_meta *meta);
/*redirection.c */
int		is_an_redir(char c);
int		check_in_1(t_list *lst);
/*redirection2.c*/
int		check_out_1(t_list *lst, int not_append);
int		check_redir(t_list *lst);
/************
 PARSING	
 *************/
/*expand_util.c*/
int		is_dollar_in(char *str, t_meta *meta);
int		is_expand_ok(char *str, t_meta *meta, int i);
char	*special_case(const char *name, t_env *minishell);
char	*delete_dollar(char *str, int i);
t_list	*remove_empty_tokens(t_list *list);
/*expand_util2.c */
void	update_token_struct(t_list *list);
char	*manage_hered(t_list *current, t_meta *meta);
void	need_expand_heredoc(t_token *token);
/*expand.c*/
char	*expand(t_env *minishell, char *str, t_meta *meta);
char	*check_which_expand(t_env *minishell, char *str, t_meta *meta, int *i);
t_list	*expand_the_list(t_env *minishell, t_list *token_list, t_meta *meta);
/*expand2.c*/
char	*expand_char(t_env *minishell, char *str, t_meta *meta, int i);
t_list	*replace_expand(t_list *list, t_list *expand_list, t_list *cur_list);
/*get_env_var.c*/
t_var	*create_classic_var(char *key, char *val);
void	clear_env_var(t_list *lst);
void	copy_env_var(t_env *minishell);
int		is_var_in_env(t_env *minishell, char *var_name);
/*get_env_var2.c*/
t_var	*get_env_var(t_env *minishell, char *var_name);
char	*var_env_value(t_env *minishell, char *var_name);
void	safe_add_env_list(t_list *lst, t_var *new_var);
/*local_var_decl.c */
t_list	*manage_var_decl(t_env *minishell);
int		nb_token_var_decl(t_list *token_list, int *nb_decl);
int		check_if_var_decl(char *str);
int		is_prev_export(char *prev_node);
/*local_var_decl2.c */
t_list	*choice_if_remove_token(t_env *minishell, t_list *token_list);
/*local_var_len.c*/
int		var_name_index_end(char *str, t_meta *meta);
int		len_manage_name(char *str);
/*local_var_list.c */
t_list	*is_var_in_local(t_env *minishell, char *var_name);
int		parse_var(char *s, char **key, char **val);
t_var	*creat_local_var(char *key, char *val);
void	clean_var(t_env *minishell);
char	*var_local_value(t_env *minishell, char *var_name);
/*local_var_name.c*/
char	*find_var_name(char *str, t_meta *meta, int i);
char	*manage_name(char *str);
/*local_var_value.c*/
char	*find_value_var(t_env *minishell, char *var_name);
/*update_local_var.c*/
void	manage_update_local_var(t_env *minishell, int nb);
void	update_local_var(t_env *minishell, char *s);
/*delete_quote.c*/
t_list	*remove_list_quotes(t_list *token_list);
char	*remove_str_quotes(char *str);
/************
  	SIG	
*************/
/*init.c*/
int		init_sig(t_sig_manage *sig_manage);
/*signals.c*/
void	sig_ctrl_c_catcher(int n);
void	sig_ctrl_c_catcher2(int n);
void	sigin_handler_heredoc(int n);
/************
  	TREE
*************/
/*EXEC*/
/*exec_one_cmd.c*/
void	exec_cmd(t_env *minishell, t_tree *node, int code);
int		exec_one_cmd(t_env *minishell, t_tree *node);
void	push_pid_in_lst(t_env *minishell, pid_t pid);
void	clear_pid_list(t_env *minishell);
/*redir_cmd.c */
int		use_redir_in_here_doc(t_tree *node);
int		use_redir_in(t_tree *node);
int		use_redir_out(t_tree *node);
/*utils.c*/
int		print_error_pipeline(char *s, char *err, int code);
int		use_redir_in_here_doc2(t_tree *node, pid_t fork_pid, char *path_p);
/*exec_pipeline.c */
int		exec_pipeline(t_env *minishell, t_tree *node, int last, int i);
/*utils*/
void	fork_dup_fd(t_env *minishell, t_tree *node, int old, int neew);
void	fork_dup(t_env *minishell, t_tree *node, int last, int i);
void	set_fd_in_node(t_env *minishell, t_tree *node, int last, int i);
void	fork_part(t_env *minishell, t_tree *node, int last, int i);
/*exec.c*/
int		exec_one_tree(t_env *minishell, t_tree *node);
int		exec_tree(t_env *minishell, t_tree *tree, t_tree *parent, int *j);
/*NODE*/
/*node.c*/
int		manage_token_redir(t_env *minishell, t_list *head, t_tree *node,
			t_token *token);
t_tree	*create_word_node_tree(t_env *minishell, t_list *head, int *valid);
/*utils.c*/
char	*cmd_is_in_path(t_env *minishell, char *cmd);
int		(*is_built_in(t_env *minishell, t_tree *node, char *str))(void *p);
/*utils2.c*/
t_tree	*init_node(t_token *token, int type);
int		manage_token_word(t_env *minishell, t_tree *node, t_token *token,
			int *has_cmd);
/*PREPARE*/
/*prepare.c*/
t_tree	*prepare(t_env *minishell, t_list *lst, int min_power, int *valid);
/*utils.c*/
t_token	*consume_token(t_list *lst);
t_token	*see_token(t_list *lst);
/*REDIR*/
/*utils.c*/
int		get_fd_redir_out(t_token *token);
void	check_token(t_token *token, t_r_out *redir_out, int *i);
/*redir.c*/
t_r_out	*get_redir_out(t_list *head);
//void	get_redir_in_here_doc(t_env *minishell, t_list *head, t_r_in *redir_in);
void	fork_heredoc(t_env *minishell, t_r_in *redir_in, t_token *token,
			t_tree *node);
/*redir2.c*/
t_r_in	*get_redir_in(t_env *minishell, t_list *head, t_tree *node);
char	*get_file_name(void);
/*heredoc.c*/
void	get_redir_in_here_doc(t_env *minishell, t_list *head, t_r_in *redir_in,
			t_tree *tree);
char	*fork_heredoc_expand(t_env *minishell, t_meta *meta, t_r_in *redir,
			char *s);
void	prepare_heredoc(t_env *minishell, t_tree *tree);
/*FREE.c*/
void	free_tree(t_tree *tree);
/*UTILS*/
/*get_next_line_utils.c*/
void	clear_buffer(t_file *file);
void	clear_file(t_file *file);
void	init_file2(t_file *file, int fd);
int		ft_strlen_gnl(char *s, int endl);
char	*ft_give_malloc(int linebreak, int size);
/*get_next_line.c*/
char	*realloc_char(t_file *file, int linebreak);
void	*read_error(t_file *file, int freee);
char	*logic(t_file *file);
char	*next_line(t_file *file);
char	*get_next_line(int fd);
/*LINKED_LIST*/
/*free_element.c*/
void	free_token_struct(t_token *token);
void	free_token_node(t_list *node);
void	free_token_list(t_list *token_list);
/*free_element2.c*/
void	free_var_node(t_list *node);
void	free_var_list(t_list *var_list);
/*linked_list_util.c*/
t_list	*ft_create_node(void *data);
t_list	*ft_add_node(t_list *list, void *data);
t_list	*ft_add_list_at_begin(t_list *list, t_list *add_list);
t_list	*ft_add_list_at_middle(t_list *list, t_list *add_list, t_list *current);
t_list	*ft_delete_node(t_list *node);
/*linked_list_util2.c*/
t_list	*ft_delete_node_var(t_list *node);
t_list	*ft_delete_remove_token(t_list *node);
t_list	*head_of_list(t_list *list);
/*libft*/
int		ft_atoi(const char *nptr);
int		ft_isalnum(int i);
int		ft_isalpha(int i);
char	*ft_itoa(int n);
void	ft_lstadd_back(t_list **lst, t_list *neww);
void	ft_lstadd_front(t_list **lst, t_list *neww);
int		ft_lstsize(t_list *lst);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char *str, char c);
void	free_split(char **split);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *src);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strlen(char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strrchr(const char *s, int c);
char	*ft_strstr(char *str, char *to_find);
/*AUTRE*/
/*tokenize_util.c */
int		is_in_charset(char c, char *charset);
int		ft_get_quote_type(const char *str);
t_tkty	ft_get_token_type(const char *data, int len);
t_token	*ft_creat_token(const char *str, int len);
/*tokenize1.c */
t_list	*tokenize_the_hell(char *str, char *charset);
/*tokenize2.c*/
char	*ft_strndup_token(const char *str, int n);
size_t	ft_token_len(char *s, char *charset, t_meta *meta);
/*read_stdin.c*/
int		wait_exec(t_env *minishell);
void	try_tokenize_hell2(t_env *minishell, int valid);
void	try_tokenize_hell(t_env *minishell, char *s, t_meta *meta);
/*read_stdin2.c */
void	read_stdin(t_env *minishell, t_meta *meta, char *str, char *tmp);

#endif
