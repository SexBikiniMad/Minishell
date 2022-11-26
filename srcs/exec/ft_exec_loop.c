/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jereverd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:16:22 by jereverd          #+#    #+#             */
/*   Updated: 2022/11/25 18:16:23 by jereverd         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_close_perror(int fd)
{
	if (close(fd) == -1)
		perror("close");
}

void	ft_dup2_close(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
		perror("dup2");
	ft_close_perror(fd1);
}

int ft_set_fd(t_shell *sh)
{
	if (sh->pipe[0] != 0)
		ft_dup2_close(sh->pipe[0], 0);
	if (sh->pipe[1] != 1)
		ft_dup2_close(sh->pipe[1], 1);
	if (sh->cmd->fd_in)
		ft_dup2_close(sh->cmd->fd_in, 0);
	if (sh->cmd->fd_out != 1)
		ft_dup2_close(sh->cmd->fd_out, 1);
	if (sh->saved_previous_fd)
		close(sh->saved_previous_fd);
	return (1);
}

int ft_fork(t_shell *sh)
{
	sh->pid = fork();
	if (sh->pid == 0)
	{
		if (ft_check_cmd(sh) == 1)
		{
			ft_set_fd(sh);
			if (is_built_in(sh->cmd) == 1)
			{
				ft_exec_built_in(sh);
			}
			if (execve(sh->cmd->path, sh->cmd->cmd, ft_env_list_to_tab(sh)) == -1)
				ft_perror_exit("minishell: exec failed", 1);
		}
		else
			ft_perror_exit("minishell: commant not found", 1);
	}
	else if (sh->pid > 0)
	{
		if (sh->pipe[0] != 0)
			close(sh->pipe[0]);
		if (sh->pipe[1] != 1)
			close(sh->pipe[1]);
		if (!sh->cmd->next) {
			waitpid(sh->pid, &sh->pid, 0);
			return_value = WEXITSTATUS(sh->pid);
			while (waitpid(-1, &sh->pid, 0) != -1)
				;
		}
	}
	return (1);
	// todo else error
}

int	ft_pipe(t_shell *sh)
{
	int temp;

	temp = sh->saved_previous_fd;
	if (sh->cmd->next)
	{
		pipe(sh->pipe);
		sh->saved_previous_fd = sh->pipe[0];
		sh->is_piped = 1;
	}
	else
		sh->pipe[1] = 1;
	sh->pipe[0] = temp;
	return (1);
}

void ft_fd_reset(t_shell *sh) {
	ft_dup2_close(sh->dup_std_fd[0], 0);
	sh->dup_std_fd[0] = dup(0);
	ft_dup2_close(sh->dup_std_fd[1], 1);
	sh->dup_std_fd[1] = dup(1);
}

int ft_exec_loop(t_shell *sh)
{
	while (sh->cmd)
	{
		ft_pipe(sh);
		if (sh->is_piped == 0 && is_built_in(sh->cmd) == 1)
		{
			ft_set_fd(sh);
			ft_exec_built_in(sh);
			ft_fd_reset(sh);
		}
		else
			ft_fork(sh);
		sh->cmd = sh->cmd->next;
	}
	return (0);
}