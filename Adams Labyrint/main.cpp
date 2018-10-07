/**
 *	Author: Adam Hjernquist
 *
 *	A getopt tool for generating a maze using a DFS algorithm.
 */

#ifdef _WIN32
#define NOMINMAX

#include <io.h>
#else
#include <unistd.h>
#endif
#include "wingetopt.h"
#include <stdio.h>		// for printf
#include <windows.h>
#include <vector>
#include <cstdio>
#include <string>
#include <iostream>
#include <cctype>
#include <cstdlib>
#include "version.h"
#include "maze.h"

/**
 *	<argc> defines the commands input by the user.
 */
int main(int argc, char **argv) 
{
	int		vflag = 0, hflag = 0, sflag = 0, cflag = 0, rflag = 0, 
			iflag = 0, oflag = 0, bflag = 0, gflag = 0;
	int		A = INT_MAX, H = INT_MAX, W = INT_MAX; // Labyrinth dimensions
	char*	cvalue = NULL;
	int		index, c;

	Version		ver;		// Program version
	Maze		minMaze;	// The labyrinth
	std::string file_in, file_out;
	opterr = 0;
	
	// Convert long to short commands
	argv = getopt_long(argv);

	// No arguments
	if (argc == 1)
	{
		hflag = 1;
	}
	
	while ((c = getopt(argc, argv, "vhs:c:r:i:o:bg")) != -1) 
	{
		switch (c) 
		{
		case 'v':				// Version number
			vflag = 1; break;
		case 'h':				// Help menu
			hflag = 1; break;
		case 's':				// Create labyrinth with specific size
			A = atoi(optarg);
			sflag = 1; break;
		case 'c':				// Create labyrinth with specific width
			W = atoi(optarg);
			cflag = 1; break;
		case 'r':				// Create labyrinth with specific height
			H = atoi(optarg);
			rflag = 1; break;
		case 'i':				// Use labyrinth on file as input
			file_in = optarg;
			iflag = 1; break;
		case 'o':				// Print labyrinth to file
			file_out = optarg;
			oflag = 1; break;
		case 'b':				// Check if there are solutions to labyrinth
			bflag = 1; break;
		case 'g':				// SECRET FEATURE: Start new game!
			gflag = 1; break;
		case '?':
			if (isprint(optopt)) 
			{
				if (char(optopt) == 's' || char(optopt) == 'c' ||
					char(optopt) == 'r' || char(optopt) == 'i' ||
					char(optopt) == 'o')
				{
					std::cerr << "Missing argument for option -" << char(optopt) << ".\nUse the help command -h for further instructions." << std::endl;
				}
				else
				{
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				}
			}
			else
			{
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			}

			exit(EXIT_FAILURE);
		default:
			abort();
		}
		
	}

	// If invalid commands were given
	if (sflag == 1 || cflag == 1 || rflag == 1) 
	{
		if (iflag == 1) 
		{
			std::cerr << "ERROR: Maze-generating commands -s  -c -r are not "
					  << "compatible with -i. Use --help for further "
					  << "instructions." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// Print version number
	if (vflag == 1) 
	{
		std::wstring executable = (L"labyrint.exe");

		LPCWSTR LPCexecutable = executable.c_str();
		ver.print(LPCexecutable);
	}

	// Show help menu
	if (hflag == 1) 
	{
		std::cout << std::endl 
			<< "\n************************ Help menu ************************\n"
			<< "Usage:\n"
			<< "  labyrint [OPTION 1] [ARGUMENT 1] [OPTION 2] [ARGUMENT 2] ...\n"
			<< "\nArguments:\n"
			<< "  filename\t\tThe full filename of the file to be used.\n"
			<< "  N\t\t\tTotal surface area. Odd number required.\n"
			<< "  W\t\t\tSurface width. Odd number required.\n"
			<< "  C\t\t\tSurface height. Odd number required.\n"
			<< "\nOption:\t\t\tArgument:\tDescription:\n"
			<< "  --version | -v \t\t\tShow program version\n"
			<< "  --help | -h \t\t\t\tShow help menu\n"
			<< "  --size | -s \t\t[N]\t\tCreate labyrinth with size N\n"
			<< "  --columns | -c \t[W]\t\tCreate labyrinth with width W\n"
			<< "  --rows | -r \t\t[H]\t\tCreate labyrinth with height H\n"
			<< "  --input | -i \t\t[filename]\tUse file as input\n"
			<< "  --output | -o \t[filename]\tUse file as output\n"
			<< "  --check | -b \t\t\t\tCheck for solution\n"
			<< "  --play | -g\t\t\t\tNew Game?\n"
			<< "\n***********************************************************\n";
	}

	// Create labyrinth
	if (sflag == 1 || cflag == 1 || rflag == 1) 
	{
		if (!minMaze.generateMaze(W, H, A)) 
		{
			exit(EXIT_FAILURE);
		}
	}

	// Read labyrinth on file
	if (iflag == 1) 
	{
		if (!minMaze.read_file(file_in))
		{
			exit(EXIT_FAILURE);
		}
		else
		{
			minMaze.print(minMaze.get_maze());
		}
	}

	// Print current labyrinth to file
	if (oflag == 1) 
	{
		// Felcheck
		if (sflag == 0 && rflag == 0 && cflag == 0 && iflag == 0) 
		{
			std::cerr << "ERROR: Argument -o cannot called be alone" << std::endl;
			exit(EXIT_FAILURE);
		}
		if (!minMaze.write_file(file_out))
		{
			exit(EXIT_FAILURE);
		}
	}

	// Check if solutions exist
	if (bflag == 1) 
	{
		if (sflag == 1 || cflag == 1 || rflag == 1 || iflag == 1) 
		{
			if (!minMaze.create_solution(minMaze.get_curr()))
				std::cout << "Solution found." << std::endl;
			else
				std::cout << "Solution not found." << std::endl;
		}
		else 
		{
			std::cout << "ERROR: No maze to work with." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	// Start new game
	if (gflag == 1) 
	{
		if (sflag == 1 || cflag == 1 || rflag == 1 || iflag == 1)
		{
			minMaze.new_game();
		}
		else 
		{
			std::cerr << "ERROR: No labyrinth selected. Game could not be established." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	for (index = optind; index < argc; index++)
	{
		printf("Non-option argument %s\n", argv[index]);
	}

	exit(EXIT_SUCCESS);
}
