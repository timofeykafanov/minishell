/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:18:19 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/21 16:26:23 by sopperma         ###   ########.fr       */
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
    if(is_double_quote(*s) && is_double_quote(*(s + 1)))
    {
        res = malloc(1);
        res[0] = '\0';
        return (res);
    }
    s++;
    while(*s)
    {
        
        if (is_dollar(*s))
        {
            var = ft_strndup(s, is_var_end(s + 1) - s);
            if (!var)
                return (NULL);
            var = expand_var(memory, var);
            if (!var)
                return (NULL);
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
        if (!res)
            return (NULL);
    }
    return (res);
}

char *expand_single(char *s)
{
    char *res;
    
    res = NULL;
    if(is_single_quote(*s) && is_single_quote(*(s + 1)))
    {
        res = malloc(1);
        res[0] = '\0';
        return (res);
    }
    s++;
    while(*s)
    {
        if(is_single_quote(*s))
            break;
        res = ft_strljoin(res, s, 1);
        if (!res)
            return (NULL);
        s++;
    }
    return (res);
}

void *expand_tokens(t_memory *memory)
{
    t_tokens *token;
    
    token = memory->tokens;
    while(token)
    {
        if (token->type == T_D_QUOTE)
        {
            token->data = expand_double(memory, token->data);
            if (!token->data)
                return (NULL);
        }
        else if (token->type == T_S_QUOTE)
        {
            token->data = expand_single(token->data);
            if (!token->data)
                return (NULL);  
        }
        else if (token->type == T_VAR)
        {
            token->data = expand_var(memory, token->data);
            if (!token->data)
                return (NULL);  
        }
        if (ft_strlen(token->data) == 0)
               token->type = T_WHITESPACE;
        token = token->next;
    }
    return (memory);
}

void print_tokens_as_string(t_memory *memory)
{
    t_tokens *token;
    
    token = memory->tokens;
    while(token)
    {
        printf("%s", (char*)token->data);
        token = token->next;
    }
        printf("\n");
}

char *join_tokens(t_memory *memory)
{
    t_tokens *token;
    char *res;

    res = NULL;    
    token = memory->tokens;
    while(token)
    {
        res = ft_strljoin(res, token->data, ft_strlen(token->data));
        token = token->next;
    }
        return (res);
}