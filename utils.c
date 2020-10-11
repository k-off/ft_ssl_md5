/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.character                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 14:25:43 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:45:13 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int			ft_strcmp(char *left, char *right)
{
	if (!left && !right)
		return (0);
	else if (!left)
		return (right[0]);
	else if (!right)
		return (left[0]);
	while (left[0] && right[0] && left[0] == right[0])
	{
		left++;
		right++;
	}
	return (left[0] - right[0]);
}

char		*ft_strchr(const char *source, int character)
{
	while (source[0] != '\0')
	{
		if (source[0] == character)
			return ((char*)source);
		source++;
	}
	if (source[0] == character)
		return ((char*)source);
	return (0);
}

void		*ft_memset(void *source, int character, size_t size)
{
	uint8_t	*temporary_pointer;

	temporary_pointer = (uint8_t*)source;
	while (size > 0)
	{
		size--;
		temporary_pointer[size] = character;
	}
	return (source);
}

void		*ft_memalloc(size_t size)
{
	void	*memory;

	memory = NULL;
	if (size > 0)
	{
		memory = malloc(size);
		if (memory)
			ft_memset(memory, 0, size);
	}
	return (memory);
}

void		*ft_memcpy(void *destination, const void *source, size_t size)
{
	uint8_t	*temporary_destination;
	uint8_t	*temporary_source;
	size_t	i;

	if (destination == NULL && source == NULL)
		return (NULL);
	i = 0;
	temporary_destination = (uint8_t*)destination;
	temporary_source = (uint8_t*)source;
	while (i < size)
	{
		temporary_destination[i] = temporary_source[i];
		i++;
	}
	return (destination);
}
