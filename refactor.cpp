// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vector>
#include <string>
#include <Windows.h>
#include "program.h"
#include <curses.h>
#define ctrl(x)           ((x) & 0x1f)

class coordinate {
public:
	int x;
	int y;

	coordinate operator-(coordinate const& that) const {
		return { abs(x - that.x) + 1, abs(y - that.y) + 1 };
	}
};

class curse {
public:
	curse() {
		initscr();
		noecho();
		curs_set(0);
		keypad(stdscr, TRUE);
		nonl();
	}
	~curse() {
		endwin();
	}

	const void refresh(WINDOW* w) {
		::refresh();
		wrefresh(w);

	}

	const void refresh() {
		::refresh();
	}

	const void line(WINDOW* w, coordinate ths, coordinate tht) {
		if (ths.x == tht.x) {
			vline(w, ths, tht);
		}
		else if (ths.y == tht.y) {
			hline(w, ths, tht);
		}
	}

	const void vline(WINDOW* w, coordinate ths, coordinate tht) {
		mvwvline(w, ths.y, ths.x, 0, abs(ths.y - tht.y));
	}

	const void hline(WINDOW* w, coordinate ths, coordinate tht) {
		mvwhline(w, ths.y, ths.x, 0, abs(ths.x - tht.x));
	}

	const void print(coordinate pnt, const std::string& msg) {
		mvprintw(pnt.y, pnt.x, msg.c_str());
	}

	const void wprint(WINDOW* w, coordinate pnt, const std::string& msg) {
		mvwprintw(w, pnt.y, pnt.x, msg.c_str());
	}

	WINDOW* window(int sx, int sy, int bx, int by) {
		return newwin(sy, sx, by, bx);
	}

	WINDOW* window(coordinate size, coordinate pnt) {
		return newwin(size.y, size.x, pnt.y, pnt.x);
	}

	WINDOW* swindow(WINDOW* parent, int sx, int sy, int bx, int by) {
		return subwin(parent, sy, sx, by, bx);
	}

	WINDOW* dwindow(WINDOW* parent, int sx, int sy, int bx, int by) {
		return derwin(parent, sy, sx, by, bx);
	}

	void dot(WINDOW* w, coordinate pnt) {
		wprint(w, pnt, "#");
	}

};

class menu_traverser {

};

class menu_ui {

};

class betyg_meny {
public:
	betyg_meny(const std::string& titel, const program& meny_program, int xmrg, int ymrg) :
		manager(),
		title{ titel },
		xmargin{ (xmrg <= 30) ? xmrg : 30 },
		ymargin{ (ymrg <= 5) ? ymrg : 5 },
		xmax{ getmaxx(stdscr) },
		ymax{ getmaxy(stdscr) },
		box_begin{ xmargin, ymargin },
		box_end{ (xmax - smargin) - xmargin,(ymax - smargin) - ymargin },
		write_begin{ dmargin + smargin, smargin },
		write_end{ (box_begin - box_end).x - dmargin, (box_begin - box_end).y - smargin },
		title_begin{ center_text((box_begin - box_end).x, title), smargin },
		first_divisor{ (box_begin - box_end).x / 3, write_end.y - smargin },
		last_divisor{ ((box_begin - box_end).x * 2) / 3, write_end.y - smargin },
		main_menu{ manager.window(box_begin - box_end, box_begin) },
		aktiv{ meny_program },
		kurserna{ aktiv.kurserna() }
	{}

	void show_ui() {
		show_border();
		show_lines();
		show_text();
		manager.refresh(main_menu);
	}

	void show_kurser() {

		// Clear
		for (auto i = dmargin * dmargin; i <= kurserna->size() + dmargin * dmargin; i++) {
			for (auto j = write_begin.x; j < write_end.x; j++) {
				manager.wprint(main_menu, { j,i }, " ");
			}
		}

		// Print
		for (auto i = 0; i < kurserna->size(); i++) {
			print_kurs(kurserna->at(i), dmargin * 2 + i);
		}

		if (kurserna->size()) {
			select(selection);
		}
		else {
			manager.refresh(main_menu);
		}
	}

	// Operators

	betyg_meny& operator++(int) { // Go down list
		unselect(selection);

		if (selection == kurserna->size() - 1) {
			selection = 0;
			select(selection);
		}
		else {
			select(++selection);
		}
		return *this;
	}
	betyg_meny& operator--(int) { // Go up list
		unselect(selection);

		if (selection == 0) {
			selection = kurserna->size() - 1;
			select(selection);
		}
		else {
			select(--selection);
		}
		return *this;
	}

private:

	// Could refactor placing coords and strings in a map

	// NCURSES Manager

	curse manager;

	// Strings

	const std::string title;
	const std::string snitt_text = "Snitt =      ";
	const std::string merit_text = "Merit =    ";
	const std::string summa_text = "Summa =      ";

	// Headers

	const std::string kurs_id = "Kurs";
	const std::string kurs_typ = "Kurstyp";
	const std::string kurs_namn = "Kursnamn";
	const std::string kurs_poäng = "Poäng";
	const std::string kurs_betyg = "Betyg";

	// Sizes

	const int xmargin;
	const int ymargin;

	const int size_kurs = 10;
	const int size_kurstyp = 7;
	const int size_kursnamn = 25;
	const int size_poäng = 5;
	const int size_betyg = 5;

	const int dmargin = 2;
	const int smargin = 1;
	const int xmax;
	const int ymax;

	const int string_padding = 8;

	// Coordinates

	const coordinate box_begin;
	const coordinate box_end;
	const coordinate write_begin;
	const coordinate write_end;

	const coordinate title_begin;

	const coordinate first_divisor;
	const coordinate last_divisor;

	const coordinate coord_kurs = { write_begin.x, dmargin };
	const coordinate coord_kurstyp = { coord_kurs.x + size_kurs + dmargin, dmargin };
	const coordinate coord_kursnamn = { coord_kurstyp.x + size_kurstyp + dmargin, dmargin };
	const coordinate coord_betyg = { write_end.x - dmargin , dmargin };
	const coordinate coord_poäng = { coord_betyg.x - dmargin - size_poäng, dmargin };

	// Functionality

	WINDOW* main_menu;

	program aktiv;

	std::vector<kurs>* kurserna;

	// Variables 

	double merit{ 0 };

	int selection{ 0 };

	// Functions

	void set_merit(int mrt) {
		if (mrt >= 0 && mrt <= 2.5) {
			merit = mrt;
		}
	}

	void display_values() {
		display_merit();
		display_snitt();
		display_summa();
	}

	void display_merit() {
		std::string new_merit = merit_text.substr(0, string_padding) + std::to_string(merit);
		manager.wprint(main_menu, { center_text((write_begin - first_divisor).x, new_merit) + smargin, write_end.y - smargin }, new_merit);
	}

	void display_summa() {
		std::string new_summa = summa_text.substr(0, string_padding) + std::to_string(aktiv.program_poäng());
		manager.wprint(main_menu, { center_text((last_divisor - write_end).x, new_summa) + (write_begin - last_divisor).x + smargin, write_end.y - smargin }, new_summa); // Summa
	}

	void display_snitt() {
		std::string new_snitt = snitt_text.substr(0, string_padding) + std::to_string(aktiv.beräkna_snitt());
		manager.wprint(main_menu, { center_text((write_begin - write_end).x, new_snitt) + smargin, write_end.y - smargin }, new_snitt); // Snitt
	}

	void show_border() {
		box(main_menu, 0, 0);
	}

	void show_text() {

		// Text

		manager.wprint(main_menu, title_begin, title); // Title

		display_values(); // Snitt Summa Merit

		// Headers

		manager.wprint(main_menu, coord_kurs, kurs_id);
		manager.wprint(main_menu, coord_kurstyp, kurs_typ);
		manager.wprint(main_menu, coord_kursnamn, kurs_namn);

		manager.wprint(main_menu, { coord_betyg.x - size_betyg, coord_betyg.y }, kurs_betyg);
		manager.wprint(main_menu, { coord_poäng.x - size_betyg, coord_poäng.y }, kurs_poäng);
	}

	void show_lines() {
		manager.line(main_menu, { write_begin.x, write_begin.y + dmargin }, { write_end.x - smargin, write_begin.y + dmargin });
		manager.line(main_menu, { write_begin.x, write_end.y - dmargin }, { write_end.x - smargin, write_end.y - dmargin });
		manager.line(main_menu, first_divisor, { first_divisor.x, first_divisor.y + smargin });
		manager.line(main_menu, last_divisor, { last_divisor.x, last_divisor.y + smargin });
	}

	void print_kurs(const kurs& krs, int y) {
		manager.wprint(main_menu, { coord_kurs.x , y }, krs.kurs_id);
		manager.wprint(main_menu, { coord_kurstyp.x , y }, krs.kurs_typ);
		manager.wprint(main_menu, { coord_kursnamn.x , y }, krs.kurs_namn);
		manager.wprint(main_menu, { coord_poäng.x - dmargin - smargin, y }, std::to_string(krs.kurs_längd));
		if (krs.betyg_bokstav > 'A') {
			manager.wprint(main_menu, { coord_betyg.x - dmargin - smargin, y }, std::string(1, krs.betyg_bokstav) + " >");
		}
		else {
			manager.wprint(main_menu, { coord_betyg.x - dmargin - dmargin - smargin, y }, "< " + std::string(1, krs.betyg_bokstav));

		}
	}

	void select(int selec) {
		line_attr({ write_begin.x,(dmargin * dmargin) + selec }, (write_begin - write_end).x - dmargin, A_REVERSE);
	}

	void unselect(int line) {
		line_attr({ write_begin.x,(dmargin * dmargin) + line }, (write_begin - write_end).x - dmargin, A_NORMAL);
	}

	void line_attr(coordinate begin, int length, chtype attribute) {
		mvwchgat(main_menu, begin.y, begin.x, length, attribute, 0, 0);
		manager.refresh(main_menu);
	}

	int center_text(int size, const std::string& text) {
		return (size - text.length()) / 2;
	}

	void remove_kurs(const kurs& krs) {
		aktiv.operator-=(krs);
		show_kurser();
	}
};

int main()
{

	// När man visar en kurs kalkylera om man höjer sitt betyg hur det påverkar snittet samma om man sänker.

	kurs matte("Matematik 1c", "MATMAT01c", "GYGEM", 'A', hel_kurs);

	kurs engelska("Engelska 5", "ENGENG05", "GYGEM", 'B', hel_kurs);

	std::vector<kurs> test = { matte, engelska, engelska, engelska, engelska, engelska };

	program a(test, 0);

	betyg_meny m("Betyg Kalkylator", a, 12, 3);

	m.show_ui();

	m.show_kurser();

	int key{ 0 };

	while ((key = getch())) {
		switch (key) {
		case KEY_UP:
			m--;
			break;
		case KEY_DOWN:
			m++;
			break;
		case KEY_RIGHT:
			mvprintw(0, 0, ">");
			break;
		case ctrl('J'):
			mvprintw(0, 0, "<");
			break;
		}
	}
}