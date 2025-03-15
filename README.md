### xflag


Tiny tool to list and modify linux file xflags.

xflags are the successor of extended file attributes,
I couldn't find any generic tool to work with them.

The extended file attribute interface is deprecated, 
according to the kernel headers, and furthermore
file system specific.
According to the documentation the xflag interface is implemented by
ext4, xfs, btrfs, and f2fs.


xflag and chattr/lsattr are however "compatible",
flags set with xflag will show up at the older interface,
and the other way round.


This was another missing part of my ikea environment. (github.com/palliumOS)


I did test this with f2fs only, any feedback for other filesystems
is highly welcome.



The most detailed information about the flags I could find is in the manpage of ioctl_xfs_fsgetxattr,
(https://man7.org/linux/man-pages/man2/ioctl_xfs_fsgetxattr.2.html)

xflag uses the generic linux ioctl calls FS_IOC_FSGETXATTR / FS_IOC_FSSETXATTR,
but the documented flags are the same for other filesystems (if implemented).

---

xflag -H

```
List or modify extended flags of files and directories.

Usage: xflag [-hHclpDgxoFiI] [-P projectid] [-a addflags] [-s setflags] [-d deleteflags]  path [path] ...

 -h                show usage
 -H                show help
 -c                compact format
 -l                long format
 -p                print projectid
 -P projectid      set projectid
 -a addflags       add flags
 -s setflags       set given flags exclusive
 -d deleteflags    delete flags
 -D                remove all flags
 -g                print flags (default)
 -x                print flags hexadecimal
 -o                print flags octal
 -F                don't display filename
 -i                set immutable
 -I                revoke immutabilty

flags:

a append
A noatime
C cowextsize
d nodump
e extsize
E extsizeinherit
f nodefrag
i immutable
n nosymlinks
p prealloc
P projectinherit
r realtime
S filestream
s sync
t rtinherit
x dax
X hasattr

```


Copied from the xfs manpage:

```


REALTIME
       The file is a realtime file.  This bit can only be changed
       on a file when it has no allocated extents.

PREALLOC
       The file has preallocated space.

IMMUTABLE
       The file is immutable - it cannot be modified, deleted or
       renamed, no link can be created to this file and no data
       can be written to the file.  Only the superuser or a
       process possessing the CAP_LINUX_IMMUTABLE capability can
       set or clear this flag.  If this flag is set before a
       XFS_IOC_FSSETXATTR call and would not be cleared by the
       call, then no other attributes can be changed and EPERM
       will be returned.

APPEND
       The file is append-only - it can only be opened in append
       mode for writing.  For directories, this means that files
       cannot be unlinked from this directory.  Only the superuser
       or a process possessing the CAP_LINUX_IMMUTABLE capability
       can set or clear this flag.

SYNC
       All writes to the file are synchronous.  If set on a
       directory and the /proc/sys/fs/xfs/inherit_sync tunable is
       set to 1, new files and subdirectories created in the
       directory will also have the flag set.

NOATIME
       When the file is accessed, its atime record is not
       modified.  If set on a directory and the
       /proc/sys/fs/xfs/inherit_noatime tunable is set to 1, new
       files and subdirectories created in the directory will also
       have the flag set.

NODUMP
       The file should be skipped by backup utilities.  If set on
       a directory and the /proc/sys/fs/xfs/inherit_nodump tunable
       is set to 1, new files and subdirectories created in the
       directory will also have the flag set.

RTINHERIT
       Realtime inheritance bit - new files created in the
       directory will be automatically created as realtime files.
       If set on a directory, new subdirectories created in the
       directory will also have the inheritance flag set.

PROJINHERIT
       Project inheritance bit - new files and directories created
       in this directory will inherit the project ID of this
       directory.  If set on a directory, new subdirectories
       created in the directory will also have the inheritance
       flag set.

NOSYMLINKS
       Disallows creation of symbolic links in the directory.
       This flag can only be set on a directory.  If set on a
       directory and the /proc/sys/fs/xfs/inherit_nosymlinks
       tunable is set to 1, new files and subdirectories created
       in the directory will also have the flag set.

EXTSIZE
       Extent size bit - if a basic extent size value is set on
       the file then the allocator will allocate in multiples of
       the set size for this file (see fsx_extsize below).  The
       extent size can only be changed on a file when it has no
       allocated extents.

EXTSZINHERIT
       Extent size inheritance bit - new files and directories
       created in the directory will inherit the extent size value
       (see fsx_extsize below) of the parent directory.  New
       subdirectories created in the directory will inherit the
       extent size inheritance bit.

NODEFRAG
       No defragment file bit - the file should be skipped during
       a defragmentation operation.  If set on a directory and the
       /proc/sys/fs/xfs/inherit_nodefrag tunable is set to 1, new
       files and subdirectories created in the directory will also
       have the flag set.

FILESTREAM
       Filestream allocator bit - allows a directory to reserve an
       allocation group for exclusive use by files created within
       that directory.  Files being written in other directories
       will not use the same allocation group and so files within
       different directories will not interleave extents on disk.
       The reservation is only active while files are being
       created and written into the directory.  If set on a
       directory, new files and subdirectories created in the
       directory will also have the flag set.

DAX
       If the filesystem lives on directly accessible persistent
       memory, reads and writes to this file will go straight to
       the persistent memory, bypassing the page cache.  If set on
       a directory, new files and subdirectories created in the
       directory will also have the flag set.  This flag cannot be
       set on filesystems which have the reflink feature enabled.

COWEXTSIZE
       Copy on Write Extent size bit - if a CoW extent size value
       is set on the file, the allocator will allocate extents for
       staging a copy on write operation in multiples of the set
       size for this file (see fsx_cowextsize below).  If set on a
       directory, new files and subdirectories created in the
       directory will have both the flag and the CoW extent size
       value set.

HASATTR
       The file has extended attributes associated with it.

```



---

About

I'm still about to sort a useful syntax for the os functions out.

I'm looking for something, what is as easy as a script language.

It would be possible to use scripts, this would also be 
possible with threads. (busybox does something very close, processes
started by scripts are directly spawned from the busybox binary)

Since I'm however looking for how much I'm able to shrink the core,
this obviously isn't the way to go.

Extending C via macros seems to me being another way.

My current direction aims at modifying C via default and named function arguments,
what can be done with macros.
Next step might be writing a transpiler.

c++ could be of advantage. 
On the other hand, in my experience, cpp tends to blow up.
(640kB shall be overkill, I'm still aiming for 128kB..)

Any thoughts are highly welcome.


Currently I'm redesigning strerror.
The error messages blow the binary with 4kB.
Since I don't want to use shared libraries, I need other possibilities:
1.fork, and execute another binary.
2.Load the error messages into shared memory.
3.Load another binary into memory(shared), 
	  and execute the function
		perror only. 
4.Have a shared memory db, which also serves as db for environment settings.

1 I did already implement. But I dislike it.
2 could be better. But I dislike the overhead.
3 I need eventually to resort to, independent of perror/strerror.
  printf just blows my binaries too much.
  I even don't really know why, but with this very problem every printf
  implementation seems to struggle.
4 - came into my mind, while typing.
 
Eventually I should put shared functions like printf into a shared memory db
as well.

So they can be shared alo between different namespoaces.

I dislike the idea of having 4kB of strings multiplied for each binary, as 
well as for each namespace.

Sametime I prefer statically linked binaries, which work without modifcation
at every 64bit system.

---

... so, I'm a little bit annoyed.

Did spend quite some time with base conversions, to realize finally,
the most simple solution without bit shifts but division
might be the best solution as well.

Sametime, I got some spagghetti with all those options, 
which solely set the output format.

Furthermore, I still haven't a solution for binary output,
to combine different tools via pipes without having to convert the 
values to and from ascii format again and again.

Might need to build a separated output logic, with patterns of cgi scripts.


---


I leave xflag the way it is for now, as I stated above, yet there is no other
tool, which is able to list and modify xflags exclusively.
Someone also did say, release early, release often...




misc147, 2025/03 

BSD 3clause




