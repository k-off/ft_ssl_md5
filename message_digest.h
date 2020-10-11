/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_digest.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: pacovali <pacovali@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/09/24 15:32:32 by pacovali      #+#    #+#                 */
/*   Updated: 2020/10/11 15:44:03 by pacovali      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MESSAGE_DIGEST_H
# define __MESSAGE_DIGEST_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "error_messaging.h"

# define ADD 1
# define P 8
# define Q 4
# define R 2
# define S 1

# define CHUNK_SIZE_BYTES 64
# define CHUNK_SIZE_BITS 512
# define TRAILING_BIT 0x80
# define MESSAGE_SIZE_ADDRESS 56

typedef struct			s_message {
	long				len;
	unsigned char		string[64];
	struct s_message	*next;
}						t_message;

typedef struct			s_data {
	char				*filename;
	int					is_file;
	char				*string;
	int					is_string;
	uint64_t			hash[8];
	uint64_t			total_len;
	t_message			*chunk;
}						t_data;

/**
@brief MD5 algorithm with 128-bit output, reads from file_descriptor or string
@param message_flag print initial message if flag is set
@param file_descriptor file descriptor
@param string input string
@return address of data structure
*/
t_data					*md5(int message_flag, int file_descriptor, char *string);

/**
@brief SHA1 algorithm with 160-bit output, reads from file_descriptor or string
@param message_flag print initial message if flag is set
@param file_descriptor file descriptor
@param string input string
@return address of data structure
*/
t_data					*sha1(int message_flag, int file_descriptor, char *string);

/**
@brief SHA2 algorithm with 224-bit output, reads from file_descriptor or string
@param message_flag print initial message if flag is set
@param file_descriptor file descriptor
@param string input string
@return address of data structure
*/
t_data					*sha224(int message_flag, int file_descriptor, char *string);

/**
@brief SHA2 algorithm with 256-bit output, reads from file_descriptor or string
@param message_flag print initial message if flag is set
@param file_descriptor file descriptor
@param string input string
@return address of data structure
*/
t_data					*sha256(int message_flag, int file_descriptor, char *string);

/**
@brief SHA2 algorithm with 384-bit output, reads from file_descriptor or string
@param message_flag print initial message if flag is set
@param file_descriptor file descriptor
@param string input string
@return address of data structure
*/
t_data					*sha384(int message_flag, int file_descriptor, char *string);

/**
@brief MD5 algorithm with 512-bit output, reads from file_descriptor or string
@param message_flag print initial message if flag is set
@param file_descriptor file descriptor
@param string input string
@return address of data structure
*/
t_data					*sha512(int message_flag, int file_descriptor, char *string);

/**
@brief Iterate through flags, set them and run function if needed
@param flags pointer to flags variable
@param argument_vector pointer to current argument
@param index function index in the functions array
@return zero on success, error number otherwise
*/
int						set_flags_md(int *flags, char *argument_vector, int index);

/**
@brief Read message chunks from string
@param data pointer to data structure
@param str pointer to string
@return zero on success, error number otherwise
*/
int						read_str(t_data *data, char *string);

/**
@brief Read message chunks from file, pad if needed and store message length
@param data pointer to data structure
@param file_descriptor file descriptor
@return zero on success, error number otherwise
*/
int						read_fd(t_data *data, int file_descriptor);

/**
@brief Free message chunks
@param chunk pointer to the head of linked list
*/
void					free_data(t_message *message);

/**
@brief Print initial input message if needed (-p flag)
@param message linked list of message chunks
@return zero on success, otherwise error number
*/
int						print_message(t_message *message);

/**
@brief Format output depending on flags and input source
@param flags options provided by user (-p -q -r -s)
@param function_name name of function to be printed
@param data data structure containing digest message
@param size digest message size in bits
@return zero on success
*/
int						print_md(int flags, char *function_name, t_data *data, int size);

/**
@brief Reverse endian of an array of 32-bit integers
@param *chunk array of integers
@param amount size of array
*/
void					endian_rev32(uint32_t *message, int message_size);

/**
@brief Copy (add) 32-bit values from source to destination depending on flag
@param destination destinacion array
@param source source array
@param amount size of array
@param add flag, add values of source to ones of destination if set
*/
void					set_vars32(uint32_t *destination, uint32_t *source, int am, int ad);

/**
@brief Copy (add) 64-bit values from source to destination depending on flag
@param destination destinacion array
@param source source array
@param amount size of array
@param add flag, add values of source to ones of destination if set
*/
void					set_vars64(uint64_t *destination, uint64_t *source, int amount, int add);

/**
@brief Reverse endian of an array of 32-bit integers
@param *chunk array of integers
@param is_last flag
*/
void					change_endian32(uint32_t *message, int is_last);

/**
@brief Reverse endian of an array of 64-bit integers
@param *chunk array of integers
@param is_last flag
*/
void					change_endian64(uint64_t *message, int is_last);
#endif
