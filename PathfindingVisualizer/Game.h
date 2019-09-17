#ifndef GAME_H
#define GAME_H
namespace gm {
	class Game{
	private:
		int window_width;
		int window_height;
		int button_width;
		int button_height;
		int sidebar_width;
		int line_width;
		int border_width;
		bool pressed;
		int algo;
		std::chrono::time_point<std::chrono::steady_clock>  previous_time;
		sf::RenderWindow app;
		gui::Sidebar sidebar;
		gb::Gameboard* board;
		sf::Vector2f mousePos;

		void events();//handles mouse events and window closing
		void states();//updates states of board and sidebar and steps pathfinding algorithms
		void update();//updates board and sidebar based on mouse position/input
		void draw();//renders sidebar and board
		
	public:
		Game();
		void loop();
	};
}
#endif GAME_H