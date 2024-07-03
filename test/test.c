#include "./minishell_main/libraries/minishell.h"

int 	main(int ac, char **av, char **env)
{
	(void **)av; 
	(void)ac;
	int i = 0;	

	if (!env[0])
	{	
		printf("++++++++++++++++++\n");
	}
	while (env[i]){
		printf("++++++++++++++++++\n");
		printf("env = %s\n",env[i++]);
	}
}
