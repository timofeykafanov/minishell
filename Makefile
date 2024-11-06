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
			$(SRCSDIR)/init_memory.c \
			$(SRCSDIR)/signals.c \
			$(SRCSDIR)/freeing.c \
			$(SRCSDIR)/lexer.c \
			$(SRCSDIR)/lexer_utils.c \
			$(SRCSDIR)/expander.c \
			$(SRCSDIR)/parser.c \
			$(SRCSDIR)/parsing_utils.c \
			$(SRCSDIR)/parsing_utils_2.c \
			$(SRCSDIR)/builtins_env_unset.c  \
			$(SRCSDIR)/builtins_pwd_exit.c \
			$(SRCSDIR)/builtins_cd.c \
			$(SRCSDIR)/export.c  \
			$(SRCSDIR)/echo.c \
			$(SRCSDIR)/executor/executor.c \
			$(SRCSDIR)/executor/executor_utils.c \
			$(SRCSDIR)/executor/handle_redir.c \
			$(SRCSDIR)/executor/single.c \
			$(SRCSDIR)/executor/first.c \
			$(SRCSDIR)/executor/next.c \
			$(SRCSDIR)/executor/last.c \
			$(SRCSDIR)/find_path.c \
			$(SRCSDIR)/export_utils.c \
			$(SRCSDIR)/heredoc.c \


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