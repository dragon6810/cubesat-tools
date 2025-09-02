#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "common.h"

void cbincmd(FILE* ptr, int argc, char** argv)
{
    int i;

    FILE *outptr;
    uint64_t len;
    char byte;

    if(argc != 4)
    {
        printf("unrecognized arguments for cmd.\n");
        exit(1);
    }

    outptr = fopen(argv[3], "w");
    if(!outptr)
    {
        printf("couldn't open file for writing \"%s\"!\n", argv[3]);
        exit(1);
    }

    fseek(ptr, 0, SEEK_END);
    len = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);

    fprintf(outptr, "char %s[] =\n{\n", argv[1]);

    for(i=0; i<len; i++)
    {
        fread(&byte, 1, 1, ptr);
        fprintf(outptr, "%hhd,", byte);
    }

    fprintf(outptr, "\n};\n");

    fclose(outptr);
}

void veccmd(FILE* ptr, int argc, char** argv)
{
    uint64_t len;
    int index;
    vec3_t vec;

    if(argc != 4)
    {
        printf("unrecognized arguments for cmd.\n");
        exit(1);
    }

    if(!sscanf(argv[3], "%d", &index))
    {
        printf("expected integer.\n");
        exit(1);
    }

    fseek(ptr, 0, SEEK_END);
    len = ftell(ptr);
    len /= sizeof(float) * 3;

    if(index < 0 || index >= len)
    {
        printf("index %d out of bounds.\n", index);
        printf("index must be between 0 and %llu.\n", len);
        exit(1);
    }

    fseek(ptr, index * sizeof(float) * 3, SEEK_SET);
    fread(vec, sizeof(float), 3, ptr);

    printf("( %f %f %f )\n", vec[0], vec[1], vec[2]);
}

void sizecmd(FILE* ptr, int argc, char** argv)
{
    uint64_t len;

    if(argc != 3)
    {
        printf("unrecognized arguments for cmd.\n");
        exit(1);
    }

    fseek(ptr, 0, SEEK_END);
    len = ftell(ptr);
    len /= sizeof(float) * 3;

    printf("%llu vector(s).\n", len);
}

void printusage(void)
{
    printf("usage: vecinfo <file> <cmd> <args>\n");
    printf("commands and their args:\n");
    
    printf("    size\n");
    printf("        get the number of vectors in the file\n");

    printf("    vec <index>\n");
    printf("        print the vector at the given index\n");

    printf("    cbin <outfile>\n");
    printf("        put the binary into a c array of chars\n");
}

int main(int argc, char** argv)
{
    FILE *ptr;

    printf("\n============ vecinfo ============\n\n");

    if(argc < 3)
    {
        printusage();
        return 1;
    }

    ptr = fopen(argv[1], "rb");
    if(!ptr)
    {
        printf("can't open file for reading \"%s\"!\n", argv[1]);
        return 1;
    }

    if(!strcmp(argv[2], "size"))
        sizecmd(ptr, argc, argv);
    else if(!strcmp(argv[2], "vec"))
        veccmd(ptr, argc, argv);
    else if(!strcmp(argv[2], "cbin"))
        cbincmd(ptr, argc, argv);
    else
    {
        printf("unknown command \"%s\".\n", argv[2]);
        return 1;
    }

    fclose(ptr);

    return 0;
}