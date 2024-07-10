/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumi <yumi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 04:21:47 by yumi              #+#    #+#             */
/*   Updated: 2024/05/27 05:16:21 by yumi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

typedef struct s_node
{
    int value;
    struct s_node *left;
    struct s_node *right;

} t_node ;

t_node *create_node(int value)
{
    t_node *node = (t_node *)malloc(sizeof(t_node));
    if (!node)
        return (NULL);
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

void print_tree(t_node *root)
{
    if (!root)
        return ;
    // printf("lfet: ");
    print_tree(root->left);
    
    printf("value: %d\n", root->value);

    // printf("right: ");
    print_tree(root->right);

    // printf("value: %d\n", root->value);
}

int main()
{
    t_node *root  = create_node(10);
    t_node *node1 = create_node(11);
    t_node *node2 = create_node(12);
    t_node *node3 = create_node(13);
    t_node *node4 = create_node(14);
    t_node *node5 = create_node(15);

    root->left = node1;
    root->right = node2;
    
    node1->left = node3;
    node1->right = NULL;

    node3->left = NULL;
    node3->right = NULL;

    node2->left = node4;
    node2->right = node5;

    node4->left = NULL;
    node4->right = NULL;

    node5->left = NULL;
    node5->right = NULL;

    print_tree(root);

    free(root);
    free(node1);
    free(node2);
    free(node3);
    free(node4);
    free(node5);


    return (0);
}