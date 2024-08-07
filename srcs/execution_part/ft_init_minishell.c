#include "../../include/minishell.h"


void print_banner()
{
    printf("\n");
    printf(CYAN   "        .---.         .-----------\n" RESET);
    printf(BLUE   "       /     \\  __  /    ------\n" RESET);
    printf(GREEN  "      / /     \\(  )/    -----\n" RESET);
    printf(YELLOW "     //////   ' \\/ `   ---      " MAGENTA " ███╗   ███╗██╗███╗   ██╗██╗\n" RESET);
    printf(RED    "    //// / // :    : ---       " MAGENTA " ████╗ ████║██║████╗  ██║██║\n" RESET);
    printf(MAGENTA"   // /   /  /`    '--         " CYAN   " ██╔████╔██║██║██╔██╗ ██║██║\n" RESET);
    printf(CYAN   "  //          //..\\\\          " BLUE   " ██║╚██╔╝██║██║██║╚██╗██║██║\n" RESET);
    printf(BLUE   "         ====UU====UU====      " GREEN  " ██║ ╚═╝ ██║██║██║ ╚████║███████╗\n" RESET);
    printf(GREEN  "             '//||\\\\`         " YELLOW " ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝\n" RESET);
    printf(YELLOW "               ''``            " RED    " ███████╗██╗  ██╗███████╗██╗     ██╗\n" RESET);
    printf(RED    "                               " MAGENTA " ██╔════╝██║  ██║██╔════╝██║     ██║\n" RESET);
    printf(MAGENTA"                               " CYAN   " ███████╗███████║█████╗  ██║     ██║\n" RESET);
    printf(CYAN   "                               " BLUE   " ╚════██║██╔══██║██╔══╝  ██║     ██║\n" RESET);
    printf(BLUE   "                               " GREEN  " ███████║██║  ██║███████╗███████╗███████╗\n" RESET);
    printf(GREEN  "                               " YELLOW " ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
    printf(YELLOW "\n          Welcome to Minishell - Version 24.07.31\n\n" RESET);
}


void	ft_init_minishell(int ac, char **av, char **env)
{
    if (ac != 1)
	{
        ft_free_all("error in pass more args\n", 1);
	}
	signal(SIGINT, ft_sig_handler);
	signal(SIGQUIT, SIG_IGN);
	print_banner();
	data = malloc(sizeof(t_data));  // i need to use calloc instead of malloc 
	if (!data)
		ft_free_all("error in alloc data\n", 1);
	ft_bzero(data, sizeof(t_data));
	data->ac = ac;
	data->av = av;
	data->old_env = env;
	ft_create_new_env();
}