/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_arg.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:14 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 11:36:28 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arg *add_argument(t_arg *head, char *value)
{
    t_arg *new_node;
    t_arg *temp;

    new_node = malloc(sizeof(t_arg));
    if (!new_node) 
        return (NULL);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;

    if (!head)
        return (new_node);
    temp = head;
    while (temp->next)
    {
        temp = temp->next;
    }
    temp->next = new_node;
    return (head);
}

void free_arguments(t_arg *head)
{
    t_arg *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp->value);
        free(temp);
    }
}
