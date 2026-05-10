Exercice 01 :
-added <string.h> to use memset to manage padding 
 -Ctrl -D works perfectly fine .
-Had a problem with connecting a second client added a "while(1) " boucle to accept other clients after the first one disconnects .
----------------------------------------------------------------------------------------
Exercice 02 :

-Had to check man pthread_detach before adding it .


----------------------------------------------------------------------------------------

Exercice 03 : 
-i added a pthread mutex so that when a clients connects two different threads wont race to add the same client to the list . and the program se lock et crash (race condition) .

- Created the repeater function so that it iterates and does the job , used the iteration struct mentionned in the list.h comment section .
-----------------------------------------------------------------------------------------
Exercice 04 : 

- wrote the two functions , added #include "utils.h" in serveur.c for the functions to work.

-------------------------------------------------------------------------------------------
Exercice 05 : 
- i added the fflush(stdout) to see what the main function prints (of the test_buffer.c file) at the exact time i type instead of buffering until i press Ctrl-D.


-------------------------------------------------------------------------------------------
Exercice 06 :

- I created a buffer connected to the socket this way the read goes through the array instead of the recv call .

- 514 is 512 bytes for the message + \r \n.
- added the translate from CRLF to LF and from LF to CRLF during the exchange.


PS : i had to change the fgets in the if statement since it was blocking poll from doing its job , so i turned it into a read so that it loops back to poll


-----------------------------------------------------------------------------------------

Exercice 07 :
- added strcspn to replace every \n after the nickname that the user adds by '\0' so that when they chat we wont have an ugly chat interface that looks like this nickname\n : (nickname 
: *chat*)
instead of (nickname:*chat*) on the same line . reference https://www.w3schools.com/c/ref_string_strcspn.php

-added strncmp to compare the first 9 caracters ("nickname ") to see if they match so that we continue 
- strchr to go through the whole string in search of our special caracter (":")
- i grabbed a list of 100 mot grossiers (badwords.h)(badwords.c) from this git repo ( https://github.com/darwiin/french-badwords-list/blob/master/list.txt) 
then had to (cat list.txt | tr -d '\r' | sed 's/.*/"&",/') to not manually enter " mot_grossier", each time , i also had an issue with (sed 's/.*/"&",/' list.txt) at first because it viewed @ and ! ad special caracters probably or the presence of "\r\n" endings since it was jumping lines eachtime after each word.
- i added a mutex in the handle_client so that we wont have chaos while multiple threads add and remove clients 
- the new message buffer is made out of 1024 + 16+2 +58 extra bytes because why not.


snprintf to combine 
ref (https://www.geeksforgeeks.org/c/snprintf-c-library/)

- had the over writing issue on the terminal but the conversations work perfectly fine .

----------------------------------------------------------------------


 
