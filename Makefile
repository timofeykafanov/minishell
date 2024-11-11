#------------------------------------------------------------------------------#
#----------------------------------PROPERTIES----------------------------------#
#------------------------------------------------------------------------------#

CC			= cc
RM			= rm -rf
CFLAGS		= -Wall -Wextra -Werror -g
DFLAGS	 	= -MD -MP
LDFLAGS     = -lreadline
IFLAGS		= -I $(INCLUDES)
MAKEFLAGS	= -j$(nproc) --no-print-directory

NAME		= minishell

SRCSDIR		= srcs
INCLUDES	= includes

SRCS		= \
			$(SRCSDIR)/main.c \
			$(SRCSDIR)/utils/init_memory.c \
			$(SRCSDIR)/utils/signals.c \
			$(SRCSDIR)/utils/freeing.c \
			$(SRCSDIR)/utils/print_error_message.c \
			$(SRCSDIR)/lexer/lexer.c \
			$(SRCSDIR)/lexer/lexer_utils.c \
			$(SRCSDIR)/lexer/process_token.c \
			$(SRCSDIR)/expander/expander.c \
			$(SRCSDIR)/expander/expander_utils.c \
			$(SRCSDIR)/expander/expand_var.c \
			$(SRCSDIR)/expander/expand_double.c \
			$(SRCSDIR)/expander/expand_single.c \
			$(SRCSDIR)/parser/parser.c \
			$(SRCSDIR)/parser/parsing_utils.c \
			$(SRCSDIR)/parser/parsing_utils_2.c \
			$(SRCSDIR)/syntax_check/syntax_check.c \
			$(SRCSDIR)/builtins/builtins_env_unset.c  \
			$(SRCSDIR)/builtins/builtins_pwd_exit.c \
			$(SRCSDIR)/builtins/builtins_cd.c \
			$(SRCSDIR)/builtins/export.c  \
			$(SRCSDIR)/builtins/export_utils.c \
			$(SRCSDIR)/builtins/echo.c \
			$(SRCSDIR)/executor/executor.c \
			$(SRCSDIR)/executor/executor_utils.c \
			$(SRCSDIR)/executor/handle_redir.c \
			$(SRCSDIR)/executor/find_path.c \
			$(SRCSDIR)/executor/single.c \
			$(SRCSDIR)/executor/first.c \
			$(SRCSDIR)/executor/next.c \
			$(SRCSDIR)/executor/last.c \
			$(SRCSDIR)/heredoc/heredoc.c \


OBJSDIR		= objs
OBJS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.o))
DEPS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.d))

#------------------------------------------------------------------------------#
#------------------------------------LIBFT-------------------------------------#
#------------------------------------------------------------------------------#

LIBDIR		= ./libft
LIBFT		= ${LIBDIR}/libft.a

#------------------------------------------------------------------------------#
#------------------------------------RULES-------------------------------------#
#------------------------------------------------------------------------------#

all		: $(NAME)

$(NAME)	: ${OBJS}
		$(MAKE) -C ${LIBDIR} all
		$(CC) ${CFLAGS} ${DFLAGS} ${IFLAGS} ${LDFLAGS} -o $@ $^ ${LIBFT}


${OBJSDIR}/%.o	: %.c
		@mkdir -p $(dir $@)
		${CC} ${CFLAGS} ${DFLAGS} ${IFLAGS} -c $< -o $@

clean	:
		$(MAKE) -C ${LIBDIR} clean
		$(RM) $(OBJSDIR)

fclean	:
		$(MAKE) -C ${LIBDIR} fclean
		$(RM) $(OBJSDIR) $(NAME)

re		:
		$(MAKE) -C ${LIBDIR} re
		$(RM) $(OBJSDIR) $(NAME)
		$(MAKE) all

-include $(DEPS)

.PHONY	: all clean fclean re