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




