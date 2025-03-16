#include "../../includes/minishell.h"

void print_redirections(t_redir *redirs, char *type)
{
    while (redirs)
    {
        printf("  %s: %s\n", type, redirs->filename);
        redirs = redirs->next;
    }
}

void print_command_list(t_command *cmd)
{
    int i;

    while (cmd)
    {
        printf("Command:\n");
        for (i = 0; cmd->args[i]; i++)
            printf("  Arg[%d]: %s\n", i, cmd->args[i]);
        print_redirections(cmd->infiles, "Input Redirection");
        print_redirections(cmd->outfiles, "Output Redirection");
        if (cmd->pipe)
            printf("  Pipe to next command\n");
        printf("\n");
        cmd = cmd->next;
    }
}

void print_token_list(t_arg *tokens)
{
    while (tokens)
    {
        printf("Token: [%s] (Type: %d)\n", tokens->value, tokens->type);
        tokens = tokens->next;
    }
}

/*
cc -Wall -Wextra -Werror get_tokens.c get_cmd.c env_var.c env_var_utils.c test_parser.c ../utils.c ../add_arg.c ../init.c parse_init.c -o test_parser
*/

int main(int argc, char **argv, char **env)
{
    t_shell shell;

    if (argc != 2)
    {
        printf("Usage: %s \"command to test\"\n", argv[0]);
        return (1);
    }

    shell.input = argv[1];
    shell.env = env;
    shell.arguments = NULL;
    shell.cmds = NULL;

    printf("\nRaw Input: %s\n", shell.input);

    shell.arguments = tokenize_input(shell.input, &shell);
    if (!shell.arguments)
    {
        printf("Tokenization failed.\n");
        return (1);
    }

    printf("\nExpanded Tokens:\n");
    print_token_list(shell.arguments);

    shell.cmds = parse_tokens(shell.arguments);
    if (!shell.cmds)
    {
        printf("Parsing failed.\n");
        free_tokens(shell.arguments);
        return (1);
    }

    printf("\nParsed Commands:\n");
    print_command_list(shell.cmds);

    free_tokens(shell.arguments);
    free_commands(shell.cmds);
    return (0);
}
