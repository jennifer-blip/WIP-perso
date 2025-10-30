/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_err_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 14:21:40 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 09:23:11 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_error_code	free_new_args(char **args, int count, t_shell_ctxt *ctx)
{
	while (--count >= 0)
		free(args[count]);
	free(args);
	return (handle_parser_err(ctx, ERR_MALLOC_FAILED));
}

t_error_code	handle_parser_err(t_shell_ctxt *ctx, t_error_code err)
{
	if (err == ERR_INVALID_REDIR)
		print_error("Parser failed while :", "to many redirs");
	if (err == ERR_MALLOC_FAILED)
		print_error("Malloc failed while :", "create command");
	if  (err == ERR_SYNTAX)
		print_error("Expand failed while", "$? syntax error");
	
	ctx->exit_status = map_error_code(err, false);
	return (err);
}