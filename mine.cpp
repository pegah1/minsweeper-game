#include "mine.h"
#include <assert.h>
using namespace sf;
using namespace std;

sf::Texture* mine::_flag_texture = NULL;
sf::Texture* mine::_mine_normal;
sf::Texture* mine::_mine_flat;

void mine::init(int value) {
	_value = value;
	_state = mine::CLOSED;

	if(!_mine_normal) {
		_mine_normal = new Texture();
		assert( _mine_normal->loadFromFile("mine-normal.png") );
	}

	if(!_mine_flat) {
		_mine_flat = new Texture();
		assert( _mine_flat->loadFromFile("mine-flat.png") );
	}

	if(!_flag_texture) {
		_flag_texture = new Texture();
		assert( _flag_texture->loadFromFile("flag.png") );
	}
	_flag.setTexture(*_flag_texture);

	update();
}

mine::mine(int value) {
	init(value);
}

mine::mine() {
	init(0);
}

int mine::value() { return _value; }
mine::STATE mine::state() { return _state; }
void mine::setValue(int value) { 
	_value = value; 
	update();
}
void mine::setState(mine::STATE state) { 
	_state = state; 
	update();
}

void mine::setMine() { setValue(mine::MINE); }
void mine::setEmpty() { setValue(0); }

bool mine::empty() { return _value == 0; }
bool mine::bomb() { return _value == mine::MINE; }

void mine::setFlagged() { setState(mine::FLAGGED); }
void mine::setOpened() { setState(mine::OPENED); }
void mine::setClosed() { setState(mine::CLOSED); }
bool mine::isFlagged() { return _state == mine::FLAGGED; }
bool mine::isOpened() { return _state == mine::OPENED; }
bool mine::isClosed() { return _state == mine::CLOSED; }

void mine::update() {

	if(_state == mine::FLAGGED) {

		setText("");

		float aval_x = (calculatedWidth() ) * 0.4;
		float aval_y = (calculatedHeight() ) * 0.4;

		float pos_x = ( (calculatedWidth() - aval_x) / 2.0f );
		float pos_y = ( (calculatedHeight() - aval_y) / 2.0f );

		float scale_x =  ( aval_x / _flag_texture->getWidth() );
		float scale_y = ( aval_y / _flag_texture->getHeight() );

		_flag.setPosition( position().x + pos_x, position().y + pos_y);
		_flag.setScale( scale_x, scale_y );

		setTexture(_mine_normal);

	} else if(_state == mine::OPENED) {

		if(_value > 0) {
			char buff[32];
			sprintf(buff, "%d", _value);
			setText(buff);
		} else if(_value == 0) setText("");
		else if(_value == mine::MINE) setText("X");

		switch(_value) {
		case 1: setForegroundColor( Color(0,0,255,255) ); break;
		case 2: setForegroundColor( Color(0,255,0,255) ); break;
		case 3: setForegroundColor( Color(255,0,0,255) ); break;
		case 4: setForegroundColor( Color(0,0,125,255) ); break;
		case 5: setForegroundColor( Color(125,0,0,255) ); break;
		case 6: setForegroundColor( Color(0,255,255,255) ); break;
		case 7: setForegroundColor( Color(255,0,125,255) ); break;
		case 8: setForegroundColor( Color(106,63,127,255) ); break;
		case mine::MINE: setForegroundColor( Color(255,0,0,255) ); break;
		default: break;
		}

		setTexture(_mine_flat);

	} else if(_state == mine::CLOSED) {

		setText("");

		setTexture(_mine_normal);
	}

}

void mine::draw(RenderTarget& target) {
	button::draw(target);

	if(_state == mine::FLAGGED) {
		target.draw(_flag);
	} 
}

void mine::mousePressEvent(sf::Mouse::Button button) {

	if(button == Mouse::Right) {
		if(state() == mine::FLAGGED) setState( mine::CLOSED );
		else if(_state == mine::CLOSED) setState( mine::FLAGGED );
	} else if(button == Mouse::Left && state() == mine::CLOSED) {
		setState(mine::OPENED);
	}
}
