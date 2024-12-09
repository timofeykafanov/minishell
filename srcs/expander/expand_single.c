/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_single.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:45:47 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/09 12:07:07 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_single(char *s)
{
	char	*res;

	res = NULL;
	if (is_single_quote(*s) && is_single_quote(*(s + 1)))
	{
		free(s);
		res = malloc(1);
		res[0] = '\0';
		return (res);
	}
	s++;
	while (*s)
	{
		if (is_single_quote(*s))
			break ;
		res = ft_strljoin(res, s, 1);
		if (!res)
			return (NULL);
		s++;
	}
	return (res);
}
