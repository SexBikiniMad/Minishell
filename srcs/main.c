/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorampon <lorampon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 12:28:44 by lorampon          #+#    #+#             */
/*   Updated: 2022/12/07 12:47:53 by lorampon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_return_value = 0;

void	ft_reset_sh(t_shell *sh)
{
	sh->is_piped = 0;
	sh->saved_previous_fd = 0;
	ft_free_arena(sh->arena);
}

int	ft_init_sh(t_shell *sh, char **envp)
{
	sh->is_piped = 0;
	sh->saved_previous_fd = 0;
	sh->saved_pwd = NULL;
	sh->dup_std_fd[0] = dup(0);
	sh->dup_std_fd[1] = dup(1);
	sh->env = NULL;
	ft_create_env_list(sh, envp);
	sh->saved_pwd = malloc(MAX_PATH);
	getcwd(sh->saved_pwd, MAX_PATH);
	if (sh->env)
		sh->home = ft_strdup(ft_env_get("HOME", sh->env));
	return (1);
}

void	ft_prompt(t_shell sh, char *buff)
{
	while (1)
	{
		ft_init_arena(&sh, ARENA_SIZE);
		signal(SIGINT, &ft_sig_ignit);
		ft_set_term(&sh);
		buff = readline("minishell: ");
		if (!buff)
		{
			printf("exit\n");
			ft_unset_term(&sh);
			exit(0);
		}
		if (ft_strlen(buff) > 0)
		{
			add_history(buff);
			sh = ft_parsing(buff, &sh);
			ft_exec_loop(&sh);
		}
		ft_reset_sh(&sh);
		free(buff);
	}
}

int	main(int ac, char **argv, char **env)
{
	char		*buff;
	t_shell		sh;

	(void)ac;
	(void)argv;
	buff = NULL;
	ft_init_sh(&sh, env);
	tcgetattr(0, &sh.old);
	ft_prompt(sh, buff);
}
