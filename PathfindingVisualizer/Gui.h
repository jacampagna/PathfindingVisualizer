#ifndef GUI_H
#define GUI_H
enum button_states { BTN_IDLE, BTN_HOVER, BTN_ACTIVE,BTN_DISABLE};
enum button_types {START_BTN,STOP_BTN,BFS_BTN,DIJ_BTN,ASTAR_BTN,IDA_BTN,RESET_BTN};
enum states { STARTING,RUNNING,STOPPED,RESET,READY};
namespace gui
{
	class Button
	{
	private:		
		short unsigned id;
		short unsigned buttonState;
		bool pressed;
		sf::RectangleShape shape;
		sf::Font font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;
	public:	
		Button();
		Button(float x, float y, float width, float height,
			std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outline_idle_color = sf::Color::Transparent, 
			sf::Color outline_hover_color = sf::Color::Transparent, 
			sf::Color outline_active_color = sf::Color::Transparent,
			short unsigned id = 0);
		//Getters
		short unsigned getState();
		//Setters
		void setState(int state);
		void setText(const std::string text);
		void setId(const short unsigned id);

		//Functions
		bool update(const sf::Vector2f mousePos);//Update button state and returns true if it was clicked
		void draw(sf::RenderWindow& app);
	};

	class Sidebar
	{
	private:
		int selected_algo;
		int current_state;
		sf::RectangleShape* sidebar;
		sf::Text title;
		sf::Text info;
		sf::Text explored;
		sf::Text path;
		sf::Font font;
	public:
		std::map<int,Button> buttons_map;
		Sidebar();
		~Sidebar();
		Sidebar(const int w, const int h, const int btn_w, const int btn_h);
		//Accessors
		int getAlgorithm();
		int getState();
		//Setters
		void setState(int state);
		//Functions
		void draw(sf::RenderWindow& app);
		void update(const sf::Vector2f mousePos,int explored,int path);
	};
}
#endif
