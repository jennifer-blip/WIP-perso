/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 18:26:56 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 09:22:55 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	split_separator(t_command **current_cmd, t_token *current_tok, \
		t_program *prog, int *idx)
{
	if (*current_cmd)
		(*current_cmd)->next_operator = current_tok->type;
	if (*current_cmd && (*current_cmd)->cmd)
	{
		add_command(&prog->cmds[*idx], *current_cmd);
		(*idx)++;
		*current_cmd = NULL;
	}
}

bool	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT || \
			type == REDIR_APPEND || type == HEREDOC);
}

bool	is_command_separator(t_token_type type)
{
	return (type == PIPE || type == AND_IF || type == OR_IF || \
			type == SEMICOLON || type == OPEN_PAREN || type == CLOSE_PAREN);
}

void	init_fill_loop(t_fill_ctx *ctx)
{
	ctx->current_cmd = NULL;
	ctx->pending_redirs = NULL;
	ctx->skip_next_word = false;
	ctx->pending_count = 0;
	ctx->idx = 0;
}

void	free_fill_loop(t_fill_ctx *ctx)
{
	free_cmd(ctx->current_cmd);
	free_redirs(ctx->pending_redirs, ctx->pending_count);
}