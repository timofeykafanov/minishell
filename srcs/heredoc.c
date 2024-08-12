/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:53:06 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/12 18:04:51 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc(char *delimiter)
{
    char	*line;
    int		fd;

    fd = open("heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    while (1)
    {
        line = readline("HEREDOC->");
        if (line == NULL || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
        {
            free(line);
            break ;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
}

char *read_heredoc_content()
{
    int fd;
    char *line;
    char *content;
    bool flag = 0;

    content = NULL;
    fd = open("heredoc.txt", O_RDONLY);
    if (fd == -1)
        return NULL;
    while ((line = get_next_line(fd, &flag,false)) != NULL)
    {
        content = ft_strljoin(content, line, ft_strlen(line));
        if (!content)
            return (NULL);
        free(line);
    }
    close(fd);
    return content;
}