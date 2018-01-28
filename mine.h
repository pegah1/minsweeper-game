#ifndef MINE_H
#define MINE_H

#include "button.h"

class mine : public button {

public:

	enum STATE {
		FLAGGED,
		OPENED,
		CLOSED
	};

private:

	static sf::Texture* _flag_texture;
	static sf::Texture* _mine_normal;
	static sf::Texture* _mine_flat;

	static const int MINE = -1;

	int _value;
	STATE _state;
	sf::Sprite _flag;

	void init(int value);

public:

	mine(int value);
	mine();

	int value();
	STATE state();
	bool empty();
	bool bomb();

	void setValue(int value);
	void setMine();
	void setEmpty();
	void setState(STATE value);

	void setFlagged();
	void setOpened();
	void setClosed();
	bool isFlagged();
	bool isOpened();
	bool isClosed();

	void update();
	void draw(sf::RenderTarget& target);

	void mousePressEvent(sf::Mouse::Button button);
};

#endif
