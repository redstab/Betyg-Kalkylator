
#include "precompile.h"
#include "title.h"
#include "window.h"
#include "curse.h"
#include "program.h"
#include "line.h"
#include "header.h"
#include "list.h"
#include "kurs.h"
#include "editor_list.h"
/*
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
	auto get_kurser() { return &kurser_; }

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

	auto show_border() const {
		wborder(win_, 0, 0, 0, 0, 0, 0, 0, 0);
		wrefresh(win_);
	}

	auto hide_border() const {
		wborder(win_, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		wrefresh(win_);
	}

	auto move_window(point pos) const {
		clear();
		mvwin(get_window(), pos.y, pos.x);
		refresh();
	}

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

	void move_item(point new_position) {
		clear_item();
		set_position(new_position);
		draw_item();
	}

	void clear_item() const {
		for (auto y = position_.y; y < position_.y + get_size().y; ++y) {
			for (auto x = position_.x; x < position_.x + get_size().x; ++x) {
				mvwprintw(win_.get_window(), y, x, " ");
			}
		}
	}

	void redraw_item() {
		clear_item();
		draw_item();
	}

	// måste defineras av härleda klasser

	virtual void draw_item() = 0;
	virtual size get_size() const = 0;
	// måste inte omdefineras
	virtual ~ui_item() {}
protected:
	point position_{};
	window win_;
};

// Generic UI Elements

// line element ex: -----------------
class line : public ui_item {
public:
	line(const window& win, point position, int length) : length_{ length }
	{
		set_position(position);
		set_window(win);
	}

	auto set_length(int length) { length_ = length; }

	auto get_length() const { return length_; }

	size get_size() const { return { length_, 1 }; }

	void draw_item() {
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

	size get_size() const {
		return { static_cast<int>(std::string(prefix_ + " = " + std::to_string(value_)).length()), 1 };
	}

	void draw_item() {
		mvwprintw(win_.get_window(), position_.y, position_.x, std::string(prefix_ + " = " + std::to_string(value_)).c_str());
	}

private:
	std::string prefix_;
	double value_;
};



// text ex: hello
class text : public ui_item {
public:
	text()
	{
		set_position({ 0,0 });
	}

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

	void draw_item() {
		mvwprintw(win_.get_window(), position_.y, position_.x, text_.c_str());
	}

private:
	std::string text_;
};

class center_title : public ui_item {
public:
	center_title(const window& win, const std::string& title, point position, int max_x_size) :
		title_{ title },
		max_x_size_{ max_x_size }
		//center_position{calculate_center(title_, max_x_size_), position_.y}
	{
		set_position(position);
		set_window(win);
	}

	std::string get_title() const {
		return title_;
	}

	void set_title(const std::string& title) {
		title_ = title;
	}

	int get_max_x() const {
		return max_x_size_;
	}

	void set_max_x(int max_x) {
		max_x_size_ = max_x;
	}

	size get_size() const {
		return {max_x_size_, 1};
	}

	void draw_item() {
		if(position_.y == 0) win_.show_border();
		center_position = { calculate_center(title_, max_x_size_), position_.y};
		mvwprintw(win_.get_window(), center_position.y, center_position.x, (" " + title_ + " ").c_str());
	}


private:
	std::string title_;
	int max_x_size_;

	point center_position;

	int calculate_center(const std::string& txt, int max) {
		return (max - txt.length()) / 2;
	}
};

struct header_item {
	std::string txt;
	int write_limit;
};

template <const int n> class header : public ui_item {
public:
	header(const window& win, std::array<header_item, n> header_text, point position, int max_size, int seperation) :
		max_size_{ max_size },
		header_text_{ header_text },
		seperation_point_{ seperation }
	{
		set_position(position);
		set_window(win);
		set_header_windows();
	}

	std::array<text, n> get_header_texts() const {
		return header_text_;
	}

	void set_header_texts(const std::array<header_item, n>& header_texts) {
		header_text_ = header_texts;
	}

	header_item& access_header_text(int header_index) {
		return header_text_.at(header_index);
	}

	std::array<point, n> get_header_positions() const {
		return positions_;
	}

	int get_max_size() const {
		return max_size_;
	}

	void set_max_size(int max_size) {
		max_size_ = { max_size, 1 };
	}

	size get_size() const {
		return {max_size_.x - position_.x, 1};
	}

	void set_seperation(int seperation_point) {
		seperation_point_ = seperation_point;
	}

	size get_seperation() const {
		return seperation_point_;
	}

	void draw_item() {
		clear_item();
		update_header_texts();
		calculate_positions(headers_);
		update_positions();

		for (auto i : headers_) {
			i.draw_item();
		}
	}

private:
	size max_size_;
	std::array<text, n> headers_;
	std::array<header_item, n> header_text_;
	std::array<point, n> positions_;
	int seperation_point_;
	const int padding = 4;

	void update_header_texts() {
		for (auto i = 0; i < n; ++i) {
			headers_.at(i).set_text(header_text_.at(i).txt);
		}
	}

	void update_positions() {
		for (auto i = 0; i < n; ++i) {
			headers_.at(i).set_position(positions_.at(i));
		}
	}

	void set_header_windows() {
		for (auto& i : headers_) {
			i.set_window(win_);
		}
	}

	int size_of_header(const header_item& item) {
		return std::max(item.write_limit, static_cast<int>(item.txt.length()));
	}

	point calculate_position(std::array<text, n> headers, int index, int& tracker) {

		int header_size = size_of_header(header_text_.at(index));

		point position;

		if (index >= seperation_point_) {
			tracker -= header_size + padding;
		}

		position = { tracker, position_.y };

		if (index < seperation_point_) {
			tracker += header_size + padding;
		}

		return position;

	}

	void calculate_positions(std::array<text, n> headers) {

		int tracker = position_.x + padding ; // + 1 because of size vs 0 indexing

		int reverse_tracker = max_size_.x; // + 1 because of size vs 0 indexing

		for (int i = 0; i < n; ++i) {

			positions_.at(i) = calculate_position(headers, i, (i < seperation_point_) ? tracker : reverse_tracker);
			// select tracker when it has not reached separation else choose reverse_tracker

		}
	}
};

// Specialized UI Elements

class kurs_list : public ui_item {
public:
	kurs_list(const window& win, const program& prg, point position, size max_size) :
		max_{ max_size },
		aktiv_program_{ prg },
		kurser_{ aktiv_program_.get_kurser() }
	{
		set_window(win);
		set_position(position);
	}

	size get_size() const {
		return { max_.x - position_.x, max_.y - position_.y };
	}

	void draw_item() {

	}

private:
	size max_;
	program aktiv_program_;
	std::vector<kurs>* kurser_;


};

class menu_ui {

};*/

int main() {
	{

	curse c;

	kurs engelska("ENGENG05", "GYGEM", "Engelska 5", 100, 'A');
	kurs historia("HISHIS01a1", "GYGEM", "Historia 1a1", 50, 'B');

	program aae({ engelska, historia });

	window win({ 113,25 });

	//win.hide_border();
	win.show_border();
	//refresh();
	header<5> s(win,
		{
			table_header(win, "Kurs", 10),
			table_header(win, "Kurstyp", 7),
			table_header(win, "Kursnamn", 30),
			table_header(win, "Poäng", 3),
			table_header(win, "Betyg", 1)
		}, { 1,2 }, win.get_size().x-1, 3);

	s.redraw_element();

	auto position = s.get_header_positions();

	list<5, kurs> aass(win, { engelska, historia }, {
			column<kurs>(win, position[0], &kurs::get_id, &kurs::set_id, string{}),
			column<kurs>(win, position[1], &kurs::get_typ, &kurs::set_typ, string{}),
			column<kurs>(win, position[2], &kurs::get_namn, &kurs::set_namn, string{}),
			column<kurs>(win, position[3], &kurs::get_poäng, &kurs::set_poäng, integer{50}),
			column<kurs>(win, position[4], &kurs::get_betyg, &kurs::set_betyg, character{ 'A', 'F' })
		}, &s,
		{ 1,4 });
	

	line b(win, { 5, 3 }, win.get_size().x - 10, orientation::horizontal);

	editor_list<5, kurs> hh(win, &aass, selection_type::column_selection);

	//hh.move_row(1);



	//hh.move_row(0);

	//hh.move_column(0);

	b.draw_element();

	aass.draw_element();

	//aass.pop_item(engelska);

	aass.redraw_element();

	title a(win, " Window Title ", 1);
	//a.set_text(" TET TILE ");
	//line b(win, {6, 7}, 30, orientation::horizontal);
	//b.set_orientation(orientation::vertical);
	//text d(win, "hellop", { 8, 7 });
	//a.draw_element();
	//b.draw_element();
	//d.draw_element();
	//a.set_text(" t ");

	//d.set_text("t");
	hh.select();
	a.draw_element();
	wrefresh(win.get_window());

	int key{ 0 };

	while ((key = getch()) && key != 'q') {
		switch (key) {
		case KEY_UP:
			hh.move_cursor(direction::up);
			break;
		case KEY_DOWN:
			hh.move_cursor(direction::down);
			break;
		case KEY_RIGHT:
			hh.move_cursor(direction::right);
			break;
		case KEY_LEFT:
			hh.move_cursor(direction::left);
			break;
		case 9:
			hh.swap_selection_type();
			break;
		case '+':
			hh.add_empty_entry();
			break;
		case '-':
			hh.remove_entry();
			break;
		case KEY_HOME:
			hh.edit_column();
			break;
		}

		wrefresh(win.get_window());

	}

	//b.redraw_element();
	//d.redraw_element();
	wrefresh(win.get_window());
	/*kurs engelska("ENGENG05", "GYGEM", "Engelska 5", 100, 'A');
	kurs historia("HISHIS01a1", "GYGEM", "Historia 1a1", 50, 'B');

	program prg({ engelska, historia });

	curse c;

	window win({ 113,25 });

	win.show_border();

	center_title b(win, "HELLO ME TITLE", { 0,0 }, 113);
	center_title f(win, "HELLO  TITLE", { 0,1 }, 113);
	center_title d(win, " ME TITLE", { 0,2 }, 113);


	header<5> a(win,
		{
			header_item{"Kurs", 10},
			header_item{"Kurstyp", 7},
			header_item{"Kursnamn", 8},
			header_item{"Po„ng", 3},
			header_item{"Betyg", 1}
		}, { 1,3 }, 112, 3);

	std::vector<ui_item*> list{&a,&b,&f,&d};

	for (auto i : list) {
		i->draw_item();
		wrefresh(win.get_window());
	}

	wrefresh(win.get_window());

	getch();*/
}

_CrtDumpMemoryLeaks();

}
