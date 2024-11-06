/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_double.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:44:34 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 13:49:08 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// TODO: add length to pointer and free var
char	*expand_double(t_memory *memory, char *s)
{
	char	*res;
	char	*var;

	res = NULL;
	if (is_double_quote(*s) && is_double_quote(*(s + 1)))
	{
		res = malloc(1);
		res[0] = '\0';
		return (res);
	}
	s++;
	while (*s)
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
			if (is_double_quote(*s))
				break ;
			res = ft_strljoin(res, s, 1);
			s++;
		}
		if (!res)
			return (NULL);
	}
	return (res);
}

// static bool	handle_var_expansion(t_memory *memory, char *s, char **res)
// {
// 	char	*var;

// 	var = ft_strndup(s, is_var_end(s + 1) - s);
// 	if (!var)
// 		return (false);
// 	var = expand_var(memory, var);
// 	if (!var)
// 		return (false);
// 	*res = ft_strljoin(*res, var, ft_strlen(var));
// 	free(var);
// 	s = is_var_end(s + 1);
// 	return (true);
// }

// static bool	expand_and_join_var(t_memory *memory, char *s, char **res)
// {
// 	while (*s)
// 	{
// 		if (is_dollar(*s))
// 		{
// 			if (!handle_var_expansion(memory, s, res))
// 				return (false);
// 		}
// 		else
// 		{
// 			if (is_double_quote(*s))
// 				break ;
// 			*res = ft_strljoin(*res, s, 1);
// 			s++;
// 		}
// 		if (!res)
// 			return (false);
// 	}
// 	return (true);
// }

// // TODO: add length to pointer and free var
// char	*expand_double(t_memory *memory, char *s)
// {
// 	char	*res;

// 	res = NULL;
// 	if (is_double_quote(*s) && is_double_quote(*(s + 1)))
// 	{
// 		res = malloc(1);
// 		res[0] = '\0';
// 		return (res);
// 	}
// 	s++;
// 	if (!expand_and_join_var(memory, s, &res))
// 	{
// 		free(res);
// 		return (NULL);
// 	}
// 	return (res);
// }
