/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jereverd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 18:15:16 by jereverd          #+#    #+#             */
/*   Updated: 2022/11/25 18:15:17 by jereverd         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_export(t_shell *sh)
{
	t_command	*ptr;
	int 		i;

	if (!sh->cmd->cmd[1])
	{
		ft_print_export(sh);
		return;
	}
	ptr = sh->cmd;
	i = 1;
	while (ptr)
	{
		if (ft_parse_export_arg(ptr->cmd) == 1)
			ft_add_env(ptr->cmd[i], sh);
		i++;
		ptr = ptr->next;
	}
}
