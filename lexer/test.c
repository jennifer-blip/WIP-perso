
#include "tests.h"
#include "lexer.h"
#include "repl.h"
#include "parser.h"
#include <stdio.h>

// TEST DE BON FONCTIONNEMENT DU LEXER - A SUPPRIMER
void print_tokens(t_token *tokens)
{
    const char *token_type_str[TOKEN_TYPE_COUNT] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "HEREDOC",
        "AND_IF", "OR_IF", "OPEN_PAREN", "CLOSE_PAREN", "SEMICOLON"
    };

    while (tokens)
    {
        // Vérification que type est valide
        if (tokens->type < 0 || tokens->type >= TOKEN_TYPE_COUNT)
        {
            ft_printf("Token: type = (invalid: %d), value = [%s]\n",
                tokens->type,
                tokens->value ? tokens->value : "(null)");
        }
        else
        {
            ft_printf("Token: type = %s, value = [%s], quoted = %d, expand = %d\n",
                token_type_str[tokens->type],
                tokens->value ? tokens->value : "(null)",
                tokens->quoted, tokens->expand);

        }
        tokens = tokens->next;
    }
}

//test parser - ASUPPRIMER

void print_program(const t_program *prog)
{
    if (!prog) {
        printf("\033[1;31mprint_program: prog == NULL\033[0m\n");
        return;
    }

    printf("\n\033[1;36mNombre de commandes (cmd_count): %d\033[0m\n", prog->cmd_count);

    if (!prog->cmds) {
        printf("\033[1;31mprint_program: prog->cmds == NULL\033[0m\n");
        return;
    }

    for (int i = 0; i < prog->cmd_count; i++) {
        printf("\n\033[1;34m-- Commande index %d --\033[0m\n", i);
        t_command *cmd = prog->cmds[i];
        if (!cmd) {
            printf("  \033[31m--> NULL command\033[0m\n");
            continue;
        }

        printf(" \033[33mNom:\033[0m \033[32m%s\033[0m\n", cmd->cmd ? cmd->cmd : "(null)");
        printf(" \033[33mcmd->arg_count:\033[0m %d\n", cmd->arg_count);

        if (cmd->args) {
            for (int j = 0; j < cmd->arg_count; j++) {
                printf("  \033[35marg[%d]:\033[0m \033[32m%s\033[0m\n", j,
                       cmd->args[j] ? cmd->args[j] : "(null)");
            }
        }

        printf(" \033[33mcmd->redir_count:\033[0m %d\n", cmd->redir_count);

        if (cmd->redirs) {
            for (int k = 0; k < cmd->redir_count; k++) {
                t_redirection *r = cmd->redirs[k];
                // printf("  \033[36mredir[%d] = %p\033[0m\n", k, (void *)r);
                // if (!r) {
                //     printf("   \033[31m--> NULL redir\033[0m\n");
                //     continue;
                // }

                printf("   \033[35mtype:\033[0m %d\n", r->type);
                printf("   \033[35mfilename:\033[0m %p, \033[32m%s\033[0m\n",
                       (void *)r->filename,
                       r->filename ? r->filename : "(null)");

                printf("   \033[35mdelimiter:\033[0m %p, \033[32m%s\033[0m\n",
                       (void *)r->delimiter,
                       r->delimiter ? r->delimiter : "(null)");

                // printf("   \033[35mtemp_path:\033[0m %p, \033[32m%s\033[0m\n",
                //        (void *)r->temp_path,
                //        r->temp_path ? r->temp_path : "(null)");

                printf("   \033[35mfd:\033[0m %d\n", r->fd);
            }
        }
    }
}