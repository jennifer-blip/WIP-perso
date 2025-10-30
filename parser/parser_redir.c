/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:35:48 by jodde             #+#    #+#             */
/*   Updated: 2025/10/24 12:31:18 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_error_code	handle_orphan_redirections(t_fill_ctx *ctx, \
	t_shell_ctxt *shell)
{
	if (ctx->pending_redirs && ctx->pending_count > 0)
		return (handle_parser_err(shell, ERR_INVALID_REDIR));
	return (ERR_NONE);
}

t_error_code	expand_redirs(t_command *cmd)
{
	t_redirection	**new_redirs;
	int				i;

	new_redirs = ft_calloc((cmd->redir_count + 1), sizeof(t_redirection *));
	if (!new_redirs)
		return (ERR_MALLOC_FAILED);
	i = 0;
	while (cmd->redirs && i < cmd->redir_count)
	{
		new_redirs[i] = cmd->redirs[i];
		i++;
	}
	if (cmd->redirs)
		free(cmd->redirs);
	cmd->redirs = new_redirs;
	return (ERR_NONE);
}

t_error_code	add_redir(t_command *cmd, t_token_type type,
					const char *filename, const char *delimiter)
{
	t_error_code	err;
	t_redirection	*redir;

	err = expand_redirs(cmd);
	if (err != ERR_NONE)
		return (err);
	redir = redir_create(type, filename, delimiter);
	if (!redir)
		return (ERR_MALLOC_FAILED);
	cmd->redirs[cmd->redir_count] = redir;
	cmd->redir_count++;
	return (ERR_NONE);
}

static int	add_pending_redir(t_fill_ctx *ctx, t_token_type type,
						char *filename, char *delimiter)
{
	t_redirection	*redir;

	redir = redir_create(type, filename, delimiter);
	if (!redir)
		return (0);
	ctx->pending_redirs = realloc_dirs(ctx->pending_redirs, \
		sizeof(t_redirection *) * ctx->pending_count, \
		sizeof(t_redirection *) * (ctx->pending_count + 1));
	if (!ctx->pending_redirs)
		return (0);
	ctx->pending_redirs[ctx->pending_count++] = redir;
	return (1);
}

int	fill_redirections(t_fill_ctx *ctx, t_token *tok)
{
	char			*delimiter;
	char			*filename;

	delimiter = NULL;
	filename = NULL;
	if (tok->next && tok->next->type == WORD)
	{
		if (tok->type == HEREDOC)
			delimiter = tok->next->value;
		else
			filename = tok->next->value;
		if (ctx->current_cmd)
			add_redir(ctx->current_cmd, tok->type, filename, delimiter);
		else
		{
			if (!add_pending_redir(ctx, tok->type, filename, delimiter))
				return (0);
		}
		ctx->skip_next_word = true;
	}
	return (1);
}
