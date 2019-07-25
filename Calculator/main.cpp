
#include <iostream>
#include <curses.h>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <optional>
#include <map>

class kurs {
public:
	explicit kurs(const std::string& id, const std::string& typ, const std::string& namn, int poäng, char betyg) :

		id_{ id },
		typ_{ typ },
		namn_{ namn },
		poäng_{ poäng },
		betyg_{ betyg }

	{}

	auto set_id(const std::string& id) { id_ = id; }
	auto set_typ(const std::string& typ) { typ_ = typ; }
	auto set_namn(const std::string& namn) { namn_ = namn; }
	auto set_poäng(int poäng) { poäng_ = poäng; }
	auto set_betyg(char betyg) { betyg_ = betyg; }

	auto get_id() const { return id_; }
	auto get_typ() const { return typ_; }
	auto get_namn() const { return namn_; }
	auto get_poäng() const { return poäng_; }
	auto get_betyg() const { return betyg_; }
	auto get_betygvärde() const { return betygs_värden.at(betyg_); }

	auto operator==(const kurs& krs) {
		return
			id_ == krs.get_id() &&
			typ_ == krs.get_typ() &&
			namn_ == krs.get_namn() &&
			poäng_ == krs.get_poäng() &&
			betyg_ == krs.get_betyg();
	}

private:

	std::map<char, double> betygs_värden{
		{'A',20},
		{'B',17.5},
		{'C',15},
		{'D',12.5},
		{'E',10},
		{'F',0}
	};

	std::string id_{};
	std::string typ_{};
	std::string namn_{};
	int poäng_{};
	char betyg_{};

};

class program {
public:
	explicit program(const std::vector<kurs>& kurser) : kurser_(kurser) {}

	auto set_kurser(const std::vector<kurs>& kurser) { kurser_ = kurser; }
	auto set_merit(double merit) { merit_ = merit; }

	auto get_merit() const { return merit_; }
	auto get_kurser() const { return &kurser_; }

	auto get_snitt() const { return (std::accumulate(kurser_.begin(), kurser_.end(), static_cast<double>(0), summa) / std::accumulate(kurser_.begin(), kurser_.end(), static_cast<double>(0), poäng)) + merit_; }
	auto get_sum() const { return std::accumulate(kurser_.begin(), kurser_.end(), static_cast<double>(0), poäng); }

	program& operator+=(const kurs& krs) {
		kurser_.push_back(krs);
		return *this;
	}

	program& operator+(const kurs& krs) {
		operator+=(krs);
		return *this;
	}

	program& operator-=(const kurs& krs) {
		auto search = std::find(kurser_.begin(), kurser_.end(), krs);

		if (search != kurser_.end()) {
			kurser_.erase(search);
		}

		return *this;
	}

	program& operator-(const kurs& krs) {
		operator-=(krs);
		return *this;
	}

private:
	std::vector<kurs> kurser_{};
	double merit_{};

	const std::function<double(double, const kurs&)> summa = [](double result, const kurs& krs) {return result + (krs.get_poäng() * krs.get_betygvärde()); };
	const std::function<double(double, const kurs&)> poäng = [](double result, const kurs& krs) {return result + krs.get_poäng(); };
};

struct point {
	int x;
	int y;
};

using size = point;

class curse {
public:
	curse() {
		initscr();
		noecho();
		curs_set(0);
		keypad(stdscr, TRUE);
		nonl();
		refresh();
	}
	~curse() {
		endwin();
	}
};

class window {
public:
	window(size window_size = { 120, 30 }, std::optional<point> begin_position = std::nullopt) :
		window_size_{ window_size },
		screen_size_{ getmaxx(stdscr),getmaxy(stdscr) },
		begin_position_{ (begin_position.has_value()) ? *begin_position : point({0,0}) },
		center_{ !begin_position.has_value() },
		win_{ make_window(window_size_) }
	{}

	auto get_size() const { return window_size_; }
	auto get_startpos() const { return begin_position_; }
	auto get_window() const { return win_; }

	auto move_horizontal(int size) {}
	auto move_vertical(int size) {}

	auto new_position(point pos) {}

private:
	size window_size_{};
	size screen_size_{};
	point begin_position_{};
	bool center_{};

	WINDOW* win_;

	WINDOW* make_window(size window_size) {
		return newwin(window_size.y, window_size.x, (center_) ? ((screen_size_.y - window_size.y) / 2) : begin_position_.y, (center_) ? ((screen_size_.x - window_size.x) / 2) : begin_position_.x);
	}
};
// bas klass för alla ui element
class ui_item {
public:

	// ärvs av alla härleda klasser

	void set_position(point position) { position_ = position; }
	point get_position() const { return position_; }
	void set_window(const window& win) { win_ = win; }
	window get_window() const { return win_; }

	void clear_item() const {
		for (auto y = position_.y; y < position_.y + get_size().y; ++y) {
			for (auto x = position_.x; x < position_.x + get_size().x; ++x) {
				mvwprintw(win_.get_window(), y, x, " ");
			}
		}
	}

	void redraw_item() const {
		clear_item();
		draw_item();
	}

	// måste defineras av härleda klasser

	virtual void draw_item() const = 0;
	virtual size get_size() const = 0;

	// måste inte omdefineras
	virtual ~ui_item() {}
protected:
	point position_;
	window win_;
};

class column_list : public ui_item {

};
// line element ex: -----------------
class line : public ui_item {
public:
	line(point position, int length, const window& win) : length_{ length }
	{
		set_position(position);
		set_window(win);
	}

	auto set_length(int length) {
		length_ = length;
	}

	auto get_length() const { return length_; }

	void draw_item() const {
		mvwhline(win_.get_window(), position_.y, position_.x, 0, length_);
	}

private:
	int length_;
};

// text = double kind of element ex: sum = 100.0
class int_text : public ui_item {
public:
	int_text(const window& win, const std::string& prefix, double initial_value, point position) :
		prefix_{ prefix },
		value_{ initial_value }
	{
		set_position(position);
		set_window(win);
	}

	auto set_prefix(const std::string& prefix) { prefix_ = prefix; }
	auto get_prefix() const { return prefix_; }

	auto set_value(double value) { value_ = value; }
	auto get_value() const { return value_; }

	auto update_prefix(const std::string& prefix) {
		clear_item();
		set_prefix(prefix);
		draw_item();
	}

	auto update_value(double value) {
		clear_item();
		set_value(value);
		draw_item();
	}

	auto update(const std::string& prefix, double value) {
		clear_item();
		set_prefix(prefix);
		set_value(value);
		draw_item();
	}

	size get_size() const {
		return {static_cast<int>(std::string(prefix_ + " = " + std::to_string(value_)).length()), 1}; 
	}

	void draw_item() const {
		mvwprintw(win_.get_window(), position_.y, position_.x, std::string(prefix_ + " = " + std::to_string(value_)).c_str());
	}

private:
	std::string prefix_;
	double value_;
};

class text : public ui_item {
public:
	text(const window& win, const std::string& text, point position) : text_{ text } {
		set_position(position);
		set_window(win);
	}

	auto set_text(const std::string& text) { text_ = text; }
	auto get_text() const { return text_; }

	size get_size() const { return { static_cast<int>(text_.length()), 1 }; }
	
	auto update_text(const std::string& text) {
		clear_item();
		set_text(text);
		draw_item();
	}

	void draw_item() const {
		mvwprintw(win_.get_window(), position_.y, position_.x, text_.c_str());
	}

private:
	std::string text_;
};

class menu_ui {

};

int main() {
	kurs krs("ENG", "GY", "ENGELSKA", 100, 'A');
	kurs krs1("ENG", "GY", "MATTE", 100, 'B');

	program prg({ krs,krs1 });

	curse();

	window win({ 20,30 });

	box(win.get_window(), 0, 0);

	wrefresh(win.get_window());

	clear();

	mvwin(win.get_window(), 0, 0);

	refresh();

	text a(win, "TEZT", { 2,1 });
	text b(win, "kys", { 0,0 });


	b.draw_item();

	a.draw_item();

	a.update_text("H");

	wrefresh(win.get_window());

	getch();
}