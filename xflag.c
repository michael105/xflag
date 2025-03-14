// license: BSD 3clause
// misc147, 2025, github.com/michael105


#define TOOL xflag
#define LICENSE BSD3clause
#define VERSION "0.1-devel-27rt"
#define GITCOMMIT 2383283754eb32345611ba33a4e6cf06690349ac

#include "tools.h"


#ifndef MLIB

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/fs.h>
#include <fcntl.h>
//#include <stropts.h>
#include <sys/ioctl.h>

int dprints( int fd, ... ){
		int ret = 0;
		const char *msg;
		va_list args;
		va_start(args,fd);
#if 1
		while( (msg=va_arg(args,char*) ) ){
			int i = write(fd,msg,strlen(msg));
			if ( i<0 )
				return(i);
			ret += i;
		}
#else
		for ( int a = va_count_c(args); a>0; a-- ){
			msg=va_arg_c(args,char*);
			int i = write(fd,msg,strlen(msg));
			if ( i<0 )
				return(i);
			ret += i;
		}
#endif

		va_end(args);
		return(ret);
}

#define prints(...) dprints(STDOUT_FILENO,__VA_ARGS__,0)
#define eprints(...) dprints(STDERR_FILENO,__VA_ARGS__,0)

#define printsl(...) prints(__VA_ARGS__,"\n")
#define eprintsl(...) prints(__VA_ARGS__,"\n")

#define ewrite(_s,_len) write(STDERR_FILENO,_s,_len)
#define ewrites(_s) ewrite(_s,sizeof(_s))
#define ewritesl(_s) ewrites(_s "\n")

#define writes(_s) write(STDOUT_FILENO,_s,sizeof(_s))
#define writesl(_s) writes(_s "\n")

#define printl() writes("\n")

#define ERRNO(_e) (errno)


#define AC_LRED "\e[31;1m"
#define AC_N "\e[37;40;0m"

typedef unsigned int uint;
typedef unsigned int uint32_t;
typedef unsigned long ulong;
typedef unsigned char uchar;

#include "macros/macrolib.h"

#include "macros/options.h"

#endif


// type of flag field in fsxattr
typedef unsigned int xflag_t;

// indicate error
#define EFLAG 0x10000000

#ifndef _options_h
#include "macros/options.h"
#endif

#define OPTIONS \
	h,,"show usage",\
	H,,"show help",\
	c,,"compact format",\
	IFEXT(l,,"long format",)\
	p,,"print projectid", \
	P,projectid,"set projectid", \
	a,addflags,"add flags", \
	s,setflags,"set given flags exclusive", \
	d,deleteflags,"delete flags", \
	D,,"remove all flags", \
	g,,"print flags (default)", \
	x,,"print flags hexadecimal", \
	o,,"print flags octal", \
	F,,"don't display filename", \
	i,,"set immutable", \
	I,,"revoke immutability"

#define INT_OPTS P
CHECK_OPTIONS;
DECLARE_SETTING;

USAGE( "path [path] ..." );

HELP("List or modify extended flags of files and directories.\n",
		R"(flags:

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
)");

MAN(R"(xflag lists or modifies extended flags of files and directories.

The flags are either displayed in long format,
or as a string, consisting of abbreviations.

list of flags

r realtime       data in realtime volume 
p prealloc       preallocated file extents 
i immutable      file cannot be modified 
a append         all writes append 
s sync           all writes synchronous 
A noatime        do not update access time 
d nodump         do not include in backups 
t rtinherit      create with rt bit set 
P projectinherit create with parents projectid 
n nosymlinks     disallow symlink creation 
e extsize        extent size allocator hint 
E extsizeinherit inherit inode extent size 
f nodefrag       do not defragment 
S filestream     use filestream allocator 
x dax            use DAX for IO 
C cowextsize     CoW extent size allocator hint, 
X hasattr        has extended attributes


)");


//#define _FLAGS	\
F(FS_XFLAG_REALTIME,r,"realtime","data in realtime volume") \
F(FS_XFLAG_PREALLOC,p,"prealloc","preallocated file extents") \
F(FS_XFLAG_IMMUTABLE,i,"immutable","file cannot be modified") \
F(FS_XFLAG_APPEND,a,"append","all writes append") \
F(FS_XFLAG_SYNC,s,"sync","all writes synchronous") \
F(FS_XFLAG_NOATIME,A,"noatime","do not update access time") \
F(FS_XFLAG_NODUMP,d,"nodump","do not include in backups") \
F(FS_XFLAG_RTINHERIT,t,"rtinherit","create with rt bit set") \
F(FS_XFLAG_PROJINHERIT,P,"projectinherit","create with parents projid") \
F(FS_XFLAG_NOSYMLINKS,n,"nosymlinks","disallow symlink creation") \
F(FS_XFLAG_EXTSIZE,e,"extsize","extent size allocator hint") \
F(FS_XFLAG_EXTSZINHERIT,E,"extsizeinherit","inherit inode extent size") \
F(FS_XFLAG_NODEFRAG,f,"nodefrag","do not defragment") \
F(FS_XFLAG_FILESTREAM,S,"filestream","use filestream allocator") \
F(FS_XFLAG_DAX,x,"dax","use DAX for IO") \
F(FS_XFLAG_COWEXTSIZE,C,"cowextsize","CoW extent size allocator hint") \
F(FS_XFLAG_HASATTR,X,"hasattr","has extended attributes") 

#define _XFLAGS	\
__XFLAG(FS_XFLAG_REALTIME,r,realtime) \
__XFLAG(FS_XFLAG_PREALLOC,p,prealloc) \
__XFLAG(FS_XFLAG_IMMUTABLE,i,immutable) \
__XFLAG(FS_XFLAG_APPEND,a,append) \
__XFLAG(FS_XFLAG_SYNC,s,sync) \
__XFLAG(FS_XFLAG_NOATIME,A,noatime) \
__XFLAG(FS_XFLAG_NODUMP,d,nodump) \
__XFLAG(FS_XFLAG_RTINHERIT,t,rtinherit) \
__XFLAG(FS_XFLAG_PROJINHERIT,P,projectinherit) \
__XFLAG(FS_XFLAG_NOSYMLINKS,n,nosymlinks) \
__XFLAG(FS_XFLAG_EXTSIZE,e,extsize) \
__XFLAG(FS_XFLAG_EXTSZINHERIT,E,extsizeinherit) \
__XFLAG(FS_XFLAG_NODEFRAG,f,nodefrag) \
__XFLAG(FS_XFLAG_FILESTREAM,S,filestream) \
__XFLAG(FS_XFLAG_DAX,x,dax) \
__XFLAG(FS_XFLAG_COWEXTSIZE,C,cowextsize) \
__XFLAG(FS_XFLAG_HASATTR,X,hasattr) 


struct flagstruct {
	xflag_t flag;
	const char letter;
	const char* shortname;
};

#define _SQ(_x) _Q(_x)[0]

// extended
#define __XFLAG(flag,letter,shortname) { flag, _SQ(letter), _Q(shortname) }, 

const struct flagstruct flagarr[] = {
	_XFLAGS
};
#undef __XFLAG

// tiny 
//#define F(a,b,c,d) a|
//const xflag_t_t flagmask = _FLAGS 0; // all possible flagbits
//#undef F
//#define F(a,b,c,d) _Q(b)
//const char flagletters[] = {_FLAGS}; // string of all letters (without ending 0)
//#undef F

#define __XFLAG(_xflag,_letter,_shortname)  _letter=_xflag,_shortname=_letter,
#define XFLAGS_ENUM enum { _XFLAGS }

// convert uint to hex.
// did have some fun with branchless experiments.
// I again prefer the in opcodes smallest solution.
static int muitohex( char* buf, uint i ){
	char *p = buf;
	//*p++ = '0'; *p++='x';
	uint digits = 1;
	for ( int a = 8; a--; ){
		ROL( 4, i );
		int b = (i&0xf);
		digits |= (-b); // get positive if b>0

		if ( digits>a ){
			if ( b > 9 ) b+= 39; //7; (7 = capitals)
			//b += ((-b+9)&0x700)>>8;
			*p++ = b + '0';
		}
	}
	*p = 0;

	return(p-buf);
}

static int muitooct( char *buf, uint i ){
	uint digits = 1; // at least one digit (0)
	char *p = buf;
	//*p++ = '0'; // prefix 0
	ROL(2,i);
	int b = (i&3);
	for ( int a = 11; a--; ){
		ROL( 3, i );
		digits |= (-b);
		if ( digits>a )
			*p++ = b + '0';
		b = (i&7);
	}
	*p = 0;

	return(p-buf);
}

// update. so. I'm a bit annoyed about myself.
// final solution below. 
// Should have thought about the smallest possible solution
// from the beginning. Those divisions are expensive in processor cycles,
// but I don't expect this to get problematic.
// I do leave the other conversions here, just for the not so possible case.
//
// writing right to left can be regarded as tribute to the chinese culture..
char* muitobase( char *buf /* size >=16 */, uint i, uint base ){
	//assert(base>4);

	char *p = buf+15; 
	*p = 0;

	do {
		*(--p) = '0' + i % base; // surprisingly, the compiler emits a single div for mod and div..
		if ( *p > '9' )
			*p += 39;
	} while ( (i = i/base) );
	
	return(p);
}


// notes: needs abstraction to be included in ls.
// -> redirect stream into buffer.
// -> prepare to redirect streams into: buffer, pipe, shared memory,
// 	socket, network encrypted, ..
//
//TODO.NOTE: streams need their callback globally.
// no need to supply the callback. (overwrites?)


int _xflag_main( uint opts, char* path, xflag_t setflags, xflag_t delflags, uint projectid ){
	# define xflag_main( opts, path, ... ) __xflag_main( opts,path, __VA_OPT__(__VA_ARGS__,) 0,0,0 )
	# define __xflag_main( opts, path, setflags, delflags, projectid, ... ) _xflag_main( opts,path, setflags, delflags, projectid )

	int fd,ret=0;
	# define LEN (sizeof(flagarr)/sizeof(struct flagstruct))

	if ( (fd = open( path, O_RDONLY ) )<1 )
		return( -ERRNO(fd) );

	struct fsxattr fsx;
	bzero( &fsx, sizeof(fsx) );

	// read fsxattr
	if ( ( ret = ioctl(fd, FS_IOC_FSGETXATTR, &fsx)) < 0 ) {
		eprintsl("Cannot read attributes: ", path);
		close(fd);
		return(-ERRNO(ret));
	}

	if ( OPT(a|s|d|D|P|i|I) ){ // modify
	
		if ( OPT(s) )
			fsx.fsx_xflags = setflags;
		if ( OPT(a) )
			fsx.fsx_xflags |= setflags;
		if ( OPT(d) )
			fsx.fsx_xflags &= ~delflags;
		if ( OPT(D) )
			fsx.fsx_xflags = 0;
		if ( OPT(P) )
			fsx.fsx_projid = projectid;
		if ( OPT(I) )
			fsx.fsx_xflags &= ~FS_XFLAG_IMMUTABLE;
		if ( OPT(i) )
			fsx.fsx_xflags |= FS_XFLAG_IMMUTABLE;

		// write
		if ( ( ret = ioctl(fd, FS_IOC_FSSETXATTR, &fsx)) < 0 ) {
			eprintsl("Cannot write attributes: ", path);
			close(fd);
			return(-ERRNO(ret));
		}

		if ( OPT(g) ){ // reread xflags
			if ( ( ret = ioctl(fd, FS_IOC_FSGETXATTR, &fsx)) < 0 ) {
				eprintsl("Cannot read attributes: ", path);
				close(fd);
				return(-ERRNO(ret));
			}
		}
	}

	close(fd);

	if ( !OPT(g|p|o|x|l|c) && OPT(a|s|d|D|P|i|I) )
		return(ret);
	
	// list flags
	char buf[LEN+2]; // 
						 
	if ( OPT(p) ){
		//muitooct( buf, fsx.fsx_xflags );
		//prints( buf,"\t" ); 
		prints(muitobase(buf,fsx.fsx_projid,10),"\t");
	} 

	if ( OPT(o) ){
		//muitooct( buf, fsx.fsx_xflags );
		//prints( buf,"\t" ); 
		prints(muitobase(buf,fsx.fsx_xflags,8),"\t");
	} 
	if ( OPT(x) ){
		//muitohex( buf, fsx.fsx_xflags );
		//prints( buf,"\t" ); 
		prints(muitobase(buf,fsx.fsx_xflags,16),"\t");
	} 

	IFEXT( if ( OPT(l) )
		if ( !OPT(F) ) prints(path,"\t");)


	if ( !(OPT(o|x) && !OPT(l) ) ){ // write flag letters
		if ( !OPT(c) ){
			memset(buf,'-',LEN);
		}

		int b = 0, w = 0;
		for ( int a = 0; fsx.fsx_xflags >= flagarr[a].flag && a<LEN; a++ ){
			if ( !OPT(c) )
				b=a;
			if ( fsx.fsx_xflags & flagarr[a].flag ){
				IFEXT( if ( OPT(l) ){
						w = prints( (w?",":OPT(F)?"":"\t"), flagarr[a].shortname );
						} else )
					buf[b] = flagarr[a].letter;
				b++;
			} 
		}

		if ( OPT(c) )
			buf[b] = 0;
		else
			buf[LEN] = 0;
	}

	IFEXT( if ( !OPT(l) ){ )
		if ( !OPT(o|x) ) prints( buf ); 
		if ( !OPT(F) ){
			if ( !OPT(x|o) ) prints( "\t" ); 
			prints (path );
		}
	IFEXT( } ) 

	printl();

	return(ret);
	# undef LEN
}


// convert a string of flag chars to xflag_t
xflag_t stringtoflags( const char* s ){
	xflag_t flags = 0;
	
	for ( const char *p = s; *p; p++ ){
		const struct flagstruct* f = flagarr; 
		for ( ; f->letter != *p; f++ ){
			if ( f == flagarr+(sizeof(flagarr)/sizeof(struct flagstruct)) ){
				char b[2] = {*p,0};
				eprintsl("Unknown flag: ", &b );
				return(EFLAG);
			}
		}
		flags |= f->flag;
	}

	return( flags );
}


MAIN{

	uint opts = 0;
	char **pargv = argv;	
	int ret, eret =0;

	if ( argc == 1 )
		usage();

	PARSEARGV( 'h': usage(), 'H': help() );

	xflag_t flags = 0, dflags = 0;
	if ( OPT(s) ){
		if ( OPT(a|d) ){
			ewrites("Cannot combine -s and -a/-d\n");
			exit(1);
		}
		flags = stringtoflags( GET(setflags) );

	} else {
		if ( OPT(a) )
			flags = stringtoflags( GET(addflags) );
		if ( OPT(d) )
			dflags = stringtoflags( GET(deleteflags) );
	}

	uint projectid = 0;
	if ( OPT(P) )
		projectid = GET(P,uint);

	if ( flags == EFLAG || dflags == EFLAG )
		exit(EINVAL);

	for ( ; *argv; argv++ ){
		if ( (ret=xflag_main( opts, *argv, flags, dflags,projectid )) < 0 ){
			eret = ERRNO(ret);
			eprintsl(AC_LRED,strerror(eret),":\t" AC_N,*argv);
		}
	}


	exit(eret);
}


MAIN_TEST{
	writesl("Test " _Q(TOOL));

	writesl("test flags");

	uint lf = 0;
	for ( int a = 0; a<sizeof(flagarr)/sizeof(struct flagstruct); a++ ){
		printf("%c - %x - %s\n",flagarr[a].letter,flagarr[a].flag,flagarr[a].shortname );
		if ( flagarr[a].flag <= lf ){
			writesl("Flag order has been changed by kernel definitions.\nFail.\n");
			exit(EINVAL);
		}
		lf = flagarr[a].flag;
	}

	#define _T(_opts) writesl( #_opts ); xflag_main(OPT_BITS(_opts),*argv);
	FOREACH(_T,,g,g+c,g+l,l+o+x);

	
	

	
	exit(0);
}


