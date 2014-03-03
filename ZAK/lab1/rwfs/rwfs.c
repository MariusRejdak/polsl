/*
 * rwfs - Read Write Filesystem
 * loosely based on rofs - read-only filesystem
 * Mount: rwfs full_source_path mount_point
 * The only write operations supported are:
 * write - to write
 * truncate - to allow "echo > file""
 * 
 */


#define FUSE_USE_VERSION 26


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <dirent.h>
#include <unistd.h>
#include <fuse.h>

#include "arc4.h"

char *rw_path;

static char* translate_path(const char* path)
{

    char *rPath= malloc(sizeof(char)*(strlen(path)+strlen(rw_path)+1));
 
    strcpy(rPath,rw_path);
    if (rPath[strlen(rPath)-1]=='/') {
        rPath[strlen(rPath)-1]='\0';
    }
    strcat(rPath,path);
    return rPath;
}

 
static int rw_getattr(const char *path, struct stat *st_data)
{
    int res;
	char *lpath=translate_path(path);
	
    res = lstat(lpath, st_data);
    free(lpath);
    if(res == -1) {
        return -errno;
    }
    return 0;
}

static int rw_readlink(const char *path, char *buf, size_t size)
{
   	int res;
	char *lpath=translate_path(path);
	
    res = readlink(lpath, buf, size - 1);
    free(lpath);
    if(res == -1) {
        return -errno;
    }
    buf[res] = '\0';
    return 0;
}

static int rw_readdir(const char *path, void *buf, fuse_fill_dir_t filler,off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    int res;

    char *lpath=translate_path(path);

    dp = opendir(lpath);
    free(lpath);
    if(dp == NULL) {
        res = -errno;
        return res;
    }

    while((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int rw_truncate(const char *path, off_t size)
{
  	return 0;
}

static int rw_open(const char *path, struct fuse_file_info *finfo)
{
	int res;
	
	int flags = finfo->flags;
	
	if ((flags & O_RDWR) || (flags & O_CREAT) || (flags & O_EXCL) || (flags & O_TRUNC) ) {
      return -EROFS;
  	}
  	
  	char *lpath=translate_path(path);
  
    res = open(lpath, flags);
 
    free(lpath);
    if(res == -1) {
        return -errno;
    }
    close(res);
    return 0;
}


//Key for rc4
//md5sum rwfs.tgz
unsigned char mykey[] = {0x97, 0x77, 0x5b, 0xca, 0x6d, 0xd8, 0x1e, 0xe3, 0xac, 0xbb, 0x92, 0x5b, 0xa3, 0x42, 0x3f, 0x5c};

void encrypt(unsigned char *rc4_key, const char* src, char* dest, size_t offset, size_t length)
{
    ArcfourContext ctx;
    size_t i;

    arcfour_init(&ctx, rc4_key, 16); //128bit key
  
    for (i = 0; i < offset; ++i)
        arcfour_byte(&ctx);

    for (i = 0; i < length; ++i)
        dest[i] = src[i] ^ arcfour_byte(&ctx);
}

static int rw_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *finfo)
{
    int fd;
    int res;
    char* cipher;

	char *lpath=translate_path(path);
    fd = open(lpath, O_RDONLY);
    free(lpath);
    if(fd == -1) {
        res = -errno;
        return res;
    }

    cipher = malloc(sizeof(unsigned char)*size);
    res = pread(fd, cipher, size, offset);
    encrypt(mykey, cipher, buf, offset, size);
    //res = pread(fd, buf, size, offset);
    free(cipher);
    
    if(res == -1) {
        res = -errno;
	}
    close(fd);
    return res;
}

static int rw_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *finfo)
{
	int fd;
    int res = 0;
    char* cipher;

	char *lpath=translate_path(path);
    fd = open(lpath, O_WRONLY);
    free(lpath);
    if(fd == -1) {
        res = -errno;
        return res;
    }

    cipher = malloc(sizeof(unsigned char)*size);
    encrypt(mykey, buf, cipher, offset, size);
    res = pwrite(fd, cipher, size, offset);
    //res = pwrite(fd, buf, size, offset);
    free(cipher);
    
    if(res == -1) {
        res = -errno;
	}
    close(fd);
    	    
    return res;
}

static int rw_statfs(const char *path, struct statvfs *st_buf)
{
  int res;
  char *lpath=translate_path(path);
  
  res = statvfs(lpath, st_buf);
  free(lpath);
  if (res == -1) {
    return -errno;
  }
  return 0;
}

static int rw_release(const char *path, struct fuse_file_info *finfo)
{
  return 0;
}

static int rw_fsync(const char *path, int crap, struct fuse_file_info *finfo)
{
  return 0;
}

static int rw_access(const char *path, int mode)
{
	int res;
  	char *lpath=translate_path(path);
  	        
  	res = access(lpath, mode);
	free(lpath);
  	if (res == -1) {
    	return -errno;
  	}
  	return res;
}

static int rw_mknod(const char *path, mode_t mode, dev_t rdev)
{
  return -ENOSYS;
}

static int rw_mkdir(const char *path, mode_t mode)
{
  return -ENOSYS;
}

static int rw_unlink(const char *path)
{
  return -ENOSYS;
}

static int rw_rmdir(const char *path)
{
  return -ENOSYS;
}

static int rw_symlink(const char *from, const char *to)
{
  return -ENOSYS;	
}

static int rw_rename(const char *from, const char *to)
{
  return -ENOSYS;
}

static int rw_link(const char *from, const char *to)
{
  return -ENOSYS;
}

static int rw_chmod(const char *path, mode_t mode)
{
  return -ENOSYS;    
}

static int rw_chown(const char *path, uid_t uid, gid_t gid)
{
  return -ENOSYS;
}

static int rw_utime(const char *path, struct utimbuf *buf)
{
  	return -ENOSYS;	
}

static int rw_setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
{
	return -ENOSYS;
}

static int rw_getxattr(const char *path, const char *name, char *value, size_t size)
{
	return -ENOSYS;
}

static int rw_listxattr(const char *path, char *list, size_t size)
{
	return -ENOSYS;
}

static int rw_removexattr(const char *path, const char *name)
{
  	return -ENOSYS;
}

struct fuse_operations rw_oper = {
    .getattr	= rw_getattr,
    .readlink	= rw_readlink,
    .readdir	= rw_readdir,
    .mknod		= rw_mknod,
    .mkdir		= rw_mkdir,
    .symlink	= rw_symlink,
    .unlink		= rw_unlink,
    .rmdir		= rw_rmdir,
    .rename		= rw_rename,
    .link		= rw_link,
    .chmod		= rw_chmod,
    .chown		= rw_chown,
    .truncate	= rw_truncate,
    .utime		= rw_utime,
    .open		= rw_open,
    .read		= rw_read,
    .write		= rw_write,
    .statfs		= rw_statfs,
    .release	= rw_release,
    .fsync		= rw_fsync,
    .access		= rw_access,
    .setxattr	= rw_setxattr,
    .getxattr	= rw_getxattr,
    .listxattr	= rw_listxattr,
    .removexattr= rw_removexattr
};

#define KEY_HELP 0

static void usage(const char* progname)
{
    fprintf(stdout,
"rwfs - simple rw fuse filesystem"
"usage: %s source mountpoint\n"
"\n"
"   Mounts source at mountpoint\n"
"   Full path for source should be specified", progname);
}

static int rwfs_parse_opt(void *data, const char *arg, int key,
        struct fuse_args *outargs)
{
    (void) data;

    switch (key)
    {
        case FUSE_OPT_KEY_NONOPT:
            if (rw_path == 0)
            {
                rw_path = strdup(arg);
                return 0;
            }
            else
            {
                return 1;
            }
        case FUSE_OPT_KEY_OPT:
            return 1;
        case KEY_HELP:
            usage(outargs->argv[0]);
            exit(0);
        default:
            fprintf(stderr, "`%s -h' for help\n", outargs->argv[0]);
            exit(1);
    }
    return 1;
}

static struct fuse_opt rwfs_opts[] = {
    FUSE_OPT_KEY("-h",          KEY_HELP),
    FUSE_OPT_KEY("--help",      KEY_HELP),
    FUSE_OPT_END
};

int main(int argc, char *argv[])
{
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    int res;

    res = fuse_opt_parse(&args, &rw_path, rwfs_opts, rwfs_parse_opt);
    if (res != 0)
    {
        fprintf(stderr, "Invalid arguments\n");
        fprintf(stderr, "`%s -h' for help\n", argv[0]);
        exit(1);
    }
    if (rw_path == 0)
    {
        fprintf(stderr, "Missing source for mount\n");
        fprintf(stderr, "`%s -h' for help\n", argv[0]);
        exit(1);
    }
    fuse_main(args.argc, args.argv, &rw_oper, NULL);

    return 0;
}
