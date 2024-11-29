#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <tuple>
#include <thread>
#include <chrono>
#include "termcolor.hpp"

struct Node {
	int row;
	int column;

	Node() :row(0), column(0) {}
	Node(int a, int b) {
		row = a;
		column = b;
	}
	Node(const Node& node) {
		this->row = node.row;
		this->column = node.column;
	}

	bool operator<(const Node& other) const {
		return std::tie(row, column) < std::tie(other.row, other.column);
	}
};



std::string maze[8][8] = {
	{"#","#","#","O","#"," ","#"," "},
	{"#"," "," "," ","#","#","#"," "},
	{"#"," ","#"," "," "," "," ","#"},
	{"#"," "," "," "," ","#"," ","#"},
	{" "," "," "," "," "," ","#"," "},
	{"#"," ","#","#","#"," ","#"," "},
	{"#"," "," "," "," "," "," ","#"},
	{"#","#","#","X"," "," ","#"," "},


};
const std::string start = "O";
const std::string  START = "O";
const std::string END = "X";

static Node FindStartingPoint(const std::string road[][8], const std::string& start) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (road[i][j] == start) {

				return Node{ i,j };
			}

		}
	}
	return Node{ -1, -1 };
}

static void DisplayShortestRoute(const std::string road[][8], const std::vector<Node>& path) {
	std::cout << "________________________" << std::endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			bool isPath = false;
			for (const auto& node : path) {
				if (i == node.row && j == node.column) {
					std::cout << termcolor::red << "*" << " ";
					isPath = true;
					break;
				}
				
			}
			if (!isPath) {
				std::cout << termcolor::blue << road[i][j] << " ";
			}
		}
		std::cout << std::endl;
	}

}
static std::vector<Node>  FindNeighbors(const std::string road[][8], int row, int column) {
	std::vector<Node> neighbors;

	if (row > 0)
		neighbors.push_back(Node{ row - 1, column });
	if (row + 1 < 8)
		neighbors.push_back(Node{row + 1, column});
	if (column > 0)
		neighbors.push_back(Node{ row, column - 1 });
	if (column +1 < 8)
		neighbors.push_back(Node{ row, column + 1 });

	return neighbors;
}

static std::vector<Node> FindPath(const std::string road[][8]) {

	// finds the shortest possible path

	std::set<Node >visited_path;
	Node start_pos = FindStartingPoint(road, START);
	std::queue<std::pair<Node, std::vector<Node>>> q;

	std::vector<Node> path;
	path.push_back(start_pos);

	std::pair<Node, std::vector<Node>> q_pair;

	q_pair = std::make_pair(start_pos, path);

	q.emplace(q_pair);



	while (!q.empty()) {
		Node current_pos;
		std::vector<Node>road_path;

		current_pos = q.front().first;
		road_path = q.front().second;
		q.pop();


		int row, col;
		row = current_pos.row;
		col = current_pos.column;

		DisplayShortestRoute(road, road_path);

		std::this_thread::sleep_for(std::chrono::milliseconds(400));

		if (road[row][col] == END)
			return road_path;

		std::vector<Node> neighbors = FindNeighbors(road, row, col);

		for (const auto& neighbor : neighbors) {
			if (visited_path.find(neighbor) != visited_path.end()) {
				continue;
			}
			int r, c;
			r = neighbor.row;
			c = neighbor.column;

			if (road[r][c] == "#")
				continue;

			std::vector<Node> new_path = road_path;


			new_path.push_back(neighbor);

			q.emplace(neighbor, new_path);

			visited_path.insert(neighbor);

		}
	
		
	}
	

	
}

int main() {


	FindPath(maze);


	std::cin.get();
}