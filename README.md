# ft_ssl_md5
A Cryptography Project at Codam (42) - Hashing algorithms MD5 SHA1 SHA2

## Description

An introduction to cryptographic hashing algorithms. Recode part of the OpenSSL program, specifically the MD5 and SHA-256 hashing algorithms. [Complete Subject](SUBJECT.pdf)

This project is tested and runs on MacOS.

Use [Evaluation sheet](https://github.com/k-off/ft_ssl_md5/wiki/Evaluation-Sheet) or [Test](00_test.sh)

### Restrictions
Functions allowed for use:
```
open(3)
close(3)
malloc(3)
free(3)
read(2)
write(2)
```
Project must be written according to the 42 coding standards, the `Norme`.

### Requirements
Rewrite the following functions:

`MD5` `SHA256`

#### Mandatory options for all functions
  - `-p` echo STDIN to STDOUT and append the checksum to STDOUT
  - `-q` output only checksum
  - `-r` reverse the format of the output
  - `-s "some string"` print the sum of the given string

[openssl man page](https://linux.die.net/man/1/openssl)

### Bonus part
Extra functions:
`SHA1` `SHA224` `SHA384` `SHA512`

## Usage
 - clone repository, go to the cloned directory and run command in terminal:

```
sh 00_test.sh
```
or
```
./ft_ssl command [ command_opts ] [ command_args ]
```
