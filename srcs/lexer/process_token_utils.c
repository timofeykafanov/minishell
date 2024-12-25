/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 19:55:30 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/25 20:01:33 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_separator(char *s, int *len)
{
	*len = find_seperator(s) - s;
	if (*find_seperator(s) == EQUALS)
		*len += 1;
}
