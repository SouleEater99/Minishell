# include "../../include/minishell.h"

void    ft_init_hostname()
{
    int fd;
    char *tmp;

    fd = open("/etc/hostname", O_RDONLY);
    if (fd < 0)
        return ;
    data->hostname = get_next_line(fd);
    close (fd);
    tmp = ft_strchr(data->hostname, '\n');
    if (tmp)
        *tmp = '\0';
}

char    *ft_get_uid()
{
    int current_pid;
    char    *path;
    char    *tmp;
    int     fd;
    
    current_pid = fork();
    if (current_pid == 0)
        ft_free_all(NULL, 0);
    wait(NULL);
    tmp = ft_itoa(--current_pid);
    path = ft_strjoin("/proc/", tmp);
    free(tmp);
    tmp = path;
    path = ft_strjoin(tmp, "/loginuid");
    free (tmp);
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (free(path), NULL);
    tmp = get_next_line(fd);
    if (!tmp)
        return (free(path) ,close(fd) , NULL);
    return (close(fd), free(path), tmp);
}

int    ft_init_username()
{
    char    *uid;
    char    *line;
    int     fd;
    char    **tab;

    uid = ft_get_uid();
    if (!uid)
        return (-1);
    fd = open("/etc/passwd", O_RDONLY);
    if (fd < 0)
        return (free(uid), -1);
    line = get_next_line(fd);
    if (!line)
        return (free(uid), close(fd), -1);
    while (line)
    {
        if (ft_strnstr(line, uid, ft_strlen(line)))
            break ;
        free(line);
        line = get_next_line(fd);
    }
    if (!line)
        return (close(fd), free(uid), 0);
    tab = ft_split(line, ':');
    if (!tab)
        return (close(fd), free(line), free(uid), 0);
    data->username = ft_strdup(tab[0]);
    return (ft_free_tab(tab), free(uid), free(line), 0);
}

char    *ft_creat_prompt(char *str1, char c, char *str2, char *str3)
{
    char *tmp;
    char   *pwd;
    char *prompt;

    if (data->username)
        tmp = ft_strjoin(str1, data->username);
    else
        tmp = ft_strjoin(str1, "username");
    if (data->hostname)
        prompt = ft_strjoin_path(tmp, data->hostname, c);
    else
        prompt = ft_strjoin_path(tmp, "hostname", c);
    free (tmp);
    tmp = ft_strjoin(prompt, str2);
    free (prompt);
    pwd = getcwd(NULL,0);
    if (!pwd)
        return (free(tmp), ft_strdup("$ "));
    prompt = ft_strjoin(tmp, pwd);
    free(pwd);
    free(tmp);
    tmp = prompt;
    prompt = ft_strjoin(tmp, str3);
    free(tmp);
    return (prompt);
}

char    *ft_prompt()
{
    if (!data->prompt && !data->hostname)
        ft_init_hostname();
    if (!data->prompt && !data->username)
        ft_init_username();
    if (data->prompt)
        free(data->prompt);
    data->prompt = ft_creat_prompt("\n┌──(", '@', ")-[",  "]\n└─$ ");
    return (data->prompt);
} 