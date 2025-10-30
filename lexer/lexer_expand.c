/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 19:02:55 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 14:56:54 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	valid_char_expand(char c)
{
	if (ft_isalnum(c) || c == '_' )
		return (1);
	return (0);
}

int	is_expand(const char *input, t_lexer lexer, t_token **tokens)
{
	t_token	*current_tok;
	
	if (!*tokens)
		return (input[lexer.input_idx] == '$');
	current_tok = *tokens;
	while (current_tok->next)
		current_tok = current_tok->next; 
	if (input[lexer.input_idx] == '$' && current_tok->type != HEREDOC)
		return (1);
	return (0);	
}

int	expand_alone(const char *input, t_lexer lexer)
{
	if (!input[lexer.input_idx + 1])
		return (1);
	if (input[lexer.input_idx + 1] == '\0')
		return (1);
	if (ft_isspace(input[lexer.input_idx + 1]))
		return (1);
	return (0);
}

char	*get_env_value(char *name, char **envp)
{
	int		j;

	j = 0;
	while (envp[j])
	{
		if (ft_strncmp(envp[j], \
				name, ft_strlen(name)) == 0 && envp[j][ft_strlen(name)] == '=')
			return (envp[j] + ft_strlen(name) + 1);
		j++;
	}
	return (NULL);
}

int	test_expand(const char *input, t_lexer *lexer, t_token **tokens)
{
	if (!is_expand(input, *lexer, tokens))
		return (0);
	if (lexer->in_single)
		return (0);
	if (lexer->in_double)
	{
		if (input[(*lexer).input_idx + 1] == '\"')
			return (0);
		if (input[(*lexer).input_idx + 1] == '\'')
			return (0);
	}
	if (input[(*lexer).input_idx + 1] == '\"')
		return (0);
	if (input[(*lexer).input_idx + 1] == '\'')
		return (0);
	if (expand_alone(input, *lexer))
		return (0);
	return (1);
}
