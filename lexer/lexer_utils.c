/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:44:08 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 12:46:00 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"lexer.h"

int	add_token(t_lexer *lexer, t_token **tokens, t_shell_ctxt *ctx)
{
	t_token	*new;

	lexer->buf[lexer->buf_idx] = '\0';
	new = create_token(lexer->buf, WORD, ctx, lexer);
	if (!new)
		return (handle_lexer_err(ctx, ERR_MALLOC_FAILED), 0);
	append_token(tokens, new, ctx);
	lexer->buf_idx = 0;
	return (1);
}

t_token	*create_token(const char *value, t_token_type type, t_shell_ctxt *ctx, t_lexer *lexer)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (handle_lexer_err(ctx, ERR_MALLOC_FAILED), NULL);
	new->value = ft_strdup(value);
	if (!new->value)
		return (handle_lexer_err(ctx, ERR_MALLOC_FAILED), NULL);
	if (lexer->in_quotes)
	{
		new->quoted = true;
		lexer->in_quotes = false;
	}
	else
		new->quoted = false;
	if (lexer->expand)
	{
		new->expand = true;
		lexer->expand = false;
	}
	else
		new->expand = false;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

int	append_token(t_token **head, t_token *new_token, t_shell_ctxt *ctx)
{
	t_token	*current;

	if (!head || !new_token)
		return (handle_lexer_err(ctx, ERR_PERMISSION_DENIED), 0);
	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		new_token->prev = current;
		current->next = new_token;
	}
	return (1);
}

void	test_empty_quote(char c, t_lexer *lexer)
{
	if (c == '\'' && lexer->in_single == 1 && lexer->buf_idx == 0)
		lexer->empty_quote = true;
	else if (c == '"' && lexer->in_double == 1 && lexer->buf_idx == 0)
		lexer->empty_quote = true;
}
t_error_code	handle_lexer_err(t_shell_ctxt *ctx, t_error_code err)
{
	if (err == ERR_MALLOC_FAILED)
		print_error("Malloc failed while :", "create token");
	if  (err == ERR_PERMISSION_DENIED)
		print_error("append_token_failed", "null argument");
	
	ctx->exit_status = map_error_code(err, false);
	return (err);
}