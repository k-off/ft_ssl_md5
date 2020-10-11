/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 14:25:43 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 13:42:18 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_H
# define __UTILS_H

# include <stdlib.h>

int			ft_strcmp(char *s1, char *s2);
char		*ft_strchr(const char *s, int c);

/**
@brief Check length of string
@param string pointer to string
@return length
*/
size_t		ft_strlen(const char *s);
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memalloc(size_t size);
void		*ft_memcpy(void *dst, const void *src, size_t n);

#endif
