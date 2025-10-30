/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_finalize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jodde <jodde@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:23:37 by jodde             #+#    #+#             */
/*   Updated: 2025/10/22 14:47:18 by jodde            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*nt finalize_buffer(t_lexer *lexer, t_token **tokens, t_shell_ctxt *ctx) 
{
    int res = 1;

    lexer->buf[lexer->buf_idx] = '\0';

    if (lexer->in_quotes || !contains_IFS(lexer->buf)) {
        res = add_token(lexer, tokens, ctx);
    } else {
        int field_count = 0;
        char **fields = split_on_IFS(lexer->buf, &field_count);
        int i = 0;
        while (i < field_count && res) {
            // Copier le champ dans le buffer temporairement
            int j = 0;
            while (fields[i][j] && j < sizeof(lexer->buf)-1) {
                lexer->buf[j] = fields[i][j];
                j++;
            }
            lexer->buf[j] = '\0';
            lexer->buf_idx = j;

            res = add_token(lexer, tokens, ctx);
            free(fields[i]);
            i++;
        }
        free(fields);
    }

    lexer->buf_idx = 0;
    return res;
}*/