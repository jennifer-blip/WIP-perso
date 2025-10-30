/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:02:55 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 14:56:54 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int is_exit(t_token *tok)
{
    return (tok->value[1] == '?');
}

static char	*expand_exit_status(t_shell_ctxt *shell)
{
	char	*tmp;
	int		i;	

	i = 0;
	tmp = ft_itoa(shell->exit_status);
	if (!tmp)
		return (handle_parser_err(shell, ERR_MALLOC_FAILED), NULL);
	while (tmp[i])
	{
		shell->last_exit_status[i] = tmp[i];
		i++;
	}
	free(tmp);
	return (shell->last_exit_status);
}

t_error_code    add_exit_arg(t_command *cmd, t_token *tok, t_shell_ctxt *shell)
{
    tok->value = expand_exit_status(shell);
    if (!tok->value)
        return (ERR_MALLOC_FAILED);
    return (add_new_arg(cmd, tok->value, shell));
}