/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_env_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jereverd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:19:22 by jereverd          #+#    #+#             */
/*   Updated: 2022/11/25 18:19:23 by jereverd         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*ft_new_env(char *str)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (env == NULL)
		return (NULL);
	env->next = NULL;
	env->name = ft_make_env_name(str);
	env->value = ft_make_env_value(str);
	return (env);
}

int	ft_env_lst_size(t_env *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}

void	ft_free_env_list(t_env *env, int size)
{
	t_env	*tmp;
	int		i;

	i = 0;
	while (i < size)
	{
		tmp = env->next;
		free(env->value);
		free(env->name);
		free(env);
		env = NULL;
		env = tmp;
		i++;
	}
}

void	ft_create_env_list(t_shell *sh, char **envp)
{
	int		i;
	t_env	*previous;
	t_env	*new;

	previous = NULL;
	new = NULL;
	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		new = ft_new_env(envp[i]);
		if (!new)
		{
			ft_free_env_list(sh->env, ft_env_lst_size(sh->env));
			ft_putstr_fd("malloc failure\n", 2);
		}
		if (previous)
			previous->next = new;
		else
			sh->env = new;
		previous = new;
		i++;
	}
}
