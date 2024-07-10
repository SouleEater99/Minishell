# minishell
minishell

To check memory leak:
```bash
    valgrind --leak-check=full --show-leak-kinds=all -s ./minishell
```

To install readline library: 
```bash
    sudo apt-get update
    sudo apt-get install libreadline-dev
```

```c
# include <stdio.h>

int main()
{
    int i = 0;

    printf("%d\n", i);
    return (0);
}
```
