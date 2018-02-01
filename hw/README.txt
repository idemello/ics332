***************************************************
Assignment #1 - Using strace to spy on system calls
Name - Isaac DeMello
Date - 2/1/18
Class - ICS 332
Professor - Henri Casanova
****************************************************


****************************************************
Exercise #1
****************************************************

----------------------------------------------------
Question #1
----------------------------------------------------

For the tracefile_java I counted 3765 system calls.
For the tracefile_c I counted 38 system calls.

I counted both by using the following commands:
grep -v "+++" tracefile_c | grep -v "resumed" tracefile_c | wc -l
grep -v "+++" tracefile_java | grep -v "resumed" tracefile_java | wc -l

The reason that java has more system calls is that java utilizes the
JVM. Because the java hello world program places systems calls that must
first go through the JVM before interacting with the systems operating
system. The C program does not have the extra layer of abstraction, because
it does not go through the JVM.

----------------------------------------------------
Question #2
----------------------------------------------------

For the tracefile_java I counted 3218 different system calls
For the tracefile_c I counted 33 different system calls

I used the following commands to get this information:
sort tracefile_java | uniq | grep -v "+++" | grep -v "resumed" | wc -l
sort tracefile_c | uniq | grep -v "+++" | grep -v "resumed" | wc -l

----------------------------------------------------
Question #3
----------------------------------------------------

Process Management
    - futex
    - gettid
Memory Management
    - madvise (2)
    - mmap (2)
File System and I\O
    - read (2)
    - write (2)
Protection and Security
    - access
    - mprotect
