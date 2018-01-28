#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

#include <string>

class button {

	sf::RectangleShape _shape;
	sf::Text _text;
	float _size[2];

	static sf::Font* _default_font;

public:

	static float getTextWidth(sf::Text const& text);
	static float getTextHeight(sf::Text const& text);

	static sf::Font& getDefaultFont();

	button(std::string const& text = "");

	float textWidth();
	float textHeight();

	float width();
	float height();
	float calculatedWidth();
	float calculatedHeight();

	sf::Color const& foregroundColor();

	sf::Vector2f const& position();

	unsigned int fontSize();
	sf::Texture const* texture();

	void setWidth(float value);
	void setHeight(float value);
	void setSize(float width, float height);

	void setForegroundColor(sf::Color const& value);

	void setPosition(sf::Vector2f const& value);
	void setPosition(float x, float y);
	void setFontSize(unsigned int value);
	void setTexture(sf::Texture* value);

	bool inside(sf::Vector2f const& point);
	bool inside(float x, float y);
	bool inside(int x, int y);

	void draw(sf::RenderTarget& target);

	std::string text();
	void setText(std::string const& value);

	void update();
};

#endif
