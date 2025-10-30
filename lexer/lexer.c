/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:23:37 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 14:47:18 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_error_code	finalize_buffer(char *buf, t_lexer *lexer, t_token **tokens, \
		t_shell_ctxt *ctx)
{
	if (lexer->buf_idx > 0 || lexer->empty_quote)
	{
		buf[lexer->buf_idx] = '\0';
		if (!add_token(lexer, tokens, ctx))
			return (free_tokens(tokens), ERR_MALLOC_FAILED);
		lexer->empty_quote = false;
		lexer->in_quotes = false;
		lexer->expand = false;
	}
	return (ERR_NONE);
}

static int	fill_buffer(char c, char *s, int *idx)
{
	if (*idx < BUF_VALID_SIZE - 1)
	{
		s[(*idx)++] = c;
		return (1);
	}
	print_error ("Buffer overflow while :", "process_char");
	map_error_code(ERR_OVERFLOW, false);
	return (0);
}

static int	process_quotes(char c, t_lexer *lexer)
{
	if ((c == '\'' && lexer->in_double == 0) || \
		(c == '"' && lexer->in_single == 0))
		test_empty_quote(c, lexer);
	if (c == '\'' && lexer->in_double == 0)
	{
		set_quotes_flag(c, &lexer->in_double, &lexer->in_single);
		lexer->in_quotes = true;
		return (1);
	}
	if (c == '\"' && lexer->in_single == 0)
	{
		set_quotes_flag(c, &lexer->in_double, &lexer->in_single);
		lexer->in_quotes = true;
		return (1);
	}
	lexer->in_quotes = false;
	return (0);
}

static t_error_code	process_char(char c, t_lexer *lexer, t_token **tokens, \
		t_shell_ctxt *ctx)
{
	if (process_quotes (c, lexer))
		return (ERR_NONE);
	if (ft_isspace(c) && !lexer->in_single && !lexer->in_double)
	{
		if (lexer->buf_idx > 0 || lexer->empty_quote)
		{
			if (!add_token(lexer, tokens, ctx))
				return (free_tokens(tokens), ERR_MALLOC_FAILED);
		}
		return (ERR_NONE);
	}
	if (!lexer->in_single && !lexer->in_double)
	{
		if (identify_op(c, lexer, tokens, ctx) != ERR_NONE)
			return (free_tokens(tokens), ERR_MALLOC_FAILED);
		if (lexer->is_valid_op)
			return (ERR_NONE);
	}
	if (!fill_buffer(c, lexer->buf, &lexer->buf_idx))
		return (free_tokens(tokens), ERR_OVERFLOW);
	return (ERR_NONE);
}

t_error_code	lexer(const char *input, t_token **tokens, \
		t_shell_ctxt *shell_ctx)
{
	t_lexer			lexer;
	t_error_code	err;

	ft_memset(&lexer, 0, sizeof(t_lexer));
	lexer.expand = false;
	lexer.in_quotes = false;
	while (input[lexer.input_idx])
	{
		lexer.next_char = input[lexer.input_idx + 1];
		if (test_expand(input, &lexer, tokens))
			lexer.expand = true;
			//err = process_expand(input, &lexer, shell_ctx);
		err = process_char(input[lexer.input_idx], &lexer, tokens, shell_ctx);
		lexer.input_idx++;
		if (lexer.skip_next)
		{
			lexer.input_idx++;
			lexer.skip_next = 0;
		}
		if (err != ERR_NONE)
			return (err);
	}
	err = finalize_buffer(lexer.buf, &lexer, tokens, shell_ctx);
	return (err);
}
