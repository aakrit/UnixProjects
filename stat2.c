#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc,char *argv[])
{
	if (argc < 2)
    	{
      		printf("Error need more than one argument!\n");
      		exit(EXIT_FAILURE);
    	}
 
	int i;
	for (i=1;i<argc;i++)
  	{
    		struct stat st;
     		lstat(argv[i], &st);
     
     	if (!S_ISLNK(st.st_mode)) 
		printf("   File:  '%s'\n",argv[i]); 
     	else 
	{
        	char target[PATH_MAX+1];
        	readlink(argv[i],target,300);
           	printf("  File:  '%s' ->'%s'\n",argv[i],target);
	}
     	printf("  Size:  %d       Blocks: %d          IO Block: %d   ",st.st_size,st.st_blocks,st.st_blksize);

  
     	if (S_ISDIR(st.st_mode))
          printf("    directory\n");
     else if(S_ISREG(st.st_mode))
          printf("    regular file\n");
     else if(S_ISLNK(st.st_mode))
          printf("    symbolic link\n");
     else printf("    unknown\n");

     printf("Device:  %xh/%dd    Inode:  %ld     Links:    %ld\n",st.st_dev,st.st_dev,(long) st.st_ino,(long) st.st_nlink);
    
    
    struct passwd * passent;
    /*Identify the owner's name and group name by userid*/
    passent=getpwuid(st.st_uid);
    struct group *group;
    group=getgrgid(st.st_gid);
   
   /*get permission mask and rwx format*/
    char *acstring=calloc(10,1);
    memset(acstring,'-',10);

    if(st.st_mode & S_IFDIR) acstring[0]='d';
    if(S_ISLNK(st.st_mode))  acstring[0]='l';
    if(st.st_mode & S_IRUSR) acstring[1]='r';
    if(st.st_mode & S_IWUSR) acstring[2]='w';
    if(st.st_mode & S_IXUSR) acstring[3]='x';
    if(st.st_mode & S_IRGRP) acstring[4]='r';
    if(st.st_mode & S_IWGRP) acstring[5]='w';
    if(st.st_mode & S_IXGRP) acstring[6]='x';
    if(st.st_mode & S_IROTH) acstring[7]='r';
    if(st.st_mode & S_IWOTH) acstring[8]='w';
    if(st.st_mode & S_IXOTH) acstring[9]='x';


    printf("Access:  (%o%o%o%o/%s)     Uid:  (%d/   %s)   Gid: (%d/  %s)\n",(st.st_mode & S_ISVTX)>>9,(st.st_mode & S_IRWXU)>>6,(st.st_mode & S_IRWXG)>>3,st.st_mode & S_IRWXO,acstring,st.st_uid,passent->pw_name,st.st_gid,group->gr_name);
    
    /*use strftime to get the access,modify and change time*/
    char date_time[40];
    strftime(date_time,sizeof(date_time),"Access:  %Y-%m-%d %H:%M:%S %z",localtime(&st.st_atim.tv_sec));
    puts(date_time);
    strftime(date_time,sizeof(date_time),"Modify:  %Y-%m-%d %H:%M:%S %z",localtime(&st.st_mtim.tv_sec));
    puts(date_time);
    strftime(date_time,sizeof(date_time),"Change:  %Y-%m-%d %H:%M:%S %z.",localtime(&st.st_ctim.tv_nsec));
    puts(date_time);

  }
}

