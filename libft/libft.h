/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:58:21 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/05 11:03:27 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdarg.h>
# include <stdbool.h>

# define SUCCESS 0
# define ERROR 1

int		ft_atoi(const char *nptr);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		count_words(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strrchr(const char *s, int c);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strncpy(char *s, int len);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	**ft_split(char const *s, char c);
void	ft_bzero(void *ptr, size_t num);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	*ft_memset(void *ptr, int value, size_t num);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memmove(void *dest, const void *src, size_t n);
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
void	*ft_realloc(void *ptr, size_t size);
char	*ft_strljoin(char *s1, char *s2, int len);
char	*ft_strndup(char *s, int len);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

int		ft_lstsize(t_list *lst);
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstadd_back(t_list **lst, t_list *item);
void	ft_lstadd_front(t_list **lst, t_list *item);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstlast(t_list *lst);

// get_next_line

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 20
# endif

char	*get_next_line(int fd, bool *flag, bool free);
int		ft_strlen_gnl(char *str);
int		include_n(char *s);
char	*ft_strjoin_gnl(char **s1, char **s2);
char	*ft_strdup_gnl(char *s);
void	free_and_null(char **p);

// ft_printf

size_t	printf_ft_strlen(const char *s);
char	*printf_ft_strdup(const char *s);
char	*printf_ft_itoa(int n);
char	*printf_ft_itoa_u(unsigned int n);
void	printf_ft_putchar(char c, int *count, int fd);
void	printf_ft_putstr(char *str, int *count, int fd);
void	put_hex(unsigned long num, char chr, int *count, int fd);
void	handle_p(va_list type, int *count, int fd, char chr);
void	handle_x(va_list type, int *count, int fd, char chr);
void	handle_c(va_list type, int *count, int fd);
void	handle_s(va_list type, int *count, int fd);
void	handle_d_and_i(va_list type, int *count, int fd);
void	handle_u(va_list type, int *count, int fd);
void	handle_percent(int *count, int fd);
int		ft_printf(const char *s, int fd, ...);

#endif
