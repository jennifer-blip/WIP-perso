/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 11:43:10 by neandrie          #+#    #+#             */
/*   Updated: 2025/10/22 09:20:01 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	finalize_command(t_fill_ctx *ctx, t_program *prog)
{
	if (ctx->current_cmd && ctx->current_cmd->cmd)
	{
		add_command(&prog->cmds[ctx->idx], ctx->current_cmd);
		ctx->current_cmd = NULL;
	}
}

t_error_code	handle_word_token(t_fill_ctx *ctx, t_token *tok, \
	t_shell_ctxt *shell)
{
	t_error_code	err;

	if (!ctx->current_cmd->cmd)
	{
		ctx->current_cmd->cmd = ft_strdup(tok->value);
		if (!ctx->current_cmd->cmd)
			return (handle_parser_err(shell, ERR_MALLOC_FAILED));
		if (ctx->pending_count > 0)
		{
			ctx->current_cmd->redirs = ctx->pending_redirs;
			ctx->current_cmd->redir_count = ctx->pending_count;
			ctx->pending_redirs = NULL;
			ctx->pending_count = 0;
		}
	}
	err = fill_commands(ctx, tok, shell);
	if (err != ERR_NONE)
		return (handle_parser_err(shell, ERR_MALLOC_FAILED));
	return (err);
}

t_error_code	handle_redirection_token(t_fill_ctx *ctx, t_token *tok, \
	t_shell_ctxt *shell)
{
	if (!fill_redirections(ctx, tok))
		return (handle_parser_err(shell, ERR_MALLOC_FAILED));
	return (ERR_NONE);
}

t_error_code	fill_program(t_token *current_tok, t_program *prog, \
	t_shell_ctxt *shell)
{
	t_fill_ctx		ctx;
	t_error_code	err;

	init_fill_loop(&ctx);
	while (current_tok && ctx.idx < prog->cmd_count)
	{
		if (!ctx.current_cmd && !cmd_new(&ctx.current_cmd))
			return (ERR_NONE);
		if (current_tok->type == WORD)
		{
			if (handle_word_token(&ctx, current_tok, shell) != ERR_NONE)
				return (ERR_MALLOC_FAILED);
		}
		else if (is_redirection(current_tok->type))
		{
			if (handle_redirection_token(&ctx, current_tok, shell) != ERR_NONE)
				return (ERR_MALLOC_FAILED);
		}
		else if (is_command_separator(current_tok->type))
			split_separator(&ctx.current_cmd, current_tok, prog, &ctx.idx);
		current_tok = current_tok->next;
	}
	finalize_command(&ctx, prog);
	err = handle_orphan_redirections(&ctx, shell);
	return (free_fill_loop(&ctx), err);
}

t_error_code	parser(t_token **tokens, t_program *prog, t_shell_ctxt *shell)
{
	t_token			*current_tok;
	t_error_code	err;

	current_tok = *tokens;
	err = ERR_NONE;
	count_commands(current_tok, &prog->cmd_count);
	if (prog->cmd_count == 0)
		return (ERR_NONE);
	prog->cmds = malloc (sizeof(t_command *) * prog->cmd_count);
	if (!prog->cmds)
		return (handle_parser_err(shell, ERR_MALLOC_FAILED));
	err = fill_program (current_tok, prog, shell);
	return (err);
}
