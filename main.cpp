#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

#include "button.h"
#include "mine.h"
#include "game.h"

float width(Text const& _text) {
	string str = _text.getString().toAnsiString();
	Font const& font = _text.getFont();
	
	float ret = 0.0f;
	for(int i = 0, len = (int)str.size(); i < len; i++) {
		char ch = str[i];

		ret += (float) font.getGlyph(ch, _text.getCharacterSize(), false ).advance;
	}

	return ret;
}

int get_input(RenderWindow& app, string const& msg) {

	Text t(msg);
	Text in("_");

	while(app.isOpen()) {
		Event e;

		while(app.pollEvent(e)) {
			if(e.type == Event::Closed) app.close();
			else if(e.type == Event::KeyReleased) {
				if(e.key.code >= Keyboard::Num0 && e.key.code <= Keyboard::Num9) {
					char ch = '0' + e.key.code - Keyboard::Num0;
					in.setString( in.getString().toAnsiString().substr(0, in.getString().getSize()-1) + string(1, ch) + string(1, '_') );
				}
				else if(e.key.code == Keyboard::Back && in.getString().getSize() > 1) {
					in.setString( in.getString().toAnsiString().substr(0, in.getString().getSize()-2)  + string(1, '_') );
				}
				else if(e.key.code == Keyboard::Return) {
					int ret;
					sscanf(in.getString().toAnsiString().substr(0, in.getString().getSize()-1).c_str(),"%d",&ret);
					return ret;
				}
			}
		}

		float t_x = ( (app.getSize().x - width(t)) / 2.0f );
		float t_y = ( (app.getSize().y - 1.5*t.getCharacterSize()) / 2.0f );
		t.setPosition(t_x, t_y);

		float in_x = ( (app.getSize().x - width(in)) / 2.0f );
		float in_y = ( (app.getSize().y + 1.5*in.getCharacterSize()) / 2.0f );
		in.setPosition(in_x, in_y);

		app.clear();
		app.draw(t);
		app.draw(in);
		app.display();
	}
}

char get_level(RenderWindow& app) {

	Text t(
		"Enter game level:\n"
		"    B) Beginner(9x9 10 mines)\n"
		"    I) Intermediate(16x16 40 mines)\n"
		"    A) Advanced(16x30 99 mines)\n"
		"    C) Custom" );

	Text _t("    I) Intermediate(16x16 40 mines)\n");

	while(app.isOpen()) {
		Event e;

		while(app.pollEvent(e)) {
			if(e.type == Event::Closed) app.close();
			if(e.type == Event::KeyReleased) {
				switch(e.key.code) {
				case Keyboard::B: return 'b';
				case Keyboard::I: return 'i';
				case Keyboard::A: return 'a';
				case Keyboard::C: return 'c';
				}
			}
		}

		float t_x = ( (app.getSize().x - width(_t)) / 2.0f );
		float t_y = ( (app.getSize().y - 5.0f*t.getCharacterSize()) / 2.0f );
		t.setPosition(t_x, t_y);

		app.clear();
		app.draw(t);
		app.display();
	}

	return 'a';
}

int main() {

	//RenderWindow app( VideoMode(1366, 768), "Minesweeper", Style::Fullscreen );
	RenderWindow app( VideoMode(800, 600), "Minesweeper", Style::Close );

	int _width = 0, _height = 0, _mines = 0;

	char mode = get_level(app);
	if(mode == 'b') {
		_width = _height = 9;
		_mines = 10;
	} 
	else if(mode == 'i') {
		_width = _height = 16;
		_mines = 40;
	}
	else if(mode == 'a') {
		_width = 16;
		_height = 30;
		_mines = 99;
	} 
	else {
		_width = get_input(app, "enter width:");
		_height = get_input(app, "enter height:");
		_mines = get_input(app, "enter mines:");
	}

	

	game g(_width,_height,_mines);
	g.play(app);
	
	Text msg("Sorry ! You have lost the game!");
	msg.setCharacterSize(40);

	if(g.win()) {
		msg.setString("Yeaah ! You Win !");
	}

	float text_x = ( (app.getSize().x - width(msg)) / 2.0f );
	float text_y = ( (app.getSize().y - msg.getCharacterSize()) / 2.0f );
	msg.setPosition(text_x, text_y);

	app.draw(msg);
	app.display();

	
	while(app.isOpen()) {
		Event e;

		while(app.pollEvent(e)) {
			if(e.type == Event::Closed) app.close();
		}

	}

}