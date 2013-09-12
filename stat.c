

#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>

static void StatCall(const struct stat *sb)
{
	printf("File type:		");
	switch (sb->st_mode & S_IFMT)
	{
		case S_IFREG: printf("reg file\n");
			break;
		case S_IFDIR: prinf("directory\n");
			break;
		case S_IFCHR: prinf("char\n");
			break;
		case S_IFBLK: prinf("block\n");
			break;
		case S_IFLNK: prinf("soft link\n");
			break;
		case S_IFIFO: prinf("FIFO/PIPE\n");
			break
		case S_IFSOCK: printf("socket\n");
			break;
		default: printf("file type not known, Please check syntax\n");
			break;
	}

	printf("I-node: major: %ld, minor: %ld\n", (long) sb->st_dev, (long) (sb->st_dev));

	printf("I-node: %ld (%s)\n", (long) sb->st_ino);


}

int main(int argc, char *argv[])
{
	struct stat sb;
	Boolean softlink;
	int fname;
	softlink = (argc > 1) && (strcmp(argv [1], "-l") == 0);
	fname = softlink ? 2 : 1;
	if(fname >= argc || (argc > 1 && strcmp(argv[1], "--help")==0))
		usageErr("%s [-l] file\n" "    -l = user lstat() not stat()\n", argv[0]);
	if (softlink)
	{
		if (lstat(argv[fname], &sb) == -1)
			errExit("lstat");
	} else 
	{
		if(stat(argv[fname]), &sb)
			errExit("stat");
	}
	StatCall(&sb);
	exit(EXIT_SUCCESS);
}
