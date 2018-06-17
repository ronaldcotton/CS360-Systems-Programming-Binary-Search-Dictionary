/* 
 * Ronald Cotton
 * CS 360 - Dick Lang, Ph.D. - Systems Programming
 * OK program - Assignment #2
 *
 * Ok Program compares the spelling of a word from argv[1] vs. a
 * preformatted dictionary utilizing read and lseek. If no argument,
 * then return usage.  If the word is found in the dictionary, then
 * return a yes, otherwise return a no.  Utilizes the Binary Search
 * Algorithm.
 *
 * Wikipedia Article - Binary Search :
 * 	https://en.wikipedia.org/wiki/Binary_search_algorithm
 *
 * Both dictionaries are in the format of one word perline, and that
 * each line is in ascending sorted order, that each line is 16 chars
 * long using binary search.  Justified to the left with spaces
 * and each word with /n at the end
 *
 * MAIN Dictionary: /encs_share/cs/class/cs360/lib/webster
 * TINY Dictionary: /encs_share/cs/class/cs360/lab/io/tiny
 *
 * Design - Use binary search via lseek and read
 */

/* libs */
#include <stdio.h> /* for fprintf() */
#include <ctype.h> /* for tolower() */
#include <errno.h> /* for error codes */
#include <string.h> /* this is for memcpy() */
#include <stdlib.h> /* for calloc(), exit() */
#include <fcntl.h> /* for open() */
#include <unistd.h> /* for read() */

/* define */
#define DEBUG 1
#define DICTFIELDSIZE 16
#define MAXWORDSIZE 15
#define DICTFILE "webster"
#define TRUE 1
#define FALSE 0
#define divideby2(x) x >> 1
#define divideby16(x) x >> 4
#define multiplyby16(x) x << 4

/* function prototypes */
int ok(int fd, char *word);

int main (int argc, char *argv[]) {
	/* exit if the number of arguments is not 1 or if the length of the
	 * word is larger than a dictionary entry */
	if (argc!=2) { fprintf(stderr, "USAGE: ok <dictionary_word>\n"); exit(1); }
	int wordLength = strlen(argv[1]);
	if (wordLength > MAXWORDSIZE) { fprintf(stderr, "ERROR: word over %d characters\n", MAXWORDSIZE); exit(1); }
	/* reprocess the argument so that the word is similiar to data in
	 * the dictionary... */

	char word[DICTFIELDSIZE+1];

	/* formatting */
	for (int i=0;i<MAXWORDSIZE;++i) {
		if (i<strlen(argv[1])) {
			word[i]=tolower(argv[1][i]);
		} else {
			word[i]=' ';
		}
	}

	/* add newline to the end of the string */
	word[DICTFIELDSIZE]='\0';

	if (DEBUG) fprintf(stderr, "# word wanted=\"%s\"\n", word);

	int fd = open(DICTFILE, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "ERROR: Unable to open dictionary file.\n%s\nExiting.\n",strerror(errno));
		close(fd);
		exit(1);
	}

	if (ok(fd, word)) fprintf(stdout, "yes\n");
	else fprintf(stdout, "no\n");

	/* close the file */
	close (fd);
} /* end main() */

/* Logic for subroutine: int ok (int fd, char *word)
 * prepare 'want' and 'have' variables per above format
 * set bot to 0 and top to last line number + 1 (use lseek)
 * repeat:
 * – if search range empty (bot >= top), return 0
 * – set mid = (bot+top)/2; read that line into 'have'
 *   (don't read newline)
 * – compare 'want' vs. 'have' (using strcmp)
 * – if they are equal, return 1
 * – if 'want' smaller than 'have', set top = mid; otherwise,
 *   set bot = mid+1
 */
int ok(int fd, char *want) {
	char have[DICTFIELDSIZE+1]; /* holds one value for the dictionary */
	int strcmpRV=0;				/* string compare return value */

	/* offsets: 0... 16... 32... etc. */
	/* first check needs to be approx the middle of the file... */
	int top = divideby16(lseek(fd, 0, SEEK_END));	/* bitwise div by 16  - Field Size*/
	int bottom = 0;
	int middle;

	while (bottom<top) {
		middle = divideby2( (top+bottom) );

		if (DEBUG) fprintf(stderr, "# search range: bottom=%d, top=%d\n", bottom, top);

		lseek(fd, multiplyby16(middle), SEEK_SET);
		if ((read(fd, have, DICTFIELDSIZE))==-1) {
			fprintf(stderr, "ERROR: unable to read file to position.\n%s\nExiting.", strerror(errno));
			close(fd);
			exit(1);
		} 
		have[DICTFIELDSIZE-1] = '\0';  /* replace newline with null */

		if (DEBUG) fprintf(stderr, "# middle=%d, word have=\"%s\"\n", middle, have);

		strcmpRV = strcmp(want,have); /* check the want and have */
		/* otherwise change the top and bottom, depending on direction */
		if (strcmpRV < 0) {
			if (DEBUG) fprintf(stderr, "# test: want < have\n");
			top = middle;
		} else if (strcmpRV > 0) {
			if (DEBUG) fprintf(stderr, "# test: want > have\n");
			bottom = middle + 1;
		} else {
			if (DEBUG) fprintf(stderr, "# test: want = have\n");
			return TRUE;
		}

		if (lseek(fd, 0, SEEK_SET)==-1) {
			fprintf(stderr, "ERROR: unable to process lseek.\n%s\nExiting.", strerror(errno));
			close(fd);
			exit(1);
		}
	}

	return FALSE;
} /* end ok */
