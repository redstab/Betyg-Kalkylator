// Calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include "program.h"
#include <curses.h>

struct coordinate {
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
		cbreak();
		curs_set(0);
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

	const void print(coordinate pnt, std::string msg) {
		mvprintw(pnt.y, pnt.x, msg.c_str());
	}

	const void wprint(WINDOW* w, coordinate pnt, std::string msg) {
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

class betyg_meny {
public:
	betyg_meny(std::string titel, program meny_program, int xmrg, int ymrg) :
		manager(),
		aktiv(meny_program),
		kurserna(aktiv.kurserna()),
		title(titel),
		xmargin((xmrg <= 30) ? xmrg : 30),
		ymargin((ymrg <= 5) ? ymrg : 5),
		xmax(getmaxx(stdscr)),
		ymax(getmaxy(stdscr)),
		box_begin{ xmargin, ymargin },
		box_end{ (xmax - smargin) - xmargin,(ymax - smargin) - ymargin },
		write_begin{ dmargin, smargin },
		write_end{ (box_begin - box_end).x - smargin, (box_begin - box_end).y - smargin },
		title_begin{ center_text((box_begin - box_end).x, title), smargin },
		first_divisor{ (box_begin - box_end).x / 3, write_end.y - smargin },
		last_divisor{ ((box_begin - box_end).x * 2) / 3, write_end.y - smargin },
		main_menu{ manager.window(box_begin - box_end, box_begin) }
	{}

	void show_ui() {
		show_border();
		show_lines();
		show_text();
		manager.refresh(main_menu);
	}

	void show_kurser() {
		for (auto i = 0; i < kurserna.size(); i++) {
			print_kurs(4 + i, kurserna.at(i));
		}
		manager.refresh(main_menu);
	}

private:

	// NCURSES Manager

	curse manager;

	// Strings

	const std::string title;
	const std::string snitt = "Snitt =     |";
	const std::string merit = "Merit =   |";
	const std::string summa = "Summa =     |";

	// Headers

	const std::string kurs_id = "Kurs";
	const std::string kurs_typ = "Kurstyp";
	const std::string kurs_namn = "Kursnamn";
	const std::string kurs_poäng = "Poäng";
	const std::string kurs_betyg = "Betyg";

	// Sizes

	const int xmargin;
	const int ymargin;

	const int skurs = 10;
	const int skurstyp = 7;
	const int skursnamn = 25;
	const int spoäng = 5;
	const int sbetyg = 5;

	const int dmargin = 2;
	const int smargin = dmargin / dmargin;
	const int xmax;
	const int ymax;

	// Coordinates

	const coordinate box_begin;
	const coordinate box_end;
	const coordinate write_begin;
	const coordinate write_end;

	const coordinate title_begin;

	const coordinate first_divisor;
	const coordinate last_divisor;

	const coordinate ckurs = { write_begin.x, dmargin };
	const coordinate ckurstyp = { ckurs.x + skurs + dmargin, dmargin };
	const coordinate ckursnamn = { ckurstyp.x + skurstyp + dmargin, dmargin };
	const coordinate cbetyg = {write_end.x - smargin - sbetyg , dmargin};
	const coordinate cpoäng = {cbetyg.x - dmargin - spoäng, dmargin};

	// Container

	WINDOW* main_menu;

	program aktiv;

	std::vector<kurs> kurserna;

	// Functions

	void show_border() {
		box(main_menu, 0, 0);
	}

	void show_text() {
		manager.wprint(main_menu, title_begin, title); // Title
		manager.wprint(main_menu, { center_text((write_begin - first_divisor).x, merit), write_end.y - smargin }, merit); // Merit
		manager.wprint(main_menu, { center_text((write_begin - write_end).x, snitt), write_end.y - smargin }, snitt); // Snitt
		manager.wprint(main_menu, { center_text((last_divisor - write_end).x, summa) + (write_begin - last_divisor).x, write_end.y - smargin }, summa); // Summa

		manager.wprint(main_menu, ckurs, kurs_id);
		manager.wprint(main_menu, ckurstyp, kurs_typ);
		manager.wprint(main_menu, ckursnamn, kurs_namn);

		manager.wprint(main_menu, cbetyg, kurs_betyg);
		manager.wprint(main_menu, cpoäng, kurs_poäng);
	}

	void show_lines() {
		manager.line(main_menu, { write_begin.x, write_begin.y + dmargin }, { write_end.x - smargin, write_begin.y + dmargin });	// Horizontal header divider
		manager.line(main_menu, { write_begin.x, write_end.y - dmargin }, { write_end.x - smargin, write_end.y - dmargin });		// Horizontal results divider
		manager.line(main_menu, first_divisor, { first_divisor.x, first_divisor.y + smargin });										// Vertical merit divider
		manager.line(main_menu, last_divisor, { last_divisor.x, last_divisor.y + smargin });										// Vertical snitt/summa divider
	}

	void print_kurs(int y, kurs krs) {
		manager.wprint(main_menu, {ckurs.x , y }, krs.kurs_id);
		manager.wprint(main_menu, {ckurstyp.x , y }, krs.kurs_typ);
		manager.wprint(main_menu, {ckursnamn.x , y }, krs.kurs_namn);
		manager.wprint(main_menu, {cpoäng.x , y }, std::to_string(krs.kurs_längd));
		manager.wprint(main_menu, {cbetyg.x , y }, std::string(1,krs.betyg_bokstav));
	}

	int center_text(int size, std::string text) {
		return (size - text.length()) / 2;
	}

};

int main()
{

	// När man visar en kurs kalkylera om man höjer sitt betyg hur det påverkar snittet samma om man sänker.

	kurs matte("Matematik 1c", "MATMAT01c", "GYGEM", 'A', hel_kurs);

	kurs engelska("Engelska 5", "ENGENG05", "GYGEM", 'B', hel_kurs);

	std::vector<kurs> test = { matte, engelska };

	program a({ matte, engelska }, 0);

	betyg_meny m("Betyg Kalkylator", a, 4, 1);

	m.show_ui();

	m.show_kurser();

	getch();
}