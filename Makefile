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
INCLUDES	= include

SRCS		= \
			$(SRCSDIR)/main.c \

OBJSDIR		= objs
OBJS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.o))
DEPS		= $(addprefix $(OBJSDIR)/, $(SRCS:.c=.d))

#------------------------------------------------------------------------------#
#------------------------------------RULES-------------------------------------#
#------------------------------------------------------------------------------#

all		: $(NAME)

$(NAME)	: ${OBJS}
		$(CC) ${CFLAGS} ${LDFLAGS} ${DFLAGS} ${IFLAGS} -o $@ $^

${OBJSDIR}/%.o	: %.c
		@mkdir -p $(dir $@)
		${CC} ${CFLAGS} ${DFLAGS} ${IFLAGS} -c $< -o $@

clean	:
		$(RM) $(OBJSDIR)

fclean	:
		$(RM) $(OBJSDIR) $(NAME)

re		:
		$(RM) $(OBJSDIR) $(NAME)
		$(MAKE) all

-include $(DEPS)

.PHONY	: all clean fclean re
