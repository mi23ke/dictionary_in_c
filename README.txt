author: Mike Urbano

This application is called dictionary and is written in the C programming
language. It takes as input, a text file called Spanish.txt. When the program
is run, the user will need to enter the name of the text file. When the enter
key is hit, the program will read the text file. It will store the English words
with the Spanish translations by using a hash function. The output will show the
total number of items hashed, the average number of probes for each entry, the
highest number of probes for an entry and the total number of probes. Then the
program will display a list of the number of items for each number of probes.
For example, for the number of zero probes, there will be zero items. The total
number of items stored using only 1 probe is 1,191. The number of items stored
using two probes, is 786; and so on and so forth.
Then the user is shown a number of options. They may enter an s to search for a
word, an i to insert and new translation, a d to delete an entry and a q to
quit. If searching a word, and it is found, the program will display the
translations for the searched word and the number of probes that were required
to find the word. The number of probes are also displayed if the word is not
found.

*************************************
To Compile application:
gcc dictionary.c -o dictionary
To run application:
./dictionary
