#define FUSE_USE_VERSION 28
#include<fuse.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<errno.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>

char dirpath[50]  = "/home/adam/Documents";  // TERGANTUNG PENGGUNA
char cckey[] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO" ;
char map[200], revmap[200];
int nominal =10;

//BAGIAN 1
//fungsi encrypt 1
void encr(char* path)
{
  if(!strcmp(path,".") || !strcmp(path,".."))
  {
    return;
  }

  int i;
  char* ext;
  char* itr;
  ext = strrchr(path,'.');
  if(ext != NULL)
  {
      for(itr=path; itr!=ext; itr++)
      {
          *itr = map[(int)(*itr)];
      }
  }
  else{
      for(i=0; i < strlen(path) ; i++)
      {
          path[i] = map[(int)path[i]];
      }
  }
}

//fungsi decrypt 1
void decr(char* path)
{
  if(!strcmp(path,".") || !strcmp(path,".."))
  {
    return;
  }
  int i;
  char* ext;
  char* itr;
  ext = strrchr(path,'.');
  if(ext != NULL)
  {
      for(itr=path; itr!=ext; itr++)
      {
          *itr = revmap[(int)(*itr)];
      }
  }
  else
  {
      for(i=0; i < strlen(path) ; i++)
      {
          path[i] = revmap[(int)path[i]];
      }
  }
}

void ganti()
{
  for(int i=0; i<200; ++i){
      map[i] = (char) i;
      revmap[i] = (char) i;
  }

  for(int i=0; i<strlen(cckey); i++){
      map[(int)*(cckey + i)] = *(cckey + (i + nominal) % strlen(cckey) );
      revmap[(int)*(cckey + (i + nominal) % strlen(cckey))] = *(cckey + i);
  }
}

bool cek(char* path){
    char fullpath[1024];
    sprintf(fullpath, "%s%s", dirpath, path);
    char* pattern = "encv1_";

    char* last = strrchr(fullpath, '/');

    if(last){
        for(char* itr = fullpath; itr < last - strlen(pattern) ; ++itr){
            if(*itr == '/'){
                if(!strncmp(itr + 1, pattern, strlen(pattern))) return true;
            }
        }
    }
    return false;
}

//BAGIAN 4

void writeI(char *text, char* path)
{
    char* info = "INFO";
	char curtime[30];
    time_t t = time(NULL);
    struct tm* p1 = localtime(&t);
	strftime(curtime, 30, "%y%m%d-%H:%M:%S", p1);
    char log[1000];
    sprintf(log, "%s::%s::%s::%s", info, curtime, text, path);
	FILE *out = fopen("/home/adam/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return 0;
}

void writeW(char *text, char* path)
{
    char* info = "WARNING";
    char curtime[30];
    time_t t = time(NULL);
    struct tm* p1 = localtime(&t);
	strftime(curtime, 30, "%y%m%d-%H:%M:%S", p1);
    char log[1000];
    sprintf(log, "%s::%s::%s::%s", info, curtime, text, path);
	FILE *out = fopen("/home/adam/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char fpath[1000];
   	writeI("LS", path);
	int res;
	
	if(cek(fpath))
	{
	char* temp;
	for(char* itr = fpath; itr < fpath + strlen(fpath) - 6; ++itr)
	{
		if(*itr == '/' && !strncmp(itr + 1, "encv1_", 6))
		{
			temp = strchr(itr+1, '/');
		}
	}
	decr(temp+1);
	}
	
	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
    	writeI("CD", path);
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(path);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		char fullpathname[1000];
	      	sprintf(fullpathname, "%s/%s", fpath, de->d_name);
	      	char temp[1000];
	      	strcpy(temp,de->d_name);
		if(cek(fullpathname))
		{
			  encr(temp);
		}
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
    writeI("CAT", path);
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    writeI("MKDIR", path);
	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    writeI("CREATE", path);
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

static int xmp_unlink(const char *path)
{

    writeW("REMOVE", path);
	int res;

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
    writeW("RMDIR", path);
	int res;

	res = rmdir(path);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    writeI("MOVE", from);
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    writeI("TRUNCATE", path);
	int res;

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    writeI("OPEN", path);
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
    writeI("WRITE", path);
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
static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.mknod = xmp_mknod,
	.unlink = xmp_unlink,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.truncate = xmp_truncate,
	.open = xmp_open,
	.read = xmp_read,
	.write = xmp_write,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
