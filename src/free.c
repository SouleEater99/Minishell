#include "../include/minishell.h"

void ft_lst_free_env()
{
    t_env *head;
    t_env *tmp;

    head = data->new_env;
    while (head)
    {
        tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
    }
}

void ft_free_all(char *error, int status)
{
    if (data)
    {
        if (data->line)
            free(data->line);
        if (data->result)
            free(data->result);
        if (data->cwd)
            free(data->cwd);
        if (data->next_line)
            free(data->next_line);
        if (data->new_env)
            ft_lst_free_env();
    }
    ft_putstr_fd(error, 2);
    exit(status);
}

// Function to initialize the memory manager
void initMemoryManager(MemoryManager* manager)
{
    manager->head = NULL;
}

// Function to allocate memory
void    *ft_malloc(MemoryManager* manager, size_t size)
{
    void    *memory;
    AllocatedAddress* allocated;

    memory = malloc(size);
    if (memory == NULL)
    {
        printf("Memory allocation failed\n"); // print in stderror this part need to fix too
        return (NULL);
    }
    allocated = (AllocatedAddress*)malloc(sizeof(AllocatedAddress)); // need to fix : protect this part of allocation and free memory  varialble too 
    allocated->address = memory;
    allocated->next = manager->head;
    manager->head = allocated;

    return (memory);
}

// Function to free allocated memory
void    ft_free(MemoryManager   *manager, void    *address)
{
    AllocatedAddress    *current = manager->head;
    AllocatedAddress    *previous = NULL;

    while (current != NULL)
    {
        if (current->address == address)
        {
            if (previous == NULL)
                manager->head = current->next;
            else
                previous->next = current->next;
            free(current->address);
            free(current);
            return;
        }

        previous = current;
        current = current->next;
    }
    printf("Invalid address. Memory not found.\n"); // Need To Fix Print Error In StdError.
}

void    ft_freeAll(MemoryManager    *manager)
{
    AllocatedAddress    *current;

    current = manager->head;
    while (current != NULL)
    {
        AllocatedAddress* temp = current;
        current = current->next;
        free(temp->address);
        free(temp);
    }
    manager->head = NULL;
}