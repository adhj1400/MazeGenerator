# MazeGenerator
A C++ maze generator for generating mazes (duh). Easy to use Windows command line tool to create mazes with specified width, height and write these to files. Files can also be checked for solutions and a secret game feature is available for the bravest of souls. IDE is Visual Studio 2017.

## Usage
```
labyrint [OPTION 1] [ARGUMENT 1] [OPTION 2] [ARGUMENT 2] ...

filename: The full filename of the file to be used.  
N: Total surface area. Odd number required.  
W: Surface width. Odd number required.  
C: Surface height. Odd number required.  

--version | -v Show program version  
--help | -h Show help menu  
--size | -s [N] Create labyrinth with size N  
--columns | -c [W] Create labyrinth with width W  
--rows | -r [H] Create labyrinth with height H  
--input | -i [filename] Use file as input  
--output | -o [filename] Use file as output  
--check | -b Check for solution  
--play | -g New Game  
```
