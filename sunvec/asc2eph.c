/***************************************************************************
***************             ASC2EPH.C v.1.6.1              *****************
****************************************************************************
** This program is a C version of an original JPL fortran code, slightly  **
** changed (interactive input added) for the user convenience.            **
**                                                                        **
** You will be prompted for start and final epochs (in JED) of the        **
** ephemeris you want to obtain in binary form.                           **
** This epochs will be rounded to the nearest integer multiply of 32 days **
** counting from 2440400.5, outside of the interval given.                **
****************************************************************************
**              REMEMBER TO ADJUST MANUALLY THE JPLBIN.H FILE             **
****************************************************************************
**  Written: May 28, 1997 by PAD  **  Last modified: April 18,2021 by PAD **
****************************************************************************
**  PAD: dr. Piotr A. Dybczynski,               e-mail: dybol@amu.edu.pl  **
** Astronomical Observatory Institute, A.Mickiewicz Univ., Poznan, Poland **
**      https://pad2.astro.amu.edu.pl/PAD/pmwiki.php?n=Dybol.JPLEph       **
***************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include"jplbin.h"

FILE *F,*F1;
void nxtgrp(char * header);
void errprt(int i,char * msg);


int main(void)
{
 char header[14], buff[102];
 double db[1030], db2z, t1, t2;
 int i, j, k, ksize;
 /* int irecsz; */
 int n, n1, nrout, ncoeff, nrw, out, first=TRUE ,last,fnum,firstblock=1;

/* cleaning cnam and cval arrays */

 for(i=0;i<OLDMAX;++i)
    {
      for(j=0;j<6;++j) R1.r1.cnam[i][j]=' ';
    }

 for(i=0;i<(NMAX-OLDMAX);++i)
    {
      for(j=0;j<6;++j) R1.r1.cnam2[i][j]=' ';
    }

 for(i=0;i<NMAX;++i)
    {
      R2.r2.cval[i]=0.0;
    }


/********************************************************************************************/
/* You can add the necessary path before a filename (if necessary) in the following line:   */

 snprintf(buff,102,"header.%03d",DENUM);

/********************************************************************************************/
 F = fopen(buff,"r");
 if(F==NULL){
              printf("Cannot open header file: %s, aborted.\n",buff);
              return 1;
            }

/*  write a fingerprint to the screen.  */

 puts("\n JPL ASCII-TO-DIRECT-I/O PROGRAM.\n C-version, translated from original fortran code from JPL\n");

/*  read the size and number of main ephemeris records. */

 fscanf(F,"KSIZE= %d NCOEFF= %d",&ksize,&ncoeff);
 printf("KSIZE = %6d  NCOEFF= %6d",ksize,ncoeff);
 if(ksize!=KSIZE || ncoeff!=NCOEFF)
   {
    puts("Wrong header file or JPLBIN.H not adjusted, aborting.\n");
    fclose(F);
    return 1;
   }


/*  now for the alphameric heading records (group 1010) */

 nxtgrp(header);
 if(strcmp(header,"GROUP   1010")) errprt(1010,"NOT HEADER");

 for(i=0;i<3;++i)
    {
     fgets(buff,100,F);
     for(j=0;j<84;++j) R1.r1.ttl[i][j]=buff[j];
     puts(buff);
    }

/*  read start, end and record span  (group 1030) */

 nxtgrp(header);
 if(strcmp(header,"GROUP   1030")) errprt(1030,"NOT HEADER");

 fscanf(F," %lf %lf %lf",&R1.r1.ss[0],&R1.r1.ss[1],&R1.r1.ss[2]);
 /* There is an error in DE403 header file.... */
 if(DENUM==403) R1.r1.ss[0]=2305424.5;

/* read number of constants and names of constants (group 1040/4). */

 nxtgrp(header);
 if(strcmp(header,"GROUP   1040")) errprt(1040,"NOT HEADER");

 fscanf(F," %d",&n);
 R1.r1.ncon = n;

/* the following two lines adjust for larger number of constants */

 if(n > OLDMAX) {n1=n-OLDMAX; n=OLDMAX;}
 else            n1=0;


 for(i=0;i<n;++i)
    {
      fscanf(F," %6c",&R1.r1.cnam[i][0]);
    }

 for(i=0;i<n1;++i)
    {
      fscanf(F," %6c",&R1.r1.cnam2[i][0]);
    }


/*  read a number of values and the values (group 1041/4)  */
 nxtgrp(header);
 if(strcmp(header,"GROUP   1041")) errprt(1041,"NOT HEADER");


 fscanf(F," %d",&n);
 fgets(buff,100,F);
 i=0;
 while(1){
           fgets(buff,100,F);
           /* C uses E instead of D in exponents */
           for(k=0;k<102;++k) if(buff[k]=='D') buff[k]='E';
           sscanf(buff," %lE",&R2.r2.cval[i]);
           if((++i)==n) break;
           sscanf(buff+26," %lE",&R2.r2.cval[i]);
           if((++i)==n) break;
           sscanf(buff+52," %lE",&R2.r2.cval[i]);
           if((++i)==n) break;
          }

 for(i=0;i<OLDMAX;++i) /* these three constants are near the beginning, i.e. in cnam[] array */
    {
     for(j=0;j<6;++j) buff[j]=R1.r1.cnam[i][j];
     buff[6]='\0';
     if(!strcmp(buff,"AU    "))  R1.r1.au    = R2.r2.cval[i];
     if(!strcmp(buff,"EMRAT "))  R1.r1.emrat = R2.r2.cval[i];
     if(!strcmp(buff,"DENUM "))  R1.r1.numde = R2.r2.cval[i];
    }

 for(i=0;i<n;++i)
    {
     if(i<OLDMAX) printf("%.6s  %24.16E\n",R1.r1.cnam[i],R2.r2.cval[i]);
     else         printf("%.6s  %24.16E\n",R1.r1.cnam2[i-OLDMAX],R2.r2.cval[i]);
    }
/*  read pointers needed by interp (group 1050)  */
 nxtgrp(header);
 if(strcmp(header,"GROUP   1050")) errprt(1050,"NOT HEADER");

/*  Zero out pointer arrays  */

 for(i=0;i<3;++i)
    {
      for(j=0;j<12;++j) R1.r1.ipt[j][i]=0.0;
      R1.r1.lpt[i]=0.0;
/*      R1.r1.rpt[i]=0.0;   kept for future releases */
/*      R1.r1.tpt[i]=0.0;   kept for future releases */
    }


 for(i=0;i<3;++i)
    {
      for(j=0;j<12;++j) fscanf(F," %d",&R1.r1.ipt[j][i]);
      fscanf(F," %d",&R1.r1.lpt[i]);
/*      fscanf(F," %d",&R1.r1.rpt[i]);   kept for future releases */
/*      fscanf(F," %d",&R1.r1.tpt[i]);   kept for future releases */
    }

 for(i=0;i<3;++i)
    {
      for(j=0;j<12;++j) printf(" %5d",R1.r1.ipt[j][i]);
      printf(" %5d",R1.r1.lpt[i]);
/*      printf(" %5d",R1.r1.rpt[i]);   kept for future releases */
/*      printf(" %5d\n",R1.r1.tpt[i]);   kept for future releases */
    }
/* Decide what interval is desired....              */
printf("\n\nDo you want to specify the subinterval for the binary ephemeris (y/n)? ");
i=getchar();
if(i=='Y' || i=='y')
  {
    printf("Enter Start Epoch (in JED): ");
    scanf(" %lf",&t1);
    if(t1>R1.r1.ss[0])
      {
       t1 -= 2440400.5;
       t1 = R1.r1.ss[2]*floor(t1/R1.r1.ss[2])+2440400.5;
       R1.r1.ss[0]=t1;
      }
    printf("Enter Final Epoch (in JED): ");
    scanf(" %lf",&t2);
    if(t2<R1.r1.ss[1])
      {
       t2 -= 2440400.5;
       t2 = R1.r1.ss[2]*ceil(t2/R1.r1.ss[2])+2440400.5;
       R1.r1.ss[1]=t2;
      }
  }
 else
  {
   t1=-99999999.;
   t2= 99999999.;
  }

/*   open direct-access output file ('jpleph.xxx') */

/***************************************************************************/
/* You can add path before filename or change name in the following line:  */

 snprintf(buff,102,"jpleph.%03d",DENUM);

/***************************************************************************/

 F1=fopen(buff,"wb");
 if(F1==NULL){
               printf("Cannot create binary output file: %s,aborted.\n",buff);
               fclose(F);
               return 1;
             }


/*  write header records onto output file.   */

 nrout = 1;
 out =fwrite(&R1,sizeof(R1),1,F1);
 if(out!=1) errprt(nrout,"THE RECORD NOT WRITTEN BECOUSE OF ERROR\n");
 else printf("1 RECORD OF SIZE %d WRITTEN\n",(int)sizeof(R1));

 nrout = 2;
 out = fwrite(&R2,sizeof(R2),1,F1);
 if(out!=1) errprt(nrout,"THE RECORD NOT WRITTEN BECOUSE OF ERROR\n");
 else printf("2 RECORD OF SIZE %d WRITTEN\n",(int)sizeof(R2));

/*  read and write the ephemeris data records (group 1070). */
 nxtgrp(header);
 if(strcmp(header,"GROUP   1070")) errprt(1070,"NOT HEADER");

 nrout  = 0;
 out    = 0;
 db2z   = 0; /* formal only */

/* main loop through available ASCII files */

 for(fnum=FIRSTFILE;fnum<=LASTFILE;fnum+=FILESTEP)   /* definitions depend on DENUM and are in jplbin.h */
    {
     fclose(F);

/********************************************************************************************/
/* You can add the necessary path before a filename (if necessary) in the following line:   */

     snprintf(buff,102,"asc%1c%05d.%03d",(fnum<0?'m':'p'),(fnum<0?-fnum:fnum),DENUM);

/********************************************************************************************/

     F=fopen(buff,"r");
     if(F==NULL)
       {
        printf("File %s is missing\n",buff);
        continue;
       }
     else printf("File: %s\n",buff);

     while(!feof(F))
      {
        fscanf(F," %d %d ",&nrw,&ncoeff);
/* there is an integer multiply of 3 coeffcients in source file */
        last=(ncoeff+2)/3;   /* Bill Gray corrected this line */
        for(k=0;k<last;++k)
           {
             fgets(buff,100,F);
/* C language uses E instead of D for exponent part */
             for(i=0;i<102;++i) if(buff[i]=='D') buff[i]='E';
             sscanf(buff," %lE %lE %lE", db+3*k, db+3*k+1, db+3*k+2);
           }
        if( 2*ncoeff != ksize ) errprt(ncoeff,"2*NCOEFF NOT EQUAL TO KSIZE");

/*  skip this data block if the end of the interval is less
    than the specified start time or if the it does not begin
    where the previous block ended.  */

        if(db[0]>=t2) {fnum=1000000;break;}
                                   /* all necessary blocks were processed */
        if(db[1]<=t1) continue;    /* wait for the first usefull block    */
        if(db[1]==db2z) continue;  /* some blocks appears twice on the
                                      source files boundary !             */
        if(first==TRUE) first=FALSE;
        else if( db[0] != db2z )
               {
                 /*  beginning of current interval is past the end
                     of the previous one.  */
                 errprt(nrw,"RECORDS DO NOT OVERLAP OR ABUT");
               }

        db2z  = db[1];
        nrout = nrout + 1;
        if(firstblock)
          {
            firstblock=0;
            R1.r1.ss[0]=db[0];
          }
        out=fwrite(db,sizeof(double),ncoeff,F1);
        if(out != ncoeff) errprt(nrout,"THE RECORD NOT WRITTEN BECAUSE OF ERROR");

/*  update the user as to our progress every 10th block.  */
        if( nrout%10 == 1 )
          {
            if( db[0] >= t1 )  printf("%d EPHEMERIS RECORDS WRITTEN. LAST JED= %12.2f\n",nrout,db[1]);
            else puts("SEARCHING FOR THE FIRST REQUESTED RECORD...\n");
          }
      }
    }
 printf("%d EPHEMERIS RECORDS WRITTEN. LAST JED = %12.2f\n",nrout,R1.r1.ss[1]);

 R1.r1.ss[1]=db2z;

 /* overwrite first record with epochs adjusted */
 fflush(F1);
 fseek(F1,0,SEEK_SET);
 fwrite(&R1,sizeof(R1),1,F1);


/*   we're through.  wrap it up.  */

 fclose(F1);
 fclose(F);
 return 0;
}
/***************************************************************************/

void errprt(int i,char * msg)
{
 printf("\nERROR #%8d  %-50s",i,msg);
 exit(1);
}
/****************************************************************************/
void nxtgrp(char * header)
{
  char buff[102];
  do{
      fgets(buff,100,F);
    }while(buff[0] == ' '|| buff[0]=='\n');
  buff[12]='\0';
  strcpy(header,buff);
  fgets(buff,100,F);
}
/***************************  THE  END  ************************************/
