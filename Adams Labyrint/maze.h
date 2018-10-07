
class Maze 
{
public:
	Maze();

	// Depth-first Search algorithm
	void DFS(int x, int y);

	void print(std::vector<std::vector<char> > M);

	bool Maze::dimension_check(int W, int H, int A);

	bool generateMaze(int W, int H, int A);

	bool read_file(std::string filename);

	bool create_solution(std::pair<int, int> P);

	bool check_status();

	bool write_file(std::string filename);

	std::vector<std::vector<char> > get_maze();

	void new_game();

	std::pair<int, int> get_curr();

	void navigate(char);

private:
	int	width = 0;
	int	height = 0;
	int	area = 0;

	// Visual variables
	const char WALL = 'O';
	const char PATH = ' ';
	const char PC = '+';
	const char GOAL = '-';
	const char VISITED = '*';

	std::vector<std::vector<char> > visited;
	std::vector<std::vector<char> > MATRIX;
	std::pair<int, int>				goal;
	std::pair<int, int>				curr;
};