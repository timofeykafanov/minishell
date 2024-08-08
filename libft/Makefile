#------------------------------------------------------------------------------#
#----------------------------------PROPERTIES----------------------------------#
#------------------------------------------------------------------------------#

CC 		= cc
RM		= rm -rf
AR		= ar rcs
CFLAGS	= -Wall -Wextra -Werror -MD -MP -g

NAME	= libft.a

SRCS	= ft_atoi.c ft_isdigit.c ft_memmove.c ft_split.c ft_strlcpy.c \
		ft_strtrim.c ft_bzero.c ft_isprint.c ft_memset.c ft_strchr.c \
		ft_strlen.c ft_substr.c ft_calloc.c ft_itoa.c ft_putchar_fd.c \
		ft_strdup.c ft_strmapi.c ft_tolower.c ft_isalnum.c ft_memchr.c \
		ft_putendl_fd.c ft_striteri.c ft_strncmp.c ft_toupper.c ft_isalpha.c \
		ft_memcmp.c ft_putnbr_fd.c ft_strjoin.c ft_strnstr.c ft_isascii.c \
		ft_memcpy.c ft_putstr_fd.c ft_strlcat.c ft_strrchr.c ft_lstnew.c \
		ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c \
		ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c \
		get_next_line.c get_next_line_utils.c \
		ft_printf/ft_printf.c ft_printf/printf_ft_itoa.c \
		ft_printf/printf_handle_c_s_d_i_u.c \
		ft_printf/printf_handle_p_x_percent.c ft_printf/printf_utils.c  \
		ft_strncpy.c ft_realloc.c ft_strljoin.c ft_strndup.c

OBJSDIR	= objs

OBJS	= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.o))
DEPS	= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.d))

#------------------------------------------------------------------------------#
#------------------------------------RULES-------------------------------------#
#------------------------------------------------------------------------------#

all		: $(NAME)

$(NAME)	: $(OBJS)
		$(AR) $(NAME) $^

$(OBJSDIR)/%.o		: %.c libft.h
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) -c $< -o $@

clean	:
		$(RM) $(OBJSDIR)

fclean	:
		$(RM) $(NAME) $(OBJSDIR)

re		:
		$(RM) $(OBJSDIR) $(NAME)
		$(MAKE) all

-include $(DEPS)

.PHONY	: all clean fclean re
