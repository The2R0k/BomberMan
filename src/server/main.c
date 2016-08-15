#include <stdio.h>
#include <time.h>
#include <sys/times.h>

static struct tms tmsBegin,tmsEnd;

void TimeStart(){
  times(&tmsBegin);
}

static long TimeCheck(){

  times(&tmsEnd);

  return (((tmsEnd.tms_utime-tmsBegin.tms_utime)+
          (tmsEnd.tms_stime-tmsBegin.tms_stime))*1000);
}



int main(){
    int i;    
    TimeStart();
    for(i=0;;i++){
         printf("test\n");
        printf("======== i %d :: %d",i,TimeCheck());
    }
    return 0;
    }
