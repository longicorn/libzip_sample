#include <stdio.h>
#include <string.h>
#include <zip.h>

int main(int argc, char **argv)
{
	int ret;
	int i;
	int errorp;
	struct zip *z;
	struct zip_file *zf;

	z = zip_open("a.zip", 0, &errorp);
	if (z == NULL) {
		fprintf(stderr, "zip_open error\n");
		return 1;
	}

	zf = zip_fopen_encrypted(z, "a", 0, "abc");
	if (zf == NULL) {
		fprintf(stderr, "zip_fopen_encrypted error\n");
		return 1;
	}

	printf("password OK\n");

	return 0;
}
