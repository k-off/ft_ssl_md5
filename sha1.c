/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sha1.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 15:32:52 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:21:54 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

/**
@brief Main compression function of SHA1 algorithm
@param temporary_hash temporary working array
@param chunk current message chunk
@param cycle number of current cycle
@return zero on success
*/
static int		compress(uint32_t *temporary_hash, uint32_t *chunk, int cycle)
{
	uint32_t		f;
	static uint32_t	k[] = {0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6};

	if (cycle < 20)
		f = (temporary_hash[1] & temporary_hash[2]) | ((~temporary_hash[1]) & temporary_hash[3]);
	else if (cycle < 40)
		f = temporary_hash[1] ^ temporary_hash[2] ^ temporary_hash[3];
	else if (cycle < 60)
		f = (temporary_hash[1] & temporary_hash[2]) | (temporary_hash[1] & temporary_hash[3]) | (temporary_hash[2] & temporary_hash[3]);
	else
		f = temporary_hash[1] ^ temporary_hash[2] ^ temporary_hash[3];
	f += ((temporary_hash[0] << 5) | (temporary_hash[0] >> (32 - 5))) + temporary_hash[4] + k[cycle / 20] + chunk[cycle];
	temporary_hash[4] = temporary_hash[3];
	temporary_hash[3] = temporary_hash[2];
	temporary_hash[2] = ((temporary_hash[1] << 30) | (temporary_hash[1] >> (32 - 30)));
	temporary_hash[1] = temporary_hash[0];
	temporary_hash[0] = f;
	return (0);
}

/**
@brief Set temporary working array of 64-bit words
@param work temporary working array
@param message current message chunk
@return temporary working array
*/
static uint32_t	*set_work(uint32_t *work, uint32_t *message)
{
	int			cycle;
	uint32_t	temporary_hash;

	ft_memset(work, 0, sizeof(uint32_t) * 80);
	set_vars32(work, (uint32_t*)message, 16, !ADD);
	cycle = 16;
	while (cycle < 80)
	{
		temporary_hash = work[cycle - 3] ^ work[cycle - 8] ^ work[cycle - 14] ^ work[cycle - 16];
		work[cycle] = (temporary_hash << 1) | (temporary_hash >> 31);
		cycle++;
	}
	return (work);
}

/**
@brief Main loop of SHA1 algorithm
@param function_name function name
@param hash array of 64-bit hash words
@param head beginning of linked list of message chunks
@return zero on success, error number otherwise
*/
static int		main_loop(char *function_name, uint32_t *hash, t_message *head)
{
	uint32_t			temporary_hash[5];
	uint32_t			work[80];
	int					cycle;

	while (head)
	{
		change_endian32((uint32_t*)head->string, head->next == NULL);
		set_work(work, (uint32_t*)head->string);
		set_vars32(temporary_hash, hash, 5, !ADD);
		cycle = 0;
		while (cycle < 80)
		{
			if (compress(temporary_hash, work, cycle))
				print_error(function_name, "", NULL, COMPRESSION_ERROR);
			cycle++;
		}
		set_vars32(hash, temporary_hash, 5, ADD);
		head = head->next;
	}
	return (0);
}

t_data			*sha1(int message_flag, int file_descriptor, char *string)
{
	static t_data		data;

	ft_memset(&data, 0, sizeof(data));
	if (file_descriptor >= 0 ? read_fd(&data, file_descriptor) : read_str(&data, string))
		print_error("sha1", "", NULL, NO_PERMISSION);
	data.is_file = file_descriptor > 0;
	data.filename = file_descriptor > 0 ? string : NULL;
	data.is_string = file_descriptor < 0;
	data.string = file_descriptor < 0 ? string : NULL;
	set_vars32((uint32_t*)data.hash, (uint32_t[]){0x67452301, 0xefcdab89,
								0x98badcfe, 0x10325476, 0xc3d2e1f0}, 5, !ADD);
	if (main_loop("sha1", (uint32_t*)data.hash, data.chunk))
		print_error("sha1", "", NULL, MAIN_LOOP_ERROR);
	if (message_flag)
		print_message(data.chunk);
	free_data(data.chunk);
	return (&data);
}
