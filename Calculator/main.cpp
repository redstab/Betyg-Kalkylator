// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include <any>
#include <typeinfo>
#include <initializer_list>
#include <functional>
#include <eh.h>
#include <Windows.h>
#include <locale>
#include "program.h"
#include <curses.h>

//template<typename T>
//class item {
//public:
//	item(std::string new_title, std::function<void(T)> func, T arg) : title(new_title), function(func), argument(arg) {}
//	std::string get_title() {
//		return title;
//	}
//	void set_argument(T arg) {
//		argument(arg);
//	}
//
//	T get_argument() {
//		return argument;
//	}
//
//	void set_title(std::string new_title) {
//		title = new_title;
//	}
//
//	void execute(T arg) {
//		function(arg);
//	}
//	void operation(std::function<void(T)> func) {
//		function = func;
//	}
//private:
//	std::string title;
//	std::function<void(T)> function;
//	T argument;
//};
//template<>
//class item<void> {
//public:
//	item(std::string new_title, std::function<void()> func) : title(new_title), function(func) {}
//	std::string get_title() {
//		return title;
//	}
//	void set_title(std::string new_title) {
//		title = new_title;
//	}
//	void execute() {
//		function();
//	}
//	void operation(std::function<void()> func) {
//		function = func;
//	}
//private:
//	std::string title;
//	std::function<void()> function;
//};
//
//struct menu {
//	menu(std::string title, std::vector<std::any> items) {
//		menu_title = title;
//
//		if (items.size() == 0) {
//			menu_items.push_back(item<void>("", [] {})); // If menu has no items, this will backfire before you know it
//		}
//		else {
//			menu_items = items;
//		}
//	};
//	std::vector<std::any> menu_items;
//	std::string menu_title;
//
//};
//
//template<typename T> class homogen_handler {
//public:
//	homogen_handler(menu m) : handled_menu(m), display_menu(m) {
//		menu_trail.push_back(display_menu);
//		alive = true;
//		initscr();
//		noecho();
//		curs_set(0);
//		keypad(stdscr, TRUE);
//	}
//
//	void operator()() {
//		std::any selected_item = display_menu.menu_items[current_selection];
//
//		if (selected_item.type() == t_value) {
//			auto itm = std::any_cast<item<T>>(selected_item);
//			itm.execute(itm.get_argument());
//		}
//		else if (selected_item.type() == t_function) {
//			std::any_cast<item<void>>(selected_item).execute();
//		}
//		else if (selected_item.type() == t_menu) {
//
//			descend();
//		}
//
//	}
//
//	void operator[](int selection) {
//		set_selection(-1);
//		current_selection = selection;
//		set_selection(current_selection);
//	}
//
//	homogen_handler& operator++(int) {
//		if (current_selection == display_menu.menu_items.size() - 1) {
//			set_selection(-1);
//			current_selection = 0;
//			set_selection(current_selection);
//		}
//		else {
//			set_selection(-1);
//			current_selection++;
//			set_selection(current_selection);
//		}
//		return *this;
//	}
//
//	homogen_handler& operator--(int) {
//		if (current_selection == 0) {
//			set_selection(-1);
//			current_selection = display_menu.menu_items.size() - 1;
//			set_selection(current_selection);
//		}
//		else {
//			set_selection(-1);
//			current_selection--;
//			set_selection(current_selection);
//		}
//		return *this;
//	}
//
//	homogen_handler& operator+=(std::any _item) {
//
//		if (_item.type() == t_value || _item.type() == t_function || _item.type() == t_menu) {
//			menu_trail.back().menu_items.push_back(_item);
//			display_menu = menu_trail.back();
//		}
//		return *this;
//	}
//
//	homogen_handler& operator+(std::any _item) {
//		operator+=(_item);
//		return *this;
//	}
//
//
//	/*
//
//	Needs +=/+ operator to add a item at runtime at bottom
//	Needs -=/- operator to remove a specifc item at runtime anywhere in the menu recursively
//	Needs implementation with rat !!!!
//
//
//	*/
//
//
//	bool isalive() {
//		return alive;
//	}
//
//	int get_key() {
//		return getch();
//	}
//
//	void show() {
//		men = center_box(2, getmaxy(stdscr), getmaxx(stdscr));
//
//		center_title(men, display_menu.menu_title);
//
//		print_menu(men, display_menu, -1);
//
//		set_selection(0);
//	}
//
//	~homogen_handler() {
//		endwin();
//	}
//
//	menu* get_menu() {
//		return &display_menu;
//	}
//
//	void set_selection(int selection) {
//		if (selection < 0) {
//			print_item(men, current_selection, false);
//			return;
//		}
//		current_selection = selection;
//		print_item(men, selection, true);
//		refresh();
//		wrefresh(men);
//	}
//
//	void descend() {
//		next_menu(display_menu, current_selection);
//	}
//
//	void ascend() {
//		next_menu(display_menu, -1);
//	}
//
//	std::string get_tail() {
//		std::string sep, output;
//		for (auto i_menu : menu_trail) {
//			output += sep + i_menu.menu_title;
//			sep = " -> ";
//		}
//		return output;
//	}
//
//private:
//	menu handled_menu;
//	menu display_menu;
//	std::vector<menu> menu_trail;
//
//	WINDOW* men;
//
//	bool alive;
//
//	int current_selection = 0;
//
//	const std::type_info& t_value = typeid(item<T>);
//	const std::type_info& t_function = typeid(item<void>);
//	const std::type_info& t_menu = typeid(menu);
//
//	WINDOW* new_box(int height, int width, int start_y, int start_x) {
//		WINDOW* local;
//		local = newwin(height, width, start_y, start_x);
//		box(local, 0, 0);
//		return local;
//	}
//
//	WINDOW* center_box(int margin, int y_max, int x_max) {
//		return new_box(y_max - margin * 2, x_max - (margin * 4), margin, margin * 2);
//	}
//
//	void center_title(WINDOW* win, std::string title) {
//		title = " " + title + " ";
//		mvwprintw(win, 0, ceil((getmaxx(win) - title.size()) / 2.0), title.c_str());
//	}
//
//	void print_item(WINDOW* win, int index, bool selected) {
//		std::any current_item = display_menu.menu_items[index];
//
//		std::string title;
//
//		if (current_item.type() == t_value) {
//			title = std::any_cast<item<T>>(current_item).get_title();
//		}
//		else if (current_item.type() == t_function) {
//			title = std::any_cast<item<void>>(current_item).get_title();
//		}
//		else if (current_item.type() == t_menu) {
//			title = std::any_cast<menu>(current_item).menu_title;
//		}
//		if (selected) {
//
//			wattron(win, A_STANDOUT);
//			mvwprintw(win, 1 + index, 2, title.c_str());
//			wattroff(win, A_STANDOUT);
//			current_selection = index;
//		}
//		else {
//			mvwprintw(win, 1 + index, 2, title.c_str());
//		}
//
//	}
//
//	void print_menu(WINDOW* win, menu men, int selected) {
//		for (auto i = 0; (i < men.menu_items.size()); i++) {
//			if (i == selected) {
//				print_item(win, i, true);
//			}
//			else {
//				print_item(win, i, false);
//			}
//		}
//	}
//
//	void redraw_menu(WINDOW* _menu) {
//		wclear(_menu);
//		wrefresh(_menu);
//		show();
//	}
//
//	void next_menu(menu _menu, int selection) {
//		if (selection == -1) {
//			if (menu_trail.size() >= 2) {
//				display_menu = (menu_trail.end()[-2]);
//				menu_trail.erase(menu_trail.end() - 1);
//				redraw_menu(men);
//			}
//		}
//		else if (selection < _menu.menu_items.size() && _menu.menu_items[selection].type() == t_menu) {
//			display_menu = std::any_cast<menu>(_menu.menu_items[selection]);
//			menu_trail.push_back(display_menu);
//			redraw_menu(men);
//		}
//	}
//};
//
//menu program_to_menu(program prg) {
//	std::vector<std::any> v;
//	for (auto krs : prg.kurserna()) {
//		v.push_back(item<kurs>(krs.namn(), [&](kurs a) {mvprintw(0,0, "1"); }, krs));
//	}
//	return menu("Calcualtor", v);
//}

struct coordinate {
	int x;
	int y;
};

class curse {
public:
	curse(){
		initscr();
		noecho();
		cbreak();
		curs_set(0);
	}
	~curse() {
		endwin();
	}

	void refresh(WINDOW* w) {
		::refresh();
		wrefresh(w);

	}	

	void refresh() {
		::refresh();
	}
	
	void vline(coordinate ths, coordinate tht) {
		mvvline(ths.y, ths.x, 0, ths.y - tht.y);
	}

	void hline(coordinate ths, coordinate tht) {
		mvhline(ths.y, ths.x, 0, ths.x - tht.x);
	}

	void print(coordinate pnt, std::string msg) {
		mvprintw(pnt.y, pnt.x, msg.c_str());
	}

	void wprint(WINDOW* w, coordinate pnt, std::string msg) {
		mvwprintw(w, pnt.y, pnt.x, msg.c_str());
	}

	WINDOW * window(int sx, int sy, int bx, int by) {
		return newwin(sy, sx, by, bx);
	}

	WINDOW* swindow(WINDOW* parent, int sx, int sy, int bx, int by) {
		return subwin(parent, sy, sx, by, bx);
	}

	WINDOW* dwindow(WINDOW* parent, int sx, int sy, int bx, int by) {
		return derwin(parent, sy, sx, by, bx);
	}

};

class betyg_meny {
	betyg_meny(std::string titel, program meny_program, int margin) : title(titel), begin{ margin * 2, margin }, end{ COLS - (margin * 2),LINES - margin }, upcorner{ end.x, begin.y }, downcorner{begin.x, end.y} {}

private:

	// Constants
	const std::string title;
	const coordinate begin;
	const coordinate end;
	const coordinate upcorner;
	const coordinate downcorner;
};

int main()
{

	// När man visar en kurs kalkylera om man höjer sitt betyg hur det påverkar snittet samma om man sänker.

	kurs matte("Matematik 1c", "MATMAT01c", "GYGEM", 'A', hel_kurs);

	kurs engelska("Engelska 5", "ENGENG05", "GYGEM", 'B', hel_kurs);

	std::vector<kurs> test = { matte, engelska };

	program a({ matte, engelska }, 0);
	
	std::locale::global(std::locale("sv"));
	
	initscr();

	noecho();

	cbreak();

	curs_set(0);

	std::string title = "Betyg Kalkylator";

	std::string merit = "Merit =    ";

	std::string snitt = "Snitt =      ";

	std::string summa = "Summa =      ";

	

	int margin = 2;

	int max_x;

	int max_y;

	getmaxyx(stdscr, max_y, max_x);

	WINDOW* test1 = newwin(max_y - margin, max_x - margin * 2, margin / 2, margin);

	box(test1, 0, 0);

	mvwhline(test1, test1->_begy + 2, test1->_begx, 0, test1->_maxx - margin * 2);

	mvwhline(test1, test1->_maxy - 3, test1->_begx, 0, test1->_maxx - margin * 2);

	mvwprintw(test1, 1, (test1->_maxx - title.length()) / 2, title.c_str());

	mvwvline(test1, test1->_maxy - 2, test1->_maxx / 3, 0, 1);

	mvwvline(test1, test1->_maxy - 2, (test1->_maxx / 3) * 2, 0, 1);

	mvwprintw(test1, test1->_maxy - 2, ((test1->_maxx / 3) - merit.length()) / 2, merit.c_str());

	mvwprintw(test1, test1->_maxy - 2, ((test1->_maxx) - snitt.length()) / 2, snitt.c_str());
	
	mvwprintw(test1, test1->_maxy - 2, (((test1->_maxx * 5) / 3) - summa.length()) / 2, summa.c_str());

	mvwprintw(test1, test1->_begy + 1, test1->_begx,      "Kurs");
	mvwprintw(test1, test1->_begy + 1, test1->_begx + 13, "Kurstyp");
	mvwprintw(test1, test1->_begy + 1, test1->_begx + 24, "Kursnamn");
	mvwprintw(test1, test1->_begy + 1, test1->_maxx - 15, "Poäng");
	mvwprintw(test1, test1->_begy + 1, test1->_maxx - 7,  "Betyg");

	for (auto i = 0; i < a.kurserna().size(); i++) {

		kurs temp = a.kurserna().at(i);

		std::string pob = std::to_string(temp.kurs_längd) + "     " + temp.betyg_bokstav + "   ";

		std::string print_kurs = temp.kurs_id + std::string(13-temp.kurs_id.length(), ' ') + temp.kurs_typ + "      " + temp.kurs_namn + std::string(test1->_maxx - (temp.kurs_id + std::string(13 - temp.kurs_id.length(), ' ') + temp.kurs_typ + "       " + temp.kurs_namn + pob).length() - (margin+margin), ' ') + pob;

		mvwprintw(test1, test1->_begy + 3 + i, margin, print_kurs.c_str());

	}

	refresh();

	wrefresh(test1);

	getch();

	endwin();

}