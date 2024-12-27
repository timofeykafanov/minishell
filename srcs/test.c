/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:08:56 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 21:04:45 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	*ft_strljoin_fail_1(char *s1, char *s2, int len)
// {
// 	char	*res;
// 	int		i;
// 	int		j;

// 	if (!s1)
// 		return (NULL);
// 	res = malloc(sizeof(char) * (ft_strlen(s1) + len + 1));
// 	if (!res)
// 		return (free(s1), NULL);
// 	i = 0;
// 	j = 0;
// 	while (s1[i])
// 	{
// 		res[i] = s1[i];
// 		i++;
// 	}
// 	while (j < len)
// 	{
// 		res[i] = s2[j];
// 		i++;
// 		j++;
// 	}
// 	res[i] = '\0';
// 	free(s1);
// 	return (res);
// }

// char	*ft_strljoin_fail_2(char *s1, char *s2, int len)
// {
// 	char	*res;
// 	int		i;
// 	int		j;

// 	if (!s1)
// 		return (ft_strndup(s2, len));
// 	res = NULL;//malloc(sizeof(char) * (ft_strlen(s1) + len + 1));
// 	if (!res)
// 		return (free(s1), NULL);
// 	i = 0;
// 	j = 0;
// 	while (s1[i])
// 	{
// 		res[i] = s1[i];
// 		i++;
// 	}
// 	while (j < len)
// 	{
// 		res[i] = s2[j];
// 		i++;
// 		j++;
// 	}
// 	res[i] = '\0';
// 	free(s1);
// 	return (res);
// }
