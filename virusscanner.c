#include <stdio.h>
#include <dirent.h>
#include <string.h>

 
int scan(char *filename, char *sigfile);

int FindFilesAndScan(char *path, char *sigfile)
{
	DIR *dir;
	struct dirent *dp;
	char filename[PATH_MAX + 1];
	
	static int virusfound = 0;

	if ((dir = opendir(path)) != NULL) 
	{
		while ((dp = readdir(dir)) != NULL) 
		{
			if((strcmp(dp->d_name,"..") != 0) && (strcmp(dp->d_name,".") != 0))
			{
				strcpy(filename, path);
				strcat(filename, "/");
				strcat(filename, dp->d_name);
			
				if(dp->d_type == DT_DIR)
				{
					if ((strlen(path) + strlen(dp->d_name) + 1) > PATH_MAX)
					{
						printf("Path to long\n");
						return virusfound;
					}

					FindFilesAndScan(filename, sigfile);
				}
				else
				{
					printf("Scanning file:%s........\n",filename);
					if(scan(filename, sigfile))
					{
						virusfound++;
		
					}
				}
			}
		}
	closedir(dir);
	}

return virusfound;
}

int scan(char *filename, char *sigfile)
{
FILE *fp1 = fopen(filename, "rb");
fseek(fp1, 0, SEEK_END);
int fsize1 = ftell(fp1);
fseek(fp1, 0, SEEK_SET);

FILE *fp2 = fopen(sigfile, "rb");
fseek(fp2, 0, SEEK_END);
int fsize2 = ftell(fp2);
fseek(fp2, 0, SEEK_SET);

printf("\nSize of file being scanned	: %d\n",fsize1);
printf("\nSize of signature file	: %d\n",fsize2);

if(!fsize1)
{

return 0;

}
char v, s;  //Virus,Signatue byte
int mb = 0; //Matched bytes
int pos = 0;
int ln = 0; //position of next signature
int i = 0;
char sb;
int found = 0;
int cursig = -1; // start position of current signature

while(1)
{

	while(1)
	{

		i++;
		if(i == fsize1)
		{
			i = 0;
			while(1)
			{
			fread(&s,1,1,fp2);
			ln++;
			if (s == '\n')
			{
				break;
			}
			
			}
		
	
			//printf("\n\nMoving to next signature...\n\n");
			fseek(fp1, 0, SEEK_SET);
			pos = mb;

			break;
		}

		fread(&v,1,1,fp1);
		fread(&s,1,1,fp2);
	
		if(s == '\n')
		{
			pos = mb;
			fseek(fp1, 0, SEEK_SET);
			break;
		}
		
	
		if(v != s)
		{
			

			fseek(fp2, ln, SEEK_SET);
			cursig = ln;
			mb = 0;
			pos = mb;

			break;
		}
		
		else
		{
			mb++;
		}
	}



if(pos)
{	

	FILE *fp = fopen("results.txt", "a");
	
	fseek(fp2, cursig, SEEK_SET);
	fwrite("Virus found in file ", strlen("Virus found in file "), 1, fp);
	fwrite(filename, strlen(filename), 1, fp);
	fwrite("...\n", strlen("...\n"), 1, fp);
	fwrite("Signature matched: ", strlen("Signature matched: "), 1, fp);
	while(1)
	{
		fread(&s,1,1,fp2);
		fwrite(&s,1,1,fp);
		ln++;
		
		if (s == '\n')
		{
			break;
		}
			
	}
	fwrite("\n\n", strlen("\n\n"), 1, fp);
	fclose(fp);

	printf("\n!!!!!!!!!!VIRUS FOUND!!!!!!!!!!\n\nBytes matched: %d\n\n",  pos);
	found = 1;
	break;

}
if(ln == fsize2)
{
	printf("\nFile clean. No virus detected. Moving to next file.\n\n");
	found = 0;
	break;
}

}
fclose(fp1);
fclose(fp2);

return found;
}


int main(int argc, char* argv[])
{

if (argc != 3)
{
printf("\n\n----------Usage---------\n\n");
printf("%s <DirToScan> <SignatureFile>\n\n", argv[0]);
return 0;

}

fclose(fopen("results.txt", "w")); //clear the results file

int virusfound = FindFilesAndScan(argv[1], argv[2]);

if(virusfound)
{
printf("\n\nTotal %d infected files detected!!\nCheck results.txt for details\n\n", virusfound);
}
return 0;

}


