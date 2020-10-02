#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

enum class TypeConnect {
	NotConnect,
	Default,
	Horizantal,
	Vertical,
	DownHorizantal,
	RightVertical
};

class Window {
public:
	Window(int width = 0, int height = 0, int x = 0, int y = 0);
	Window(Window& parent_window, TypeConnect type_connect,
		   int width = 0, int height = 0, int x = 0, int y = 0);
	~Window();


	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setPosition(int x, int y);
	void setFrameThickness(int thickness);
	void setIndent(int left = 0, int right = 0, int up = 0, int down = 0);
	void setStableFrame(std::string side);

	void setBackgroundColor(sf::Color color);
	void setFrameColor(sf::Color color);

	sf::Vector2f getSize();
	sf::Vector2f getPosition();
	TypeConnect getTypeConnect();


	bool mouseInBound();
	void catchEvent(sf::Event& event);
	void draw(sf::RenderWindow& window);
private:
	sf::Vector2f size_;
	sf::Vector2f min_size_;

	sf::Vector2f position_ = sf::Vector2f(0,0);

	sf::RectangleShape frame_;
	struct{
		bool left = 1, right = 1, up = 1, down = 1;
	} resize_frame_, frame_resizable;

	sf::Vector2i d_mouse_;
	void checkOutFrameHit();
	void clearFrameHit();
	void moveFrame();
	void moveFrameDependParent();
	
	// For dependent windows
	Window* parent_window_ = nullptr;
	TypeConnect type_connect_;
	struct {
		int left = 0, right = 0, up = 0, down = 0;
	} indent_frame_;
};

class ScrollBar : public Window {
public:
	ScrollBar(Window& parent_window, TypeConnect type_connect, int thikness);

	void setDelta(double k);
	void setBarColor(sf::Color color);

	double getValue();

	void catchEvent(sf::Event& event);
	void draw(sf::RenderWindow& window);
private:
	double delta_ = 1;
	//int right_point_ = 0;
	//int value_ = 0;
	double val_ = 0;

	sf::RectangleShape bar_;
	bool move_bar_ = 0;
	sf::Vector2f d_mouse_;
};

class Text : public Window{
public:	
	Text(int size = 0, int x = 0, int y = 0, std::string text = "", int font_size = 25, int font_thickness = 15);
	Text(Window& parent_window, TypeConnect type_connect, int size = 0, int x = 0, int y = 0,
		 std::string text = "", float font_size = 25, float font_thickness = 15);
	~Text();


	void setText(std::string text);
	void setShiftText(float dx);
	void setTextColor(sf::Color color);
	void setFrameColor(sf::Color color);
	void setVisiableFrame(bool is_visiable);

	std::string getText();
	int getMaxTextSize();

	void insert(int pos, std::string x);
	void erase(int position);
	bool mouseInBound();

	void draw(sf::RenderWindow& window);
private:
	std::string text_ = "";
	sf::Color color_text_ = sf::Color::Blue;

	sf::Color color_frame_ = sf::Color::Green;
	bool visiable_frame_ = 0;

	float font_size_;
	float font_thickness_;

	std::string load_font_adress = "Font/texgyrecursor_regular.otf";
	const sf::Vector2f shift_all_text_ = sf::Vector2f(0, 5);

	int shift_text_ = 0;
	std::string substr(const std::string& x, int a, int b) {
		std::string s = "";
		if (a >= b) return s;
		for (int i = a; i < (x.size() > b ? b : x.size()); ++i) {
			s += x[i];
		}
		return s;
	}
};

class ScrollEditPanel : public Window {
public:
	ScrollEditPanel(int type, int width, int height, int x, int y) : Window(width, height, x, y) {
		setBackgroundColor(sf::Color(32, 32, 32));
		setFrameColor(sf::Color(59, 59, 59));
		setFrameThikness(15);
		Window::setMinSize(150, 50);

		type_ = type;
		if (type_ == 0) {
			setStableFrame("up");
			setStableFrame("left");
		} else if (type_ == 1) {
			setStableFrame("up");
			setStableFrame("right");
		} else if (type_ == 2) {
			setStableFrame("down");
			setStableFrame("left");
			setStableFrame("right");
		}

		down_scroll_ = new ScrollBar(*this, TypeConnect::DownHorizantal, 20);
		down_scroll_->setIndent(1, 1, 0, 1);
		down_scroll_->setBackgroundColor(sf::Color(82, 82, 82));
		down_scroll_->setBarColor(sf::Color(117, 117, 116));

		right_scroll_ = new ScrollBar(*this, TypeConnect::RightVertical, 20);
		right_scroll_->setIndent(0, 1, 1, 22);
		right_scroll_->setBackgroundColor(sf::Color(82, 82, 82));
		right_scroll_->setBarColor(sf::Color(117, 117, 116));
		
		for (int i = 0; i < 50; ++i) {
			line_numbering_[i] = new Text(*this, TypeConnect::Default, 5, 0, i * (font_size_ + 2) + 3, "", font_size_, font_width_);
			line_numbering_[i]->setTextColor(sf::Color(180, 180, 180));
		}

		for (int i = 0; i < 50; ++i) {
			edit_text_[i] = new Text(*this, TypeConnect::Horizantal, 0, 0, i * (font_size_ + 2) + 3, "", font_size_, font_width_);
			edit_text_[i]->setIndent(font_width_ * 5, 23, 0, 0);
			edit_text_[i]->setFrameColor(sf::Color(87, 87, 87));
			edit_text_[i]->setTextColor(sf::Color(182, 178, 129));
		}
		
		if (type_ != 1) {
			text_data_.push_back("");
		}
		//new_line(0, "K");
		//new_line(3, "KK");
		//text_data_.push_back("asd");
		//text_data_.push_back("wr124r+32");
	}

	void setBackgroundColor(sf::Color color) {
		Window::setBackgroundColor(color);
	}
	void setFrameColor(sf::Color color) {
		Window::setFrameColor(color);
	}
	void setFrameThikness(float thikness) {
		Window::setFrameThickness(thikness);
	}
	void setData(std::vector<std::string>& data) {
		text_data_ = data;
	}
	void setData(std::map<std::string, bool>& data) {
		//text_data_.clear();
		//text_data_.resize(0);
		for (auto it = data.begin(); it != data.end(); ++it) {
			if (it->second) {
				if (var_name_.count(it->first) == 0) {
					new_var(it->first); 
				}
				//text_data_.push_back(u.first);
			} else {
				if (var_name_.count(it->first)) {
					std::cout << "IN\n";
					delete_var(it->first);
				}
			}
		}
	}
	
	std::vector<std::string>& getData() {
		return text_data_;
	}

	void catchEvent(sf::Event& event) {
		Window::catchEvent(event);

		down_scroll_->catchEvent(event);
		right_scroll_->catchEvent(event);
		
		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.key.code == sf::Mouse::Left) {
				sf::Vector2i mouse = sf::Mouse::getPosition();
				set_active_edit(nullptr);
				for (int i = 0; i < 50; ++i) {
					if (edit_text_[i]->mouseInBound()) {
						set_active_edit(edit_text_[i]);
						set_stick_position((mouse.x - edit_text_[i]->getPosition().x) / font_width_);
					}
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::TextEntered:
			if (active_edit_ != nullptr) {
				if (event.text.unicode == 8) { // BackSpace
					if (stick_position_ != 0) {
						active_edit_->erase(stick_position_ + dx);
						set_stick_position(stick_position_ - 1);
						for (int i = 0; i < 50; ++i) {
							if (edit_text_[i] == active_edit_) {
								text_data_[i + dy] = edit_text_[i]->getText();
							}
						}
					}
				} else if (event.text.unicode == 9) { // Tab
					break;
				} else if (event.text.unicode == 13) { // Enter
					// Перевод строки
					text_data_.push_back("");
					//std::cout << dx << "\n";
					break;
				}else if (event.text.unicode < 128) {
					char a = event.text.unicode;
					std::string x = std::string(&a, 1);
					active_edit_->insert(stick_position_ + dx, x);

					// Сохраняем изменения
					for (int i = 0; i < 50; ++i) {
						if (edit_text_[i] == active_edit_) {
							text_data_[i + dy] = edit_text_[i]->getText();
						}
					}
					set_stick_position(stick_position_ + 1);
				}
			}
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Right) {
				set_stick_position(stick_position_ + 1);
			} else if (event.key.code == sf::Keyboard::Left) {
				set_stick_position(stick_position_ - 1);
			} else if (event.key.code == sf::Keyboard::Delete) {
				if (stick_position_ != active_edit_->getText().size()) {
					active_edit_->erase(stick_position_ + 1);
				}
			} else if (event.key.code == sf::Keyboard::Up) {
				up_stick();
			} else if (event.key.code == sf::Keyboard::Down) {
				set_stick_position(stick_position_ - 1);
			}
			break;
		default:
			break;
		}
	}
	void draw(sf::RenderWindow& window) {
		Window::draw(window);
		down_scroll_->draw(window);
		right_scroll_->draw(window);

		check_scroll_bar();

		for (int i = 0; i < 50; ++i) {
			if (i + dy < text_data_.size()) {
				std::string numer = std::to_string(i + 1 + dy) + " ";
				while (numer.size() != 5) { numer = " " + numer; }
				line_numbering_[i]->setText(numer);
				if ((font_size_ + 2) * (i + 1) < getSize().y - 25) line_numbering_[i]->draw(window);
			}
		}

		for (int i = 0; i < 50; ++i) {
			if (i + dy < text_data_.size()) {
				edit_text_[i]->setText(text_data_[i + dy]);
				edit_text_[i]->setShiftText(dx);
				if ((font_size_ + 2) * (i + 1) < getSize().y - 25) edit_text_[i]->draw(window);
			}
		}

		draw_stick(window);
	}
private:
	int type_ = 0;
	ScrollBar* down_scroll_;
	ScrollBar* right_scroll_;
	void check_scroll_bar() {
		float max_text_length = 0;
		for (int i = 0; i < 50; ++i) {
			max_text_length = std::max(max_text_length, float(edit_text_[i]->getText().size()));
		}

		float lenght_window_text = ceil((getSize().x - font_width_ * 5 - 10) / font_width_);
		double max_dx = max_text_length + 10 - lenght_window_text;
		if (max_dx < 0) max_dx = 0;

		down_scroll_->setDelta(lenght_window_text / (max_dx + lenght_window_text));
		//dx = max_dx * down_scroll_->getValue();
		//if (dx < 0) dx = 0;
	}

	int font_size_ = 25;
	int font_width_ = 15;

	Text* line_numbering_[50];
	Text* edit_text_[50];

	int stick_position_ = 0;
	sf::Color stick_color_ = sf::Color::White;
	Text* active_edit_ = nullptr;
	void set_stick_position(int position) {
		std::cout << position << "\n";
		if (active_edit_ == nullptr) return;
		if (position < 0) {
			set_dx(dx - 1);
			position = 0;
		}
		if (position > active_edit_->getText().size() - dx) position = active_edit_->getText().size() - dx;

		if (active_edit_->getMaxTextSize() < position) {
			set_dx(dx + 1);
			position -= 1;
		}
		stick_position_ = position;
	}
	void up_stick() {
		std::cout << "K";
		if (active_edit_ == nullptr) {
			std::cout << "Bre\n";
			return;
		}
		for (int i = 0; i < 50; ++i) {
			if (edit_text_[i]->getPosition() == active_edit_->getPosition()) {
				std::cout << "OK";
				
				if (i != 0) {
					set_active_edit(edit_text_[i - 1]);
				}
			}
			return;
		}
	}
	void down_stick();
	void draw_stick(sf::RenderWindow& window) {
		if (active_edit_ == nullptr) return;
		sf::VertexArray stick_line(sf::PrimitiveType::Lines, 2);

		sf::Vector2f char_size = sf::Vector2f(font_width_ * stick_position_, 0);
		stick_line[0].position = active_edit_->getPosition() + char_size;
		stick_line[1].position = active_edit_->getPosition() - sf::Vector2f(0, -font_size_) + char_size;

		stick_line[0].color = stick_color_;
		stick_line[1].color = stick_color_;

		window.draw(stick_line);
	}
	void set_active_edit(Text* text_line) {
		if (active_edit_ != nullptr) {
			active_edit_->setVisiableFrame(0);
		}
		active_edit_ = text_line;
		if (active_edit_ != nullptr) {
			active_edit_->setVisiableFrame(1);
		}
	}


	std::vector<std::string> text_data_;
	void new_line(int position, std::string text = "") {
		text_data_.push_back("");
		for (int i = text_data_.size() - 1; i > position; --i) {
			text_data_[i] = text_data_[i - 1];
		}
		text_data_[position] = text;
	}
	void delete_line(int position) {
		for (int i = position; i < text_data_.size() - 1; ++i) {
			text_data_[i] = text_data_[i + 1];
		}
		text_data_.pop_back();
	}

	std::map<std::string, std::vector<std::string>::iterator> var_name_;
	void new_var(std::string name) {
		text_data_.push_back("");
		std::vector<std::string>::iterator it = text_data_.end();
		var_name_[name] = --it;
	}
	void delete_var(std::string name) {
		text_data_.erase(var_name_[name]);

		var_name_.erase(name);
	}

	int dx = 0, dy = 0;
	void set_dx(int new_dx) {
		if (new_dx < 0) new_dx = 0;
		dx = new_dx;
	}
	void set_dy(int new_dy) {
		dy = new_dy;
	}
};

class SetVarPanel : public Window {
public:
	SetVarPanel(int width, int height, int x, int y) : Window(width, height, x, y) {
		setBackgroundColor(sf::Color(32, 32, 32));
		setFrameColor(sf::Color(59, 59, 59));
		Window::setFrameThickness(15);
		Window::setMinSize(150, 50);

		setStableFrame("up");
		setStableFrame("right");

		for (int i = 0; i < 50; ++i) {
			edit_text_[i] = new Text(*this, TypeConnect::Horizantal, 0, 0, i * (font_size_ + 2) + 3, "", font_size_, font_width_);
			edit_text_[i]->setIndent(font_width_ * 5, 23, 0, 0);
			edit_text_[i]->setFrameColor(sf::Color(87, 87, 87));
			edit_text_[i]->setTextColor(sf::Color(182, 178, 129));
		}
	}

	std::map<std::string, std::string> getVarData() {
		return variable_;
	}
	void upadteVarData(std::map<std::string, bool>& var_name_map) {
		for (auto it = variable_.begin(); it != variable_.end(); ++it) {
			if (!var_name_map.count(it->first) || var_name_map[it->first] == 0) { // 
				variable_.erase(it); // 
			}
		}

		for (auto it = var_name_map.begin(); it != var_name_map.end(); ++it) {
			if (it->second && !variable_.count(it->first)) {
				//variable_.insert({ it->first, "" });
				//std::cout << it->first << " ";
				variable_[it->first] = "";
			}
		}
	}

	void catchEvent(sf::Event& event) {
		Window::catchEvent(event);

		switch (event.type) {
		case sf::Event::MouseButtonPressed:
			if (event.key.code == sf::Mouse::Left) {
				sf::Vector2i mouse = sf::Mouse::getPosition();
				set_active_edit(nullptr);
				for (int i = 0; i < 50; ++i) {
					if (edit_text_[i]->mouseInBound()) {
						set_active_edit(edit_text_[i]);
						set_stick_position((mouse.x - edit_text_[i]->getPosition().x) / font_width_);
					}
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			break;
		case sf::Event::TextEntered:
			if (active_edit_ != nullptr) {
				if (event.text.unicode == 8) { // BackSpace
					if (stick_position_ != 0) {
						std::string text = active_edit_->getText();
						std::string name = "";
						int j = 0;
						while (text[j] != ':') {
							name += text[j++];
						}

						if (stick_position_ + dx <= name.size() + 1) return;

						active_edit_->erase(stick_position_ + dx);
						set_stick_position(stick_position_ - 1);
						text = active_edit_->getText();
						variable_[name] = substr(text, j + 1, text.size());
					}
				} else if (event.text.unicode == 9) { // Tab
					break;
				} else if (event.text.unicode == 13) { // Enter
					// Перевод строки
					std::cout << dx << "\n";
					break;
				} else if (event.text.unicode < 128) {
					char a = event.text.unicode;
					if (a >= '0' && a <= '9' || a == '.') {
						std::string x = std::string(&a, 1);
						active_edit_->insert(stick_position_ + dx, x);

						// Сохраняем изменения
						std::string text = active_edit_->getText();
						std::string name = "";
						int j = 0;
						while (text[j] != ':') {
							name += text[j++];
						}
						variable_[name] = substr(text, j + 1, text.size());
						//variable_[name] += "dd";
						//std::cout << variable_[name] << "\n";

						set_stick_position(stick_position_ + 1);
					}
				}
			}
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Right) {
				set_stick_position(stick_position_ + 1);
			} else if (event.key.code == sf::Keyboard::Left) {
				set_stick_position(stick_position_ - 1);
			} else if (event.key.code == sf::Keyboard::Delete) {
				if (stick_position_ != active_edit_->getText().size()) {
					active_edit_->erase(stick_position_ + 1);

					std::string text = active_edit_->getText();
					std::string name = "";
					int j = 0;
					while (text[j] != ':') {
						name += text[j++];
					}
					variable_[name] = substr(text, j + 1, text.size());
				}
			}
			break;
		default:
			break;
		}
	}

	void draw(sf::RenderWindow& window) {
		Window::draw(window);

		//check_scroll_bar();

		int i = 0;
		for (auto it = variable_.begin(); it != variable_.end(); ++it) {
			edit_text_[i]->setText(it->first + ":" + it->second);
			edit_text_[i]->setShiftText(dx);
			edit_text_[i]->draw(window);
			++i;
			if (i >= 50) break;
		}

		draw_stick(window);
	}
private:
	int font_size_ = 25;
	int font_width_ = 15;

	Text* edit_text_[50];

	std::map<std::string, std::string> variable_;

	int stick_position_ = 0;
	Text* active_edit_ = nullptr;

	int dx = 0, dy = 0;
	void set_dx(int new_dx) {
		if (new_dx < 0) new_dx = 0;
		dx = new_dx;
	}
	void set_dy(int new_dy) {
		dy = new_dy;
	}
	

	void set_stick_position(int position) {
		std::cout << position << "\n";
		if (active_edit_ == nullptr) return;
		if (position < 0) {
			set_dx(dx - 1);
			position = 0;
		}
		if (position > active_edit_->getText().size() - dx) position = active_edit_->getText().size() - dx;

		if (active_edit_->getMaxTextSize() < position) {
			set_dx(dx + 1);
			position -= 1;
		}
		stick_position_ = position;
	}
	void draw_stick(sf::RenderWindow& window) {
		if (active_edit_ == nullptr) return;
		sf::VertexArray stick_line(sf::PrimitiveType::Lines, 2);

		sf::Vector2f char_size = sf::Vector2f(font_width_ * stick_position_, 0);
		stick_line[0].position = active_edit_->getPosition() + char_size;
		stick_line[1].position = active_edit_->getPosition() - sf::Vector2f(0, -font_size_) + char_size;

		stick_line[0].color = sf::Color::White;
		stick_line[1].color = sf::Color::White;

		window.draw(stick_line);
	}
	void set_active_edit(Text* text_line) {
		if (active_edit_ != nullptr) {
			active_edit_->setVisiableFrame(0);
		}
		active_edit_ = text_line;
		if (active_edit_ != nullptr) {
			active_edit_->setVisiableFrame(1);
		}
	}


	std::string substr(const std::string& x, int a, int b) {
		std::string s = "";
		if (a >= b) return s;
		for (int i = a; i < (x.size() > b ? b : x.size()); ++i) {
			s += x[i];
		}
		return s;
	}
};