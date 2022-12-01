/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorampon <lorampon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 12:30:06 by lorampon          #+#    #+#             */
/*   Updated: 2022/12/01 15:22:01 by lorampon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h> 
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <termios.h>


# define MAX_PATH 4096
# define SYNTAX_ERROR -11

extern int g_return_value;

typedef struct s_arena
{
	size_t	size;
	size_t	cursor;
	void	*data;
}t_arena;

typedef struct s_command
{
	int					fd_in;
	int					fd_out;
	char				**cmd;
	char 				*path;
	struct s_command	*next;
}t_command;

typedef struct s_env
{
	char			*name;
	char 			*value;
	struct s_env	*next;
}t_env;

typedef struct s_shell
{
	bool			is_piped;
	int 			pipe[2];
	int 			dup_std_fd[2];
	int 			saved_previous_fd;
	char 			*saved_pwd;
	char 			*home;
	pid_t 			pid;
	t_command		*cmd;
	t_arena			arena;
	t_env			*env;
	struct termios	old;
	struct termios	new;
}t_shell;

//############## EXEC #######################

// arena storage pool
int		ft_init_arena(t_arena *arena, size_t size);
void	ft_free_arena(t_arena *arena);
void 	*ft_arena_alloc(t_arena *arena, size_t size);
void	*ft_alloc(size_t size, t_arena *arena);
char	*ft_strjoin_arena(char const *s1, char const *s2, t_arena *arena);
char	**ft_split_arena(const char *str, char c, t_arena *arena);
char	*ft_strdup_arena(const char *src, t_arena *arena);
char	*ft_substr_arena(char const *s, unsigned int start, size_t len, t_arena *arena);

//built_in
void	ft_echo(char **av);
void	ft_exit(t_shell *sh);
void	ft_env(t_shell *sh);
void	ft_unset(t_shell *sh);
void	ft_cd(t_shell *sh);
void 	ft_pwd(t_shell *sh);
void	ft_export(t_shell *sh);

//exec
int		ft_exec_loop(t_shell *sh);
void	ft_exec_built_in(t_shell *sh);
int 	ft_fork(t_shell *sh);
int		ft_pipe(t_shell *sh);

//exec_utils
int		is_built_in(t_command *cmd);
void	ft_check_cmd(t_shell *sh);
void	ft_check_validity(t_shell *sh);

//set_env
t_env	*ft_new_env(char *str);
void	ft_create_env_list(t_shell *sh, char **envp);
char	*ft_make_env_name(char *str);
char	*ft_make_env_value(char *str);
void	ft_add_env(char *str, t_shell *sh);
void	push_back_env(t_env *ptr, char *str);
void	ft_modify_env_value(t_env *ptr, char *str, int is_equal);
void	ft_last_list_elem(t_env **ptr);
//env_search
char	*ft_env_get(char *str, t_env *env);
char	*ft_env_get_name(char *str, t_env *env);
t_env	*ft_env_get_struct(char *str, t_env *env);
//env_utils
void	ft_del_env(char *str, t_shell *sh);
void	ft_del_list_elem(t_env *ptr);
void	ft_free_env_list(t_env *env, int size);
int 	ft_env_lst_size(t_env *lst);
char	**ft_env_list_to_tab(t_shell *sh);
int 	ft_parse_export_arg(char *str, t_arena *arena, int flag);
int 	ft_check_valid_indentifier(char *str, int flag);

void	ft_print_export(t_shell *sh);

//utils
void	ft_free_tab(char **tab);
void	ft_perror(char *str);
void	ft_perror_exit(char *str, int code);
void 	rl_replace_line (const char *text, int clear_undo);
int	 	ft_only_equal(char *str);
int 	ft_check_char(char *str, char c);
void	ft_set_term(t_shell *shell);
void	ft_unset_term(t_shell *shell);
void	ft_signal_reset(int nothing);
int		ft_signal_handle(int pid);
void	ft_sig_ignit(int signal);

//############## PARSING #######################

//Parsing
t_shell	ft_parsing(char *str, t_shell *shell);
int		check_syntax(char *str);
int		check_double_pipe(char *str);

size_t	ft_strlen_to_c(char *str, char c);
size_t	ft_strlen_alnum(char *str);
size_t 	nb_pipe(char *str);
int		ft_pass_quote(const char *str, int i);

char 	*ft_clean_str(char *str);


int		ft_fd_out(char *str);
int		ft_fd_in(char *str);
int		ft_fd_out_help(char *str, int fd, int i, bool in_or_out);

char	*find_var_name(char *str, int i, t_arena *arena);
char	*replace_var_help(char *var_name, t_env *env);
char	 *ft_fill_final(char *str, char *var, int size, int i, t_arena *arena);
char	*replace_var(char *str, t_shell *shell);
char	*replace_var_final(char *str, t_shell *shell, int i);
char	**ft_split_immune_quote(const char *str, char c);
char	**ft_split_quote(const char *str, char c, t_arena *arena);
char	**ft_split_cmd(char *str, char c, t_arena *arena);
int	ft_error_msg(int error);


// cmd bloquant + signaux bloquant
// < in abc > out > in | truc bidule | alal

#endif