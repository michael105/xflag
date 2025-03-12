// license: AGPL
// misc147, 2025
#ifndef _options_h
#define _options_h



// vt100 ctrl sequence, goto column 20 in the summaries
// (insert a tab, for parsing)
#define VTPOS "\t\e[20G"
								

//-------

#define _IF_STR_OPTION(a,c,...) __VA_OPT__( a, __VA_ARGS__, c, )
#define _STR_OPTION(a,b,c) _IF_STR_OPTION(a,c,b)
#define __STRING_OPTIONS(...) _FOREACH_TRIPLE(_STR_OPTION,,__VA_ARGS__)
#define _STRING_OPTIONS __STRING_OPTIONS(OPTIONS)
//_STRING_OPTIONS

#define __EMPTY x,x
#define __M2IF_OPT(a,c,x,...) __VA_OPT__( a,,c, )
#define _M2IF_OPT(a,c,b) __M2IF_OPT(a,c,b)
#define __IF_NOTOPT(a,c,exp) _M2IF_OPT( a,c,__EMPTY##exp)
#define _SHORT_OPT(a,b,c) __IF_NOTOPT( a,c,b )
#define ___SHORT_OPTIONS(...) FOREACH_TRIPLE(_SHORT_OPT,,__VA_ARGS__)
#define __SHORT_OPTIONS(...) ___SHORT_OPTIONS(__VA_ARGS__)
#define _OPTIONS __SHORT_OPTIONS(OPTIONS)
//_OPTIONS

#define __OPT(a,b,c) a,
#define __OPTS(...) _FOREACH_TRIPLE(__OPT,,__VA_ARGS__)
#define _OPTS __OPTS(_OPTIONS)
//_OPTS

#define __STRING_OPT(a,b,c) a,
#define __STRING_OPTS(...) _FOREACH_TRIPLE(__STRING_OPT,,__VA_ARGS__)
#define _STRING_OPTS __STRING_OPTS(_STRING_OPTIONS)
//_STRING_OPTS


#define __OPTSTR(...) CONCATS( __VA_ARGS__ )
#define _OPTSTR __OPTSTR( _STRING_OPTS _OPTS )
//_OPTSTR

#define _SHORT_OPTSTR __OPTSTR( _OPTS )
# define SHORT_OPTSTR __OPTSTR( _OPTS )
//_OPTSTR


#define _ASSIGN_STRN(a,b,c) ,b=a, _##b=_##a
#define __ASSIGN_STRING_NAMES(...) _FOREACH_TRIPLE(_ASSIGN_STRN,,__VA_ARGS__)
#define _ASSIGN_STRING_NAMES __ASSIGN_STRING_NAMES(_STRING_OPTIONS)
//_LONG_ASSIGN


// build a enum of options, prepended with _ (_a,_b,..) then 
// append the optiones, shifted by it's number. (a=1<<_a,..)
#define _OPT_ENUM(_opt) _opt=(1<<_##_opt)
#define _OPTIONS_ENUM(...) enum { ANY=-1, PREP(_,__VA_ARGS__), _FOREACH_K(_OPT_ENUM,__VA_ARGS__) _ASSIGN_STRING_NAMES }
# define OPTIONS_ENUM _OPTIONS_ENUM( _STRING_OPTS _OPTS )
//OPS ENUM
//OPTIONS_ENUM


#define _ifdef_SETTINGS
# define IFDEF_SETTINGS APPEND_K(SETTINGS)

#define _SETTINGS IFDEF_LIST(SETTINGS)


#define _ASSIGN_NAME(a,b,c) b=a,
#define __ASSIGN_STRINGS(...) _FOREACH_TRIPLE(_ASSIGN_NAME,,__VA_ARGS__)
#define _ASSIGN_STRINGS __ASSIGN_STRINGS(_STRING_OPTIONS)

#define _SETTINGS_ENUM(...) enum { __VA_ARGS__ _ASSIGN_STRINGS }
# define SETTINGS_ENUM _SETTINGS_ENUM( _STRING_OPTS _SETTINGS )
//SETTINGS_ENUM

#define _M3_STR_OPT(a,b,c) "[-" #a " " #b  "] " 
#define __STRING_OPTIONS_USAGE(...) _FOREACH_TRIPLE(_M3_STR_OPT,,__VA_ARGS__)
# define STRING_OPTIONS_USAGE __STRING_OPTIONS_USAGE(_STRING_OPTIONS)
//STRING_OPTIONS_USAGE


#define __SHORT_OPTIONS_USAGE(...) __VA_OPT__( "[-"_SHORT_OPTSTR"] " )
#define _SHORT_OPTIONS_USAGE(...) __SHORT_OPTIONS_USAGE(__VA_ARGS__)
# define SHORT_OPTIONS_USAGE _SHORT_OPTIONS_USAGE(_OPTS)
//SHORT_OPTIONS_USAGE

# define OPTIONS_USAGE SHORT_OPTIONS_USAGE STRING_OPTIONS_USAGE
//OPTIONS_USAGE


#ifdef VTPOS
#define _STR_OPT_SUMMARY(a,b,c) " -"#a " " #b VTPOS c "\n"
#else
#define _STR_OPT_SUMMARY(a,b,c) " -"#a " " #b "\t" c "\n"
#endif
#define __STRING_OPTIONS_SUMMARY(...) __VA_OPT__(_FOREACH_TRIPLE(_STR_OPT_SUMMARY,,__VA_ARGS__))
# define STRING_OPTIONS_SUMMARY __STRING_OPTIONS_SUMMARY(_STRING_OPTIONS)
//STRING_OPTIONS_SUMMARY


#ifdef VTPOS
#define _SHORT_OPT_SUMMARY(a,b,c) " -"#a   VTPOS c "\n"
#else
#define _SHORT_OPT_SUMMARY(a,b,c) " -"#a "\t\t" c "\n"
#endif
#define __SHORT_OPTIONS_SUMMARY(...) _FOREACH_TRIPLE(_SHORT_OPT_SUMMARY,,__VA_ARGS__)
# define SHORT_OPTIONS_SUMMARY __SHORT_OPTIONS_SUMMARY(_OPTIONS)
//SHORT_OPTIONS_SUMMARY


#ifdef VTPOS
#define _OPT_SUMMARY(a,b,c) " -"#a " " #b  VTPOS c "\n"
#else
// trouble: inserts always two \t
#define _OPT_SUMMARY(a,b,c) " -"#a IFOPT(b,b) IFOPT(_STRING_OPTIONS,IFNOT_OPT(b,"")) "\t" c "\n"
#endif
#define __OPTIONS_SUMMARY(...) _FOREACH_TRIPLE(_OPT_SUMMARY,,__VA_ARGS__)
# define OPTIONS_SUMMARY __OPTIONS_SUMMARY(OPTIONS)
//OPTIONS_SUMMARY



// needed for IF_DEF
#define _ifdef_LONGINT_OPTS
#define _ifdef_INT_OPTS
#define _ifdef_OPTS



#define _CHECK_OPTION(a,b,c) { a, #b, c },
#define _CHECK_OPTIONS(...) enum { OPTIONS_not_checked_CHECK_OPTIONS=0 }; inline void __attribute__((always_inline))check_options(){\
	enum letters { a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z };\
	struct { enum letters opt; char* name; char* help; } options[] = { _FOREACH_TRIPLE(_CHECK_OPTION,,__VA_ARGS__) }; }

# define CHECK_OPTIONS _CHECK_OPTIONS(OPTIONS)

// GET/SET variables of the setting.
// defaults to char*, 2nd/3rd optional argument is the type:
// str,ulong,long,uint,int,char,uchar
//
// get setting variable, default char* (str)
# define GET(_name,...) ({ _GET(_name,__VA_OPT__(__VA_ARGS__,) str); })
#define _GET(_name,_type,...) ({ SETTINGS_ENUM; setting[_name].value_ ##_type; })


// set a value of the setting. Default: char* (str)
# define SET(_name,_value,...) { _SET(_name,_value,__VA_OPT__(__VA_ARGS__,) str ); }
#define _SET(_name,_value,_type,...) { SETTINGS_ENUM; setting[_name].value_ ##_type = _value; }


typedef union {
		char *value_str;
		ulong value_ulong;
		long value_long;
		uint value_uint;
		int value_int;
		char value_char;
		uchar value_uchar;
} _set_value;



#define __DECLARE_SETTING(...) __VA_OPT__( _set_value setting[ COUNT(__VA_ARGS__) + OPTIONS_not_checked_CHECK_OPTIONS ] )
#define _DECLARE_SETTING( _s ) __DECLARE_SETTING(_s)

// string opts, and setting variables
// _SETTINGS can be optionally defined, to add other settings to the setting array
# define DECLARE_SETTING _DECLARE_SETTING( _STRING_OPTS _SETTINGS  )



# define OPT(_opt) ({ OPTIONS_ENUM; opts & ( _opt ); })
# define SETOPT(_opt) ({ OPTIONS_ENUM; opts |= ( _opt ); })
# define DELOPT(_opt) ({ OPTIONS_ENUM; opts = opts & ~( _opt ); })

# define OPT_BITS(_opt) ({ OPTIONS_ENUM; _opt; })
# define OPT_NUM(_opt)  ({ OPTIONS_ENUM; _opt; })



#define __OPTION_CASE(_arg) case _arg; break;

# define PARSEARGV(...) OPTIONS_not_checked_CHECK_OPTIONS; while ( *++argv && **argv=='-' ){\
		for ( const char *o = *argv+1; *o; o++ ){\
			const char* _emsg = "Unknown option:   -",\
					*optstr = _OPTSTR, *p=optstr;\
			while ( *p && *p != *o) p++;\
			opts |= 1 << (p-optstr);\
			IF_OPT( _STRING_OPTS ,\
				if ( p-optstr < COUNT(_STRING_OPTS) ){\
					*argv++;\
					if ( !*argv ){\
						p = "";\
						_emsg = "Missing argument: -";\
					} else {\
						IF_DEF( LONGINT_OPTS, if ( OPT_BITS( LONGINT_OPTS ) & ( 1<<(p-optstr) ) ) \
							SET(p-optstr,strtol(*argv,0,0),long) else )\
						IF_DEF( INT_OPTS, if ( OPT_BITS( INT_OPTS ) & ( 1<<(p-optstr) ) ) \
							SET(p-optstr,atoi(*argv),int) else )\
						SET(p-optstr,*argv);\
					}\
				} )\
			switch (*o){\
				FOREACH(__OPTION_CASE,,__VA_ARGS__)\
				default: \
					if ( *p ) break;\
					ewrite(_emsg,20);\
					char b[2] = {*o,'\n'};\
					ewrite(b,2);\
					_usage(EINVAL); \
			}\
		}\
	}
	

# define FOREACH_ARGV(_var) for (char *_var=*argv;(_var=*argv);*argv++) 

# define IF_ARGV() if ( *argv )


// ============================

#endif
