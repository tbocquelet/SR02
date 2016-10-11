#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// http://manpagesfr.free.fr/man/man2/readv.2.html

int main (){

	char *str0 = "hello ";
	char *str1 = "world\n";

	struct iovec iov[2];
	ssize_t nwritten;

	iov[0].iov_base = str0;  // adresse de debut
	iov[0].iov_len = strlen(str0); // nombre d'octets a transferer
	iov[1].iov_base = str1;
	iov[1].iov_len = strlen(str1);

	nwritten = writev(STDOUT_FILENO, iov, 2);

	return 0;
}

/*
http://stackoverflow.com/questions/10520182/linux-when-to-use-scatter-gather-io-readv-writev-vs-a-large-buffer-with-frea

In scatter and gather (i.e. readv and writev), Linux reads into multiple buffers and writes from multiple buffers.
Hence, I am confused: For which cases should scatter/gather be used and when should a single large buffer be used ?

The main convenience offered by readv, writev is:

	It allows working with non contiguous blocks of data. i.e. buffers need not be part of an array, but separately allocated.
	The I/O is 'atomic'. i.e. If you do a writev, all the elements in the vector will be written in one contiguous operation, and 
writes done by other processes will not occur in between them. e.g. say, your data is naturally segmented, and comes from different sources

Now, all three 'buffers' are not one big contiguous block. But you want to write them contiguously into a file, for 
whatever reason (say for example, they are fields in a file header for a file format).

If you use write you have to choose between:

	Copying them over into one block of memory using, say, memcpy (overhead), followed by a single write call. Then the write will be atomic.
	Making three separate calls to write (overhead). Also, write calls from other processes can intersperse between these writes (not atomic).

If you use writev instead, its all good:

	You make exactly one system call, and no memcpy to make a single buffer from the three.
	Also, the three buffers are written atomically, as one block write. i.e. if other processes also write, then these writes will not come 
in between the writes of the three vectors.

*/