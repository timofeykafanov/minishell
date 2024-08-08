/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/08 12:22:57 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int is_single_quote(char c)
{
    return (c == '\'');
}
int is_double_quote(char c)
{
    return (c == '\"');
}
char *clean_var(char *s)
{
    char *res;
    
    res = NULL;
    while(*s)
    {
        if (is_single_quote(*s))
        {
            s = ft_strljoin(res, s, ft_strchr(s + 1, '\'') - s);
        }
        else
        {
            s = ft_strljoin(res, s, ft_strchr(s, '\'') - s);
        }
    } 
    return (res);
}