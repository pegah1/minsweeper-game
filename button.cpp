#include "button.h"
#include <assert.h>

using namespace sf;
using namespace std;

Font* button::_default_font = NULL;

Font& button::getDefaultFont() { 
	if(_default_font) return *_default_font;
	_default_font = new Font();
	assert( _default_font->loadFromFile("cheeseburger.ttf") );
	return *_default_font;
}

button::button(string const& text) : _text(text, button::getDefaultFont()) {
	_size[0] = _size[1] = 0.0f;
	_shape.setFillColor( Color(255,255,255,255) );
	_text.setColor(Color(255,255,255,255));
}

void button::update() {

	float calc_width = textWidth();
	float calc_height = textHeight();
	if(calc_width < width()) calc_width = width();
	if(calc_height < height()) calc_height = height();

	_shape.setSize( Vector2f(calc_width, calc_height) );
	
	float text_x = ( (calc_width - textWidth()) / 2.0f );
	float text_y = ( (calc_height - textHeight()) / 2.0f );

	_text.setPosition(position().x + text_x, position().y + text_y);
}

float button::width() { return _size[0]; }
float button::height() { return _size[1]; }
float button::calculatedWidth() { return _shape.getSize().x; }
float button::calculatedHeight() { return _shape.getSize().y; }
Color const& button::foregroundColor() { return _text.getColor(); }
Vector2f const& button::position() { return _shape.getPosition(); }
unsigned int button::fontSize() { return _text.getCharacterSize(); }
Texture const* button::texture() { return _shape.getTexture(); }

void button::setWidth(float value) {
	_size[0] = value;
	update();
}

void button::setHeight(float value) {
	_size[1] = value;
	update();
}

void button::setSize(float width, float height) {
	setWidth(width);
	setHeight(height);
	update();
}

void button::setForegroundColor(Color const& value) {
	_text.setColor(value);
	update();
}

void button::setPosition(Vector2f const& value) {
	_shape.setPosition(value);
	update();
}

void button::setPosition(float x, float y) {
	setPosition( Vector2f(x,y) );
}

void button::setFontSize(unsigned int value) {
	_text.setCharacterSize(value);
	update();
}

void button::setTexture(Texture* value) {
	_shape.setTexture(value);
	update();
}

float button::getTextWidth(sf::Text const& text) {
	string str = text.getString().toAnsiString();
	Font const& font = text.getFont();
	
	float ret = 0.0f;
	for(int i = 0, len = (int)str.size(); i < len; i++) {
		char ch = str[i];

		ret += (float) font.getGlyph(ch, text.getCharacterSize(), false ).advance;
	}

	return ret;
}
float button::getTextHeight(sf::Text const& text) {
	string str = text.getString().toAnsiString();
	Font const& font = text.getFont();
	
	float ret = text.getCharacterSize();

	return ret;
}

float button::textWidth() {
	return getTextWidth(_text);
}

float button::textHeight() {
	return getTextHeight(_text);
}

void button::draw(RenderTarget& target) {

	target.draw(_shape);

	target.draw(_text);
}

bool button::inside(Vector2f const& point) {

	return point.x >= position().x && point.y >= position().y
		&& point.x  <= position().x + calculatedWidth()
		&& point.y <= position().y + calculatedHeight();
	
}

bool button::inside(float x, float y) { return inside(Vector2f(x,y)); }
bool button::inside(int x, int y) { return inside((float)x, (float)y); }

string button::text() {
	return _text.getString().toAnsiString();
}

void button::setText(string const& value) {
	_text.setString(value);
	update();
}

