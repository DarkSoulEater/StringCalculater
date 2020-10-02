#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

class BaseWindow {
public:
	enum class TypeConnect {
		NotConnect,
		ConnectDefault,
		ConnectVertical,
		ConnectHorizantal,
		ConnectDownHorizantal,
		UpConnect,
		UpConnectVertical,
		UpConnectHorizantal,
		DownConnect,
		DownConnectVertical,
		DownConnectHorizantal,
		RightConnect,
		RightConnectVertical,
		RightConnectHorizantal,
		LeftConnect,
		LeftConnectVertical,
		LeftConnectHorizantal
	};

	BaseWindow() {
		setSize(10, 10);
		setPosition(0, 0);
		setFrameThickness(1);
		setBackgroundColor(sf::Color::White);
		setFrameColor(sf::Color::Black);
	}
	BaseWindow(float width, float height) {
		setSize(width, height);
		setPosition(0, 0);
		setFrameThickness(1);
		setBackgroundColor(sf::Color::White);
		setFrameColor(sf::Color::Black);
	}
	BaseWindow(float width, float height, float x, float y) {
		setSize(width, height);
		setPosition(x, y);
		setFrameThickness(1);
		setBackgroundColor(sf::Color::White);
		setFrameColor(sf::Color::Black);
	}
	BaseWindow(BaseWindow& parent_window, float width, float height) {
		parent_window_ = &parent_window;
		type_connect_ = TypeConnect::ConnectDefault;
		setSize(width, height);
		setPosition(0, 0);
		setFrameThickness(1);
		setBackgroundColor(sf::Color::White);
		setFrameColor(sf::Color::Black);
	}
	BaseWindow(BaseWindow& parent_window, TypeConnect type_connect, float width, float height) {
		parent_window_ = &parent_window;
		type_connect_ = type_connect;
		setSize(width, height);
		setPosition(0, 0);
		setFrameThickness(1);
		setBackgroundColor(sf::Color::White);
		setFrameColor(sf::Color::Black);
	}
	BaseWindow(BaseWindow& parent_window, TypeConnect type_connect, float width, float height, float x, float y) {
		parent_window_ = &parent_window;
		type_connect_ = type_connect;
		setSize(width, height);
		setPosition(x, y);
		setFrameThickness(1);
		setBackgroundColor(sf::Color::White);
		setFrameColor(sf::Color::Black);
	}

	void setSize(float x, float y) {
		if (x < min_size_.x) x = min_size_.x;
		if (y < min_size_.y) y = min_size_.y;
		size_.x = x, size_.y = y;
		frame_.setSize(size_);
	}
	void setMinSize(float x, float y) {
		min_size_.x = x, min_size_.y = y;
	}
	void setPosition(float x, float y) {
		position_.x = x, position_.y = y;
		frame_.setPosition(position_);
	}
	void setPosition(sf::Vector2i position) {
		position_ = sf::Vector2f(position);
		frame_.setPosition(position_);
	}
	void setBackgroundColor(sf::Color color) {
		frame_.setFillColor(color);
	}
	void setFrameColor(sf::Color color) {
		frame_.setOutlineColor(color);
	}
	void setFrameThickness(float thickness) {
		frame_.setOutlineThickness(thickness);
	}
	void setShift(float left, float right, float up, float down) {
		up_shift_ = up;
		dowm_shift_ = down;
		right_shift_ = right;
		left_shift_ = left;
	}
	void setStableFrame(int k) {
		if (k == 0) {
			can_move_left_frame_ = 0;
		} else if (k == 1) {
			can_move_right_frame_ = 0;
		} else if (k == 2) {
			can_move_up_frame_ = 0;
		} else if (k == 3) {
			can_move_down_frame_ = 0;
		}
	}

	void catchEvent(sf::Event& event) {
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.key.code == sf::Mouse::Left) {
				checkFrameOutHit();
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
	bool mouseInBound() {
		sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition());
		return frame_.getGlobalBounds().contains(mouse.x, mouse.y);
	}

	sf::Vector2f getPosition() {
		return position_;
	}
	sf::Vector2f getSize() {
		return size_;
	}
	sf::Vector2f getFramePosition() {
		return frame_.getPosition();
	}
	sf::Vector2f getFrameSize() {
		return frame_.getSize();
	}

	void draw(sf::RenderWindow& window) {
		if (type_connect_ != TypeConnect::NotConnect) {
			moveFrameDependParent();
		} else {
			moveFrame();
		}
		window.draw(frame_);
	}
private:
	sf::Vector2f size_;
	sf::Vector2f min_size_ = sf::Vector2f(0, 0);
	// bool can_resize_x_ = 1, can_resize_y_ = 1;
	sf::RectangleShape frame_; // 

	sf::Vector2f position_;
	// bool can_move_x_ = 1, can_move_y_ = 1;

	bool move_up_frame_ = 0, move_down_frame_ = 0; //
	bool move_right_frame_ = 0, move_left_frame_ = 0; //
	bool can_move_up_frame_ = 1, can_move_down_frame_ = 1; //
	bool can_move_right_frame_ = 1, can_move_left_frame_ = 1; //

	sf::Vector2i d_mouse_;
	void checkFrameOutHit() {
		sf::Vector2i mouse = sf::Mouse::getPosition();
		d_mouse_ = mouse - sf::Vector2i(position_);
		
		if (!frame_.getGlobalBounds().contains(mouse.x, mouse.y)) return;

		if (can_move_up_frame_ && mouse.y < position_.y) {
			move_up_frame_ = 1;
		}
		if (can_move_down_frame_ && mouse.y >=  position_.y + size_.y) {
			move_down_frame_ = 1;
		}
		if (can_move_right_frame_ && mouse.x >= position_.x + size_.x) {
			move_right_frame_ = 1;
		}
		if (can_move_left_frame_ && mouse.x < position_.x) {
			move_left_frame_ = 1;
		}
	}
	void clearFrameHit() {
		move_up_frame_ = move_down_frame_ = move_right_frame_ = move_left_frame_ = 0;
	}
	void moveFrame() {
		sf::Vector2i mouse = sf::Mouse::getPosition();
		if (move_up_frame_) {
			setSize(size_.x, size_.y - (mouse.y - position_.y - d_mouse_.y));
			if (size_.y) setPosition(position_.x, mouse.y - d_mouse_.y);
		}
		if (move_down_frame_) {
			setSize(size_.x, mouse.y - position_.y);
		}
		if (move_right_frame_) {
			setSize(mouse.x - position_.x, size_.y);
		}
		if (move_left_frame_) {
			setSize(size_.x - (mouse.x - position_.x - d_mouse_.x), size_.y);
			if (size_.x) setPosition(mouse.x - d_mouse_.x, position_.y);
		}
	}

	BaseWindow* parent_window_ = nullptr;
	float up_shift_ = 0, dowm_shift_ = 0, right_shift_ = 0, left_shift_ = 0;
	TypeConnect type_connect_ = TypeConnect::NotConnect;
	void moveFrameDependParent() {
		sf::Vector2f parent_position = parent_window_->getPosition();
		sf::Vector2f parent_size = parent_window_->getSize();

		sf::Vector2f position;	
		sf::Vector2f size;

		switch (type_connect_) {
		case BaseWindow::TypeConnect::NotConnect:
			break;
		case BaseWindow::TypeConnect::ConnectDefault:
			position = position_ + parent_position;
			size = size_;			
			break;
		case BaseWindow::TypeConnect::ConnectVertical:
			position = position_ + parent_position;
			position_.y = parent_position.y + up_shift_;

			size = size_;
			size.y = parent_size.y - dowm_shift_ - up_shift_;
			break;
		case BaseWindow::TypeConnect::ConnectHorizantal:
			position = position_ + parent_position;
			position.x = parent_position.x + left_shift_;

			size = size_;
			size.x = parent_size.x - left_shift_ - right_shift_;
			break;
		case BaseWindow::TypeConnect::UpConnect:
			break;
		case BaseWindow::TypeConnect::UpConnectVertical:
			break;
		case BaseWindow::TypeConnect::UpConnectHorizantal:
			break;
		case BaseWindow::TypeConnect::DownConnect:
			break;
		case BaseWindow::TypeConnect::DownConnectVertical:
			break;
		case BaseWindow::TypeConnect::DownConnectHorizantal:
			size = size_;
			size.x = parent_size.x - left_shift_ - right_shift_;

			position = parent_position;
			position.x += left_shift_;
			position.y = parent_position.y + parent_size.y - size.y - dowm_shift_;
			break;
		case BaseWindow::TypeConnect::RightConnect:
			break;
		case BaseWindow::TypeConnect::RightConnectVertical:
			size = size_;
			size.y = parent_size.y - up_shift_ - dowm_shift_;

			position = parent_position;
			position.x = parent_position.x + parent_size.x - size.x - right_shift_;
			position.y += up_shift_;
			break;
		case BaseWindow::TypeConnect::RightConnectHorizantal:
			break;
		case BaseWindow::TypeConnect::LeftConnect:
			break;
		case BaseWindow::TypeConnect::LeftConnectVertical:
			break;
		case BaseWindow::TypeConnect::LeftConnectHorizantal:
			break;
		default:
			break;
		}

		frame_.setPosition(position);
		frame_.setSize(size);
	}
};

class ScrollBar : public BaseWindow {
public:
	/*ScrollBar() : BaseWindow(400, 30, 50, 550) {
		//BaseWindow::setBackgroundColor(sf::Color::Blue);
	}*/
	ScrollBar(BaseWindow& parent_window, BaseWindow::TypeConnect type_connect, float thikness) : BaseWindow(parent_window, type_connect, thikness, thikness) {
		bar_.setFillColor(sf::Color::Magenta);
	}

	void setDelta(float k) {
		//delta_ = 1 / k;
		delta_ = k;
	}
	void setBarColor(sf::Color color) {
		bar_.setFillColor(color);
	}

	float getVal() {
		return val_;
	}

	void catchEvent(sf::Event& event) {
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

	void draw(sf::RenderWindow& window) {
		BaseWindow::draw(window);

		sf::Vector2f frame_position_ = BaseWindow::getFramePosition();
		sf::Vector2f frame_size_ = BaseWindow::getFrameSize();

		if (move_bar_ && delta_ < 1) {
			sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition());
			sf::Vector2f move = (mouse - bar_.getPosition() - d_mouse_);
			val_ += move.x / (frame_size_.x - (frame_size_.x - 10) * delta_ - 10);
			if (val_ < 0) val_ = 0;
			if (val_ > 1) val_ = 1;
		}
		
		sf::Vector2f size;
		size.x = (frame_size_.x - 10) * delta_;
		size.y = frame_size_.y - 10;

		sf::Vector2f position;
		position.x = frame_position_.x + ((frame_size_.x - size.x - 10)* delta_) * (val_ / delta_);
		position.y = frame_position_.y;

		bar_.setPosition(position + sf::Vector2f(5, 5));
		bar_.setSize(size);

		window.draw(bar_);
	}
private:
	float delta_ = 1;
	float val_ = 0;

	sf::RectangleShape bar_;
	bool move_bar_ = 0;
	sf::Vector2f d_mouse_;
};

class ViewText : public BaseWindow {
public:	
	ViewText(int size, float x, float y, float font_size = 25, float font_thickness = 15) :
		BaseWindow(size * font_thickness, font_size, x, y) {
		BaseWindow::setBackgroundColor(sf::Color(0, 0, 0, 0));
		font_size_ = font_size, font_thickness_ = font_thickness;
	}
	ViewText(std::string text, int size, float x, float y, float font_size = 25, float font_thickness = 15) : 
		BaseWindow(size * font_thickness, font_size, x, y) {
		BaseWindow::setBackgroundColor(sf::Color(0, 0, 0, 0));
		font_size_ = font_size, font_thickness_ = font_thickness;
		text_ = text;
	}
	ViewText(BaseWindow& parent_window, BaseWindow::TypeConnect type_connect, int size, float x, float y, float font_size = 25, float font_thickness = 15) :
		BaseWindow(parent_window, type_connect, size * font_thickness, font_size, x, y) {
		BaseWindow::setBackgroundColor(sf::Color(0, 0, 0, 0));
		font_size_ = font_size, font_thickness_ = font_thickness;
	}
	ViewText(std::string text, BaseWindow& parent_window, BaseWindow::TypeConnect type_connect, int size, float x, float y, float font_size = 25, float font_thickness = 15) :
		BaseWindow(parent_window, type_connect, size * font_thickness, font_size, x, y) {
		BaseWindow::setBackgroundColor(sf::Color(0, 0, 0, 0));
		font_size_ = font_size, font_thickness_ = font_thickness;
		text_ = text;
	}
	ViewText(BaseWindow& parent_window, float y, float font_size = 25, float font_thickness = 15) : BaseWindow(parent_window, BaseWindow::TypeConnect::ConnectHorizantal, 0, font_size, 0, y) {
		BaseWindow::setBackgroundColor(sf::Color(0, 0, 0, 0));
		BaseWindow::setFrameColor(color_frame_);
		font_size_ = font_size, font_thickness_ = font_thickness;
	}

	void setText(std::string text) {
		text_ = text;
	}
	void setShiftText(float dx) {
		shift_text_ = dx;
	}
	void setTextColor(sf::Color color) {
		color_text_ = color;
	}
	void setFrameColor(sf::Color color) {
		color_frame_ = color;
	}
	void setVisiableFrame(bool is_visiable) {
		visiable_frame_ = is_visiable;
	}

	std::string getText() {
		return text_;
	}

	void insert(int pos, std::string x) {
		if (pos >= text_.size()) {
			text_ += x;
		} else {
			text_ = Substr(text_, 0, pos) + x + Substr(text_, pos, text_.size());
		}
	}
	bool mouseInBound() {
		return BaseWindow::mouseInBound();
	}

	void draw(sf::RenderWindow& window) {
		visiable_frame_ ? BaseWindow::setFrameColor(color_frame_) : BaseWindow::setFrameColor(sf::Color(0, 0, 0, 0));
		BaseWindow::draw(window);

		sf::Font font_;
		font_.loadFromFile(load_font_adress);

		int dx = shift_text_;
		int max_size_line = BaseWindow::getFrameSize().x / font_thickness_;

		sf::Text text_(Substr(text_, dx, max_size_line + dx), font_, font_size_);
		text_.setColor(color_text_);
		text_.setPosition(BaseWindow::getFramePosition() - shift_all_text_);
		window.draw(text_);
	}
private:
	std::string text_ = "sadawdasdawdasdawdasdaw";
	sf::Color color_text_ = sf::Color::Blue;

	sf::Color color_frame_ = sf::Color::Green;
	bool visiable_frame_ = 0;

	float font_size_ = 25;
	float font_thickness_ = 15;

	std::string load_font_adress = "Font/texgyrecursor_regular.otf";
	const sf::Vector2f shift_all_text_ = sf::Vector2f(0, 5);

	int shift_text_ = 0;
	std::string Substr(const std::string& x, int a, int b) {
		std::string s = "";
		if (a >= b) return s;
		for (int i = a; i < (x.size() > b ? b : x.size()); ++i) {
			s += x[i];
		}
		return s;
	}
};

class EditText : public BaseWindow {
public:
	EditText(float width, float x, float y, float font_size = 25, float font_thickness = 15) : BaseWindow(width, font_size, x, y) {
		BaseWindow::setBackgroundColor(sf::Color(0, 0, 0, 0));
		BaseWindow::setFrameColor(sf::Color::Red);
		font_size_ = font_size, font_thickness_ = font_thickness;
	}
	EditText(BaseWindow& parent_window, float y, float font_size = 25, float font_thickness = 15) : BaseWindow(parent_window, BaseWindow::TypeConnect::ConnectHorizantal, 0, font_size, 0, y) {
		BaseWindow::setBackgroundColor(sf::Color(0, 0, 0, 0));
		BaseWindow::setFrameColor(sf::Color::Green);
		font_size_ = font_size, font_thickness_ = font_thickness;
	}

	void setText(std::string text) {
		edit_text_ = text;
	}
	void setStick(float fpos) {
		int pos = round(fpos);
		if (pos < 0) pos = 0;
		if (pos + shift_x_ > edit_text_.size()) pos = edit_text_.size() - shift_x_;
		pos = std::min(pos, int(BaseWindow::getFrameSize().x / font_thickness_));
		stick_position_ = pos;
	}
	void setActive() {
		edit_now_ = 1;
		setStick(getStick());
	}
	void setShiftX(float dx) {
		shift_x_ = dx;
	}
	void setNotActive() {
		edit_now_ = 0;
	}
	void setTextColor(sf::Color color) {
		edit_text_color_ = color;
	}
	void setStickColor(sf::Color color) {
		stick_color_ = color;
	}
	void setFrameColor(sf::Color color) {
		frame_color_ = color;
	}

	std::string getText() {
		return edit_text_;
	}
	int getStick() {
		return stick_position_;
	}

	bool isActive() {
		return edit_now_;
	}

	void catchEvent(sf::Event& event) {
		int dx = round(shift_x_);
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.key.code == sf::Mouse::Left) {
				sf::Vector2i mouse = sf::Mouse::getPosition();
				sf::Vector2f position = BaseWindow::getFramePosition();
				if (BaseWindow::mouseInBound()) {
					setActive();
					setStick((mouse.x - position.x) / font_thickness_);
				} else {
					setNotActive();
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::TextEntered:
			if (edit_now_) {
				if (event.text.unicode == 8) { // BackSpace
					if (stick_position_ != 0) {
						edit_text_ = Substr(edit_text_, 0, stick_position_ + dx - 1) + Substr(edit_text_, stick_position_ + dx, edit_text_.size());
						setStick(stick_position_ - 1);
					}
				} else if (event.text.unicode == 9 || event.text.unicode == 13) { // Tab or Enter
					break;
				} else if (event.text.unicode < 128) {
					char x = char(event.text.unicode);
					edit_text_ = Substr(edit_text_, 0, stick_position_ + dx) + x + Substr(edit_text_, stick_position_ + dx, edit_text_.size());
					setStick(stick_position_ + 1);
				}
			}
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Right) {
				setStick(stick_position_ + 1);
			} else if (event.key.code == sf::Keyboard::Left) {
				setStick(stick_position_ - 1);
			} else if (event.key.code == sf::Keyboard::Delete) {
				if (stick_position_ != edit_text_.size()) {
					edit_text_ = Substr(edit_text_, 0, stick_position_ + dx) + Substr(edit_text_, stick_position_ + 1 + dx, edit_text_.size());
				}
			}
			break;
		default:
			break;
		}
	}

	void draw(sf::RenderWindow& window) {
		edit_now_ ? BaseWindow::setFrameColor(frame_color_) : BaseWindow::setFrameColor(sf::Color(0, 0, 0, 0));
		BaseWindow::draw(window);
		if (edit_now_) drawEditLine(window);
		
		sf::Font font_;
		font_.loadFromFile(load_font_adress);

		int dx = shift_x_;
		int max_size = BaseWindow::getFrameSize().x / font_thickness_;

		sf::Text text_(Substr(edit_text_, dx, max_size + dx), font_, font_size_);
		text_.setColor(edit_text_color_);
		text_.setPosition(BaseWindow::getFramePosition() - shift_all_text_);
		window.draw(text_);
	}
	void drawEditLine(sf::RenderWindow& window) {
		sf::VertexArray edit_line(sf::PrimitiveType::Lines, 2);

		sf::Vector2f char_size = sf::Vector2f(font_thickness_ * stick_position_, 0);
		edit_line[0].position = BaseWindow::getFramePosition() + char_size;
		edit_line[1].position = BaseWindow::getFramePosition() - sf::Vector2f(0, -font_size_) + char_size;

		edit_line[0].color = stick_color_;
		edit_line[1].color = stick_color_;

		window.draw(edit_line);
	}

private:
	std::string edit_text_ = "";

	sf::Color edit_text_color_ = sf::Color::Blue;
	sf::Color stick_color_ = sf::Color::Green;
	sf::Color frame_color_ = sf::Color::Cyan;

	bool edit_now_ = 0;
	int stick_position_ = 0;
	
	float font_size_;
	float font_thickness_;

	std::string load_font_adress = "Font/texgyrecursor_regular.otf";

	int shift_x_ = 0;
	const sf::Vector2f shift_all_text_ = sf::Vector2f(0, 5);

	std::string Substr(const std::string& x, int a, int b) {
		std::string s = "";
		if (a >= b) return s;
		for (int i = a; i < (x.size() > b ? b : x.size()); ++i) {
			s += x[i];
		}
		return s;
	}
};

class ScrollEditPanel : BaseWindow{
public:
	ScrollEditPanel(float width, float height, float x, float y) : BaseWindow(width, height, x, y) {
		//setBackgroundColor(sf::Color(59, 59, 59));
		//setFrameColor(sf::Color(32, 32, 32));
		setBackgroundColor(sf::Color(32, 32, 32));
		setFrameColor(sf::Color(59, 59, 59));
		setFrameThikness(15);
		BaseWindow::setMinSize(150, 50);

		down_scroll_ = new ScrollBar(*this, BaseWindow::TypeConnect::DownConnectHorizantal, 20);
		down_scroll_->setShift(1, 1, 0, 1);
		down_scroll_->setBackgroundColor(sf::Color(82, 82, 82));
		down_scroll_->setBarColor(sf::Color(117, 117, 116));

		right_scroll_ = new ScrollBar(*this, BaseWindow::TypeConnect::RightConnectVertical, 20);
		right_scroll_->setShift(0, 1, 1, 22);
		right_scroll_->setBackgroundColor(sf::Color(82, 82, 82));
		right_scroll_->setBarColor(sf::Color(117, 117, 116));

		for (int i = 0; i < view_text_arr_size_; ++i) {
			std::string num = std::to_string(i + 1) + " ";
			while (num.size() != 5) { num = " " + num; }
			view_text_[i] = new ViewText(num, *this, BaseWindow::TypeConnect::ConnectDefault, 5, 0, i * (font_size_ + 2) + 3, font_size_, font_thickness_);
			view_text_[i]->setTextColor(sf::Color(180, 180, 180));
		}

		for (int i = 0; i < edit_text_arr_size_; ++i) {
			edit_text_[i] = new ViewText(*this, i * (font_size_ + 2) + 3, font_size_, font_thickness_);
			edit_text_[i]->setShift(font_thickness_ * 5, 23, 0, 0);
			edit_text_[i]->setFrameColor(sf::Color(87, 87, 87));
			edit_text_[i]->setTextColor(sf::Color(182, 178, 129));
			//edit_text_[i]->setStickColor(sf::Color(230, 230, 230));
		}

		BaseWindow::setStableFrame(2);
		BaseWindow::setStableFrame(0);
	}

	void setBackgroundColor(sf::Color color) {
		BaseWindow::setBackgroundColor(color);
	}
	void setFrameColor(sf::Color color) {
		BaseWindow::setFrameColor(color);
	}
	void setFrameThikness(float thikness) {
		BaseWindow::setFrameThickness(thikness);
	}

	void set_dx_(float dx) {

	}
	float get_dx_() {
		return down_scroll_->getVal();
	}

	void catchEvent(sf::Event& event) {
		BaseWindow::catchEvent(event);
		down_scroll_->catchEvent(event);
		right_scroll_->catchEvent(event);

		for (int i = 0; i < edit_text_arr_size_; ++i) {
			//edit_text_[i]->catchEvent(event);
		}

		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.key.code == sf::Mouse::Left) {
				sf::Vector2i mouse = sf::Mouse::getPosition();
				setActiveEdit(nullptr);
				for (int i = 0; i < edit_text_arr_size_; ++i) {
					if (edit_text_[i]->mouseInBound()) {
						setActiveEdit(edit_text_[i]);
						setStick((mouse.x - edit_text_[i]->getFramePosition().x) / font_thickness_);
					}
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::TextEntered:
			if (active_edit_ != nullptr) {
				if (event.text.unicode < 128) {
					char x = char(event.text.unicode);
					active_edit_->insert(stick_position_, "d");
					//edit_text_ = Substr(edit_text_, 0, stick_position_ + dx) + x + Substr(edit_text_, stick_position_ + dx, edit_text_.size());
					setStick(stick_position_ + 1);
				}
			}
			/*if (edit_now_) {
				if (event.text.unicode == 8) { // BackSpace
					if (stick_position_ != 0) {
						edit_text_ = Substr(edit_text_, 0, stick_position_ + dx - 1) + Substr(edit_text_, stick_position_ + dx, edit_text_.size());
						setStick(stick_position_ - 1);
					}
				} else if (event.text.unicode == 9 || event.text.unicode == 13) { // Tab or Enter
					break;
				} else if (event.text.unicode < 128) {
					char x = char(event.text.unicode);
					edit_text_ = Substr(edit_text_, 0, stick_position_ + dx) + x + Substr(edit_text_, stick_position_ + dx, edit_text_.size());
					setStick(stick_position_ + 1);
				}
			}*/
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Right) {
				setStick(stick_position_ + 1);
			} else if (event.key.code == sf::Keyboard::Left) {
				setStick(stick_position_ - 1);
			} /*else if (event.key.code == sf::Keyboard::Delete) {
				if (stick_position_ != edit_text_.size()) {
					edit_text_ = Substr(edit_text_, 0, stick_position_ + dx) + Substr(edit_text_, stick_position_ + 1 + dx, edit_text_.size());
				}
			}*/
			break;
		default:
			break;
		}
	}

	void draw(sf::RenderWindow& window) {
		BaseWindow::draw(window);


		down_scroll_->draw(window);
		right_scroll_->draw(window);

		for (int i = 0; i < view_text_arr_size_; ++i) {
			if ((font_size_ + 2) * (i + 1) < BaseWindow::getSize().y - 25) view_text_[i]->draw(window);
		}

		for (int i = 0; i < edit_text_arr_size_; ++i) {
			// edit_text_[i]->setVisiableFrame(1);
			if ((font_size_ + 2) * (i + 1) < BaseWindow::getSize().y - 25) edit_text_[i]->draw(window);
		}

		if (active_edit_ != nullptr) {
			draw_stick(window);
		}

		/*active_edit_ = nullptr;
		float max_length_line = 0;
		for (int i = 0; i < edit_text_arr_size_; ++i) {
			if ((font_size_ + 2) * (i + 1) < BaseWindow::getSize().y - 25) edit_text_[i]->draw(window);
			if (edit_text_[i]->isActive()) active_edit_ = edit_text_[i];
			max_length_line = std::max(max_length_line, float(edit_text_[i]->getText().size()));
		}
		
		float lenght_window_text = ceil(BaseWindow::getFrameSize().x / font_thickness_);
		down_scroll_->setDelta(lenght_window_text / std::max(max_length_line + 10, lenght_window_text));

		for (int i = 0; i < edit_text_arr_size_; ++i) {
			edit_text_[i]->setShiftX(lenght_window_text * down_scroll_->getVal());
		}*/
	}
private:
	ScrollBar *down_scroll_;
	ScrollBar *right_scroll_;

	const int view_text_arr_size_ = 40;
	ViewText* view_text_[40];

	const int edit_text_arr_size_ = 40;
	ViewText *edit_text_[40];
	ViewText *active_edit_ = nullptr;

	sf::Color background_color_;
	sf::Color frame_color_;
	//sf::Color
	//sf::Color
	//sf::Color

	float font_size_ = 25;
	float font_thickness_ = 15;

	int stick_position_ = 0;
	sf::Color stick_color_ = sf::Color::White;

	void setActiveEdit(ViewText* vt) {
		if (active_edit_ != nullptr) {
			active_edit_->setVisiableFrame(0);
		}
		active_edit_ = vt;
		if (active_edit_ != nullptr) {
			active_edit_->setVisiableFrame(1);
		}
	}
	void setStick(int position) {
		if (active_edit_ == nullptr) return;
		if (position < 0) position = 0;
		if (position > active_edit_->getText().size()) position = active_edit_->getText().size();
		stick_position_ = position;
	}
	void draw_stick(sf::RenderWindow& window) {
		if (active_edit_ == nullptr) return;
		sf::VertexArray stick_line(sf::PrimitiveType::Lines, 2);

		sf::Vector2f char_size = sf::Vector2f(font_thickness_ * stick_position_, 0);
		stick_line[0].position = active_edit_->getFramePosition() + char_size;
		stick_line[1].position = active_edit_->getFramePosition() - sf::Vector2f(0, -font_size_) + char_size;

		stick_line[0].color = stick_color_;
		stick_line[1].color = stick_color_;

		window.draw(stick_line);
	}
}; 