#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int rfd;

long roll1 (long n, long m) {
	double lf = ceil (log2 (m)/8);
	long l = lf;
	long x = 0;
	uint8_t cs[1];

	for (; l > 0; l--) {
		if (read (rfd, cs, 1) < 1) err (-1, "failed to read");
		x = (x << 8) + cs[0];
	}

	return (m * exp2 (-8*lf) * x + 1);
}

int main (int argc, char *argu[]) {
	long n, m; /* ndm */
	
	rfd = open ("/dev/urandom", O_RDONLY);
	if (rfd < 0) err (-1, "failed to open /dev/urandom");
	
	for (int ii = 1; ii < argc; ii++) {
		if (!(argu[ii][0] >= '0' && argu[ii][0] <= '9')) errx (-1, "bad format");
		n = strtoul (argu[ii], &argu[ii], 10);
		if (argu[ii][0] != 'd') errx (-1, "bad format");
		argu[ii]++;
		m = strtoul (argu[ii], &argu[ii], 10);
		if (argu[ii][0]) errx (-1, "bad format");
		for (; n > 0; n--) {
			printf ("%ld%c", roll1 (n, m), n == 1 ? '\n' : '\t');
		}
	}
	return 0;
}
