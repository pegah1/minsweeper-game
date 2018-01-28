#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "mine.h"

class game {

	int _width;
	int _height;
	int _mines;
	bool _win;
	sf::Clock _play_time;
	sf::Text _play_time_text;
	sf::Text _mines_text;

	mine** _map;

	void init_playground();
	void init_board(int w, int h);
	void render_board(sf::RenderWindow& app);
	void open_neighbors(int x, int y);
	bool iswin();
	void move_mine(int x, int y);
	void drawPlayTime(sf::RenderWindow& app);
	void drawMineCount(sf::RenderWindow& app);

public:

	game(int width, int height, int mines);
	~game();

	bool win();

	void play(sf::RenderWindow& app);
};

#endif
