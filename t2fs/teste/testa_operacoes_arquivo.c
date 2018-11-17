#include "t2fs_internal.h"

#include <stdio.h>

int main() {

    FILE2 handle;
    char buffer[20];
    char buffer2[20];
    char buffer3[20];
    char buffer4[20];
    char buffer5[6];
    char *buffer6;
    char buffer7[200];
    int i;

    buffer6 = malloc (2200);
	
    for (i = 0; i < 2199; i++)
	buffer6[i] = 't';
	
    buffer [2199] = '\0';

    handle = create2("helloworld");
    if (handle == -1) {
	printf("erro\n");
	return 0;
    }
    write2(handle, "helloworld", 11);
    seek2(handle, 0);
    read2(handle, buffer, 11);
    printf("%s\n", buffer);
    seek2(handle, 2);
    read2(handle, buffer2, 9);
    printf("%s\n", buffer2);
    seek2(handle, 2);
    truncate2(handle);
    write2(handle, "teste", 6);
    seek2(handle, 0);
    read2(handle, buffer3, 8);
    printf("%s\n", buffer3);
    close2(handle);
    handle = open2("helloworld");
    read2(handle, buffer4, 8);
    printf("%s\n", buffer4);
    handle = create2("helloworld");
    seek (handle, -1);
    write2(handle, "hello", 6)
    seek (handle, 0);
    read2(handle, buffer5, 20);
    printf("%s\n", buffer5);
    seek (handle, 6);
    write2(handle, buffer6, 2200);
    seek (handle, 0);
    read2(handle, buffer7, 200);
    printf("%s\n", buffer7);
    delete2("helloworld");
    handle = open2("helloworld");
    printf("%d\n", handle);

    free(buffer6);

    return 0;

}
