#include "../includes/minishell.h"

void print_redirections(t_redir *redirs, char *type)
{
    while (redirs)
    {
        printf("   %s: %s\n", type, redirs->filename);
        redirs = redirs->next;
    }
}

void print_command_list(t_command *cmd)
{
    int i;
	int cmd_count;

	cmd_count = 0;
    while (cmd)
    {
        printf("Command %d:\n", cmd_count);
		i = 0;
		while (cmd->args[i])
		{
            printf("   Arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
        print_redirections(cmd->infiles, "Input Redirection");
        print_redirections(cmd->outfiles, "Output Redirection");
        if (cmd->pipe)
            printf("	Pipe to next command\n");
        printf("\n");
        cmd = cmd->next;
		cmd_count++;
    }
}
/*
cc -Wall -Wextra -Werror get_tokens.c get_cmd.c test_parser.c utils.c add_arg.c init.c parse_init.c -o test_parser
*/

int main()
{
    char *test_input = "echo hello > out1.txt >> out2.txt < in1.txt < in2.txt | cat file.txt > log.txt";
    t_arg *tokens = tokenize_input(test_input);
    t_command *cmds = parse_tokens(tokens);
    printf("Parsed Commands:\n");
    print_command_list(cmds);
	free_tokens(tokens);
	free_commands(cmds);
    return 0;
}
