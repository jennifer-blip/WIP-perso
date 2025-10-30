/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:00:21 by jodde             #+#    #+#             */
/*   Updated: 2025/10/24 12:31:50 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_error_code    deepcopy_args(char ***dest, char **src, int arg_count, t_shell_ctxt *shell)
{
    int i;

    i = 0;
    *dest = malloc (sizeof(char *) * (arg_count + 2));
    if (!(*dest))
		return (handle_parser_err(shell, ERR_MALLOC_FAILED));
    while (src && i < arg_count)
	{
		(*dest)[i] = ft_strdup(src[i]);
		if (!(*dest)[i])
			return (free_new_args(*dest, i, shell));
		i++;
	}
    (*dest)[i] = NULL;
    free_args (src);
    return (ERR_NONE);
}

t_error_code    add_new_arg(t_command *cmd, const char *value, \
	t_shell_ctxt *shell)
{
	char	        **new_args;
    t_error_code    err;

    err = ERR_NONE;
   // printf("add_new_arg : %s\n", value);
    new_args = NULL;
    err = deepcopy_args(&new_args, cmd->args, cmd->arg_count, shell);
        return (err);
	new_args[cmd->arg_count] = ft_strdup(value);
	if (!new_args[cmd->arg_count])
		return (free_new_args(new_args, cmd->arg_count + 1, shell));
	new_args[cmd->arg_count + 1] = NULL;
	cmd->arg_count++;
    cmd->args = new_args;
  /*  for (int k = 0; k < cmd->arg_count; k++)
    {
        printf ("contenu de cmd->args\n");
        printf ("%s\n", cmd->args[k]);
    }*/
    return (err);
}

t_error_code	expand_args(t_command *cmd, t_token *tok, \
	t_shell_ctxt *shell)
{
    if (tok->expand)
        return (process_expand(cmd, tok, shell));
    return (add_new_arg(cmd, tok->value, shell));
}