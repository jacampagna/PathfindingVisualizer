
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include "Gui.h"
#include <iterator> 
#include <map> 
#include <string>  

//BUTTON ==============================================================================================
gui::Button::Button() {
}
gui::Button::Button(float x, float y, float width, float height,
	std::string text, unsigned character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color,
	short unsigned id)
{	
	this->pressed = false;
	this->buttonState = BTN_IDLE;
	this->id = id;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_color);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_idle_color);

	if (!this->font.loadFromFile("open-sans/OpenSans-Regular.ttf"))
	{
		std::cout << "font not found";
	}

	this->text.setFont(this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y
	);
	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;
	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->activeColor = active_color;
	this->outlineIdleColor = outline_idle_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlineActiveColor = outline_active_color;
}

//Getters
short unsigned gui::Button::getState() {
	return this->buttonState;
}
//Setters
void gui::Button::setText(const std::string text)
{
	this->text.setString(text);
}
void gui::Button::setState(int state) {
	this->buttonState = state;
}
void gui::Button::setId(const short unsigned id)
{
	this->id = id;
}
//Functions
//Update button state and returns true if it was clicked
bool gui::Button::update(const sf::Vector2f mousePos) {
	bool activated = false;
	if (this->buttonState != BTN_ACTIVE) {
		if (this->shape.getGlobalBounds().contains(mousePos)) {
			this->buttonState = BTN_HOVER;
			if (!pressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				pressed = true;
				this->buttonState = BTN_ACTIVE;
				activated = true;
			}
		}else {
			this->buttonState = BTN_IDLE;
		}
	}
	else if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>((mousePos)))) {
		if (!pressed && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			pressed = true;
			this->buttonState = BTN_HOVER;
		}
	}
	if (this->buttonState == BTN_HOVER) {
		this->shape.setFillColor(this->hoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		this->text.setFillColor(this->textHoverColor);
	}
	else if (this->buttonState == BTN_ACTIVE) {
		this->shape.setFillColor(this->activeColor);
		this->shape.setOutlineColor(this->outlineActiveColor);
		this->text.setFillColor(this->textActiveColor);
	}
	else {
		this->shape.setFillColor(this->idleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		this->text.setFillColor(this->textIdleColor);
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		pressed = false;
	}
	return activated;
}
void gui::Button::draw(sf::RenderWindow &app) {
	app.draw(this->shape);
	app.draw(this->text);
}

//Sidebar =================================================================================
gui::Sidebar::Sidebar() {}
gui::Sidebar::~Sidebar() {
	delete this->sidebar;
}
gui::Sidebar::Sidebar(const int w, const int h, const int btn_w, const int btn_h) {
	this->selected_algo = BFS_BTN;
	this->current_state = READY;

	if (!this->font.loadFromFile("open-sans/OpenSans-Regular.ttf"))
	{
		std::cout << "font not found";
	}
	//sidebar formatting
	this->title.setString("Pathfinding \n  Visualizer");
	this->title.setFont(this->font);
	this->title.setCharacterSize(30);
	this->title.setFillColor(sf::Color::Black);
	this->title.setPosition(40, 40);
	this->title.setStyle(1);

	this->info.setFont(this->font);
	this->info.setString("\t\t\t Drag start and goal \n\t\t\tnodes to move them.\n\nClick or drag nodes to create walls.\n\n\tNumber of nodes explored: \n\t\t\t\t\t\t\t Path Length: ");
	this->info.setCharacterSize(12);
	this->info.setFillColor(sf::Color::Black);
	this->info.setPosition(10.0f, 400.0f);
	this->info.setStyle(1);

	this->explored.setFont(this->font);
	this->explored.setString("0");
	this->explored.setCharacterSize(12);
	this->explored.setFillColor(sf::Color::Blue);
	this->explored.setPosition(212.0f, 480.5f);
	this->explored.setStyle(1);

	this->path.setFont(this->font);
	this->path.setString("0");
	this->path.setCharacterSize(12);
	this->path.setFillColor(sf::Color(255, 50, 0));
	this->path.setPosition(212.0f, 497.0f);
	this->path.setStyle(1);

	this->sidebar = new sf::RectangleShape(sf::Vector2f(w, h));
	this->sidebar->setFillColor(sf::Color(200, 200, 180));
	//add all sidebar buttons
	this->buttons_map.insert(std::pair<int,gui::Button>(START_BTN,*new Button(20.0f, 170.0f, (float)btn_w, (float)btn_h,
		"Start", 13,
		sf::Color::White, sf::Color::White, sf::Color(80, 80, 80),
		sf::Color(0, 60, 6), sf::Color(0,90,20), sf::Color(0, 40, 6),
		sf::Color::Black, sf::Color::White, sf::Color::Black,
		START_BTN)));
	this->buttons_map.insert(std::pair<int, gui::Button>(STOP_BTN, *new Button(140.0f, 170.0f, (float)btn_w, (float)btn_h,
		" Stop ", 13,
		sf::Color::White, sf::Color::White, sf::Color(80,80,80),
		sf::Color(60,5,5), sf::Color(90,10,10), sf::Color(40,5,5),
		sf::Color::Black, sf::Color::White, sf::Color::Black,
		STOP_BTN)));
	this->buttons_map.insert(std::pair<int, gui::Button>(RESET_BTN, *new Button(80.0f, 220.0f, (float)btn_w, (float)btn_h,
		"Reset", 13,
		sf::Color::White, sf::Color::White, sf::Color(80, 80, 80),
		sf::Color(60, 60, 5), sf::Color(90, 90, 10), sf::Color(60, 60, 5),
		sf::Color::Black, sf::Color::White, sf::Color::Black,
		RESET_BTN)));
	this->buttons_map.insert(std::pair<int, gui::Button>(BFS_BTN, *new Button(20.0f, 300.0f, (float)btn_w, (float)btn_h,
		"Breadth-First", 12,
		sf::Color::White, sf::Color::White, sf::Color(10,80,10),
		sf::Color(5,5,60), sf::Color(10,10,90), sf::Color(5,5,40),
		sf::Color::Black, sf::Color::White, sf::Color::Black,
		BFS_BTN)));
	this->buttons_map.insert(std::pair<int, gui::Button>(DIJ_BTN, *new Button(140.0f, 300.0f, (float)btn_w, (float)btn_h,
		"Dijkstra", 12,
		sf::Color::White, sf::Color::White, sf::Color(10, 80, 10),
		sf::Color(5, 5, 60), sf::Color(10, 10, 90), sf::Color(5, 5, 40),
		sf::Color::Black, sf::Color::White, sf::Color::Black,
		DIJ_BTN)));
	this->buttons_map.insert(std::pair<int, gui::Button>(ASTAR_BTN, *new Button(20.0f, 350.0f, (float)btn_w, (float)btn_h,
		"A-Star", 12,
		sf::Color::White, sf::Color::White, sf::Color(10, 80, 10),
		sf::Color(5, 5, 60), sf::Color(10, 10, 90), sf::Color(5, 5, 40),
		sf::Color::Black, sf::Color::White, sf::Color::Black,
		ASTAR_BTN)));
	this->buttons_map.insert(std::pair<int, gui::Button>(IDA_BTN, *new Button(140.0f, 350.0f, (float)btn_w, (float)btn_h,
		 " Best First ", 12,
		sf::Color::White, sf::Color::White, sf::Color(10, 80, 10),
		sf::Color(5, 5, 60), sf::Color(10, 10, 90), sf::Color(5, 5, 40),
		sf::Color::Black, sf::Color::White, sf::Color::Black,
		IDA_BTN)));
	this->buttons_map.find(BFS_BTN)->second.setState(BTN_ACTIVE);//BFS is default
}

//Getters
int gui::Sidebar::getAlgorithm() {
	return this->selected_algo;
}
int gui::Sidebar::getState() {
	return this->current_state;
}
//Setters
void gui::Sidebar::setState(int state) {
	this->current_state = state;
}
//Functions
void gui::Sidebar::update(const sf::Vector2f mousePos, int explored,int path) {
	this->explored.setString(std::to_string(explored));	//track visited nodes
	this->path.setString(std::to_string(path));//track shortest path
	std::map<int, gui::Button>::iterator itr;
	//check if a button was pressed and update states
	for (itr = this->buttons_map.begin(); itr != this->buttons_map.end(); ++itr) {
		if (itr->second.update(mousePos)) {
			switch (itr->first) {
			case START_BTN:
				if (this->current_state == READY) {
					this->current_state = STARTING;
				}
				else {
					this->current_state = RUNNING;
				}	
				this->buttons_map.find(STOP_BTN)->second.setState(BTN_IDLE);
				this->buttons_map.find(RESET_BTN)->second.setState(BTN_IDLE);
				break;
			case STOP_BTN:
				this->buttons_map.find(START_BTN)->second.setState(BTN_IDLE);
				this->buttons_map.find(RESET_BTN)->second.setState(BTN_IDLE);
				if(this->current_state == RUNNING) this->current_state = STOPPED;
				if (this->current_state == RUNNING) this->current_state = STOPPED;
				break;
			case RESET_BTN:
				this->buttons_map.find(START_BTN)->second.setState(BTN_IDLE);
				this->buttons_map.find(STOP_BTN)->second.setState(BTN_IDLE);
				this->current_state = RESET;
				break;
			default:
				selected_algo = itr->first;
				break;
			}
		}
		if (itr->first != selected_algo && itr->first != START_BTN && itr->first != STOP_BTN && itr->first != RESET_BTN){
			this->buttons_map.find(itr->first)->second.setState(BTN_IDLE);
		}
	}
}
void gui::Sidebar::draw(sf::RenderWindow& app) {
	app.draw(*this->sidebar);
	app.draw(this->title);
	app.draw(this->info);
	app.draw(this->explored);
	app.draw(this->path);
	std::map<int, gui::Button>::iterator itr;
	for (itr = buttons_map.begin(); itr != buttons_map.end(); ++itr) {
		itr->second.draw(app);
	}
}

