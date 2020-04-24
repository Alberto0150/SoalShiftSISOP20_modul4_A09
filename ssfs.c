#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

#define HAVE_SETXATTR

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

static  const  char *dirpath = "/home/alberto/praktikum4/tempatngefuze";
static  const  char cckey = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
int ind = 10;



//////// no 1 ?????????????????????????????
//fungsi encrypt
void encr(char* asaltujuan)
{
  char fileasal[1000];
  char newecn[1000];
  int i,panjang,j=0;
  strcpy(fileasal, asaltujuan);

  char* hasil = strtok(fileasal,"/");
  if(strcmp(hasil,"encv1_")==0)
  {
    sprintf(asaltujuan,hasil);
    hasil = strtok(NULL,"/");
    while (hasil != NULL)
    {
      strcpy(newecn,hasil);
      panjang = strlen(newecn);
      for(i=0;i<panjang;i++)
      {
        j=0;
        // while(1)
        // {
        //   if(newecn[i] == cckey[j])
        //   {
        //     newecn[i] = cckey[j+ind];
        //   }
        //   j++;
        }
      }
      strcpy(asaltujuan,newecn);
      strcat(asaltujuan,"/");
      strcat(asaltujuan,hasil);
      hasil=strtok(NULL,"/");
    }
  }

}

//fungsi decrypt
void decr(char* asaltujuan)
{
  char fileasal[1000];
  char newecn[1000];
  int i,panjang,j;
  strcpy(fileasal, asaltujuan);

  char* hasil = strtok(fileasal,"/");
  if(strcmp(hasil,"encv1_")==0)
  {
    sprintf(asaltujuan,hasil);
    hasil = strtok(NULL,"/");
    while (hasil != NULL)
    {
      strcpy(newecn,hasil);
      panjang = strlen(newecn);
      for(i=0;i<panjang;i++)
      {
        j = strlen(cckey);
        // while(1)
        // {
        //   if(newecn[i] == cckey[j])
        //   {
        //     newecn[i] = cckey[j-ind];
        //   }
        //   j--;
        // }
      }
      strcpy(asaltujuan,newecn);
      strcat(asaltujuan,"/");
      strcat(asaltujuan,hasil);
      hasil=strtok(NULL,"/");
    }
  }
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	close(fd);
	return res;
}

// static int xmp_truncate(const char *path, off_t size)
// {
// 	int res;
//   char fpath[1000];
//   char name[1000];
//   sprintf(name,"%s",path);
//   sprintf(fpath, "%s%s",dirpath,name);
// 	res = truncate(path, size);
// 	if (res == -1)
// 		return -errno;
// 	return 0;
// }

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
		int res = 0;
	DIR *dp;
	struct dirent *de;
	(void) offset;
	(void) fi;
	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL)
	{
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
		if(res!=0) break;
	}
closedir(dp);
return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
struct fuse_file_info *fi)
{
	char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
		int res = 0;

	int fd = 0 ;
	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;
	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);
	if (res == -1)
		return -errno;
	return 0;
}

// static int xmp_access(const char *path, int mask)
// {
// 	int res;
//
// 	res = access(path, mask);
// 	if (res == -1)
// 		return -errno;
//
// 	return 0;
// }

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;

	res = readlink(path, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(path, mode);
	else
		res = mknod(path, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}


static int xmp_symlink(const char *from, const char *to)
{
	int res;

	res = symlink(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_link(const char *from, const char *to)
{
	int res;

	res = link(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;

	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;

	res = lchown(path, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(path, tv);
	if (res == -1)
		return -errno;

	return 0;
}

// static int xmp_open(const char *path, struct fuse_file_info *fi)
// {
// 	int res;
//
// 	res = open(path, fi->flags);
// 	if (res == -1)
// 		return -errno;
//
// 	close(res);
// 	return 0;
// }

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
	int res;

	res = statvfs(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    (void) fi;

    int res;
    res = creat(path, mode);
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}


static int xmp_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) fi;
	return 0;
}

static int xmp_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

#ifdef HAVE_SETXATTR
static int xmp_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags)
{
	int res = lsetxattr(path, name, value, size, flags);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_getxattr(const char *path, const char *name, char *value,
			size_t size)
{
	int res = lgetxattr(path, name, value, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_listxattr(const char *path, char *list, size_t size)
{
	int res = llistxattr(path, list, size);
	if (res == -1)
		return -errno;
	return res;
}

static int xmp_removexattr(const char *path, const char *name)
{
	int res = lremovexattr(path, name);
	if (res == -1)
		return -errno;
	return 0;
}
#endif /* HAVE_SETXATTR */


static struct fuse_operations xmp_oper =
{

	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.write =xmp_write,
	.truncate = xmp_truncate,

  .access		= xmp_access,
  .readlink	= xmp_readlink,
  .mknod		= xmp_mknod,
  .mkdir		= xmp_mkdir,
  .symlink	= xmp_symlink,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.link		= xmp_link,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
  .utimens	= xmp_utimens,
	.open		= xmp_open,
  .statfs		= xmp_statfs,
	.create   = xmp_create,
	.release	= xmp_release,
	.fsync		= xmp_fsync,
  #ifdef HAVE_SETXATTR
	.setxattr	= xmp_setxattr,
	.getxattr	= xmp_getxattr,
	.listxattr	= xmp_listxattr,
	.removexattr	= xmp_removexattr,
#endif
};



int  main(int  argc, char *argv[])

{

umask(0);

return fuse_main(argc, argv, &xmp_oper, NULL);

}
