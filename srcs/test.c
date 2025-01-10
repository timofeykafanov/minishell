/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 20:08:56 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/10 12:57:09 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void print_commands(t_memory *memory)
// {
// 	t_command	*current;
// 	int			i;
// 	t_redir_out	*redir;
// 	current = memory->commands;
// 	while (current)
// 	{
// 		redir = current->redir_struct;
// 		i = 0;
// 		printf("\ncommand: %s\n", current->name);
// 		while (current->args[i])
// 		{
// 			printf("arg %d: %s\n", i, current->args[i]);
// 			i++;
// 		}
// 		i = 0;
// 		// while (current->redir_out[i])
// 		// {
// 		// 	printf("red %d: %s\n", i, current->redir_out[i]);
// 		// 	i++;
// 		// }
// 		while (redir)
// 		{
// 			printf("red %d: %s type: %d quoted: %d heredoc_filename: %s\n", i, redir->file_name, redir->type, redir->was_quoted, redir->heredoc_file_name);
// 			redir = redir->next;
// 			i++;
// 		}
// 		current = current->next;
// 	}
// }

// int	print_tokens(t_memory *memory)
// {
// 	t_tokens	*current;
// 	int			i;

// 	i = 1;
// 	current = memory->tokens;
// 	printf("Tokens:\n");
// 	while (current)
// 	{
// 		if (current->was_quoted)
// 			printf("Nr: %d\nToken: %s Type: %d Quoted : %d\n", i++, \
// 			(char *)current->data, current->type, current->was_quoted);
// 		else
// 			printf("Nr: %d\nToken: %s Type: %d Quoted : %d\n", i++, \
// 			(char *)current->data, current->type, current->was_quoted);
// 		current = current->next;
// 	}
// 	return (1);
// }

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
