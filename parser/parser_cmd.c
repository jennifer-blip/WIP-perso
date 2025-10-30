/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:37:10 by neandrie          #+#    #+#             */
/*   Updated: 2025/10/24 11:18:06 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	cmd_new(t_command **cmd)
{
	*cmd = malloc(sizeof(t_command));
	if (!(*cmd))
		return (0);
	(*cmd)->cmd = NULL;
	(*cmd)->args = NULL;
	(*cmd)->arg_count = 0;
	(*cmd)->redirs = NULL;
	(*cmd)->redir_count = 0;
	(*cmd)->next_operator = TNULL;
	return (1);
}

void	count_commands(t_token *current, int *count)
{
	bool	expect_command = true;
	t_token	*next;

	while (current)
	{
		if (expect_command)
		{
			if (current->type == WORD)
			{
				(*count)++;
				expect_command = false;
			}
			else if (is_redirection(current->type))// Vérifie que la redirection a bien une cible valide (un WORD)
			{
				next = current->next;
				if (next && next->type == WORD)// 👇 Cas redirection orpheline ou en tête de commande
				{
					(*count)++;
					expect_command = false;// On saute la cible pour éviter de la reconsidérer
					current = next;
				}
				else// Redirection incomplète -> erreur syntaxique, on peut stopper
					break ;
			}
			else if (is_command_separator(current->type))// Rien à faire, on reste en attente d’une commande
				expect_command = true;
			else// Autre token inattendu (cas rare)
				expect_command = false;
		}
		else if (is_command_separator(current->type))
			expect_command = true;
		current = current->next;
	}
}

void	add_command(t_command **dest, t_command *src)
{
	if (!dest || !src)
		return ;
	*dest = src;
}

t_error_code	fill_commands(t_fill_ctx *ctx, t_token *tok, \
	t_shell_ctxt *shell)
{
	t_error_code	err;

	if (!(ctx->current_cmd)->cmd)
	{
		(ctx->current_cmd)->cmd = ft_strdup(tok->value);
		if (!(ctx->current_cmd)->cmd)
			return (handle_parser_err(shell, ERR_MALLOC_FAILED));
	}
	if (ctx->skip_next_word)
		ctx->skip_next_word = false;
	else
	{
		err = expand_args(ctx->current_cmd, tok, shell);
		if (err != ERR_NONE)
			return (err);
	}
	return (ERR_NONE);
}
