// default settings and common definitions
//


// tiny and extended versions

//#define TINY_TOOL
#define EXTENDED_TOOL

#ifdef EXTENDED_TOOL
#define IFEXT(...) __VA_ARGS__
#define IFTINY(...)
#endif

#ifdef TINY_TOOL
#define IFEXT(...) 
#define IFTINY(...) __VA_ARGS__
#endif

#ifndef IFEXT
#define IFEXT(...) 
#define IFTINY(...) 
#endif


#ifdef MAIN
#undef MAIN
#endif

#ifdef TEST
#define MAIN_TEST int main(int argc,char **argv, char **__envp)
#define MAIN int tmain(int argc,char **argv, char **__envp)
#else
#define MAIN int main(int argc,char **argv, char **__envp)
#define MAIN_TEST int __attribute__((unused))__main_test(int argc,char **argv, char **__envp)
#endif


# define BUFSIZE 4000


#define _ifdef_LICENSE
#define _ifdef_VERSION

#ifndef VERSION
#define VERSION "0.1-devel"
#endif

# define COPYRIGHT(_year) "(c) 2014-" #_year " misc147 " IFDEF_ELSE(LICENSE,_Q(LICENSE),"AGPL") ", www.github.com/michael105\n"

#define _TOOL _Q(TOOL)

# define USAGE(...) \
void _usage(int exc){\
	ewritesl("Usage: " _TOOL " " OPTIONS_USAGE __VA_OPT__(" " __VA_ARGS__) );\
	if ( exc>=0 ) exit(exc);\
}

# define usage(...) _usage( __VA_ARGS__ +0 )




//# define HELP(...) _HELP(__VA_ARGS__,"")
# define HELP(_helptext,...) \
void __attribute__((noreturn))help(){\
	ewrites(_Q(TOOL) " "VERSION"\n" _helptext "\n"); \
	usage(-1); \
	ewrites("\n" \
			OPTIONS_SUMMARY \
			"\n" \
			__VA_OPT__(__VA_ARGS__ "\n") \
			COPYRIGHT(2025));\
	exit(0);\
}


# define MAN(...) 


