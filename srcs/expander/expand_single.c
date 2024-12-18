/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_single.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:45:47 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/19 12:47:47 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_single(char *s)
{
	char	*res;
	char	*start;

	res = NULL;
	start = s;
	if (is_single_quote(*s) && is_single_quote(*(s + 1)))
	{
		free(s);
		return (ft_strdup(""));
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
	free(start);
	return (res);
}
