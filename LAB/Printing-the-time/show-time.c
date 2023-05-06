#include <time.h>
#include <stdio.h>

int main()
{

   time_t rawtime;
   struct tm *info;
   char buffer[80];

   time( &rawtime );

   info = localtime( &rawtime );

   strftime(buffer,80,"%T", info);
   printf("Formatted time : %s\n", buffer );

   return(0);
}