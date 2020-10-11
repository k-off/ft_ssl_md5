/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sha256.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 15:32:52 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:26:32 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_digest.h"

uint32_t g_primes_sha256[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/**
@brief Main compression function for 32-bit version of SHA2 algorithm
@param temporary_hash temporary working array
@param message current message chunk casted to uint_32
@param cycle number of current cycle
@return zero on success
*/
static int		compress(uint32_t *temporary_hash, uint32_t *message, int cycle)
{
	uint32_t	s[2];
	uint32_t	tmp[2];
	uint32_t	ch;
	uint32_t	maj;

	s[1] = ((temporary_hash[4] >> 6) | (temporary_hash[4] << (32 - 6))) ^
			((temporary_hash[4] >> 11) | (temporary_hash[4] << (32 - 11))) ^
			((temporary_hash[4] >> 25) | (temporary_hash[4] << (32 - 25)));
	ch = (temporary_hash[4] & temporary_hash[5]) ^ ((~temporary_hash[4]) & temporary_hash[6]);
	tmp[0] = temporary_hash[7] + s[1] + ch + g_primes_sha256[cycle] + message[cycle];
	s[0] = ((temporary_hash[0] >> 2) | (temporary_hash[0] << (32 - 2))) ^
			((temporary_hash[0] >> 13) | (temporary_hash[0] << (32 - 13))) ^
			((temporary_hash[0] >> 22) | (temporary_hash[0] << (32 - 22)));
	maj = (temporary_hash[0] & temporary_hash[1]) ^ (temporary_hash[0] & temporary_hash[2]) ^ (temporary_hash[1] & temporary_hash[2]);
	tmp[1] = s[0] + maj;
	temporary_hash[7] = temporary_hash[6];
	temporary_hash[6] = temporary_hash[5];
	temporary_hash[5] = temporary_hash[4];
	temporary_hash[4] = temporary_hash[3] + (uint32_t)tmp[0];
	temporary_hash[3] = temporary_hash[2];
	temporary_hash[2] = temporary_hash[1];
	temporary_hash[1] = temporary_hash[0];
	temporary_hash[0] = (uint32_t)(tmp[0] + tmp[1]);
	return (0);
}

/**
@brief Set temporary working array of 32-bit words
@param work temporary working array
@param message current message chunk casted to uint_32
@return temporary working array
*/
static uint32_t	*set_work(uint32_t *work, uint32_t *message)
{
	int			i;
	uint32_t	s0;
	uint32_t	s1;

	ft_memset(work, 0, sizeof(uint32_t) * 64);
	set_vars32(work, (uint32_t*)message, 16, !ADD);
	i = 16;
	while (i < 64)
	{
		s0 = ((work[i - 15] >> 7) | (work[i - 15] << (32 - 7))) ^
			((work[i - 15] >> 18) | (work[i - 15] << (32 - 18))) ^ (work[i - 15] >> 3);
		s1 = ((work[i - 2] >> 17) | (work[i - 2] << (32 - 17))) ^
			((work[i - 2] >> 19) | (work[i - 2] << (32 - 19))) ^ (work[i - 2] >> 10);
		work[i] = work[i - 16] + s0 + work[i - 7] + s1;
		i++;
	}
	return (work);
}

/**
@brief Main loop of 32-bit version of SHA2 algorithm
@param function_name function name
@param hash array of 32-bit hash words
@param head beginning of linked list of message chunks
@return zero on success, error number otherwise
*/
static int		main_loop(char *function_name, uint32_t *hash, t_message *head)
{
	uint32_t			temporary_hash[8];
	uint32_t			work[64];
	int					i;

	while (head)
	{
		change_endian32((uint32_t*)head->string, head->next == NULL);
		set_work(work, (uint32_t*)head->string);
		set_vars32(temporary_hash, hash, 8, !ADD);
		i = 0;
		while (i < 64)
		{
			if (compress(temporary_hash, work, i))
				print_error(function_name, "", NULL, COMPRESSION_ERROR);
			i++;
		}
		set_vars32(hash, temporary_hash, 8, ADD);
		head = head->next;
	}
	return (0);
}

t_data			*sha256(int message_flag, int file_descr, char *string)
{
	static t_data		data;

	ft_memset(&data, 0, sizeof(data));
	if (file_descr >= 0 ? read_fd(&data, file_descr) : read_str(&data, string))
		print_error("sha256", "", NULL, NO_PERMISSION);
	data.is_file = file_descr > 0;
	data.filename = file_descr > 0 ? string : NULL;
	data.is_string = file_descr < 0;
	data.string = file_descr < 0 ? string : NULL;
	set_vars32((uint32_t*)data.hash, (uint32_t[]){0x6a09e667, 0xbb67ae85,
				0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c,
				0x1f83d9ab, 0x5be0cd19}, 8, !ADD);
	if (main_loop("sha256", (uint32_t*)data.hash, data.chunk))
		print_error("sha256", "", NULL, MAIN_LOOP_ERROR);
	if (message_flag)
		print_message(data.chunk);
	free_data(data.chunk);
	return (&data);
}

t_data			*sha224(int message_flag, int file_descr, char *string)
{
	static t_data		data;

	ft_memset(&data, 0, sizeof(data));
	if (file_descr >= 0 ? read_fd(&data, file_descr) : read_str(&data, string))
		print_error("sha224", "", NULL, NO_PERMISSION);
	data.is_file = file_descr > 0;
	data.filename = file_descr > 0 ? string : NULL;
	data.is_string = file_descr < 0;
	data.string = file_descr < 0 ? string : NULL;
	set_vars32((uint32_t*)data.hash, (uint32_t[]){0xc1059ed8, 0x367cd507,
								0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511,
								0x64f98fa7, 0xbefa4fa4}, 8, !ADD);
	if (main_loop("sha224", (uint32_t*)data.hash, data.chunk))
		print_error("sha224", "", NULL, MAIN_LOOP_ERROR);
	if (message_flag)
		print_message(data.chunk);
	free_data(data.chunk);
	return (&data);
}
