#include "libbb.h"
#include <userenv.h>
#include <stmicroelectronics/longpath.h>
#include "lazyload.h"
#if ENABLE_FEATURE_EXTRA_FILE_DATA
#include <aclapi.h>
#endif
#if ENABLE_FEATURE_READLINK2
#include <ntdef.h>
#endif

#if defined(__MINGW64_VERSION_MAJOR)
#if ENABLE_GLOBBING
int _dowildcard = -1;
#else
int _dowildcard = 0;
#endif

#undef _fmode
int _fmode = _O_BINARY;
#endif

#if !defined(__MINGW64_VERSION_MAJOR)
#if ENABLE_GLOBBING
int _CRT_glob = 1;
#else
int _CRT_glob = 0;
#endif

unsigned int _CRT_fmode = _O_BINARY;
#endif

smallint bb_got_signal;

int err_win_to_posix(void)
{
	int error = ENOSYS;
	switch(GetLastError()) {
	case ERROR_ACCESS_DENIED: error = EACCES; break;
	case ERROR_ACCOUNT_DISABLED: error = EACCES; break;
	case ERROR_ACCOUNT_RESTRICTION: error = EACCES; break;
	case ERROR_ALREADY_ASSIGNED: error = EBUSY; break;
	case ERROR_ALREADY_EXISTS: error = EEXIST; break;
	case ERROR_ARITHMETIC_OVERFLOW: error = ERANGE; break;
	case ERROR_BAD_COMMAND: error = EIO; break;
	case ERROR_BAD_DEVICE: error = ENODEV; break;
	case ERROR_BAD_DRIVER_LEVEL: error = ENXIO; break;
	case ERROR_BAD_EXE_FORMAT: error = ENOEXEC; break;
	case ERROR_BAD_FORMAT: error = ENOEXEC; break;
	case ERROR_BAD_LENGTH: error = EINVAL; break;
	case ERROR_BAD_PATHNAME: error = ENOENT; break;
	case ERROR_BAD_PIPE: error = EPIPE; break;
	case ERROR_BAD_UNIT: error = ENODEV; break;
	case ERROR_BAD_USERNAME: error = EINVAL; break;
	case ERROR_BROKEN_PIPE: error = EPIPE; break;
	case ERROR_BUFFER_OVERFLOW: error = ENAMETOOLONG; break;
	case ERROR_BUSY: error = EBUSY; break;
	case ERROR_BUSY_DRIVE: error = EBUSY; break;
	case ERROR_CALL_NOT_IMPLEMENTED: error = ENOSYS; break;
	case ERROR_CANNOT_MAKE: error = EACCES; break;
	case ERROR_CANTOPEN: error = EIO; break;
	case ERROR_CANTREAD: error = EIO; break;
	case ERROR_CANTWRITE: error = EIO; break;
	case ERROR_CRC: error = EIO; break;
	case ERROR_CURRENT_DIRECTORY: error = EACCES; break;
	case ERROR_DEVICE_IN_USE: error = EBUSY; break;
	case ERROR_DEV_NOT_EXIST: error = ENODEV; break;
	case ERROR_DIRECTORY: error = EINVAL; break;
	case ERROR_DIR_NOT_EMPTY: error = ENOTEMPTY; break;
	case ERROR_DISK_CHANGE: error = EIO; break;
	case ERROR_DISK_FULL: error = ENOSPC; break;
	case ERROR_DRIVE_LOCKED: error = EBUSY; break;
	case ERROR_ENVVAR_NOT_FOUND: error = EINVAL; break;
	case ERROR_EXE_MARKED_INVALID: error = ENOEXEC; break;
	case ERROR_FILENAME_EXCED_RANGE: error = ENAMETOOLONG; break;
	case ERROR_FILE_EXISTS: error = EEXIST; break;
	case ERROR_FILE_INVALID: error = ENODEV; break;
	case ERROR_FILE_NOT_FOUND: error = ENOENT; break;
	case ERROR_GEN_FAILURE: error = EIO; break;
	case ERROR_HANDLE_DISK_FULL: error = ENOSPC; break;
	case ERROR_INSUFFICIENT_BUFFER: error = ENOMEM; break;
	case ERROR_INVALID_ACCESS: error = EACCES; break;
	case ERROR_INVALID_ADDRESS: error = EFAULT; break;
	case ERROR_INVALID_BLOCK: error = EFAULT; break;
	case ERROR_INVALID_DATA: error = EINVAL; break;
	case ERROR_INVALID_DRIVE: error = ENODEV; break;
	case ERROR_INVALID_EXE_SIGNATURE: error = ENOEXEC; break;
	case ERROR_INVALID_FLAGS: error = EINVAL; break;
	case ERROR_INVALID_FUNCTION: error = ENOSYS; break;
	case ERROR_INVALID_HANDLE: error = EBADF; break;
	case ERROR_INVALID_LOGON_HOURS: error = EACCES; break;
	case ERROR_INVALID_NAME: error = EINVAL; break;
	case ERROR_INVALID_OWNER: error = EINVAL; break;
	case ERROR_INVALID_PARAMETER: error = EINVAL; break;
	case ERROR_INVALID_PASSWORD: error = EPERM; break;
	case ERROR_INVALID_PRIMARY_GROUP: error = EINVAL; break;
	case ERROR_INVALID_SIGNAL_NUMBER: error = EINVAL; break;
	case ERROR_INVALID_TARGET_HANDLE: error = EIO; break;
	case ERROR_INVALID_WORKSTATION: error = EACCES; break;
	case ERROR_IO_DEVICE: error = EIO; break;
	case ERROR_IO_INCOMPLETE: error = EINTR; break;
	case ERROR_LOCKED: error = EBUSY; break;
	case ERROR_LOCK_VIOLATION: error = EACCES; break;
	case ERROR_LOGON_FAILURE: error = EACCES; break;
	case ERROR_MAPPED_ALIGNMENT: error = EINVAL; break;
	case ERROR_META_EXPANSION_TOO_LONG: error = E2BIG; break;
	case ERROR_MORE_DATA: error = EPIPE; break;
	case ERROR_NEGATIVE_SEEK: error = ESPIPE; break;
	case ERROR_NOACCESS: error = EFAULT; break;
	case ERROR_NONE_MAPPED: error = EINVAL; break;
	case ERROR_NOT_ENOUGH_MEMORY: error = ENOMEM; break;
	case ERROR_NOT_READY: error = EAGAIN; break;
	case ERROR_NOT_SAME_DEVICE: error = EXDEV; break;
	case ERROR_NO_DATA: error = EPIPE; break;
	case ERROR_NO_MORE_SEARCH_HANDLES: error = EIO; break;
	case ERROR_NO_PROC_SLOTS: error = EAGAIN; break;
	case ERROR_NO_SUCH_PRIVILEGE: error = EACCES; break;
	case ERROR_OPEN_FAILED: error = EIO; break;
	case ERROR_OPEN_FILES: error = EBUSY; break;
	case ERROR_OPERATION_ABORTED: error = EINTR; break;
	case ERROR_OUTOFMEMORY: error = ENOMEM; break;
	case ERROR_PASSWORD_EXPIRED: error = EACCES; break;
	case ERROR_PATH_BUSY: error = EBUSY; break;
	case ERROR_PATH_NOT_FOUND: error = ENOENT; break;
	case ERROR_PIPE_BUSY: error = EBUSY; break;
	case ERROR_PIPE_CONNECTED: error = EPIPE; break;
	case ERROR_PIPE_LISTENING: error = EPIPE; break;
	case ERROR_PIPE_NOT_CONNECTED: error = EPIPE; break;
	case ERROR_PRIVILEGE_NOT_HELD: error = EACCES; break;
	case ERROR_READ_FAULT: error = EIO; break;
	case ERROR_SEEK: error = EIO; break;
	case ERROR_SEEK_ON_DEVICE: error = ESPIPE; break;
	case ERROR_SHARING_BUFFER_EXCEEDED: error = ENFILE; break;
	case ERROR_SHARING_VIOLATION: error = EACCES; break;
	case ERROR_STACK_OVERFLOW: error = ENOMEM; break;
	case ERROR_SWAPERROR: error = ENOENT; break;
	case ERROR_TOO_MANY_LINKS: error = EMLINK; break;
	case ERROR_TOO_MANY_MODULES: error = EMFILE; break;
	case ERROR_TOO_MANY_OPEN_FILES: error = EMFILE; break;
	case ERROR_UNRECOGNIZED_MEDIA: error = ENXIO; break;
	case ERROR_UNRECOGNIZED_VOLUME: error = ENODEV; break;
	case ERROR_WAIT_NO_CHILDREN: error = ECHILD; break;
	case ERROR_WRITE_FAULT: error = EIO; break;
	case ERROR_WRITE_PROTECT: error = EROFS; break;
	case ERROR_CANT_RESOLVE_FILENAME: error = ELOOP; break;
	}
	return error;
}

static int zero_fd = -1;
static int rand_fd = -1;

/*
 * Determine if 'filename' corresponds to one of the supported
 * device files.  Constants for these are defined as an enum
 * in mingw.h.
 */
int get_dev_type(const char *filename)
{
	int i;
	const char *devname[NOT_DEVICE] = { "null", "zero", "urandom" };

	if (filename && !strncmp(filename, "/dev/", 5)) {
		for (i=0; i<NOT_DEVICE; ++i ) {
			if (!strcmp(filename+5, devname[i])) {
				return i;
			}
		}
	}

	return NOT_DEVICE;
}

void update_dev_fd(int dev, int fd)
{
	if (dev == DEV_ZERO)
		zero_fd = fd;
	else if (dev == DEV_URANDOM)
		rand_fd = fd;
}

#undef open
int mingw_open (const char *filename, int oflags, ...)
{
	va_list args;
	unsigned mode;
	int fd;
	int special = (oflags & O_SPECIAL);
	int dev = get_dev_type(filename);

	/* /dev/null is always allowed, others only if O_SPECIAL is set */
	if (dev != NOT_DEVICE && (dev == DEV_NULL || special)) {
		filename = "nul";
		oflags = O_RDWR;
	}

	va_start(args, oflags);
	mode = va_arg(args, int);
	va_end(args);

	fd = open(filename, oflags&~O_SPECIAL, mode);
	if (fd >= 0) {
		update_dev_fd(dev, fd);
	}
	else if ((oflags & O_ACCMODE) != O_RDONLY && errno == EACCES) {
		DWORD attrs = GetFileAttributes(filename);
		if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY))
			errno = EISDIR;
	}
	return fd;
}

int mingw_xopen(const char *pathname, int flags)
{
	int ret;

	/* allow use of special devices */
	ret = mingw_open(pathname, flags|O_SPECIAL);
	if (ret < 0) {
		bb_perror_msg_and_die("can't open '%s'", pathname);
	}
	return ret;
}

#undef fopen
FILE *mingw_fopen (const char *filename, const char *otype)
{
	if (filename && !strcmp(filename, "/dev/null"))
		filename = "nul";
	return fopen(filename, otype);
}

#undef read
ssize_t mingw_read(int fd, void *buf, size_t count)
{
	if (fd == zero_fd) {
		memset(buf, 0, count);
		return count;
	}
	else if (fd == rand_fd) {
		return get_random_bytes(buf, count);
	}
	return read(fd, buf, count);
}

#undef close
int mingw_close(int fd)
{
	if (fd == zero_fd) {
		zero_fd = -1;
	}
	if (fd == rand_fd) {
		rand_fd = -1;
	}
	return close(fd);
}

#undef dup2
int mingw_dup2 (int fd, int fdto)
{
	int ret = dup2(fd, fdto);
	return ret != -1 ? fdto : -1;
}

/*
 * The unit of FILETIME is 100-nanoseconds since January 1, 1601, UTC.
 * Returns the 100-nanoseconds ("hekto nanoseconds") since the epoch.
 */
static inline long long filetime_to_hnsec(const FILETIME *ft)
{
	long long winTime = ((long long)ft->dwHighDateTime << 32) + ft->dwLowDateTime;
	/* Windows to Unix Epoch conversion */
	return winTime - 116444736000000000LL;
}

static inline time_t filetime_to_time_t(const FILETIME *ft)
{
	return (time_t)(filetime_to_hnsec(ft) / 10000000);
}

static inline mode_t file_attr_to_st_mode(DWORD attr)
{
	mode_t fMode = S_IRUSR|S_IRGRP|S_IROTH;
	if (attr & FILE_ATTRIBUTE_DIRECTORY)
		fMode |= S_IFDIR|S_IWUSR|S_IWGRP|S_IXUSR|S_IXGRP|S_IXOTH;
	else
		fMode |= S_IFREG;
	if (!(attr & FILE_ATTRIBUTE_READONLY))
		fMode |= S_IWUSR|S_IWGRP;
	return fMode;
}

static inline int get_file_attr(const char *fname, WIN32_FILE_ATTRIBUTE_DATA *fdata)
{
	size_t len;

	if (GetFileAttributesExA(fname, GetFileExInfoStandard, fdata))
		return 0;

	if (GetLastError() == ERROR_SHARING_VIOLATION) {
		HANDLE hnd;
		WIN32_FIND_DATA fd;

		if ((hnd=FindFirstFile(fname, &fd)) != INVALID_HANDLE_VALUE) {
			fdata->dwFileAttributes = fd.dwFileAttributes;
			fdata->ftCreationTime = fd.ftCreationTime;
			fdata->ftLastAccessTime = fd.ftLastAccessTime;
			fdata->ftLastWriteTime = fd.ftLastWriteTime;
			fdata->nFileSizeHigh = fd.nFileSizeHigh;
			fdata->nFileSizeLow = fd.nFileSizeLow;
			FindClose(hnd);
			return 0;
		}
	}

	switch (GetLastError()) {
	case ERROR_ACCESS_DENIED:
	case ERROR_SHARING_VIOLATION:
	case ERROR_LOCK_VIOLATION:
	case ERROR_SHARING_BUFFER_EXCEEDED:
		return EACCES;
	case ERROR_BUFFER_OVERFLOW:
		return ENAMETOOLONG;
	case ERROR_NOT_ENOUGH_MEMORY:
		return ENOMEM;
	case ERROR_INVALID_NAME:
		len = strlen(fname);
		if (len > 1 && (fname[len-1] == '/' || fname[len-1] == '\\'))
			return ENOTDIR;
	default:
		return ENOENT;
	}
}

#undef umask
mode_t mingw_umask(mode_t new_mode)
{
	static mode_t old_mode = DEFAULT_UMASK;
	mode_t tmp_mode;

	tmp_mode = old_mode;
	old_mode = new_mode;

	umask((new_mode & S_IWUSR) ? _S_IWRITE : 0);

	return tmp_mode;
}

/*
 * Examine a file's contents to determine if it can be executed.  This
 * should be a last resort:  in most cases it's much more efficient to
 * check the file extension.
 *
 * We look for two types of file:  shell scripts and binary executables.
 */
static int has_exec_format(const char *name)
{
	int n, sig;
	unsigned int offset;
	unsigned char buf[1024];

	/* special case: skip DLLs, there are thousands of them! */
	if (is_suffixed_with_case(name, ".dll"))
		return 0;

	n = open_read_close(name, buf, sizeof(buf));
	if (n < 4)	/* at least '#!/x' and not error */
		return 0;

	/* shell script */
	if (buf[0] == '#' && buf[1] == '!') {
		return 1;
	}

	/*
	 * Poke about in file to see if it's a PE binary.  I've just copied
	 * the magic from the file command.
	 */
	if (buf[0] == 'M' && buf[1] == 'Z') {
		offset = (buf[0x19] << 8) + buf[0x18];
		if (offset > 0x3f) {
			offset = (buf[0x3f] << 24) + (buf[0x3e] << 16) +
						(buf[0x3d] << 8) + buf[0x3c];
			if (offset < sizeof(buf)-100) {
				if (memcmp(buf+offset, "PE\0\0", 4) == 0) {
					sig = (buf[offset+25] << 8) + buf[offset+24];
					if (sig == 0x10b || sig == 0x20b) {
						sig = (buf[offset+23] << 8) + buf[offset+22];
						if ((sig & 0x2000) != 0) {
							/* DLL */
							return 0;
						}
						sig = buf[offset+92];
						return (sig == 1 || sig == 2 || sig == 3 || sig == 7);
					}
				}
			}
		}
	}

	return 0;
}

#if ENABLE_FEATURE_EXTRA_FILE_DATA
static uid_t file_owner(HANDLE fh)
{
	PSID pSidOwner;
	PSECURITY_DESCRIPTOR pSD;
	static PTOKEN_USER user = NULL;
	static int initialised = 0;
	int equal;
	uid_t uid = 0;
	DWORD *ptr;
	unsigned char prefix[] = {
			0x01, 0x05, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x05,
			0x15, 0x00, 0x00, 0x00
	};

	/*  get SID of current user */
	if (!initialised) {
		HANDLE token;
		DWORD ret = 0;

		initialised = 1;
		if (OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &token) ||
				OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
			GetTokenInformation(token, TokenUser, NULL, 0, &ret);
			if (ret <= 0 || (user=malloc(ret)) == NULL ||
					!GetTokenInformation(token, TokenUser, user, ret, &ret)) {
				free(user);
				user = NULL;
			}
			CloseHandle(token);
		}
	}

	if (user == NULL)
		return DEFAULT_UID;

	/* get SID of file's owner */
	if (GetSecurityInfo(fh, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION,
			&pSidOwner, NULL, NULL, NULL, &pSD) != ERROR_SUCCESS)
		return 0;

	equal = EqualSid(pSidOwner, user->User.Sid);
	LocalFree(pSD);

	if (equal)
		return DEFAULT_UID;

	/* for local or domain users use the RID as uid */
	if (memcmp(pSidOwner, prefix, sizeof(prefix)) == 0) {
		ptr = (DWORD *)pSidOwner;
		if (ptr[6] >= 500 && ptr[6] < DEFAULT_UID)
			uid = (uid_t)ptr[6];
	}
	return uid;

#if 0
	/* this is how it would be done properly using the API */
	{
		PSID_IDENTIFIER_AUTHORITY auth;
		unsigned char *count;
		PDWORD subauth;
		unsigned char nt_auth[] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x05
		};

		if (IsValidSid(pSidOwner) ) {
			auth = GetSidIdentifierAuthority(pSidOwner);
			count = GetSidSubAuthorityCount(pSidOwner);
			subauth = GetSidSubAuthority(pSidOwner, 0);
			if (memcmp(auth, nt_auth, sizeof(nt_auth)) == 0 &&
					*count == 5 && *subauth == 21) {
				subauth = GetSidSubAuthority(pSidOwner, 4);
				if (*subauth >= 500 && *subauth < DEFAULT_UID)
					uid = (uid_t)*subauth;
			}
		}
		return uid;
	}
#endif
}
#endif

static int is_symlink(DWORD attr, const char *pathname, WIN32_FIND_DATAA *fbuf)
{
	if (attr & FILE_ATTRIBUTE_REPARSE_POINT) {
		HANDLE handle = FindFirstFileA(pathname, fbuf);
		if (handle != INVALID_HANDLE_VALUE) {
			FindClose(handle);
			return ((fbuf->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) &&
					fbuf->dwReserved0 == IO_REPARSE_TAG_SYMLINK);
		}
	}
	return 0;
}

/* If follow is true then act like stat() and report on the link
 * target. Otherwise report on the link itself.
 */
static int do_lstat(int follow, const char *file_name, struct mingw_stat *buf)
{
	int err = EINVAL;
	WIN32_FILE_ATTRIBUTE_DATA fdata;
	WIN32_FIND_DATAA findbuf;
#if ENABLE_FEATURE_EXTRA_FILE_DATA
	DWORD flags;
	BY_HANDLE_FILE_INFORMATION hdata;
	HANDLE fh;
#endif

	while (file_name && !(err=get_file_attr(file_name, &fdata))) {
		buf->st_ino = 0;
		buf->st_uid = DEFAULT_UID;
		buf->st_gid = DEFAULT_GID;
		buf->st_dev = buf->st_rdev = 0;

		if (is_symlink(fdata.dwFileAttributes, file_name, &findbuf)) {
			char *name = auto_string(xmalloc_realpath(file_name));

			if (follow) {
				/* The file size and times are wrong when Windows follows
				 * a symlink.  Use the canonicalized path to try again. */
				err = errno;
				file_name = name;
				continue;
			}

			/* Get the contents of a symlink, not its target. */
			buf->st_mode = S_IFLNK|S_IRWXU|S_IRWXG|S_IRWXO;
			buf->st_size = name ? strlen(name) : 0; /* should use readlink */
			buf->st_atime = filetime_to_time_t(&(findbuf.ftLastAccessTime));
			buf->st_mtime = filetime_to_time_t(&(findbuf.ftLastWriteTime));
			buf->st_ctime = filetime_to_time_t(&(findbuf.ftCreationTime));
		}
		else {
			/* The file is not a symlink. */
			buf->st_mode = file_attr_to_st_mode(fdata.dwFileAttributes);
			if (S_ISREG(buf->st_mode) &&
					(has_exe_suffix(file_name) || has_exec_format(file_name)))
				buf->st_mode |= S_IXUSR|S_IXGRP|S_IXOTH;
			buf->st_size = fdata.nFileSizeLow |
				(((off64_t)fdata.nFileSizeHigh)<<32);
			buf->st_atime = filetime_to_time_t(&(fdata.ftLastAccessTime));
			buf->st_mtime = filetime_to_time_t(&(fdata.ftLastWriteTime));
			buf->st_ctime = filetime_to_time_t(&(fdata.ftCreationTime));
		}
		buf->st_nlink = S_ISDIR(buf->st_mode) ? 2 : 1;

#if ENABLE_FEATURE_EXTRA_FILE_DATA
		flags = FILE_FLAG_BACKUP_SEMANTICS;
		if (S_ISLNK(buf->st_mode))
			flags |= FILE_FLAG_OPEN_REPARSE_POINT;
		fh = CreateFile(file_name, READ_CONTROL, 0, NULL,
							OPEN_EXISTING, flags, NULL);
		if (fh != INVALID_HANDLE_VALUE) {
			if (GetFileInformationByHandle(fh, &hdata)) {
				buf->st_dev = hdata.dwVolumeSerialNumber;
				buf->st_ino = hdata.nFileIndexLow |
						(((ino_t)hdata.nFileIndexHigh)<<32);
				buf->st_nlink = S_ISDIR(buf->st_mode) ? 2 :
							hdata.nNumberOfLinks;
			}
			buf->st_uid = buf->st_gid = file_owner(fh);
			CloseHandle(fh);
		}
		else {
			buf->st_uid = 0;
			buf->st_gid = 0;
			buf->st_mode &= ~(S_IROTH|S_IWOTH|S_IXOTH);
		}
#endif

		/*
		 * Assume a block is 4096 bytes and calculate number of 512 byte
		 * sectors.
		 */
		buf->st_blksize = 4096;
		buf->st_blocks = ((buf->st_size+4095)>>12)<<3;
		return 0;
	}
	errno = err;
	return -1;
}

int mingw_lstat(const char *file_name, struct mingw_stat *buf)
{
	return do_lstat(0, file_name, buf);
}

int mingw_stat(const char *file_name, struct mingw_stat *buf)
{
	return do_lstat(1, file_name, buf);
}

int mingw_fstat(int fd, struct mingw_stat *buf)
{
	HANDLE fh = (HANDLE)_get_osfhandle(fd);
	BY_HANDLE_FILE_INFORMATION fdata;

	if (fh == INVALID_HANDLE_VALUE) {
		errno = EBADF;
		return -1;
	}
	/* direct non-file handles to MS's fstat() */
	if (GetFileType(fh) != FILE_TYPE_DISK) {
		struct _stati64 buf64;

		if ( _fstati64(fd, &buf64) != 0 )  {
			return -1;
		}
		buf->st_mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH;
		buf->st_size = buf64.st_size;
		buf->st_atime = buf64.st_atime;
		buf->st_mtime = buf64.st_mtime;
		buf->st_ctime = buf64.st_ctime;
		buf->st_blocks = ((buf64.st_size+4095)>>12)<<3;
#if ENABLE_FEATURE_EXTRA_FILE_DATA
		buf->st_dev = 0;
		buf->st_ino = 0;
		buf->st_nlink = S_ISDIR(buf->st_mode) ? 2 : 1;
#endif
		goto success;
	}

	if (GetFileInformationByHandle(fh, &fdata)) {
		buf->st_mode = file_attr_to_st_mode(fdata.dwFileAttributes);
		buf->st_size = fdata.nFileSizeLow |
			(((off64_t)fdata.nFileSizeHigh)<<32);
		buf->st_atime = filetime_to_time_t(&(fdata.ftLastAccessTime));
		buf->st_mtime = filetime_to_time_t(&(fdata.ftLastWriteTime));
		buf->st_ctime = filetime_to_time_t(&(fdata.ftCreationTime));
		buf->st_blocks = ((buf->st_size+4095)>>12)<<3;
#if ENABLE_FEATURE_EXTRA_FILE_DATA
		buf->st_dev = fdata.dwVolumeSerialNumber;
		buf->st_ino = fdata.nFileIndexLow |
			(((uint64_t)fdata.nFileIndexHigh)<<32);
		buf->st_nlink = S_ISDIR(buf->st_mode) ? 2 : fdata.nNumberOfLinks;
#endif
 success:
#if !ENABLE_FEATURE_EXTRA_FILE_DATA
		buf->st_dev = 0;
		buf->st_ino = 0;
		buf->st_nlink = S_ISDIR(buf->st_mode) ? 2 : 1;
#endif
		buf->st_rdev = 0;
		buf->st_uid = DEFAULT_UID;
		buf->st_gid = DEFAULT_GID;
		buf->st_blksize = 4096;
		return 0;
	}

	errno = EBADF;
	return -1;
}

static inline void timeval_to_filetime(const struct timeval tv, FILETIME *ft)
{
	long long winTime = ((tv.tv_sec * 1000000LL) + tv.tv_usec) * 10LL + 116444736000000000LL;
	ft->dwLowDateTime = winTime;
	ft->dwHighDateTime = winTime >> 32;
}

int utimes(const char *file_name, const struct timeval tims[2])
{
	FILETIME mft, aft;
	HANDLE fh;
	int rc = 0;

	fh = CreateFile(file_name, FILE_WRITE_ATTRIBUTES, 0,
				NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if ( fh == INVALID_HANDLE_VALUE ) {
		errno = err_win_to_posix();
		return -1;
	}

	if (tims) {
		timeval_to_filetime(tims[0], &aft);
		timeval_to_filetime(tims[1], &mft);
	}
	else {
		GetSystemTimeAsFileTime(&mft);
		aft = mft;
	}

	if (!SetFileTime(fh, NULL, &aft, &mft)) {
		errno = EINVAL;
		rc = -1;
	}
	CloseHandle(fh);
	return rc;
}

unsigned int sleep (unsigned int seconds)
{
	Sleep(seconds*1000);
	return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{
	if (req->tv_nsec < 0 || 1000000000 <= req->tv_nsec) {
		errno = EINVAL;
		return -1;
	}

	Sleep(req->tv_sec*1000 + req->tv_nsec/1000000);

	/* Sleep is not interruptible.  So there is no remaining delay.  */
	if (rem != NULL) {
		rem->tv_sec = 0;
		rem->tv_nsec = 0;
	}

	return 0;
}

/*
 * Windows' mktemp returns NULL on error whereas POSIX always returns the
 * template and signals an error by making it an empty string.
 */
#undef mktemp
char *mingw_mktemp(char *template)
{
	if ( mktemp(template) == NULL ) {
		template[0] = '\0';
	}

	return template;
}

int mkstemp(char *template)
{
	char *filename = mktemp(template);
	if (filename == NULL)
		return -1;
	return open(filename, O_RDWR | O_CREAT, 0600);
}

int gettimeofday(struct timeval *tv, void *tz UNUSED_PARAM)
{
	FILETIME ft;
	long long hnsec;

	GetSystemTimeAsFileTime(&ft);
	hnsec = filetime_to_hnsec(&ft);
	tv->tv_sec = hnsec / 10000000;
	tv->tv_usec = (hnsec % 10000000) / 10;
	return 0;
}

int pipe(int filedes[2])
{
	if (_pipe(filedes, PIPE_BUF, 0) < 0)
		return -1;
	return 0;
}

struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
	/* gmtime() in MSVCRT.DLL is thread-safe, but not reentrant */
	memcpy(result, gmtime(timep), sizeof(struct tm));
	return result;
}

struct tm *localtime_r(const time_t *timep, struct tm *result)
{
	/* localtime() in MSVCRT.DLL is thread-safe, but not reentrant */
	memcpy(result, localtime(timep), sizeof(struct tm));
	return result;
}

#undef getcwd
char *mingw_getcwd(char *pointer, int len)
{
	char *ret = getcwd(pointer, len);
	if (!ret)
		return ret;
	bs_to_slash(ret);
	return ret;
}

#undef rename
int mingw_rename(const char *pold, const char *pnew)
{
	DWORD attrs;

	/*
	 * Try native rename() first to get errno right.
	 * It is based on MoveFile(), which cannot overwrite existing files.
	 */
	if (!rename(pold, pnew))
		return 0;
	if (errno != EEXIST)
		return -1;
	if (MoveFileEx(pold, pnew, MOVEFILE_REPLACE_EXISTING))
		return 0;
	/* TODO: translate more errors */
	if (GetLastError() == ERROR_ACCESS_DENIED &&
	    (attrs = GetFileAttributes(pnew)) != INVALID_FILE_ATTRIBUTES) {
		if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
			errno = EISDIR;
			return -1;
		}
		if ((attrs & FILE_ATTRIBUTE_READONLY) &&
		    SetFileAttributes(pnew, attrs & ~FILE_ATTRIBUTE_READONLY)) {
			if (MoveFileEx(pold, pnew, MOVEFILE_REPLACE_EXISTING))
				return 0;
			/* revert file attributes on failure */
			SetFileAttributes(pnew, attrs);
		}
	}
	errno = EACCES;
	return -1;
}

static char *gethomedir(void)
{
	static char *buf = NULL;
	DWORD len = MAX_LONG_PATH;
	HANDLE h;

	if (!buf)
		buf = xzalloc(MAX_LONG_PATH);

	if (buf[0])
		return buf;

	if ( !OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h) )
		return buf;

	GetUserProfileDirectory(h, buf, &len);
	CloseHandle(h);
	bs_to_slash(buf);

	return buf;
}

#define NAME_LEN 100
static char *get_user_name(void)
{
	static char *user_name = NULL;
	char *s;
	DWORD len = NAME_LEN;

	if ( user_name == NULL ) {
		user_name = xzalloc(NAME_LEN);
	}

	if ( user_name[0] != '\0' ) {
		return user_name;
	}

	if ( !GetUserName(user_name, &len) ) {
		return NULL;
	}

	for ( s=user_name; *s; ++s ) {
		if ( *s == ' ' ) {
			*s = '_';
		}
	}

	return user_name;
}

int getuid(void)
{
	int ret = DEFAULT_UID;
	HANDLE h;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h)) {
		TOKEN_ELEVATION elevation;
		DWORD size = sizeof(TOKEN_ELEVATION);

		if (GetTokenInformation(h, TokenElevation, &elevation,
					sizeof(elevation), &size)) {
			if (elevation.TokenIsElevated)
				ret = 0;
		}
		CloseHandle(h);
	}
	return ret;
}

struct passwd *getpwnam(const char *name)
{
	const char *myname;

	if ( (myname=get_user_name()) != NULL &&
			strcmp(myname, name) == 0 ) {
		return getpwuid(DEFAULT_UID);
	}
	else if (strcmp(name, "root") == 0) {
		return getpwuid(0);
	}

	return NULL;
}

struct passwd *getpwuid(uid_t uid)
{
	static struct passwd p;

	if (uid == 0) {
		static char *buf = NULL;
		char dir[PATH_MAX];

		if (!buf) {
			buf = xzalloc(PATH_MAX);
			GetSystemDirectory(dir, PATH_MAX);
			realpath(dir, buf);
		}

		p.pw_name = (char *)"root";
		p.pw_dir = buf;
	}
	else if (uid == DEFAULT_UID && (p.pw_name=get_user_name()) != NULL) {
		p.pw_dir = gethomedir();
	}
	else {
		return NULL;
	}

	p.pw_passwd = (char *)"";
	p.pw_gecos = p.pw_name;
	p.pw_shell = NULL;
	p.pw_uid = uid;
	p.pw_gid = uid;

	return &p;
}

struct group *getgrgid(gid_t gid)
{
	static char *members[2] = { NULL, NULL };
	static struct group g;

	if (gid == 0) {
		g.gr_name = (char *)"root";
	}
	else if (gid != DEFAULT_GID || (g.gr_name=get_user_name()) == NULL) {
		return NULL;
	}
	g.gr_passwd = (char *)"";
	g.gr_gid = gid;
	members[0] = g.gr_name;
	g.gr_mem = members;

	return &g;
}

int getgrouplist(const char *user UNUSED_PARAM, gid_t group,
					gid_t *groups, int *ngroups)
{
	if ( *ngroups == 0 ) {
		*ngroups = 1;
		return -1;
	}

	*ngroups = 1;
	groups[0] = group;
	return 1;
}

int getgroups(int n, gid_t *groups)
{
	if ( n == 0 ) {
		return 1;
	}

	groups[0] = getgid();
	return 1;
}

int getlogin_r(char *buf, size_t len)
{
	char *name;

	if ( (name=get_user_name()) == NULL ) {
		return -1;
	}

	if ( strlen(name) >= len ) {
		errno = ERANGE;
		return -1;
	}

	strcpy(buf, name);
	return 0;
}

long sysconf(int name)
{
	if ( name == _SC_CLK_TCK ) {
		return TICKS_PER_SECOND;
	}
	errno = EINVAL;
	return -1;
}

clock_t times(struct tms *buf)
{
	memset(buf, 0, sizeof(*buf));
	return 0;
}

int link(const char *oldpath, const char *newpath)
{
	DECLARE_PROC_ADDR(BOOL, CreateHardLinkA, LPCSTR, LPCSTR,
						LPSECURITY_ATTRIBUTES);

	if (!INIT_PROC_ADDR(kernel32.dll, CreateHardLinkA)) {
		errno = ENOSYS;
		return -1;
	}
	if (!CreateHardLinkA(newpath, oldpath, NULL)) {
		errno = err_win_to_posix();
		return -1;
	}
	return 0;
}

static char *normalize_ntpathA(char *buf)
{
	/* fix absolute path prefixes */
	if (buf[0] == '\\') {
		/* strip NT namespace prefixes */
		if (!strncmp(buf, "\\??\\", 4) ||
		    !strncmp(buf, "\\\\?\\", 4))
			buf += 4;
		else if (!strncasecmp(buf, "\\DosDevices\\", 12))
			buf += 12;
		/* replace remaining '...UNC\' with '\\' */
		if (!strncasecmp(buf, "UNC\\", 4)) {
			buf += 2;
			*buf = '\\';
		}
	}
	return buf;
}

static char *resolve_symlinks(char *path)
{
	HANDLE h;
	DWORD status;
	char *ptr = NULL;
	DECLARE_PROC_ADDR(DWORD, GetFinalPathNameByHandleA, HANDLE,
						LPSTR, DWORD, DWORD);

	/* need a file handle to resolve symlinks */
	h = CreateFileA(path, 0, 0, NULL, OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		if (!INIT_PROC_ADDR(kernel32.dll, GetFinalPathNameByHandleA)) {
			ptr = path;
			goto end;
		}

		/* normalize the path and return it on success */
		status = GetFinalPathNameByHandleA(h, path, MAX_PATH,
							FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
		if (status != 0 && status < MAX_PATH) {
			ptr = normalize_ntpathA(path);
			goto end;
		}
	}

	errno = err_win_to_posix();
 end:
	CloseHandle(h);
	return ptr;
}

/*
 * Emulate realpath in two stages:
 *
 * - _fullpath handles './', '../' and extra '/' characters.  The
 *   resulting path may not refer to an actual file.
 *
 * - resolve_symlinks checks that the file exists (by opening it) and
 *   resolves symlinks by calling GetFinalPathNameByHandleA.
 */
char *realpath(const char *path, char *resolved_path)
{
	char buffer[MAX_LONG_PATH];
	char *real_path, *p;

	/* enforce glibc pre-2.3 behaviour */
	if (path == NULL || resolved_path == NULL) {
		errno = EINVAL;
		return NULL;
	}

	if (_fullpath(buffer, path, MAX_PATH) &&
			(real_path=resolve_symlinks(buffer))) {
		strcpy(resolved_path, real_path);
		bs_to_slash(resolved_path);
		p = last_char_is(resolved_path, '/');
		if (p && p > resolved_path && p[-1] != ':')
			*p = '\0';
		return resolved_path;
	}
	return NULL;
}

#if ENABLE_FEATURE_READLINK2
static wchar_t *normalize_ntpath(wchar_t *wbuf)
{
	int i;
	/* fix absolute path prefixes */
	if (wbuf[0] == '\\') {
		/* strip NT namespace prefixes */
		if (!wcsncmp(wbuf, L"\\??\\", 4) ||
		    !wcsncmp(wbuf, L"\\\\?\\", 4))
			wbuf += 4;
		else if (!wcsnicmp(wbuf, L"\\DosDevices\\", 12))
			wbuf += 12;
		/* replace remaining '...UNC\' with '\\' */
		if (!wcsnicmp(wbuf, L"UNC\\", 4)) {
			wbuf += 2;
			*wbuf = '\\';
		}
	}
	/* convert backslashes to slashes */
	for (i = 0; wbuf[i]; i++)
		if (wbuf[i] == '\\')
			wbuf[i] = '/';
	return wbuf;
}
#endif

#if 0
#define SRPB rptr->SymbolicLinkReparseBuffer
ssize_t readlink(const char *pathname, char *buf, size_t bufsiz)
{
	HANDLE h;

	h = CreateFile(pathname, 0, 0, NULL, OPEN_EXISTING,
				FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (h != INVALID_HANDLE_VALUE) {
		DWORD nbytes;
		BYTE rbuf[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
		PREPARSE_DATA_BUFFER rptr = (PREPARSE_DATA_BUFFER)rbuf;
		BOOL status;

		status = DeviceIoControl(h, FSCTL_GET_REPARSE_POINT, NULL, 0,
					rptr, sizeof(rbuf), &nbytes, NULL);
		CloseHandle(h);

		if (status && rptr->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
			size_t len = SRPB.SubstituteNameLength/sizeof(WCHAR);
			WCHAR *name = SRPB.PathBuffer +
							SRPB.SubstituteNameOffset/sizeof(WCHAR);

			name[len] = 0;
			name = normalize_ntpath(name);
			len = wcslen(name);
			if (len > bufsiz)
				len = bufsiz;
			if (WideCharToMultiByte(CP_ACP, 0, name, len, buf, bufsiz, 0, 0)) {
				return len;
			}
		}
	}
	errno = err_win_to_posix();
	return -1;
}
#endif

const char *get_busybox_exec_path(void)
{
	static char *path = NULL;

	if (!path) {
		path = xzalloc(PATH_MAX);
	}

	if (!*path) {
		GetModuleFileName(NULL, path, PATH_MAX);
		bs_to_slash(path);
	}
	return path;
}

#undef mkdir
int mingw_mkdir(const char *path, int mode UNUSED_PARAM)
{
	int ret;
	struct stat st;
	int lerrno = 0;

	if ( (ret=mkdir(path)) < 0 ) {
		lerrno = errno;
		if ( lerrno == EACCES && stat(path, &st) == 0 ) {
			ret = 0;
			lerrno = 0;
		}
	}

	errno = lerrno;
	return ret;
}

#undef chdir
int mingw_chdir(const char *dirname)
{
	struct stat st;
	int ret = -1;
	const char *realdir = dirname;

	if (lstat(dirname, &st) == 0 && S_ISLNK(st.st_mode)) {
		realdir = auto_string(xmalloc_readlink(dirname));
		if (realdir)
			fix_path_case((char *)realdir);
	}

	if (realdir)
		ret = chdir(realdir);

	return ret;
}

#undef chmod
int mingw_chmod(const char *path, int mode)
{
	WIN32_FILE_ATTRIBUTE_DATA fdata;

	if ( get_file_attr(path, &fdata) == 0 &&
			fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
		mode |= 0222;
	}

	return chmod(path, mode);
}

int fcntl(int fd, int cmd, ...)
{
	va_list arg;
	int result = -1;
	char *fds;
	int target, i, newfd;

	va_start(arg, cmd);

	switch (cmd) {
	case F_GETFD:
	case F_SETFD:
	case F_GETFL:
		/*
		 * Our fake F_GETFL won't matter if the return value is used as
		 *    fcntl(fd, F_SETFL, ret|something);
		 * because F_SETFL isn't supported either.
		 */
		result = 0;
		break;
	case F_DUPFD:
		target = va_arg(arg, int);
		fds = xzalloc(target);
		while ((newfd = dup(fd)) < target && newfd >= 0) {
			fds[newfd] = 1;
		}
		for (i = 0; i < target; ++i) {
			if (fds[i]) {
				close(i);
			}
		}
		free(fds);
		result = newfd;
		break;
	default:
		errno = ENOSYS;
		break;
	}

	va_end(arg);
	return result;
}

#undef unlink
int mingw_unlink(const char *pathname)
{
	/* read-only files cannot be removed */
	chmod(pathname, 0666);
	return unlink(pathname);
}

#undef strftime
size_t mingw_strftime(char *buf, size_t max, const char *format, const struct tm *tm)
{
	size_t ret;
	char buffer[64];
	const char *replace;
	char *t;
	char *fmt;
	struct tm tm2;

	/*
	 * Emulate some formats that Windows' strftime lacks.
	 * - '%e' day of the month with space padding
	 * - '%s' number of seconds since the Unix epoch
	 * - '%T' same as %H:%M:%S
	 * - '%z' timezone offset
	 * Also, permit the '-' modifier to omit padding.  Windows uses '#'.
	 */
	fmt = xstrdup(format);
	for ( t=fmt; *t; ++t ) {
		if ( *t == '%' ) {
			replace = NULL;
			if ( t[1] == 'e' ) {
				if ( tm->tm_mday >= 0 && tm->tm_mday <= 99 ) {
					sprintf(buffer, "%2d", tm->tm_mday);
				}
				else {
					strcpy(buffer, "  ");
				}
				replace = buffer;
			}
			else if ( t[1] == 's' ) {
				tm2 = *tm;
				sprintf(buffer, "%d", (int)mktime(&tm2));
				replace = buffer;
			}
			else if ( t[1] == 'T' ) {
				replace = "%H:%M:%S";
			}
			else if ( t[1] == 'z' ) {
				_tzset();
				if ( tm->tm_isdst >= 0 ) {
					int offset = (int)_timezone - (tm->tm_isdst > 0 ? 3600 : 0);
					int hr, min;

					if ( offset > 0 ) {
						buffer[0] = '-';
					}
					else {
						buffer[0] = '+';
						offset = -offset;
					}

					hr = offset / 3600;
					min = (offset % 3600) / 60;
					sprintf(buffer+1, "%04d", hr*100 + min);
				}
				else {
					buffer[0] = '\0';
				}
				replace = buffer;
			}
			else if ( t[1] == '-' && t[2] != '\0' &&
						strchr("dHIjmMSUwWyY", t[2]) ) {
				/* Microsoft uses '#' rather than '-' to remove padding */
				t[1] = '#';
			}
			else if ( t[1] != '\0' ) {
				++t;
			}

			if (replace) {
				int m;
				char *newfmt;

				*t = '\0';
				m = t - fmt;
				newfmt = xasprintf("%s%s%s", fmt, replace, t+2);
				free(fmt);
				t = newfmt + m + strlen(replace);
				fmt = newfmt;
			}
		}
	}

	ret = strftime(buf, max, fmt, tm);
	free(fmt);

	return ret;
}

int stime(time_t *t UNUSED_PARAM)
{
	errno = EPERM;
	return -1;
}

#undef access
int mingw_access(const char *name, int mode)
{
	int ret;
	struct stat s;

	/* Windows can only handle test for existence, read or write */
	if (mode == F_OK || (mode & ~X_OK)) {
		ret = _access(name, mode & ~X_OK);
		if (ret < 0 || !(mode & X_OK)) {
			return ret;
		}
	}

	if (!mingw_stat(name, &s)) {
		if ((s.st_mode&S_IXUSR)) {
			return 0;
		}
		errno = EACCES;
	}

	return -1;
}

#undef rmdir
int mingw_rmdir(const char *path)
{
	/* read-only directories cannot be removed */
	chmod(path, 0666);
	return rmdir(path);
}

#define NUMEXT 5
static const char win_suffix[NUMEXT][4] = { "sh", "com", "exe", "bat", "cmd" };

static int has_win_suffix(const char *name, int start)
{
	const char *dot = strrchr(bb_basename(name), '.');
	int i;

	if (dot != NULL && strlen(dot) < 5) {
		for (i=start; i<NUMEXT; ++i) {
			if (!strcasecmp(dot+1, win_suffix[i])) {
				return 1;
			}
		}
	}
	return 0;
}

int has_bat_suffix(const char *name)
{
	return has_win_suffix(name, 3);
}

int has_exe_suffix(const char *name)
{
	return has_win_suffix(name, 0);
}

int has_exe_suffix_or_dot(const char *name)
{
	return last_char_is(name, '.') || has_win_suffix(name, 0);
}

/* Check if path can be made into an executable by adding a suffix.
 * The suffix is added to the end of the argument which must be
 * long enough to allow this.
 *
 * If the return value is TRUE the argument contains the new path,
 * if FALSE the argument is unchanged.
 */
int add_win32_extension(char *p)
{
	if (!has_exe_suffix_or_dot(p)) {
		int i, len = strlen(p);

		p[len] = '.';
		for (i=0; i<NUMEXT; ++i) {
			strcpy(p+len+1, win_suffix[i]);
			if (file_is_executable(p))
				return TRUE;
		}
		p[len] = '\0';
	}
	return FALSE;
}

/* Check if path can be made into an executable by adding a suffix.
 * Return an allocated string containing the path if it can;
 * return NULL if not.
 *
 * If path already has a suffix don't even bother trying.
 */
char *alloc_win32_extension(const char *p)
{
	if (!has_exe_suffix_or_dot(p)) {
		int len = strlen(p);
		char *path = strcpy(xmalloc(len+5), p);

		if (add_win32_extension(path))
			return path;
		free(path);
	}
	return NULL;
}

void FAST_FUNC bs_to_slash(char *p)
{
	for (; *p; ++p) {
		if ( *p == '\\' ) {
			*p = '/';
		}
	}
}

void FAST_FUNC slash_to_bs(char *p)
{
	for (; *p; ++p) {
		if ( *p == '/' ) {
			*p = '\\';
		}
	}
}

size_t FAST_FUNC remove_cr(char *p, size_t len)
{
	ssize_t i, j;

	for (i=j=0; i<len; ++i) {
		if (p[i] != '\r')
			p[j++] = p[i];
	}
	return j;
}

#undef opendir
DIR *mingw_opendir(const char *path)
{
	char name[4];

	if (isalpha(path[0]) && path[1] == ':' && path[2] == '\0') {
		strcpy(name, path);
		name[2] = '/';
		name[3] = '\0';
		path = name;
	}

	return opendir(path);
}

off_t mingw_lseek(int fd, off_t offset, int whence)
{
	HANDLE h = (HANDLE)_get_osfhandle(fd);
	if (h == INVALID_HANDLE_VALUE) {
		errno = EBADF;
		return -1;
	}
	if (GetFileType(h) != FILE_TYPE_DISK) {
		errno = ESPIPE;
		return -1;
	}
	return _lseeki64(fd, offset, whence);
}

#if ENABLE_FEATURE_PS_TIME || ENABLE_FEATURE_PS_LONG
#undef GetTickCount64

ULONGLONG CompatGetTickCount64(void)
{
	DECLARE_PROC_ADDR(ULONGLONG, GetTickCount64, void);

	if (!INIT_PROC_ADDR(kernel32.dll, GetTickCount64)) {
		return (ULONGLONG)GetTickCount();
	}

	return GetTickCount64();
}
#endif

#if ENABLE_FEATURE_INSTALLER
/*
 * Enumerate the names of all hard links to a file.  The first call
 * provides the file name as the first argument; subsequent calls must
 * set the first argument to NULL.  Returns 0 on error or when there are
 * no more links.
 */
int enumerate_links(const char *file, char *name)
{
	static HANDLE h = INVALID_HANDLE_VALUE;
	char aname[PATH_MAX];
	wchar_t wname[PATH_MAX];
	DWORD length = PATH_MAX;
	DECLARE_PROC_ADDR(HANDLE, FindFirstFileNameW, LPCWSTR, DWORD, LPDWORD,
						PWSTR);
	DECLARE_PROC_ADDR(BOOL, FindNextFileNameW, HANDLE, LPDWORD, PWSTR);

	if (!INIT_PROC_ADDR(kernel32.dll, FindFirstFileNameW) ||
			!INIT_PROC_ADDR(kernel32.dll, FindNextFileNameW))
		return 0;

	if (file != NULL) {
		wchar_t wfile[PATH_MAX];
		MultiByteToWideChar(CP_ACP, 0, file, -1, wfile, PATH_MAX);
		h = FindFirstFileNameW(wfile, 0, &length, wname);
		if (h == INVALID_HANDLE_VALUE)
			return 0;
	}
	else if (!FindNextFileNameW(h, &length, wname)) {
		FindClose(h);
		h = INVALID_HANDLE_VALUE;
		return 0;
	}
	WideCharToMultiByte(CP_ACP, 0, wname, -1, aname, PATH_MAX, NULL, NULL);
	realpath(aname, name);
	return 1;
}
#endif

#if ENABLE_ASH_NOCONSOLE
void hide_console(void)
{
	DWORD dummy;
	DECLARE_PROC_ADDR(DWORD, GetConsoleProcessList, LPDWORD, DWORD);
	DECLARE_PROC_ADDR(BOOL, ShowWindow, HWND, int);

	if (INIT_PROC_ADDR(kernel32.dll, GetConsoleProcessList) &&
			INIT_PROC_ADDR(user32.dll, ShowWindow)) {
		if (GetConsoleProcessList(&dummy, 1) == 1) {
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		}
	}
}
#endif

#define is_path_sep(x) ((x) == '/' || (x) == '\\')
#define is_unc_path(x) (is_path_sep(x[0]) && is_path_sep(x[1]))

/* Return the length of the root of a UNC path, i.e. the '//host/share'
 * component, or 0 if the path doesn't look like that. */
int unc_root_len(const char *dir)
{
	const char *s = dir + 2;
	int len;

	if (!is_unc_path(dir))
		return 0;
	len = strcspn(s, "/\\");
	if (len == 0)
		return 0;
	s += len + 1;
	len = strcspn(s, "/\\");
	if (len == 0)
		return 0;
	s += len;

	return s - dir;
}

/* Return the length of the root of a path, i.e. either the drive or
 * UNC '//host/share', or 0 if the path doesn't look like that. */
int root_len(const char *path)
{
	if (path == NULL)
		return 0;
	if (isalpha(*path) && path[1] == ':')
		return 2;
	return unc_root_len(path);
}

char *get_system_drive(void)
{
	struct passwd *pwd;
	char *drive = NULL;
	int len;

	pwd = getpwuid(0);
	if (pwd != NULL && (len=root_len(pwd->pw_dir))) {
		drive = xstrdup(pwd->pw_dir);
		drive[len] = '\0';
	}

	return drive;
}

int chdir_system_drive(void)
{
	char *sd = get_system_drive();
	int ret = -1;

	if (sd) {
		strcat(sd, "/");
		ret = chdir(sd);
	}
	free(sd);
	return ret;
}

/*
 * This function is used to make relative paths absolute before a call
 * to chdir_system_drive().  It's unlikely to be useful in other cases.
 *
 * If the argument is an absolute path or a relative path which resolves
 * to a path on the system drive return 'path'.  If it's a relative path
 * which resolves to a path that isn't on the system drive return an
 * allocated string containing the resolved path.  Die on failure,
 * which is most likely because the file doesn't exist.
 */
char *xabsolute_path(char *path)
{
	char *rpath, *sd;

	if (root_len(path) != 0)
		return path;	// absolute path
	rpath = xmalloc_realpath(path);
	if (rpath) {
		sd = auto_string(get_system_drive());
		if (sd && is_prefixed_with_case(rpath, sd)) {
			free(rpath);
			return path;	// resolved path is on system drive
		}
		return rpath;
	}
	bb_perror_msg_and_die("can't open '%s'", path);
}

char *get_drive_cwd(const char *path, char *buffer, int size)
{
	char drive[3] = { *path, ':', '\0' };
	DWORD ret;

	ret = GetFullPathName(drive, size, buffer, NULL);
	if (ret == 0 || ret > size)
		return NULL;
	bs_to_slash(buffer);
	return buffer;
}

void fix_path_case(char *path)
{
	char resolved[MAX_LONG_PATH];
	int len;

	// Canonicalise path: for physical drives this makes case match
	// what's stored on disk.  For mapped drives, not so much.
	if (realpath(path, resolved) && strcasecmp(path, resolved) == 0)
		strcpy(path, resolved);

	// make drive letter or UNC hostname uppercase
	len = root_len(path);
	if (len == 2) {
		*path = toupper(*path);
	}
	else if (len != 0) {
		for (path+=2; !is_path_sep(*path); ++path) {
			*path = toupper(*path);
		}
	}
}
