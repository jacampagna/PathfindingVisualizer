#ifndef GAMEBOARD_H
#define GAMEBOARD_H
enum tile_states { EMPTY, WALL, VISITED, PATH, START, TARGET };
enum pressed_states { PRESSED, RELEASED,PICKUP_START,PICKUP_GOAL };
enum algorithms { BFS, DIJKSTRA, ASTAR, IDASTAR};
enum running_states { BOARD_READY, BOARD_ON, BOARD_FINISHED, BOARD_IDLE,BOARD_TRACEBACK};
enum parents {TOP,BOTTOM,LEFT,RIGHT};

namespace gb {
	class CompareDist {//Comparator on tiles for priority queue
	public:
		bool operator()(std::pair<int, sf::Vector2i> n1, std::pair<int, sf::Vector2i> n2);
	};
	class Gameboard {
	private:
		int path_length;
		int num_boxes;
		int box_width;
		int weight;
		int state;
		int explored;
		int algorithm;
		int tiles[32][32][2];
		int sidebar_width;
		std::priority_queue<std::pair<int, sf::Vector2i>, std::vector<std::pair<int, sf::Vector2i>>, CompareDist> q;
		sf::Vector2i current;
		sf::Vector3i last;
		sf::RectangleShape* tile;
		sf::Vector2i start;
		sf::Vector2i goal;
	public:
		Gameboard();
		~Gameboard();
		Gameboard(sf::Vector2i start, sf::Vector2i goal, int algorithm, const int window_width, const int window_height, const int sidebar_width, const int border_width);
		//Getters
		int getAlgorithm();
		int getState();
		sf::Vector2i getStart();
		sf::Vector2i getGoal();
		int getExplored();
		int getPath();
		//Setters
		void setState(int state);
		void setAlgorithm(int algorithm);
		void setStart(sf::Vector2i start);
		void setGoal(sf::Vector2i goal);
		//functions
		void draw(sf::RenderWindow& app);
		int dist(sf::Vector2i v1, sf::Vector2i v2);//Manhattan distance between 2 nodes
		void search_setup(); //initialize priority queue for search algorithms
		void search_step(); //explore 1 node and visit its neighbors
		void traceback_step(); //traceback 1 node
		void update(sf::Vector2f mousePos);
	};



}



#endif GAMEBOARD_H