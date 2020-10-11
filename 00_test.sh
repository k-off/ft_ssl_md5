#!/bin/sh

# usage: sh test.sh

#======COLORS======#

EOC="\033[0m";
GREEN="\033[32m";
YELLOW="\033[33m";
RED="\033[0;31m";
BLUE="\033[0;36m";
HIGH="\033[1m";

clear

# ##########################################################################
# TEST AUTHOR
# ##########################################################################

echo
echo "${HIGH}Author:${EOC}"
cat -e author
ATR1=$(cat -e author | wc -l | tr -d ' ')
ATR2=$(cat -e author | wc -c | tr -d ' ')
if [ $ATR1 -ne '1' ] || [ $ATR2 -lt '6' ]
then
echo "${RED}Author-file\tfailed${EOC}"
else
echo "${GREEN}Author-file\tpassed${EOC}"
fi

# ##########################################################################
# TEST MAKEFILE
# ##########################################################################

MK0=$(make fclean)
MK1=$(make | grep "Nothing" | wc -l | tr -d ' ')
MK2=$(make | grep "Nothing" | wc -l | tr -d ' ')
MK3=$(make clean | grep "Nothing" | wc -l | tr -d ' ')
MK4=$(make | grep "Nothing" | wc -l | tr -d ' ')
MK5=$(make fclean | grep "Nothing" | wc -l | tr -d ' ')
MK6=$(make | grep "Nothing" | wc -l | tr -d ' ')
if [ $MK1 -eq '0' ] && [ $MK2 -eq '1' ] && [ $MK6 -eq '0' ]
then
echo "${GREEN}Makefile\tpassed${EOC}"
else
echo "${RED}Makefile\tfailed${EOC}"
fi
make clean

sleep 0.5

# ##########################################################################
# CREATE TEST FILES AND FOLDERS
# ##########################################################################

rm -rf testout

mkdir testout
touch testout/md5_my testout/md5_orig

echo "And above all," > file

mkdir directory
touch no_premission_file
chmod 000 no_premission_file
touch empty_file

# ##########################################################################
# TEST FT_SSL ERROR HANDLING
# ##########################################################################

echo "${HIGH}\nError handling:${EOC}"

./ft_ssl w

./ft_ssl md5 -w

md5 directory
./ft_ssl md5 directory

md5 no_premission_file
./ft_ssl md5 no_premission_file

md5 empty_file >> testout/md5_orig
./ft_ssl md5 empty_file >> testout/md5_my

sleep 0.5

# ##########################################################################
# TEST MD5 AND FLAGS
# ##########################################################################

echo "${HIGH}\nTEST md5 AND FLAGS${EOC}"

echo "pickle rick" | md5 >> testout/md5_orig
echo "pickle rick" | ./ft_ssl md5 >> testout/md5_my

echo "Do not pity the dead, Harry." | md5 -p >> testout/md5_orig
echo "Do not pity the dead, Harry." | ./ft_ssl md5 -p >> testout/md5_my

echo "Pity the living." | md5 -q -r >> testout/md5_orig
echo "Pity the living." | ./ft_ssl md5 -q -r >> testout/md5_my

md5 file >> testout/md5_orig
./ft_ssl md5 file >> testout/md5_my

md5 -r file >> testout/md5_orig
./ft_ssl md5 -r file >> testout/md5_my

md5 -s "pity those that aren't following baerista on spotify." >> testout/md5_orig
./ft_ssl md5 -s "pity those that aren't following baerista on spotify." >> testout/md5_my

echo "be sure to handle edge cases carefully" | md5 -p file >> testout/md5_orig
echo "be sure to handle edge cases carefully" | ./ft_ssl md5 -p file >> testout/md5_my

echo "some of this will not make sense at first" | md5 file >> testout/md5_orig
echo "some of this will not make sense at first" | ./ft_ssl md5 file >> testout/md5_my

echo "but eventually you will understand" | md5 -p -r file >> testout/md5_orig
echo "but eventually you will understand" | ./ft_ssl md5 -p -r file >> testout/md5_my

echo "GL HF let's go" | md5 -p -s "foo" file >> testout/md5_orig
echo "GL HF let's go" | ./ft_ssl md5 -p -s "foo" file >> testout/md5_my

echo "one more thing" | md5 -r -p -s "foo" file -s "bar" >> testout/md5_orig
echo "one more thing" | ./ft_ssl md5 -r -p -s "foo" file -s "bar" >> testout/md5_my

echo "just to be extra clear" | md5 -r -q -p -s "foo" file >> testout/md5_orig
echo "just to be extra clear" | ./ft_ssl md5 -r -q -p -s "foo" file >> testout/md5_my

MD5DIFF=$(diff testout/md5_my testout/md5_orig | wc -l)

if [ $MD5DIFF -ne '0' ]
then
echo "${RED}md5_my and md5_orig differ:\tfailed${EOC}"
else
echo "${GREEN}md5_my and md5_orig same:\tpassed${EOC}"
fi

# ##########################################################################
# TEST OTHER HASHES
# ##########################################################################

declare -a functions=("sha1" "sha224" "sha256" "sha384" "sha512")

for i in "${functions[@]}"
do
    sleep 0.5
    echo "${HIGH}TEST ${i}${EOC}"

    echo "" | openssl ${i} >> testout/${i}_orig
    echo "" | ./ft_ssl ${i} >> testout/${i}_my

    echo "123456789012345678901234567890123456789012345678901234" | openssl ${i} >> testout/${i}_orig
    echo "123456789012345678901234567890123456789012345678901234" | ./ft_ssl ${i} >> testout/${i}_my

    echo "1234567890123456789012345678901234567890123456789012345" | openssl ${i} >> testout/${i}_orig
    echo "1234567890123456789012345678901234567890123456789012345" | ./ft_ssl ${i} >> testout/${i}_my

    echo "12345678901234567890123456789012345678901234567890123456" | openssl ${i} >> testout/${i}_orig
    echo "12345678901234567890123456789012345678901234567890123456" | ./ft_ssl ${i} >> testout/${i}_my

    echo "12345678901234567890123456789012345678901234567890123456789012" | openssl ${i} >> testout/${i}_orig
    echo "12345678901234567890123456789012345678901234567890123456789012" | ./ft_ssl ${i} >> testout/${i}_my

    echo "123456789012345678901234567890123456789012345678901234567890123" | openssl ${i} >> testout/${i}_orig
    echo "123456789012345678901234567890123456789012345678901234567890123" | ./ft_ssl ${i} >> testout/${i}_my

    echo "1234567890123456789012345678901234567890123456789012345678901234" | openssl ${i} >> testout/${i}_orig
    echo "1234567890123456789012345678901234567890123456789012345678901234" | ./ft_ssl ${i} >> testout/${i}_my

    echo "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123" | openssl ${i} >> testout/${i}_orig
    echo "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123" | ./ft_ssl ${i} >> testout/${i}_my

    echo "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901231234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123" | openssl ${i} >> testout/${i}_orig
    echo "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901231234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123" | ./ft_ssl ${i} >> testout/${i}_my
   
    DIFF=$(diff testout/${i}_my testout/${i}_orig | wc -l)

    if [ $DIFF -ne '0' ]
    then
    echo "${RED}${i}_my and ${i}_orig differ:\tfailed${EOC}"
    else
    echo "${GREEN}${i}_my and ${i}_orig same:\tpassed${EOC}"
    fi
done

rm -f no_premission_file empty_file file
rm -rf directory