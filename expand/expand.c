/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:36:07 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 12:27:16 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static char	*in_brackets(int idx, char **envp, const char *input)
{
	int		j;
	char	tmp[BUF_VALID_SIZE];

	j = 0;
	while (input[idx] && input[idx] != '}' && !ft_isspace(input[idx]))
	{
		if (j >= (int) sizeof(tmp) - 1)
			break ;
		tmp[j++] = input[(idx)++];
	}
	if (input[idx] != '}')
		return (NULL);
	tmp[j] = '\0';
	(idx)++;
	return (get_env_value(tmp, envp));
}

static char	*no_brackets(int idx, char **envp, const char *input)
{
	int		j;
	char	tmp[BUF_VALID_SIZE];

	j = 0;
	while (input[idx] && valid_char_expand(input[idx]))
	{
		if (j >= (int) sizeof(tmp) - 1)
			break ;
		tmp[j++] = input[(idx)++];
	}
	tmp[j] = '\0';
	if (j == 0)
		return (NULL);
	return (get_env_value(tmp, envp));
}

t_error_code	expand_var(char *input, char **exp_value, t_shell_ctxt *shell)
{
	int		idx;

	idx = 0;
	if (input[idx] != '$')
		return (handle_parser_err(shell, ERR_SYNTAX));
	if (input[idx++] == '{')
	{
		idx++;
		*exp_value = in_brackets(idx, shell->envp, input);
		return (ERR_NONE);
	}
	else
	{
		*exp_value = no_brackets(idx, shell->envp, input);
		return (ERR_NONE);
	}
}

t_error_code	process_expand(t_command *cmd, t_token *tok, \
		t_shell_ctxt *shell)
{
	char			*exp_value;
	t_error_code	err;

	exp_value = NULL;
	if (is_exit(tok))
		return (add_exit_arg(cmd, tok, shell));
	err = expand_var(tok->value, &exp_value, shell);
	if (err != ERR_NONE)
		return (handle_parser_err(shell, err));
	if (!exp_value)
		exp_value = "";
	if (tok->quoted)
		return (add_new_arg(cmd, exp_value, shell));
	if (!tok->quoted)
		return (split_and_add(cmd, exp_value, shell));
	return (ERR_NONE);
}
