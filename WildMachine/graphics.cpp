#include "graphics.h"
#include <iostream>

// Class Window
Window::Window(int width, int height, int x, int y) {
	setSize(width, height);
	setMinSize(0, 0);
	setPosition(x, y);
	setFrameThickness(1);
	setBackgroundColor(sf::Color::White);
	setFrameColor(sf::Color::Black);
	clearFrameHit();
}
Window::Window(Window& parent_window, TypeConnect type_connect,
	   int width, int height, int x, int y) {
	parent_window_ = &parent_window;
	type_connect_ = type_connect;

	setSize(width, height);
	setMinSize(0, 0);
	setPosition(x, y);
	setFrameThickness(1);
	setBackgroundColor(sf::Color::White);
	setFrameColor(sf::Color::Black);
	clearFrameHit();
}
Window::~Window() {}


void Window::setSize(int width, int height) {
	if (width < min_size_.x) width = min_size_.x;
	if (height < min_size_.y) height = min_size_.y;
	size_ = sf::Vector2f(width, height);
	frame_.setSize(size_);
}
void Window::setMinSize(int width, int height) {
	min_size_ = sf::Vector2f(width, height);
}
void Window::setPosition(int x, int y) {
	position_ = sf::Vector2f(x, y);
	frame_.setPosition(position_);
}
void Window::setFrameThickness(int thickness) {
	frame_.setOutlineThickness(thickness);
}
void Window::setIndent(int left, int right, int up, int down) {
	indent_frame_.left = left;
	indent_frame_.right = right;
	indent_frame_.up = up;
	indent_frame_.down = down;
}
void Window::setStableFrame(std::string side) {
	if (side == "left") {
		frame_resizable.left = 0;
	}
	if (side == "right") {
		frame_resizable.right = 0;
	}
	if (side == "up") {
		frame_resizable.up = 0;
	}
	if (side == "down") {
		frame_resizable.down = 0;
	}
	if (side == "all") {
		frame_resizable.left = 0;
		frame_resizable.right = 0;
		frame_resizable.up = 0;
		frame_resizable.down = 0;
	}
}

sf::Vector2f Window::getSize() {
	return size_;
}
sf::Vector2f Window::getPosition() {
	if (parent_window_ != nullptr) return frame_.getPosition();
	return position_;
}
TypeConnect Window::getTypeConnect() {
	return type_connect_;
}


void Window::setBackgroundColor(sf::Color color) {
	frame_.setFillColor(color);
}
void Window::setFrameColor(sf::Color color) {
	frame_.setOutlineColor(color);
}


bool Window::mouseInBound() {
	sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition());
	return frame_.getGlobalBounds().contains(mouse.x, mouse.y);
}
void Window::catchEvent(sf::Event& event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.key.code == sf::Mouse::Left) {
			checkOutFrameHit();
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (event.key.code == sf::Mouse::Left) {
			clearFrameHit();
		}
	default:
		break;
	}
}
void Window::draw(sf::RenderWindow& window) {
	if (parent_window_ != nullptr) {
		moveFrameDependParent();
	} else {
		moveFrame();
	}
	window.draw(frame_);
}

// For resizing frame
void Window::checkOutFrameHit() {
	sf::Vector2i mouse = sf::Mouse::getPosition();
	d_mouse_ = mouse - sf::Vector2i(position_);

	if (!frame_.getGlobalBounds().contains(mouse.x, mouse.y)) return;
	
	if (frame_resizable.left && mouse.x < position_.x) {
		resize_frame_.left = 1;
	}
	if (frame_resizable.right && mouse.x >= position_.x + size_.x) {
		resize_frame_.right = 1;
	}
	if (frame_resizable.up && mouse.y < position_.y) {
		resize_frame_.up = 1;
	}
	if (frame_resizable.down && mouse.y >= position_.y + size_.y) {
		resize_frame_.down = 1;
	}
}
void Window::clearFrameHit() {
	resize_frame_.left = resize_frame_.right =
		resize_frame_.up = resize_frame_.down = 0;
}
void Window::moveFrame() {
	sf::Vector2i mouse = sf::Mouse::getPosition();
	if (resize_frame_.left) {
		setSize(size_.x - (mouse.x - position_.x - d_mouse_.x), size_.y);
		if (size_.x) setPosition(mouse.x - d_mouse_.x, position_.y);
	}
	if (resize_frame_.right) {
		setSize(mouse.x - position_.x, size_.y);
	}
	if (resize_frame_.up) {
		setSize(size_.x, size_.y - (mouse.y - position_.y - d_mouse_.y));
		if (size_.y) setPosition(position_.x, mouse.y - d_mouse_.y);
	}
	if (resize_frame_.down) {
		setSize(size_.x, mouse.y - position_.y);
	}
}
void Window::moveFrameDependParent() {
	if (parent_window_ == nullptr) return;
	sf::Vector2f parent_position = parent_window_->getPosition();
	sf::Vector2f parent_size = parent_window_->getSize();
	//std::cout << parent_position.x << " " << parent_position.y << "\n";
	//std::cout << parent_size.x << " " << parent_size.y << "\n";

	sf::Vector2f new_position;
	sf::Vector2f new_size;

	switch (type_connect_) {
	case TypeConnect::NotConnect:
		break;
	case TypeConnect::Default:
		new_position = position_ + parent_position;
		new_size = size_;
		break;
	case TypeConnect::Horizantal:
		new_position = position_ + parent_position;
		new_position.x = parent_position.x + indent_frame_.left;

		new_size = size_;
		new_size.x = parent_size.x - indent_frame_.left - indent_frame_.right;
		break;
	case TypeConnect::Vertical:
		new_position = position_ + parent_position;
		position_.y = parent_position.y + indent_frame_.up;

		new_size = size_;
		new_size.y = parent_size.y - indent_frame_.down - indent_frame_.up;
		break;
	case TypeConnect::DownHorizantal:
		new_size = size_;
		new_size.x = parent_size.x - indent_frame_.left - indent_frame_.right;

		new_position = parent_position;
		new_position.x += indent_frame_.left;
		new_position.y = parent_position.y + parent_size.y - new_size.y - indent_frame_.down;
		break;
	case TypeConnect::RightVertical:
		new_size = size_;
		new_size.y = parent_size.y - indent_frame_.up - indent_frame_.down;

		new_position = parent_position;
		new_position.x = parent_position.x + parent_size.x - new_size.x - indent_frame_.right;
		new_position.y += indent_frame_.up;
		break;
	default:
		break;
	}
	
	setSize(new_size.x, new_size.y);
	frame_.setPosition(new_position);
}


// Class ScrollBar
ScrollBar::ScrollBar(Window& parent_window, TypeConnect type_connect, int thikness) : Window(parent_window, type_connect, thikness, thikness) {
	bar_.setFillColor(sf::Color::Magenta);
}

void ScrollBar::setDelta(double k) {
	delta_ = k;
}
void ScrollBar::setBarColor(sf::Color color) {
	bar_.setFillColor(color);
}

double ScrollBar::getValue() {
	return val_;
}

void ScrollBar::catchEvent(sf::Event& event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.key.code == sf::Mouse::Left) {
			sf::Vector2i mouse = sf::Mouse::getPosition();
			if (bar_.getGlobalBounds().contains(mouse.x, mouse.y)) {
				move_bar_ = 1;
				d_mouse_ = sf::Vector2f(sf::Mouse::getPosition()) - bar_.getPosition();
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (event.key.code == sf::Mouse::Left) {
			move_bar_ = 0;
		}
	default:
		break;
	}
}
void ScrollBar::draw(sf::RenderWindow& window) {
	Window::draw(window);

	sf::Vector2f frame_position_ = Window::getPosition();
	sf::Vector2f frame_size_ = Window::getSize();

	if (move_bar_ && delta_ < 1) {
		sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition());
		sf::Vector2f move = (mouse - bar_.getPosition() - d_mouse_);
		if (Window::getTypeConnect() == TypeConnect::DownHorizantal) {
			val_ += move.x / (frame_size_.x - (frame_size_.x - 10) * delta_ - 10);
		} else {
			val_ += move.y / (frame_size_.y - (frame_size_.y - 10) * delta_ - 10);
		}
		if (val_ < 0) val_ = 0;
		if (val_ > 1) val_ = 1;
	}

	sf::Vector2f size, position;
	if (Window::getTypeConnect() == TypeConnect::DownHorizantal) {
		size.x = (frame_size_.x - 10) * delta_;
		size.y = frame_size_.y - 10;

		position.x = frame_position_.x + ((frame_size_.x - size.x - 10) * delta_) * (val_ / delta_);
		position.y = frame_position_.y;
	} else {
		size.x = (frame_size_.x - 10);
		size.y = (frame_size_.y - 10) * delta_;

		position.x = frame_position_.x;
		position.y = frame_position_.y + ((frame_size_.y - size.y - 10) * delta_) * (val_ / delta_);
	}

	bar_.setPosition(position + sf::Vector2f(5, 5));
	bar_.setSize(size);

	window.draw(bar_);
}

// Class Text
Text::Text(int size, int x, int y, std::string text, int font_size, int font_thickness) :
	Window(size* font_thickness, font_size, x, y) {
	Window::setBackgroundColor(sf::Color(0, 0, 0, 0));
	text_ = text;
	font_size_ = font_size, font_thickness_ = font_thickness;
}
Text::Text(Window& parent_window, TypeConnect type_connect, int size, int x, int y, std::string text, float font_size, float font_thickness) :
	Window(parent_window, type_connect, size* font_thickness, font_size, x, y) {
	Window::setBackgroundColor(sf::Color(0, 0, 0, 0));
	font_size_ = font_size, font_thickness_ = font_thickness;
	text_ = text;
}
Text::~Text() {}


void Text::setText(std::string text) {
	text_ = text;
}
void Text::setShiftText(float dx) {
	shift_text_ = dx;
}
void Text::setTextColor(sf::Color color) {
	color_text_ = color;
}
void Text::setFrameColor(sf::Color color) {
	color_frame_ = color;
}
void Text::setVisiableFrame(bool is_visiable) {
	visiable_frame_ = is_visiable;
}

std::string Text::getText() {
	return text_;
}
int Text::getMaxTextSize() {
	return getSize().x / font_thickness_;
}

void Text::insert(int pos, std::string x) {
	if (pos >= text_.size()) {
		text_ += x;
	} else {
		text_ = substr(text_, 0, pos) + x + substr(text_, pos, text_.size());
	}
}
void Text::erase(int position) {
	text_ = substr(text_, 0, position - 1) + substr(text_, position, text_.size());
}
bool Text::mouseInBound() {
	return Window::mouseInBound();
}

void Text::draw(sf::RenderWindow& window) {
	visiable_frame_ ? Window::setFrameColor(color_frame_) : Window::setFrameColor(sf::Color(0, 0, 0, 0));
	Window::draw(window);

	sf::Font font_;
	font_.loadFromFile(load_font_adress);

	int dx = shift_text_;
	int max_size_line = Window::getSize().x / font_thickness_;

	sf::Text text_(substr(text_, dx, max_size_line + dx), font_, font_size_);
	text_.setColor(color_text_);
	text_.setPosition(Window::getPosition() - shift_all_text_);
	window.draw(text_);
}