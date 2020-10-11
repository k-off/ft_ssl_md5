/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prepare_md.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/25 21:32:54 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:11:06 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

int				read_str(t_data *data, char *str)
{
	t_message	*current;

	data->chunk = (t_message*)ft_memalloc(sizeof(t_message) * 1);
	current = data->chunk;
	while (ft_strlen(str))
	{
		current->len = ft_strlen(str) > CHUNK_SIZE_BYTES ? CHUNK_SIZE_BYTES : ft_strlen(str);
		ft_memcpy(current->string, str, current->len);
		data->total_len += current->len * 8;
		str += current->len;
		if (current->len && current->len < CHUNK_SIZE_BYTES)
			current->string[current->len] = (char)TRAILING_BIT;
		if (current->len < MESSAGE_SIZE_ADDRESS)
			break ;
		current->next = (t_message*)ft_memalloc(sizeof(t_message) * 1);
		current = current->next;
	}
	if (data->total_len % CHUNK_SIZE_BITS == 0)
		current->string[0] = (char)TRAILING_BIT;
	ft_memcpy(&current->string[MESSAGE_SIZE_ADDRESS], &data->total_len, 8);
	return (0);
}

int				allocate_chunk(t_message **current)
{
	current[0]->next = (t_message*)ft_memalloc(sizeof(t_message) * 1);
	current[0] = current[0]->next;
	return (current[0] == NULL);
}

/**
@brief Read message chunks from stdin, pad if needed and store message length
@param data pointer to data structure
@return zero on success, error number otherwise
*/
static int		read_stdin(t_data *data)
{
	t_message	*current;
	uint64_t	current_length;

	data->chunk = (t_message*)ft_memalloc(sizeof(t_message) * 1);
	current = data->chunk;
	current_length = 1;
	while (!(current_length == 0 && current->string[current->len] == TRAILING_BIT))
	{
		current_length = read(0, &current->string[current->len], CHUNK_SIZE_BYTES - current->len);
		current->len += current_length > 0 ? current_length : 0;
		data->total_len += current_length > 0 ? current_length * 8 : 0;
		if (current->len >= 0 && current->len < CHUNK_SIZE_BYTES)
			current->string[current->len] = TRAILING_BIT;
		if (current->len >= CHUNK_SIZE_BYTES)
			allocate_chunk(&current);
	}
	if (current->len >= MESSAGE_SIZE_ADDRESS)
		allocate_chunk(&current);
	if (data->total_len % CHUNK_SIZE_BITS == 0)
		current->string[current->len] = TRAILING_BIT;
	ft_memcpy(&current->string[MESSAGE_SIZE_ADDRESS], &data->total_len, 8);
	return (0);
}

int				read_fd(t_data *data, int file_descriptor)
{
	t_message	*current;
	uint64_t	current_length;

	if (!file_descriptor)
		return (read_stdin(data));
	data->chunk = (t_message*)ft_memalloc(sizeof(t_message) * 1);
	current = data->chunk;
	current_length = 1;
	while ((!data->total_len || current_length == CHUNK_SIZE_BYTES) && !(file_descriptor && !current_length))
	{
		current_length = read(file_descriptor, current->string, CHUNK_SIZE_BYTES);
		current->len = current_length < 0 ? 0 : current_length;
		data->total_len += current->len * 8;
		if (current->len < CHUNK_SIZE_BYTES)
			current->string[current->len] = TRAILING_BIT;
		if (current->len >= MESSAGE_SIZE_ADDRESS)
			allocate_chunk(&current);
	}
	if (data->total_len % CHUNK_SIZE_BITS == 0)
		current->string[current->len] = TRAILING_BIT;
	ft_memcpy(&current->string[MESSAGE_SIZE_ADDRESS], &data->total_len, 8);
	return (0);
}

void			free_data(t_message *chunk)
{
	t_message *current;

	while (chunk)
	{
		current = chunk;
		chunk = chunk->next;
		free(current);
	}
}
