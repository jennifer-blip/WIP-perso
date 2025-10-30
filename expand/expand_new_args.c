/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:36:07 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 12:27:16 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static int  is_ifs(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

t_error_code    split_and_add(t_command *cmd, char *exp_value, t_shell_ctxt *shell)
{
    char    tmp[BUF_VALID_SIZE];
    int i;
    int j;

    i = 0;
    j = 0;
    while (exp_value[i] && is_ifs(exp_value[i]))
        i++;
    while (exp_value[i] && j < (BUF_VALID_SIZE - 1))
    {
        if (is_ifs(exp_value[i]))
        {
            if (j > 0)
            {
                tmp[j] = '\0';
                printf("tmp sent to add_new_arg=%s\n", tmp);
                if (!add_new_arg(cmd, tmp, shell))
                    return (ERR_MALLOC_FAILED);
                j = 0;
            }
            while (exp_value[i] && is_ifs(exp_value[i]))
                i++; // sauter tous les séparateurs
            continue; // repartir sur le mot suivant
        }
        tmp[j++] = exp_value[i++];
    }
    if (j > 0)
    {
        tmp[j] = '\0';
        if (!add_new_arg(cmd, tmp, shell))
                return (ERR_MALLOC_FAILED);
    }
    return (ERR_NONE);
}

/*static t_error_code add_multiple_args(t_command *cmd, char **words, int word_count, t_shell_ctxt *shell)
{
    int     i;
    int     j;
    int     old_count;
    int     new_count;
    char    **new_args;

    i = 0;
    j = 0;
    old_count = cmd->arg_count;
    new_count = word_count + old_count; 
    new_args = malloc(sizeof(char *) * (new_count + 1));
    if (!new_args)
        return (handle_parser_err(shell, ERR_MALLOC_FAILED));
    while (i < old_count)
    {
        new_args[i] = ft_strdup(cmd->args[i]);
        if (!new_args[i])
            return (free_new_args(new_args, i, shell));
    }
    while (words && j < new_count)
    {
        new_args[old_count + j] = ft_strdup(words[i]);
            if (!new_args[old_count + j])
            return (free_new_args(new_args, old_count + j, shell));
    }
    new_args[new_count] = NULL;
    free_args(cmd->args);
    cmd->args = new_args;
    cmd->arg_count = new_count;
    return (ERR_NONE);
}

t_error_code split_and_add(t_command *cmd, char *exp_value, t_shell_ctxt *shell)
{
    char    *words[BUF_VALID_SIZE]; // tableau temporaire de pointeurs
    int     word_count = 0;
    char    tmp[BUF_VALID_SIZE];
    int     i;
    int     j;

    i = 0;
    j = 0;
    while (exp_value[i] && is_ifs(exp_value[i]))
        i++;
    while (exp_value[i] && j < (BUF_VALID_SIZE - 1))
    {
        if (is_ifs(exp_value[i]))
        {
            tmp[j] = '\0';
            if (j > 0)
            {
                words[word_count++] = strdup(tmp);
                j = 0;
            }
            while (exp_value[i] && is_ifs(exp_value[i]))
                i++;
            continue;
        }
        tmp[j++] = exp_value[i++];
    }

    if (j > 0)
    {
        tmp[j] = '\0';
        words[word_count++] = strdup(tmp);
    }

    // ajout final en une seule fois :
    if (word_count > 0)
    {
        t_error_code err = add_multiple_args(cmd, words, word_count, shell);
        // libère les duplicats temporaires
        for (int k = 0; k < word_count; k++)
            free(words[k]);
        return err;
    }

    return ERR_NONE;
}*/
