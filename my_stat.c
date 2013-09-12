#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h> //need for bool C type
#include <linux/limits.h> //need for PATH_MAX

void displaystat(struct stat *st);

int main(int argc,char *argv[])
{
 /* if no argument my_stat will exit */
    struct stat sb;
    bool statLink;
    extern int name;

    statLink = (argc > 1) && (strcmp(argv[1], "-l") == 0); // if -l is given then set statLink to 0 which is False boolean
    name = statLink ? 2 : 1; //if statLink is True set name ot 2 else name is 1
    if (name >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
    {
            usageErr("%s [-l] file\n use lstat() not stat()", argv[0]);
            exit(EXIT_FAILURE);
    }
    if(statLink)
    {
        if (lstat(argv[name], &sb) == -1)
            errExit("lstat");
    }
    else
    {
        if (stat(argv[name], &sb) == -1)
            errExit("stat");
    }
    if (!S_ISLNK(st.st_mode)) 
        printf("   File:  '%s'\n",argv[name]); 
    else 
    {
        char target[PATH_MAX + 1];
        // int readlink(const char *path,char *buf, size_t bufsiz);
        readlink(argv[name],target,300);
        printf("  File:  '%s' ->'%s'\n",argv[name],target);
    }

    displaystat(&sb); //call function
    
    exit(EXIT_SUCCESS);
}

void displaystat(struct stat *st)
{
     /* display  file name: if it is a symbolic link ,also show link file name */

     /*display file size,block,io block and file type->*/
        printf("  Size:  %d       Blocks: %d          IO Block: %d   ", 
            st->st_size, st->st_blocks, st->st_blksize);
          
        if (S_ISDIR(st->st_mode))
            printf("    directory\n");
        else if(S_ISREG(st->st_mode))
            printf("    regular file\n");
        else if(S_ISLNK(st->st_mode))
            printf("    symbolic (soft) link\n");
        else if(S_IFCHR(st->st_mode))
            printf("    character device\n");
        else if(S_IFBLK(st->st_mode))
            prinf("     block device\n");
        else if(S_IFIFO(st->st_mode))
            printf("    FIFO or pipe\n");
        else if(S_IFSOCK(st->st_mode))
            printf("    socket\n");
        else printf("   file type unknown?\n");

        printf("Device:  %xh/%dd    Inode:  %ld     Links:    %ld\n",
            st->st_dev, st->st_dev, (long) st->st_ino, (long) st->st_nlink);
        // creating pointers to struct passwd and group type to get owner name and group by userid
        struct passwd *passent; 
        passent=getpwuid(st->st_uid);

        struct group *group;
        group=getgrgid(st->st_gid);
       
       /*get permission mask and rwx format*/
        char *ac=calloc(10,1); //calloc intializes buffer space 10 elements each 1 unit long and return pointer to this location
        memset(ac,'-',10);

        if(st->st_mode & S_IFDIR) ac[0]='d';
        if(S_ISLNK(st->st_mode))  ac[0]='l';
        if(st->st_mode & S_IRUSR) ac[1]='r';
        if(st->st_mode & S_IWUSR) ac[2]='w';
        if(st->st_mode & S_IXUSR) ac[3]='x';
        if(st->st_mode & S_IRGRP) ac[4]='r';
        if(st->st_mode & S_IWGRP) ac[5]='w';
        if(st->st_mode & S_IXGRP) ac[6]='x';
        if(st->st_mode & S_IROTH) ac[7]='r';
        if(st->st_mode & S_IWOTH) ac[8]='w';
        if(st->st_mode & S_IXOTH) ac[9]='x';


        printf("Access:  (%o%o%o%o/%s)     Uid:  (%d/   %s)   Gid: (%d/  %s)\n", 
        (st->st_mode & S_ISVTX)>>9, (st->st_mode & S_IRWXU)>>6, (st->st_mode & S_IRWXG)>>3,
        st->st_mode & S_IRWXO, ac, st->st_uid, passent->pw_name, st->st_gid, group->gr_name);
        
        /*use strftime to get the access,modify and change time*/
        char date_time[40];
        strftime(date_time,sizeof(date_time),"Access:  %Y-%m-%d %H:%M:%S %z",localtime(&st->st_atim->tv_sec));
        puts(date_time);
        strftime(date_time,sizeof(date_time),"Modify:  %Y-%m-%d %H:%M:%S %z",localtime(&st->st_mtim->tv_sec));
        puts(date_time);
        strftime(date_time,sizeof(date_time),"Change:  %Y-%m-%d %H:%M:%S %z",localtime(&st->st_ctim->tv_nsec));
        puts(date_time);
}


/*  Notes from Pg 280 LPI book
struct stat {
dev_t st_dev; * IDs of device on which file resides 
ino_t st_ino; * I-node number of file 
mode_t st_mode; * File type and permissions 
nlink_t st_nlink; * Number of (hard) links to file 
uid_t st_uid; * User ID of file owner 
gid_t st_gid; * Group ID of file owner 
dev_t st_rdev; * IDs for device special files 
off_t st_size; * Total file size (bytes) 
blksize_t st_blksize; * Optimal block size for I/O (bytes) 
blkcnt_t st_blocks; * Number of (512B) blocks allocated 
time_t st_atime; * Time of last file access 
time_t st_mtime; * Time of last file modification 
time_t st_ctime;  Time of last status change 
*/
//
