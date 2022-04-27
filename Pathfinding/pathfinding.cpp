#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include <fstream>
#include <vector>

sf::RenderWindow window(sf::VideoMode(1024, 1024), "Pathfinding");

const float gridSize = 32.f;
sf::RectangleShape block(sf::Vector2f(gridSize, gridSize));
std::vector<sf::RectangleShape> blocks;

const int N = 32;

void loadMaze(int Maze[][N])
{
	std::string s;
	std::ifstream in("map.txt");
	for (size_t i = 0; i < N; i++)
	{
		in >> s;
		for (size_t j = 0; j < N; j++)
		{
			if (s[j] == '#')
				Maze[i][j] = -1;
			else
				Maze[i][j] = 0;
		}
	}
	//Maze[0][1] = -3;
	in.close();
}

void drawMaze(int Maze[][N])
{
	float pos_x = 0;
	float pos_y = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (Maze[i][j] == -1)
				block.setFillColor(sf::Color::Black);
			else if (Maze[i][j] == -2)
				block.setFillColor(sf::Color(255, 0, 0, 200));
			else if (Maze[i][j] == -3)
				block.setFillColor(sf::Color::Green);
			else
				block.setFillColor(sf::Color::White);

			block.setPosition(pos_x, pos_y);
			blocks.push_back(block);
			pos_x += block.getSize().x;
		}
		pos_y += block.getSize().y;
		pos_x = 0;
	}

	window.clear();
	for (size_t i = 0; i < blocks.size(); i++)
		window.draw(blocks[i]);
	window.display();
}

bool path(int Maze[][N], int r, int c, int x, int& r1, int& c1)
{
	Maze[r][c] = x;
	if (r == 0 && c == 1)
	{
		r1 = r;
		c1 = c;
		return true;
	}
	if (Maze[r - 1][c] == 0 && path(Maze, r - 1, c, x + 1, r1, c1))
		return true;
	if (Maze[r + 1][c] == 0 && path(Maze, r + 1, c, x + 1, r1, c1))
		return true;
	if (Maze[r][c - 1] == 0 && path(Maze, r, c - 1, x + 1, r1, c1))
		return true;
	if (Maze[r][c + 1] == 0 && path(Maze, r, c + 1, x + 1, r1, c1))
		return true;
	return false;
}

void mark(int Maze[][N], int r, int c)
{
	int x = Maze[r][c];
	Maze[r][c] = -2;
	while (x > 1)
	{
		x--;
		if (r > 0 && Maze[r - 1][c] == x)
			r--;
		else if (r < N - 1 && Maze[r + 1][c] == x)
			r++;
		else if (c > 0 && Maze[r][c - 1] == x)
			c--;
		else
			c++;
		Maze[r][c] = -2;
	}
}

int main()
{
	window.setPosition(sf::Vector2i(0, 0));

	int r, c, r1, c1;
	int Maze[N][N];
	loadMaze(Maze);
	drawMaze(Maze);
	std::cout << "Start coords: \n";
	std::cout << "r = ";
	std::cin >> r;
	std::cout << "c = ";
	std::cin >> c;
	if (path(Maze, r, c, 1, r1, c1))
	{
		mark(Maze, r1, c1);
		drawMaze(Maze);
	}
	else
		std::cout << "None \n";
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
}