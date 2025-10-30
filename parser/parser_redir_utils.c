/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:00:21 by jodde             #+#    #+#             */
/*   Updated: 2025/10/24 12:31:50 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_redirection **realloc_dirs(t_redirection **old_ptr, int old_count, int new_count)
{
    t_redirection **new_ptr;
	int				i;

	i = 0;
    if (!old_ptr)
        return (malloc(sizeof(t_redirection *) * new_count));
    if (new_count <= 0)
    {
        free_redirs(old_ptr, old_count);
        return (free(old_ptr), NULL);
    }
    new_ptr = malloc(sizeof(t_redirection *) * new_count);
    if (!new_ptr)
    {
        free_redirs(old_ptr, old_count);
        return (free(old_ptr), NULL);
    }
    while (i < old_count)
	{
        new_ptr[i] = old_ptr[i];
		i++;
	}
    return (free(old_ptr), new_ptr);
}

t_redirection	*redir_new(t_token_type type)
{
	t_redirection	*redir;

	redir = ft_calloc(1, sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->fd = -1;
	return (redir);
}

t_redirection	*redir_create(t_token_type type,
					const char *filename, const char *delimiter)
{
	t_redirection	*redir;
	t_error_code	err;

	redir = redir_new(type);
	if (!redir)
		return (NULL);
	err = redir_set_filename(redir, filename);
	if (err != ERR_NONE)
		return (NULL);
	err = redir_set_delimiter(redir, delimiter);
	if (err != ERR_NONE)
		return (NULL);
	return (redir);
}

t_error_code	redir_set_filename(t_redirection *redir,
					const char *filename)
{
	if (!filename)
		return (ERR_NONE);
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

t_error_code	redir_set_delimiter(t_redirection *redir,
					const char *delimiter)
{
	if (!delimiter)
		return (ERR_NONE);
	redir->delimiter = ft_strdup(delimiter);
	if (!redir->delimiter)
	{
		free(redir->filename);
		free(redir);
		return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}
