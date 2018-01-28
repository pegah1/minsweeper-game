#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <stack>
using namespace sf;
using namespace std;

game::game(int width, int height, int mines) 
	: _play_time_text("", button::getDefaultFont()) ,
	_mines_text("", button::getDefaultFont()) {
	_width = width;
	_height = height;
	_mines = mines;

	_map = new mine*[_height];
	for(int i=0; i < _height; ++i) {
		_map[i] = new mine[_width];
	}

	init_playground();
}

game::~game() {
	for(int i=0; i < _height; ++i) {
		delete [] _map[i];
	}
	delete [] _map;
}

void game::init_playground() {

	for(int i=0; i < _height; ++i) {
		for(int j=0; j < _width; ++j) {
			_map[i][j].setEmpty();
			_map[i][j].setClosed();
		}
	}

	int len = _width * _height;
	pair<int,int>* p = new pair<int,int>[len];
	for(int i=0; i < _height; ++i)
		for(int j=0; j <_width; ++j)
			p[i * _width + j] = make_pair(i, j);
	srand( time(0) );
	for(int i=0; i < len; ++i) {
		int j = i + rand() % (len - i);
		swap(p[i], p[j]);
	}
	for(int i=0; i < _mines; ++i) {
		int y = p[i].first;
		int x = p[i].second;

		_map[y][x].setMine();

		const int xmoves[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		const int ymoves[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		const int moves = 8;

		for(int move = 0; move < moves; move++) {
			int ny = y + ymoves[move];
			int nx = x + xmoves[move];
			if(nx >= 0 && ny >= 0 && nx < _width && ny < _height)
				if(!_map[ny][nx].bomb())
					_map[ny][nx].setValue( _map[ny][nx].value() + 1 );
		}
	}
	delete [] p;
}

void game::init_board(int w, int h) {
	float accel_x = (float)(w*0.8) / (float)_width;
	float accel_y = (float)(h*0.8) / (float)_height;

	for(int i=0; i < _height; ++i) {
		for(int j=0; j < _width; ++j) {
			_map[i][j].setSize(accel_x, accel_y);
			_map[i][j].setPosition( w*0.1 + accel_x * (float)j , h*0.1 + accel_y * (float)i );
			if(_height > 20) _map[i][j].setFontSize(15);
		}
	}
}

void game::render_board(sf::RenderWindow& app) {
	for(int i=0; i < _height; ++i) 
		for(int j=0; j < _width; ++j)
			_map[i][j].draw(app);
}

void game::open_neighbors(int x, int y) {
	bool** mark = new bool*[_height];
	for(int i=0; i < _height; ++i) {
		mark[i] = new bool[_width];
		memset(mark[i], 0, _width * sizeof(bool));
	}

	stack< pair<int,int> > s;
	s.push( make_pair(y,x) );
	mark[y][x] = 1;

	while(!s.empty()) {

		pair<int,int> p = s.top();
		s.pop();
		_map[p.first][p.second].setOpened();

		const int xmoves[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		const int ymoves[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		const int moves = 8;

		for(int move = 0; move < moves; move++) {
			int ny = p.first + ymoves[move];
			int nx = p.second + xmoves[move];
			if(nx >= 0 && ny >= 0 && nx < _width && ny < _height && !mark[ny][nx]) {
				if(_map[ny][nx].empty()) {
					mark[ny][nx] = 1;
					s.push( make_pair(ny,nx) );
				} else if(!_map[ny][nx].bomb()) {
					_map[ny][nx].setOpened();
				}
			}
		}

	}

	for(int i=0; i < _height; ++i) delete [] mark[i];
	delete [] mark;
}

bool game::win() { return _win; }

void game::move_mine(int x, int y) {
	int len = _width * _height;
	pair<int,int>* p = new pair<int,int>[len];
	for(int i=0; i < _height; ++i)
		for(int j=0; j <_width; ++j)
			p[i * _width + j] = make_pair(i, j);
	srand( time(0) );
	for(int i=0; i < len; ++i) {
		int j = i + rand() % (len - i);
		swap(p[i], p[j]);
	}

	int xmoves[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int ymoves[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	_map[y][x].setValue(0);
	for(int move=0; move < 8; move++) {
		int nx = x + xmoves[move];
		int ny = y + ymoves[move];
		if(nx >= 0 && ny >= 0 && nx < _width && ny < _height) {
			if(!_map[ny][nx].bomb()) _map[ny][nx].setValue( _map[ny][nx].value() - 1 );
			else _map[y][x].setValue( _map[y][x].value() + 1 );
		}
	}

	for(int i = 0; i < len; ++i) {
		int py = p[i].first;
		int px = p[i].second;
		if(!_map[py][px].bomb() && (px != x || py != y)) {
			_map[py][px].setMine();
			for(int move=0; move < 8; move++) {
				int nx = px + xmoves[move];
				int ny = py + ymoves[move];
				if(nx >= 0 && ny >= 0 && nx < _width && ny < _height) {
					if(!_map[ny][nx].bomb()) _map[ny][nx].setValue( _map[ny][nx].value() + 1 );
				}
			}
			break;
		}
	}

	delete [] p;
}

void game::drawPlayTime(sf::RenderWindow& app) {

	char buff[128];
	sprintf(buff, "Duration: %d", (int)(_play_time.getElapsedTime().asSeconds()));
	_play_time_text.setString(buff);
	_play_time_text.setPosition(10, 10);
	app.draw(_play_time_text);

}

void game::drawMineCount(sf::RenderWindow& app) {
	int flagged = 0;
	for(int i = 0; i < _height; i++)
		for(int j = 0; j < _width; j++)
			if(_map[i][j].isFlagged())
				flagged ++;
	int left = _mines - flagged;

	char buff[128];
	sprintf(buff, "Mines: %d", left);
	_mines_text.setString(buff);
	_mines_text.setPosition( app.getSize().x - 10 - button::getTextWidth(_mines_text), 10 );
	app.draw(_mines_text);
}

void game::play(RenderWindow& app) {
	_win = false;
	bool first_selected = true;
	_play_time.restart();
	
	while(app.isOpen()) {
		Event e;
		while(app.pollEvent(e)) {
			if(e.type == Event::Closed) app.close();

			if(e.type == Event::MouseButtonReleased) {
				int selected_y = -1, selected_x = -1;
				for(int i = 0; selected_x < 0 && i < _height; ++i) {
					for(int j = 0; selected_x < 0 && j < _width; ++j) {
						if(_map[i][j].inside(e.mouseButton.x, e.mouseButton.y)) {
							selected_x = j;
							selected_y = i;
						}
					}
				}
				if(selected_x >= 0 && selected_y >= 0) {
					_map[selected_y][selected_x].mousePressEvent(e.mouseButton.button);
					if(_map[selected_y][selected_x].empty() && _map[selected_y][selected_x].isOpened()) {
						open_neighbors(selected_x, selected_y);
						first_selected = false;
					}
					else if(_map[selected_y][selected_x].bomb() && _map[selected_y][selected_x].isOpened()) {

						if(first_selected) {
							move_mine(selected_x, selected_y);
							open_neighbors(selected_x, selected_y);
							first_selected = false;
						}
						else {
							for(int i = 0; i < _height; i++) {
								for(int j = 0; j < _width; j++) {
									_map[i][j].setOpened();
								}
							}

							RectangleShape grey( Vector2f(app.getSize().x, app.getSize().y) );
							grey.setFillColor(Color(0,0,0,100));
							app.clear();
							render_board(app);
							drawPlayTime(app);
							drawMineCount(app);
							app.draw(grey);
							//app.display();

							return;
						}
					}
					else {
						first_selected = false;
					}
				}
			}
		}
	
		if(iswin()) {

			for(int i = 0; i < _height; i++) {
				for(int j = 0; j < _width; j++) {
					if(!_map[i][j].isFlagged()) _map[i][j].setOpened();
				}
			}

			RectangleShape grey( Vector2f(app.getSize().x, app.getSize().y) );
			grey.setFillColor(Color(0,0,0,100));
			app.clear();
			render_board(app);
			drawPlayTime(app);
			drawMineCount(app);
			app.draw(grey);
			//app.display();

			return;
		}
		
		app.clear();

		init_board(app.getSize().x , app.getSize().y);
		render_board(app);

		drawPlayTime(app);
		drawMineCount(app);
		
		app.display();
	}
}

bool game::iswin() {
	int c = 0;
	for(int i = 0; i < _height; i++) {
		for(int j = 0; j < _width; j++) {
			if(_map[i][j].bomb() && _map[i][j].isFlagged())
				c++;
			else if(_map[i][j].isFlagged())
				return false;
		}
	}
	if(c == _mines)
		_win = true;
	return _win;
}
