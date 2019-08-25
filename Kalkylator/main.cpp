
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

int main() {

	curse c;

	kurs engelska("ENGENG05", "GYGEM", "Engelska 5", 100, 'A');
	kurs historia("HISHIS01a1", "GYGEM", "Historia 1a1", 50, 'B');

	window win({ 113,25 });

	header<5> s(win,
		{
			table_header(win, "Kurs", 10),
			table_header(win, "Kurstyp", 7),
			table_header(win, "Kursnamn", 30),
			table_header(win, "Poäng", 3),
			table_header(win, "Betyg", 1)
		}, { 1,2 }, win.get_size().x-1, 3);


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

	program aae(aass.get_elements());
	
	editor_list<5, kurs> hh(win, &aass, selection_type::column_selection, [&]() {aae.set_kurser(aass.get_elements()); });

	title a(win, " Window Title ", 1);
	
	line seperation(win, { 5, win.get_size().y - 3 }, win.get_size().x - 10, orientation::horizontal);
	
	line hsep1(win, { (win.get_size().x / 3), win.get_size().y - 2 }, 1, orientation::vertical);
	
	line hsep2(win, { (win.get_size().x / 3) * 2, win.get_size().y - 2 }, 1, orientation::vertical);

	std::vector<ui_element*> elements{ &s, &b, &aass, &a, &seperation, &hsep1, &hsep2 };

	for (auto& element : elements) {
		element->draw_element();
	}
	
	win.show_border();
	hh.select();
	
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
			hh.edit();
			break;
		}

		wrefresh(win.get_window());

	}
}
