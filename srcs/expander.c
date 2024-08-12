/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/12 15:24:03 by sopperma         ###   ########.fr       */
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
int is_dollar(char c)
{
    return (c == '$');
}

//needs Null terminated char pointer
//returns the corresponting value as a malloced string
char *expand_var(t_memory *memory, char *var)
{
    char *value;
    int i;
    char *temp;

    i = 0;
    value = malloc(1);
    value[0] = '\0';
    temp = ft_strjoin(var, "=");
    temp++;
    while(memory->env[i])
    {
        // printf("comparing %s with %s\n", memory->env[i], var);
        if (ft_strncmp(memory->env[i], temp, ft_strlen(temp)) == 0)
        {
            free(value);
            value = ft_strdup(ft_strchr(memory->env[i], '=') + 1);
            break;
        }
        i++;
    }
    free(temp - 1);
    free(var);
    var = NULL;
    return (value);
}

//todo add length to pointer and free var
char *expand_double(t_memory *memory, char *s)
{
    char *res;
    char *var;
    
    res = NULL;
    s++;
    while(*s)
    { 
        if (is_dollar(*s))
        {
            var = ft_strndup(s, is_var_end(s + 1) - s);
            var = expand_var(memory, var);
            res = ft_strljoin(res, var, ft_strlen(var));
            free(var);
            s = is_var_end(s + 1);
        }
        else
        {
            if(is_double_quote(*s))
                break;
            res = ft_strljoin(res, s, 1);
            s++;
        } 
    }
    return (res);
}