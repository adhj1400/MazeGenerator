/**
 *	File:		POSIX getopt for Windows
 *	License:	AT&T Public License
 *	Modified:	Adam Hjernquist
 *
 *	Code given out at the 1985 UNIFORUM conference in Dallas.
 */

#ifndef __GNUC__

#include "wingetopt.h"
#include <cstdio>
#include <cstring>
#include <string>

#define NULL	0
#define EOF	(-1)
#define ERR(s, c) if(opterr){\
	char errbuf[2];\
	errbuf[0] = c; errbuf[1] = '\n';\
	fputs(argv[0], stderr);\
	fputs(s, stderr);\
	fputc(c, stderr);}

// Define variables
int	opterr = 1;
int	optind = 1;
int	optopt;
char* optarg;

/**
 *	Main getopt method for long commands (e.g. --help).
 */
char** getopt_long(char** argv) 
{
	// konvertera long till short-kommandon genom att iterera igenom hela argv (ett fult knep, jag vet)
	for (int x = 1; argv[x] != nullptr; x++) 
	{
		std::string temp = argv[x];

		if (temp == "--version")
			argv[x] = "-v";
		else if (temp == "--help")
			argv[x] = "-h";
		else if (temp == "--size")
			argv[x] = "-s";
		else if (temp == "--columns")
			argv[x] = "-c";
		else if (temp == "--rows")
			argv[x] = "-r";
		else if (temp == "--input")
			argv[x] = "-i";
		else if (temp == "--output")
			argv[x] = "-o";
		else if (temp == "--check")
			argv[x] = "-b";
		else if (temp == "--play")
			argv[x] = "-g";
	}
	
	return argv;
}

/**
 *	Main getopt method for short commands (e.g. -h).
 */
int getopt(int argc, char** argv, char* opts)
{
	static int sp = 1;
	register int c;
	register char* cp;

	if (sp == 1)
	{
		if (optind >= argc || 
			argv[optind][0] != '-' || 
			argv[optind][1] == '\0')
		{
			return(EOF);
		}
		else if (strcmp(argv[optind], "--") == NULL) 
		{
			optind++;
			return(EOF);
		}

		optopt = c = argv[optind][sp];

		if (c == ':' || (cp = strchr(opts, c)) == NULL) 
		{
			ERR(": illegal option -- ", c);

			if (argv[optind][++sp] == '\0') 
			{
				optind++;
				sp = 1;
			}

			return('?');
		}
		if (*++cp == ':') 
		{
			if (argv[optind][sp + 1] != '\0')
			{
				optarg = &argv[optind++][sp + 1];
			}
			else if (++optind >= argc) 
			{
				ERR(": option requires an argument -- ", c);
				sp = 1;
				return('?');
			}
			else
			{
				optarg = argv[optind++];
			}

			sp = 1;
		}
		else 
		{
			if (argv[optind][++sp] == '\0') 
			{
				sp = 1;
				optind++;
			}

			optarg = NULL;
		}

		return(c);
	}
}

#endif  /* __GNUC__ */
