#pragma once

#define NOMINMAX

#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <ctime>
#include "maze.h"

Maze::Maze() 
{
	curr = std::make_pair(1, 1); // Start point
}

/**
 *	Get current location.
 */
std::pair<int, int> Maze::get_curr() 
{
	return curr;
}

/**
 *	Write to file.
 */
bool Maze::write_file(std::string filename) 
{
	std::ofstream file;

	file.open(filename); // Open file

	// Columns
	for (int x = 0; x < height; x++) 
	{
		// Rows
		for (int y = 0; y < width; y++)
		{
			file << MATRIX[x][y];
		}

		file << std::endl; // Change row
	}

	file.close(); // Close file

	return true;
}

/**
 *	Read file.
 */
bool Maze::read_file(std::string filename) 
{
	std::ifstream input;
	std::string str;
	int count = 0;

	input.open(filename);	// Open file

	if (!input.is_open()) 
	{
		std::cerr << "ERROR: File could not be opened." << std::endl;
		return false;
	}

	// Check dimensions by using the first row of the file
	while (!input.eof()) 
	{
		std::getline(input, str);

		// If empty or newline
		if (str.length() == 0) break;
		
		width = str.length(); // Set width

		count++;
		height = count; // Update height
	}

	area = height * width; // Update area

	// Check dimensions
	if (!dimension_check(width, height, area))
	{
		return false;
	}

	// Set goal
	goal = std::make_pair(height - 2, width - 2);

	// Set matrix dimensions
	MATRIX.resize(height);

	for (int x = 0; x < height; x++)
	{
		MATRIX[x].resize(width);
	}

	// Go to files begining
	input.clear();
	input.seekg(0, std::ios::beg);

	// Read to matrix
	for (int x = 0; x < height; x++) {
		std::string temp;

		std::getline(input, temp);
		char* v = new char[temp.size() + 1];
		v[temp.size()] = 0;
		memcpy(v, temp.c_str(), temp.size());

		for (size_t y = 0; y < temp.length(); y++) 
		{
			if (v[y] == PATH || v[y] == WALL || v[y] == GOAL || v[y] == PC)
			{
				MATRIX[x][y] = v[y];
			}
			else 
			{
				std::cerr << "ERROR: Input maze data characters inside file [" 
						  << filename << "] are not allowed." << std::endl;
				return false;
			}
		}

		delete v;
	}

	visited = MATRIX;
	input.close();

	return true;
}

bool Maze::create_solution(std::pair<int, int> P) 
{
	// Add point <v> to stack
	std::stack<std::pair<int, int> > minStack;
	minStack.push(P);

	while (true) 
	{
		// Take from stack
		P = minStack.top();
		minStack.pop();
		
		// If <v> is not visited
		if (visited[P.first][P.second] != VISITED) 
		{
			// Mask <v> as visited
			visited[P.first][P.second] = VISITED;

			// Add non-visited neightbours to stack
			if (MATRIX[P.first - 1][P.second] ==
				PATH && visited[P.first - 1][P.second] !=
				VISITED && P.first - 1 > 0)
			{
				minStack.push(std::make_pair(P.first - 1, P.second));
			}
			if (MATRIX[P.first + 1][P.second] ==
				PATH && visited[P.first + 1][P.second] !=
				VISITED && P.first + 1 < height)
			{
				minStack.push(std::make_pair(P.first + 1, P.second));
			}
			if (MATRIX[P.first][P.second - 1] ==
				PATH && visited[P.first][P.second - 1] !=
				VISITED && P.second - 1 > 0)
			{
				minStack.push(std::make_pair(P.first, P.second - 1));
			}
			if (MATRIX[P.first][P.second + 1] ==
				PATH && visited[P.first][P.second + 1] !=
				VISITED && P.second + 1 < width)
			{
				minStack.push(std::make_pair(P.first, P.second + 1));
			}
		}

		// Om lösning hittades.
		if (P == goal)
			return false;
		if (minStack.empty())
			break;
	}
	// Om ingen lösning hittades.
	return true;
}

bool Maze::dimension_check(int W, int H, int A) 
{
	// Check if too small values
	if (A < 9 && A != INT_MAX || H < 3 && H != INT_MAX || W < 3 && W != INT_MAX) 
	{
		std::cerr << "ERROR: Dimensions too small." << std::endl;
		return false;
	}
	else if (A % 2 == 0 || H % 2 == 0 || W % 2 == 0) // Check if numbers are even
	{
		std::cerr << "ERROR: Only odd numbers are accepted." << std::endl;
		return false;
	}

	// Kontrollera sammanhang mellan bestämda dimensioner.
	if (H != INT_MAX && W != INT_MAX && A != INT_MAX) 
	{			// Om H, W och A inte behöver beräknas.
		if (H * W != A) 
		{
			std::cerr << "ERROR: Dimensions do not match." << std::endl;
			return false;
		}
	}
	else if (H == INT_MAX && W == INT_MAX && A != INT_MAX) // ...height and width
	{
		// Find closest height and width from A
		for (int x = 2; x <= sqrt(A); x++) 
		{
			for (int y = 2; y <= A / 2; y++) 
			{
				if (A == x * y)
					H = x; W = y;
			}
		}

		// ... else it must be prime ^-^
		if (H == INT_MAX) 
		{
			std::cerr << "ERROR: Prime number not allowed." << std::endl;
			return false;
		}
	}
	else if (H == INT_MAX && W != INT_MAX && A == INT_MAX) // ...height and area
	{	
		H = W; A = H * W;
	}
	else if (H != INT_MAX && W == INT_MAX && A == INT_MAX) // ...width and area
	{	
		W = H; 
		A = W * H;
	}
	else if (H != INT_MAX && W != INT_MAX && A == INT_MAX) // Calculate area
	{	
		A = H * W;
	}
	else if (H != INT_MAX && W == INT_MAX && A != INT_MAX) // Calculate width
	{
		double tempA = A, tempH = H, tempW = tempA / tempH;

		if (floor(tempW) == ceil(tempW))
		{
			W = A / H;
		}
		else 
		{
			std::cerr << "ERROR: Area and height resulted in a float point "
					  << "number.";
			return false;
		}
	}
	else if (H == INT_MAX && W != INT_MAX && A != INT_MAX) // Calculate height
	{
		double tempA = A, tempH = H, tempW = tempA / tempH;

		if (floor(tempW) == ceil(tempW)) // If not float
		{
			H = A / W;
		}
		else 
		{
			std::cerr << "ERROR: Area and width resulted in a float point "
					  << "number.";
			return false;
		}
	}

	// Set final dimansions
	width = W; 
	area = A; 
	height = H;

	// Check if dimensions are too big
	if (area > 35721 && A != INT_MAX) 
	{
		std::cerr << "ERROR: Dimensions too big. Be modest. Why would you even "
				  << "need that big of a maze??" << std::endl;
		return false;
	}

	return true;
}

bool Maze::generateMaze(int W, int H, int A) 
{
	// Check if dimensions are valid for a 2D square grid
	if (!dimension_check(W, H, A))
	{
		return false;
	}

	goal = std::make_pair(height - 2, width - 2);

	// Resize actions
	MATRIX.resize(height);

	for (int x = 0; x < height; x++)
	{
		MATRIX[x].resize(width);
	}
	
	// Fyll 2D-vector med väggar.
	for (int x = 0; x < height; x++) 
	{
		for (int y = 0; y < width; y++) 
		{
			MATRIX[x][y] = WALL;
		}
	}

	// Create "randomize" time variable
	srand(static_cast<unsigned int>(time(NULL)));

	// Use DFS recursively backwards (from goal)
	DFS(goal.first, goal.second);
	MATRIX[curr.first][curr.second] = PC;	// Start position
	MATRIX[height - 2][width - 1] = GOAL;	// End position
	
	visited = MATRIX;
	print(MATRIX);

	return true;
}

/**
 * Depth-first Search to find a neighbour two point away
 * from x and y.
 */
void Maze::DFS(int x, int y) 
{
	// Markera nuvarande punkt som giltig väg.
	MATRIX[x][y] = PATH;
	int count = 0;			// Håller reda på hur många väggar som testats
	bool flag_1 = true, flag_2 = true, flag_3 = true, flag_4 = true;
	
	// Find random neighbour to node x, y
	while (true)
	{
		// Create random seed number
		int r = rand() % 4 + 1;

		// Randomly choose a direction to travel
		if (r == rand() % 4 + 1) // Up
		{
			if (x > 2 && (x < height) && MATRIX[x - 2][y] == WALL) 
			{
				MATRIX[x - 1][y] = PATH; // Mark as path
				MATRIX[x - 2][y] = PATH;
				DFS(x - 2, y); // Recurse
			}
			if (flag_1)  // If direction already is tested
			{
				count++; 
				flag_1 = false;
			}
		}
		if (r == rand() % 4 + 1) // Down
		{
			if ((x + 2 < height) && MATRIX[x + 2][y] == WALL) 
			{
				MATRIX[x + 1][y] = PATH;
				MATRIX[x + 2][y] = PATH;
				DFS(x + 2, y);
			}
			if (flag_2)
			{
				count++;
				flag_2 = false;
			}
		}
		if (r == rand() % 4 + 1) // Left
		{
			if (y > 2 && (y < width) && MATRIX[x][y - 2] == WALL) 
			{
				MATRIX[x][y - 1] = PATH;
				MATRIX[x][y - 2] = PATH;
				DFS(x, y - 2);
			}

			if (flag_3)
			{
				count++;
				flag_3 = false;
			}
		}
		if (r == rand() % 4 + 1) // Right
		{
			if ((y + 2 < width) && MATRIX[x][y + 2] == WALL) 
			{
				MATRIX[x][y + 1] = PATH;
				MATRIX[x][y + 2] = PATH;
				DFS(x, y + 2);
			}

			if (flag_4)
			{
				count++;
				flag_4 = false;
			}
		}

		// If all directions are tested
		if (count == 4)
		{
			break;
		}
	}
}

/**
 *	Return the matrix, Neo.
 */
std::vector<std::vector<char> > Maze::get_maze() 
{
	return MATRIX;
}

/**
 *	Print the entire matrix.
 */
void Maze::print(std::vector<std::vector<char> > M) 
{
	// Columns
	for (int x = 0; x < height; x++) 
	{
		std::cout << std::endl;

		// Rows
		for (int y = 0; y < width; y++) 
		{
			std::cout << M[x][y];
		}
	}

	std::cout << std::endl;
}

/**
 * Create a new game instance. Funny stuff ensues.
 */
void Maze::new_game() 
{
	bool first = true;

	std::cout << "Help Addie find his girlfriend Subby.\nThey feel "
			  << "like as if they were worlds apart.\nWill they ever "
			  << "be reunited? \n(Enter P at any time to quit) \n\n"
			  << "Navigate with the W, A, S, D keys and press RETUR "
			  << "to confirm: ";

	// Main game loop
	while (true) 
	{
		char nav; // Navigation direction

		// Check if goal is reached
		if (check_status())
		{
			break; // User won!
		}

		if (first == false) 
		{
			// Visa labyrint och nuvarande position.
			std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
					  << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
			print(MATRIX);

			// Användaren matar in navigeringsalternativ, wasd.
			std::cout << "Direction (w, a, s, d | p = exit): ";
		}
		else
		{
			first = false;
		}

		// Await user input
		std::cin >> nav;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		nav = tolower(nav);

		// Check input
		if (!std::cin.fail() || nav == 'w' || nav == 'a' || 
			nav == 's' || nav == 'd')
		{
			navigate(nav);
		}
		else if (nav == 'p') 
		{
			std::cout << "You lost." << std::endl;
		}
	}

	std::cout << "\n\nCongratulations! You reunited Addie and Subby! Together "
			  << "they lived happily ever after (for about 60 or so more years)."
			  << "\n\n" << std::endl;
}

void Maze::navigate(char v) 
{
	if (v == 'w') // Up
	{
		// If wall exists
		if (MATRIX[curr.first - 1][curr.second] == PATH) 
		{
			// Mark new position in grid
			MATRIX[curr.first - 1][curr.second] = PC;	

			// Remove old mark
			MATRIX[curr.first][curr.second] = PATH;	  

			// Update PC position
			curr.first -= 1;
		}
	}
	else if (v == 'a') // Left
	{
		if (MATRIX[curr.first][curr.second - 1] == PATH) 
		{
			MATRIX[curr.first][curr.second - 1] = PC;
			MATRIX[curr.first][curr.second] = PATH;

			// Update y-axis value
			curr.second -= 1;
		}
	}
	else if (v == 's') // Down
	{
		if (MATRIX[curr.first + 1][curr.second] == PATH) 
		{
			MATRIX[curr.first + 1][curr.second] = PC;
			MATRIX[curr.first][curr.second] = PATH;
			curr.first += 1;
		}
	}
	else if (v == 'd') // Right
	{
		if (MATRIX[curr.first][curr.second + 1] == PATH) 
		{
			MATRIX[curr.first][curr.second + 1] = PC;
			MATRIX[curr.first][curr.second] = PATH;
			curr.second += 1;
		}
	}

	return;
}

/**
 * Return if player is at final destination.
 */
bool Maze::check_status() 
{
	return goal == curr;
}
