#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <queue>
#include "Gameboard.h"


bool gb::CompareDist::operator()(std::pair<int, sf::Vector2i> n1, std::pair<int, sf::Vector2i> n2) {
	return (n1.first > n2.first);
}
//constructors
gb::Gameboard::Gameboard() {}
gb::Gameboard::~Gameboard(){
	delete tile;
}
gb::Gameboard::Gameboard(sf::Vector2i start, sf::Vector2i goal, int algorithm,
	const int window_width, const int window_height,
	const int sidebar_width, const int border_width) {
	this->path_length = 0;
	this->explored = 0;
	this->start = start;
	this->goal = goal;
	this->weight = 0;
	this->algorithm = algorithm;
	this->num_boxes = 32;
	this->box_width = (window_height) / num_boxes;
	this->sidebar_width = sidebar_width;
	for (int i = 0; i < num_boxes; i++) {
		for (int j = 0; j < num_boxes; j++) {
			this->tiles[i][j][0] = EMPTY;
		}
	}
	//initial special tiles
	this->tiles[this->start.x][this->start.y][0] = START;
	this->tiles[this->goal.x][this->goal.y][0] = TARGET;
	//initialize a generic tile
	this->tile = new sf::RectangleShape(sf::Vector2f(box_width, box_width));
	this->tile->setOutlineThickness(-border_width / 4);
	this->tile->setOutlineColor(sf::Color(100, 100, 100));
	this->tile->setFillColor(sf::Color(20, 20, 20));
}
//Getters
int gb::Gameboard::getAlgorithm() {
	return this->algorithm;
}
int gb::Gameboard::getPath() {
	return this->path_length;
}
sf::Vector2i gb::Gameboard::getStart() {
	return this->start;
}
sf::Vector2i gb::Gameboard::getGoal() {
	return this->goal;
}
//Setters
void gb::Gameboard::setAlgorithm(int algorithm) {
	this->algorithm = algorithm;
}
void gb::Gameboard::setStart(sf::Vector2i start) {
	this->start = start;
}
void gb::Gameboard::setGoal(sf::Vector2i goal) {
	this->goal = goal;
}
//Functions
int gb::Gameboard::dist(sf::Vector2i v1, sf::Vector2i v2) {//Manhattan distance between 2 nodes
	int dist = 0;
	int dx = v1.x - v2.x;
	if (dx < 0) {
		dx = -dx;
	}
	int dy = v1.y - v2.y;
	if (dy < 0) {
		dy = -dy;
	}
	return dx + dy;
}
void gb::Gameboard::draw(sf::RenderWindow& app) {
	//Paint each tile
	for (int i = 0; i < num_boxes; i++) {
		for (int j = 0; j < num_boxes; j++) {
			switch (this->tiles[i][j][0]) {
				this->tile->setFillColor(sf::Color(150, 150, 150));
				break;
			case EMPTY:
				this->tile->setFillColor(sf::Color(40, 40, 40));
				break;
			case VISITED:
				this->tile->setFillColor(sf::Color(0, 0, 120));
				break;
			case START:
				this->tile->setFillColor(sf::Color(0, 40, 0));
				break;
			case PATH:
				this->tile->setFillColor(sf::Color(255, 50, 0));
				break;
			case TARGET:
				this->tile->setFillColor(sf::Color(40, 0, 0));
				break;
			case WALL:
				this->tile->setFillColor(sf::Color(10, 10, 10));
				break;
			}
			this->tile->setPosition(sf::Vector2f(sidebar_width + i * box_width, j*box_width));
			app.draw(*this->tile);
		}
	}
	//Dragging start/goal nodes with mouse
	if (this->last.z == PICKUP_START) {
		this->tile->setFillColor(sf::Color(0, 40, 0));
		this->tile->setPosition(app.mapPixelToCoords(sf::Mouse::getPosition(app)));
		app.draw(*this->tile);
	}
	else if (this->last.z == PICKUP_GOAL) {
		this->tile->setFillColor(sf::Color(40, 0, 0));
		this->tile->setPosition(app.mapPixelToCoords(sf::Mouse::getPosition(app)));
		app.draw(*this->tile);
	}
}
//setup priority queue for search algorithms
void gb::Gameboard::search_setup() {
	q = std::priority_queue<std::pair<int, sf::Vector2i>, std::vector<std::pair<int, sf::Vector2i>>, CompareDist>();
	if (this->algorithm == BFS || this->algorithm == DIJKSTRA) {
		this->q.push(std::pair<int, sf::Vector2i>(this->weight, this->start));
	}
	else {
		this->q.push(std::pair<int, sf::Vector2i>(this->weight, this->start));
	}
}
//explore 1 node and visit its neighbors
void gb::Gameboard::search_step() {
	if (this->q.size() > 0) {
		this->current = this->q.top().second;
		this->q.pop();
		if (this->algorithm == BFS || this->algorithm == DIJKSTRA) {
			this->weight++;
		}
		if (this->current.x > 0 && this->tiles[current.x - 1][current.y][0] != VISITED && this->tiles[current.x - 1][current.y][0] != START && this->tiles[current.x - 1][current.y][0] != WALL) {
			this->explored++;
			if (this->tiles[this->current.x - 1][this->current.y][0] == TARGET) {
				this->state = BOARD_TRACEBACK;
				return;
			}
			if (this->algorithm == ASTAR) {
				int d = dist(sf::Vector2i((this->current.x - 1), this->current.y), this->goal);
				this->weight = d;
			}
			this->tiles[(this->current.x) - 1][this->current.y][1] = RIGHT;
			this->tiles[(this->current.x) - 1][this->current.y][0] = VISITED;
			this->q.push(std::pair<int, sf::Vector2i>(this->weight, sf::Vector2i((this->current.x) - 1, this->current.y)));
		}
		if (this->current.x < this->num_boxes - 1 && this->tiles[this->current.x + 1][this->current.y][0] != VISITED && this->tiles[this->current.x + 1][this->current.y][0] != START && this->tiles[this->current.x + 1][this->current.y][0] != WALL) {
			this->explored++;
			if (this->tiles[this->current.x + 1][this->current.y][0] == TARGET) {
				this->state = BOARD_TRACEBACK;
				return;
			}
			if (this->algorithm == ASTAR) {
				int d = dist(sf::Vector2i(((this->current.x) + 1), this->current.y), this->goal);
				this->weight = d;
			}
			this->tiles[(this->current.x) + 1][this->current.y][0] = VISITED;
			this->tiles[(this->current.x) + 1][this->current.y][1] = LEFT;
			this->q.push(std::pair<int, sf::Vector2i>(this->weight, sf::Vector2i((this->current.x) + 1, this->current.y)));
		}
		if (this->current.y < this->num_boxes - 1 && this->tiles[this->current.x][(this->current.y) + 1][0] != VISITED && this->tiles[this->current.x][(this->current.y) + 1][0] != START && this->tiles[this->current.x][(this->current.y) + 1][0] != WALL) {
			this->explored++;
			if (this->tiles[this->current.x][(this->current.y) + 1][0] == TARGET) {
				this->state = BOARD_TRACEBACK;
				return;
			}
			if (this->algorithm == ASTAR) {
				int d = dist(sf::Vector2i((this->current.x), ((this->current.y) + 1)), this->goal);
				this->weight = d;
			}
			this->tiles[this->current.x][(this->current.y) + 1][0] = VISITED;
			this->tiles[this->current.x][(this->current.y)+1][1] = BOTTOM;
			this->q.push(std::pair<int, sf::Vector2i>(this->weight, sf::Vector2i(this->current.x, (this->current.y) + 1)));
		}
		if (this->current.y > 0 && this->tiles[this->current.x][(this->current.y) - 1][0] != VISITED && this->tiles[this->current.x][(this->current.y) - 1][0] != START && this->tiles[this->current.x][(this->current.y) - 1][0] != WALL) {
			this->explored++;
			if (this->tiles[this->current.x][(this->current.y) - 1][0] == TARGET) {
				this->state = BOARD_TRACEBACK;
				return;
			}
			if (this->algorithm == ASTAR) {
				int d = dist(sf::Vector2i((this->current.x), ((this->current.y) - 1)), goal);
				this->weight = d;
			}
			this->tiles[this->current.x][(this->current.y) - 1][0] = VISITED;
			this->tiles[this->current.x][(this->current.y) - 1][1] = TOP;
			this->q.push(std::pair<int, sf::Vector2i>(this->weight, sf::Vector2i(this->current.x, (this->current.y) - 1)));
		}
	}
	
}
void gb::Gameboard::traceback_step() {//traceback 1 node down the shortest path
	this->path_length++;
	int parent = this->tiles[this->current.x][this->current.y][1];
	if (this->tiles[this->current.x][this->current.y][0] == START) {
		this->state = BOARD_FINISHED;
		return;
	}
	else {
		this->tiles[this->current.x][this->current.y][0] = PATH;
	}
	switch (parent) {
	case TOP:
		this->current.y += 1;
		break;
	case BOTTOM:
		this->current.y -= 1;
		break;
	case RIGHT:
		this->current.x += 1;
		break;
	case LEFT:
		this->current.x -= 1;
		break;

	}
	
}
void gb::Gameboard::setState(int state) {
	this->state = state;
}
int gb::Gameboard::getExplored() {
	return this->explored;
}
int gb::Gameboard::getState() {
	return this->state;
}
void gb::Gameboard::update(sf::Vector2f mousePos) {
	int x = (mousePos.x - sidebar_width) / box_width;
	int y = mousePos.y / box_width;
	if (x >= 0 && x < num_boxes && y >= 0 && y < num_boxes && last.x == x && last.y == y 
		&& last.z == RELEASED && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {//
		this->last.z = PRESSED;
		if (this->tiles[x][y][0] == WALL) {
			this->tiles[x][y][0] = EMPTY;
		}
		else if (this->tiles[x][y][0] == EMPTY) {
			this->tiles[x][y][0] = WALL;
		}
		else if (this->tiles[x][y][0] == START && this->state == BOARD_READY) {//attach start to cursor
			this->tiles[x][y][0] = EMPTY;
			this->last.z = PICKUP_START;	
		}else if (this->tiles[x][y][0] == TARGET) {//attach goal to cursor
			this->tiles[x][y][0] = EMPTY;
			this->last.z = PICKUP_GOAL;		
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (last.z == PICKUP_START) {
			if(this->tiles[x][y][0] == WALL || this->tiles[x][y][0] == EMPTY){
				this->tiles[x][y][0] = START;
				this->start = sf::Vector2i(x, y);
			}else {
				this->tiles[this->start.x][this->start.y][0] = START;
			}
		}
		else if (last.z == PICKUP_GOAL) {
			if (this->tiles[x][y][0] == WALL || this->tiles[x][y][0] == EMPTY) {
				this->tiles[x][y][0] = TARGET;
				this->goal = sf::Vector2i(x, y);
			}else {
				this->tiles[this->goal.x][this->goal.y][0] = TARGET;
			}
		}
		last.z = RELEASED;
	}
	else if (last.z==PRESSED && (last.x != x || last.y != y)) {
		last.z = RELEASED;
	}
	last.x = x;
	last.y = y;
}

