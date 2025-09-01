#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "SpiceUsr.h"

#include "common.h"

#define SAMPLE_PERIOD_SEC 60
// lets say around 5 years for now
#define N_SAMPLES ((60*60*24*365*5)/60)
// start of 2026
#define SAMPLE_BEGIN 1767243600

SpiceChar failmsg[1841];

vec3_t samples[N_SAMPLES];

void checkfail(void)
{
    if(!failed_c())
        return;

    getmsg_c("LONG", sizeof(failmsg), failmsg);
    puts(failmsg);
    exit(1);
}

int main(int argc, char** argv)
{
    int i;

    clock_t start, stop;
    time_t t;
    struct tm *tinfo;
    char tstr[64];
    SpiceDouble tspice;
    int percentinc;

    if(argc != 3)
    {
        printf("usage: sunvec <tls> <bsp>\n");
        return 1;
    }

    if(access(argv[1], F_OK))
    {
        printf("inaccessible tls file \"%s\"!\n", argv[1]);
        return 1;
    }

    if(access(argv[2], F_OK))
    {
        printf("inaccessible tls file \"%s\"!\n", argv[1]);
        return 1;
    }

    furnsh_c(argv[1]);
    checkfail();

    printf("generating vectors...");
    fflush(stdout);

    start = clock();

    percentinc = N_SAMPLES / 100;

    for(i=0, t=SAMPLE_BEGIN; i<N_SAMPLES; i++, t+=SAMPLE_PERIOD_SEC)
    {
        tinfo = gmtime(&t);
        strftime(tstr, sizeof(tstr), "%Y-%m-%d %H:%M:%S UTC", tinfo);
        str2et_c(tstr, &tspice);

        if(i && !(i % percentinc))
            printf(".");

        if(!(i % (percentinc * 10)))
            printf("\n%d / %d (%d%%) ", i, N_SAMPLES, i * 100 / N_SAMPLES);

        fflush(stdout);
    }

    printf("\n");

    stop = clock();
    printf("vectors generated in %.3f seconds.\n", (double) (stop - start) / CLOCKS_PER_SEC);
    
    return 0;
}