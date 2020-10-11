# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: pacovali <pacovali@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/07/25 12:09:19 by pacovali      #+#    #+#                  #
#    Updated: 2020/10/11 15:49:40 by pacovali      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

CC = @gcc
CFLAGS = -Wall -Wextra -Werror -g3 -O0

SRCS = main.c error_messaging.c md5.c message_digest.c \
output_md.c prepare_md.c set_flags.c sha1.c sha256.c sha512.c utils_md5.c \
utils.c

OBJS = $(SRCS:.c=.o) 

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) -o $@ $^

clean:
	@$(RM) *.o *~ .DS_Store

fclean: clean
	@$(RM) $(NAME)

re: fclean all