/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:25:29 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 12:55:47 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"lexer.h"

static int	is_valid_op_char(char c, char next_c)
{
	if ((c == '>' && next_c == '>') || \
		(c == '<' && next_c == '<') || \
		(c == '&' && next_c == '&') || \
		(c == '|' && next_c == '|'))
		return (1);
	return (0);
}

t_error_code	identify_op(char c, t_lexer *lexer, t_token **tokens, \
			t_shell_ctxt *ctx)
{
	if (is_valid_op_char(c, lexer->next_char))
	{
		if (lexer->buf_idx > 0)
		{
			if (!add_token(lexer, tokens, ctx))
				return (ERR_MALLOC_FAILED);
		}
		tokenize_op2(c, lexer->next_char, lexer, tokens, ctx);
		lexer->skip_next = 1;
		lexer->is_valid_op = true;
		return (ERR_NONE);
	}
	else if (c == '|' || c == '<' || c == '>')
	{
		if (lexer->buf_idx > 0)
		{
			if (!add_token(lexer, tokens, ctx))
				return (ERR_MALLOC_FAILED);
		}
		tokenize_op1(c, lexer, tokens, ctx);
		lexer->is_valid_op = true;
		return (ERR_NONE);
	}
	lexer->is_valid_op = false;
	return (ERR_NONE);
}

t_error_code	tokenize_op1(char c, t_lexer *lexer, t_token **tokens, \
		t_shell_ctxt *ctx)
{
	t_token_type	type;
	t_token			*new_token;

	if (c == '|')
		type = PIPE;
	else if (c == '<')
		type = REDIR_IN;
	else
		type = REDIR_OUT;
	lexer->buf[0] = c;
	lexer->buf[1] = '\0';
	new_token = create_token(lexer->buf, type, ctx, lexer);
	if (!new_token)
		return (ERR_MALLOC_FAILED);
	if (!append_token(tokens, new_token, ctx))
		return (ERR_PERMISSION_DENIED);
	return (ERR_NONE);
}

t_error_code	tokenize_op2(char c1, char c2, t_lexer *lexer, t_token **tokens, \
		t_shell_ctxt *ctx)
{
	t_token_type	type;
	t_token			*new_token;

	if (c1 == '>' && c2 == '>')
		type = REDIR_APPEND;
	else if (c1 == '<' && c2 == '<')
		type = HEREDOC;
	else if (c1 == '&' && c2 == '&')
		type = AND_IF;
	else
		type = OR_IF;
	lexer->buf[0] = c1;
	lexer->buf[1] = c2;
	lexer->buf[2] = '\0';
	new_token = create_token(lexer->buf, type, ctx, lexer);
	if (!new_token)
		return (ERR_MALLOC_FAILED);
	append_token(tokens, new_token, ctx);
	return (ERR_NONE);
}
