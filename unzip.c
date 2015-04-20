#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zip.h>

static unsigned int crc_table[256];
void init_crc32()
{
	unsigned int poly = 0xEDB88320, u, i, j;

	for(i = 0; i < 256; i++){
		u = i;

		for(j = 0; j < 8; j++){
			if(u & 0x1){  
				u = (u >> 1) ^ poly;
			}else{
				u >>= 1;
			}
		}

		crc_table[i] = u;
	}
}

unsigned int check_crc32(unsigned char *buffer, unsigned int bufferlen, unsigned int crc32_start)
{
	unsigned int result = crc32_start;
	for(unsigned int i = 0; i < bufferlen; i++){
		result = (result >> 8) ^ crc_table[buffer[i] ^ (result & 0xFF)];
	}
	return ~result;
}

int main(int argc, char **argv)
{
	int ret;
	int i;
	int errorp;
	struct zip *z;
	struct zip_file *zf;
	struct zip_stat sb;
	unsigned int crc;
	unsigned char *zbuf;

	init_crc32();

	z = zip_open("a.zip", 0, &errorp);
	if (z == NULL) {
		fprintf(stderr, "zip_open error\n");
		return 1;
	}

	ret = zip_stat_index(z, 0, 0, &sb);
	if (ret < 0) {
		fprintf(stderr, "zip_stat_index error\n");
		return 1;
	}

	zf = zip_fopen_encrypted(z, "a", 0, "abc");
	if (zf == NULL) {
		fprintf(stderr, "zip_fopen_encrypted error\n");
		return 1;
	}

	zbuf = malloc(sb.size);
	if (zbuf == NULL) {
		return 1;
	}
	memset(zbuf, 0, sb.size);
	ret = zip_fread(zf, zbuf, sb.size);
	zip_fclose(zf);

	crc = check_crc32(zbuf, sb.size, 0xFFFFFFFF);
	if (crc != sb.crc) {
		fprintf(stderr, "crc error\n");
		return 1;
	}

	printf("password OK\n");

	return 0;
}
