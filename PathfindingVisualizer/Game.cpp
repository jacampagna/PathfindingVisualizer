#include "Game.h"
#include "Gameboard.h"
#include "Gui.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
gm::Game::Game()
:app(sf::VideoMode(this->window_width, this->window_height), "Pathfinding Visualizer"),
	pressed(false),
	previous_time(std::chrono::steady_clock::now()),
	algo(BFS),
	sidebar(*(new gui::Sidebar(sidebar_width, window_height, button_width, button_height))),
	board(new gb::Gameboard(sf::Vector2i(10, 16), sf::Vector2i(20, 16), algo, window_width, window_height, sidebar_width, border_width)),
	window_width(1024),
	window_height(768),
	button_width(100),
	button_height(20),
	sidebar_width(window_width - window_height),
	line_width(1),
	border_width(4),
	mousePos(app.mapPixelToCoords(sf::Mouse::getPosition(app)))
{
	app.setKeyRepeatEnabled(false);
}
void gm::Game::loop() {
	while (app.isOpen())
	{
		this->events();
		this->states();
		this->update();
		this->draw();
	}
}
void gm::Game::events() {//handles mouse events and window closing
	sf::Event event;
	while (app.pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			app.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (!this->pressed && event.mouseButton.button == sf::Mouse::Left) {
				this->pressed = true;
			}
			break;
		case sf::Event::MouseButtonReleased:
			this->pressed = false;
			break;
		case sf::Event::KeyPressed:
			break;
		}
	}
}
void gm::Game::states() {//updates states of board and sidebar and steps pathfinding algorithms
	if (this->board->getState() == BOARD_FINISHED) {//Goal found and traceback finished
		this->board->setState(BOARD_IDLE);
		this->sidebar.setState(STOPPED);
		this->sidebar.buttons_map.find(START_BTN)->second.setState(BTN_IDLE);
		this->sidebar.buttons_map.find(STOP_BTN)->second.setState(BTN_ACTIVE);
	}
	else if (this->sidebar.getState() == RESET) {//Reset board state when reset pressed
		sf::Vector2i s = board->getStart();
		sf::Vector2i g = board->getGoal();
		this->board = (new gb::Gameboard(s, g, algo, window_width, window_height, sidebar_width, border_width));
		this->board->search_setup();
		this->board->setState(BOARD_READY);
		this->sidebar.setState(READY);
	}
	else if (this->sidebar.getState() == STARTING) {//start pathfinding whenstart button pressed
		this->algo = sidebar.getAlgorithm();
		if (this->algo == BFS_BTN || this->algo == DIJ_BTN) {
			this->algo = BFS;
		}
		else {
			this->algo = ASTAR;
		}
		this->board->setState(BOARD_ON);
		this->sidebar.setState(RUNNING);
		this->board->search_setup();
		this->board->setAlgorithm(algo);
	}
	else {//step pathfinding algorithm
		//limit rate of steps
		auto current_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> dt = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - this->previous_time);
		if (dt.count() > .05) {
			if (this->sidebar.getState() == RUNNING && this->board->getState() == BOARD_ON) {//search
				this->previous_time = std::chrono::steady_clock::now();
				this->board->search_step();
			}
			else if (this->sidebar.getState() == RUNNING && board->getState() == BOARD_TRACEBACK) {//traceback
				this->previous_time = std::chrono::steady_clock::now();
				this->board->traceback_step();
			}
		}
	}
}
void gm::Game::update() {//updates board and sidebar based on mouse position/input
	mousePos = app.mapPixelToCoords(sf::Mouse::getPosition(app));
	this->board->update(mousePos);
	this->sidebar.update(mousePos, board->getExplored(), board->getPath());
}
void gm::Game::draw() {//renders sidebar and board
	app.clear();
	this->board->draw(app);
	this->sidebar.draw(app);
	app.display();
}