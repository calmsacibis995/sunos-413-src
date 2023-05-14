/*	@(#)vaccess.h 1.1 92/07/30 SMI	*/

#ifndef _sys_vaccess_h
#define	_sys_vaccess_h

#define	VA_OPEN		(0)
#define	VA_CLOSE	(VA_OPEN+1)
#define	VA_RDWR		(See the next two)
#define	VA_READ		(VA_CLOSE+1)
#define	VA_WRITE	(VA_READ+1)
#define	VA_SELECT	(VA_WRITE+1)
#define	VA_GETATTR	(VA_SELECT+1)
#define	VA_SETATTR	(See the next two)
#define	VA_CHANGE	(VA_GETATTR+1)
#define	VA_TRUNC	(VA_CHANGE+1)
#define	VA_ACCESS	(VA_TRUNC+1)
#define	VA_LOOKUP	(VA_ACCESS+1)
#define	VA_CREATE	(VA_LOOKUP+1)
#define	VA_REMOVE	(VA_CREATE+1)
#define	VA_LINK		(VA_REMOVE+1)
#define	VA_RENAME	(VA_LINK+1)
#define	VA_MKDIR	(VA_RENAME+1)
#define	VA_RMDIR	(VA_MKDIR+1)
#define	VA_READDIR	(VA_RMDIR+1)
#define	VA_SYMLINK	(VA_READDIR+1)
#define	VA_READLINK	(VA_SYMLINK+1)
#define	VA_FSYNC	(VA_READLINK+1)
#define	VA_INACTIVE	(VA_FSYNC+1)
#define	VA_LOCKCTL	(VA_INACTIVE+1)
#define	VA_FID		(VA_LOCKCTL+1)
#define	VA_GETPAGE	(See the next three)
#define	VA_GETPRIVATE	(VA_FID+1)
#define	VA_GETREAD	(VA_GETPRIVATE+1)
#define	VA_GETWRITE	(VA_GETREAD+1)
#define	VA_PUTPAGE	(VA_GETWRITE+1)
#define	VA_MAP		(VA_PUTPAGE+1)
#define	VA_CNTL		(VA_MAP+1)
#define	VA_MAX		(32)

#define	VA_B_OPEN	(1<<VA_OPEN)
#define	VA_B_CLOSE	(1<<VA_CLOSE)
#define	VA_B_READ	(1<<VA_READ)
#define	VA_B_WRITE	(1<<VA_WRITE)
#define	VA_B_SELECT	(1<<VA_SELECT)
#define	VA_B_GETATTR	(1<<VA_GETATTR)
#define	VA_B_CHANGE	(1<<VA_CHANGE)
#define	VA_B_TRUNC	(1<<VA_TRUNC)
#define	VA_B_ACCESS	(1<<VA_ACCESS)
#define	VA_B_LOOKUP	(1<<VA_LOOKUP)
#define	VA_B_CREATE	(1<<VA_CREATE)
#define	VA_B_REMOVE	(1<<VA_REMOVE)
#define	VA_B_LINK	(1<<VA_LINK)
#define	VA_B_RENAME	(1<<VA_RENAME)
#define	VA_B_MKDIR	(1<<VA_MKDIR)
#define	VA_B_RMDIR	(1<<VA_RMDIR)
#define	VA_B_READDIR	(1<<VA_READDIR)
#define	VA_B_SYMLINK	(1<<VA_SYMLINK)
#define	VA_B_READLINK	(1<<VA_READLINK)
#define	VA_B_FSYNC	(1<<VA_FSYNC)
#define	VA_B_INACTIVE	(1<<VA_INACTIVE)
#define	VA_B_LOCKCTL	(1<<VA_LOCKCTL)
#define	VA_B_FID	(1<<VA_FID)
#define	VA_B_GETPRIVATE	(1<<VA_GETPRIVATE)
#define	VA_B_GETREAD	(1<<VA_GETREAD)
#define	VA_B_GETWRITE	(1<<VA_GETWRITE)
#define	VA_B_PUTPAGE	(1<<VA_PUTPAGE)
#define	VA_B_MAP	(1<<VA_MAP)
#define	VA_B_CNTL	(1<<VA_CNTL)

#define	LOCKFS_ULOCK_MASK	(0)

#define	LOCKFS_WLOCK_MASK	\
	(VA_B_CHANGE	|	VA_B_CREATE	|	VA_B_GETWRITE	| \
	VA_B_LINK	|	VA_B_MKDIR	|	VA_B_REMOVE	| \
	VA_B_RENAME	|	VA_B_RMDIR	|	VA_B_SYMLINK	| \
	VA_B_TRUNC	|	VA_B_WRITE)

#define	LOCKFS_NLOCK_MASK	(VA_B_REMOVE | VA_B_RENAME | VA_B_RMDIR)

#define	LOCKFS_DLOCK_MASK	(VA_B_REMOVE | VA_B_RMDIR)

#define	LOCKFS_HLOCK_MASK	\
	(VA_B_OPEN	|				VA_B_READ	| \
	VA_B_WRITE	|	VA_B_SELECT	|	VA_B_GETATTR	| \
	VA_B_CHANGE	|	VA_B_TRUNC	|	VA_B_ACCESS	| \
	VA_B_LOOKUP	|	VA_B_CREATE	|	VA_B_REMOVE	| \
	VA_B_LINK	|	VA_B_RENAME	|	VA_B_MKDIR	| \
	VA_B_RMDIR	|	VA_B_READDIR	|	VA_B_SYMLINK	| \
	VA_B_READLINK	|	VA_B_FSYNC	| \
	VA_B_LOCKCTL	|	VA_B_FID	|	VA_B_GETPRIVATE	| \
	VA_B_GETREAD	|	VA_B_GETWRITE	| \
	VA_B_MAP	|	VA_B_CNTL)

#endif /* !_sys_vaccess_h */
