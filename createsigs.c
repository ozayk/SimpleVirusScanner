#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[1])
{
if (argc != 2)
{
printf("\n\nUsage:\n %s <signatureslistfile>\n\n", argv[0]);
return 0;
}

FILE *fp1 = fopen(argv[1], "rt");
fseek(fp1, 0, SEEK_END);
int fsize1 = ftell(fp1);
fseek(fp1, 0, SEEK_SET);

FILE *fp2 = fopen("signatures.bin", "wb");


char st;

for (int i=0; i<fsize1; i++)
{
fread(&st,1,1,fp1);
fwrite(&st,1,1,fp2);
}
fclose(fp1);
fclose(fp2);

return 0;
}
