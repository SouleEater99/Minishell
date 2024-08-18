/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_minishell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-maim <ael-maim@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:08:33 by ael-maim          #+#    #+#             */
/*   Updated: 2024/08/07 17:08:35 by ael-maim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void print_banner()
// {
//     printf("\n");
//     printf(CYAN   "        .---.         .-----------\n" RESET);
//     printf(BLUE   "       /     \\  __  /    ------\n" RESET);
//     printf(GREEN  "      / /     \\(  )/    -----\n" RESET);
//     printf(YELLOW "     //////   ' \\/ `   ---      " MAGENTA " ███╗   ███╗██╗███╗   ██╗██╗\n" RESET);
//     printf(RED    "    //// / // :    : ---       " MAGENTA " ████╗ ████║██║████╗  ██║██║\n" RESET);
//     printf(MAGENTA"   // /   /  /`    '--         " CYAN   " ██╔████╔██║██║██╔██╗ ██║██║\n" RESET);
//     printf(CYAN   "  //          //..\\\\          " BLUE   " ██║╚██╔╝██║██║██║╚██╗██║██║\n" RESET);
//     printf(BLUE   "         ====UU====UU====      " GREEN  " ██║ ╚═╝ ██║██║██║ ╚████║███████╗\n" RESET);
//     printf(GREEN  "             '//||\\\\`         " YELLOW " ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝\n" RESET);
//     printf(YELLOW "               ''``            " RED    " ███████╗██╗  ██╗███████╗██╗     ██╗\n" RESET);
//     printf(RED    "                               " MAGENTA " ██╔════╝██║  ██║██╔════╝██║     ██║\n" RESET);
//     printf(MAGENTA"                               " CYAN   " ███████╗███████║█████╗  ██║     ██║\n" RESET);
//     printf(CYAN   "                               " BLUE   " ╚════██║██╔══██║██╔══╝  ██║     ██║\n" RESET);
//     printf(BLUE   "                               " GREEN  " ███████║██║  ██║███████╗███████╗███████╗\n" RESET);
//     printf(GREEN  "                               " YELLOW " ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n" RESET);
//     printf(YELLOW "\n          Welcome to Minishell - Version 24.07.31\n\n" RESET);
// }

void	ft_init_minishell(int ac, char **av, char **env)
{
	if (ac != 1)
	{
		ft_free_all("error in pass more args\n", 1);
	}
	signal(SIGINT, ft_sig_handler);
	signal(SIGQUIT, SIG_IGN);
	// print_banner();
	g_data = malloc(sizeof(t_data)); // i need to use calloc instead of malloc
	if (!g_data)
		ft_free_all("error in alloc g_data\n", 1);
	ft_bzero(g_data, sizeof(t_data));
	g_data->ac = ac;
	g_data->av = av;
	g_data->old_env = env;
	ft_create_new_env();
}
