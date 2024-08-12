/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:53:06 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/12 17:45:52 by sopperma         ###   ########.fr       */
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
    char *temp;
    bool flag = 0;
    size_t content_size;

    // Open the heredoc file for reading
    fd = open("heredoc.txt", O_RDONLY);
    if (fd == -1)
        return NULL;

    content = NULL;
    content_size = 0;

    // Read each line using get_next_line and concatenate
    while ((line = get_next_line(fd, &flag,false)) != NULL)
    {
        size_t line_len = ft_strlen(line);
        temp = realloc(content, content_size + line_len + 1);
        if (!temp)
        {
            free(content);
            free(line);
            close(fd);
            return NULL;
        }
        content = temp;
        strcpy(content + content_size, line);
        content_size += line_len;
        free(line);
    }

    // Close the file
    close(fd);

    // Null-terminate the string
    if (content)
    {
        temp = realloc(content, content_size + 1);
        if (!temp)
        {
            free(content);
            return NULL;
        }
        content = temp;
        content[content_size] = '\0';
    }

    return content;
}