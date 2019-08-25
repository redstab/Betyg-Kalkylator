#pragma once
#include "precompile.h"
#include "list_traverser.h"

template<int col, typename T> class editor_list : public list_traverser<col, T> {
public:
	editor_list(const window& win, list<col, T>* lst, selection_type type, std::function<void()> on_update);

	void add_empty_entry();

	void remove_entry();

	void edit();

private:
	window window_;

	std::string get_input(std::string begin, point position, int begin_pos, int length, std::variant<string, integer, character> type);

	std::string get_string_input(std::string begin, point position, int begin_pos, int length);

	std::string get_int_input(std::string begin, point position, int begin_pos, int length, integer limit);

	std::string get_char_input(std::string begin, point position, int begin_pos, int length, character limit);

	void edit_row();

	void edit_column();

	std::function<void()> on_update_;

};

template<int col, typename T>
inline editor_list<col, T>::editor_list(const window& win, list<col, T>* lst, selection_type type, std::function<void()> on_update) : list_traverser<col, T>(win, lst, type), window_(win), on_update_{on_update} {}

template<int col, typename T>
inline void editor_list<col, T>::add_empty_entry()
{
	if (this->type_ == selection_type::row_selection) {
		this->list_->push_item(T());
		this->list_->draw_element();
		this->select();
	}
}

template<int col, typename T>
inline void editor_list<col, T>::remove_entry()
{
	if (this->type_ == selection_type::row_selection) {

		this->list_->pop_item(this->list_->get_elements().at(this->cursor_.y));
		this->list_->redraw_element();

		if ((this->cursor_.y == this->list_->get_max_rows()) and this->cursor_.y != 0) {
			this->move_cursor(direction::up);
		}
		else if (this->cursor_.y == 0) {
			add_empty_entry();
		}

		this->select();

	}
}

template<int col, typename T>
inline void editor_list<col, T>::edit()
{
	if (this->type_ == selection_type::row_selection) {
		edit_row();
	}
	else {
		edit_column();
	}
}

template<int col, typename T>
inline void editor_list<col, T>::edit_row()
{
	this->unselect();
	
	this->set_cursor_column(0);

	this->set_selection_type(selection_type::column_selection);

	for (auto i = 0; i < this->list_->get_column_positions().size(); ++i) {
		edit_column();
		this->move_cursor(direction::right);
	}

	this->set_selection_type(selection_type::row_selection);

}

template<int col, typename T>
inline void editor_list<col, T>::edit_column()
{
	auto col_length = this->list_->get_data_length().at(this->cursor_.x);

	std::vector<row<col, T>>& rows = this->list_->get_rows();

	row<col, T>& row = rows.at(this->cursor_.y);

	point col_pos = row.columns.at(this->cursor_.x).position;

	std::variant<string, integer, character> type = row.columns.at(this->cursor_.x).type;

	std::string(T:: * str_get)() const = this->list_->get_function_pointer().at(this->cursor_.x).getter;

	void (T:: * str_set)(const std::string&) = this->list_->get_function_pointer().at(this->cursor_.x).setter;

	auto set_col_txt = [&](const std::string& str) {(row.object.*str_set)(str); };
	auto get_col_txt = [&]() {return (row.object.*str_get)(); };


	// make selection whole column

	std::string col_text;

	int begin_cursor_pos = 0;

	if (this->list_->get_elements().at(this->cursor_.y) == T()) {
		set_col_txt(std::string(col_length, ' '));
		col_text = "";
		this->list_->redraw_element();
	}
	else {
		this->selection_length_ = col_length;
		col_text = get_col_txt();
		begin_cursor_pos = col_text.length();
	}

	this->set_selection_type(selection_type::column_selection);
	this->select();

	set_col_txt(get_input(col_text, col_pos, begin_cursor_pos, col_length, type));

	this->list_->redraw_element();

	this->reselect();
}

template<int col, typename T>
inline std::string editor_list<col, T>::get_input(std::string begin, point position, int begin_pos, int length, std::variant<string, integer, character> type)
{
	if (std::holds_alternative<string>(type)) {
		return get_string_input(begin, position, begin_pos, length);
	}
	else if (std::holds_alternative<integer>(type)) {
		return get_int_input(begin, position, begin_pos, length, std::get<integer>(type));
	}
	else {
		return get_char_input(begin, position, begin_pos, length, std::get<character>(type));
	}
}

template<int col, typename T>
inline std::string editor_list<col, T>::get_string_input(std::string begin, point position, int begin_pos, int length)
{

	//highlight column

	std::string output = begin;
	auto count = begin_pos;

	bool end = false;

	while (!end) {

		int key = getch();

		// if at max length and the user does not hit bs break, because it would overwrite or when user hits enter break

		if ((count >= length-1 and !(key == 127 or key == '\b' or key == KEY_BACKSPACE)) or key == 13) {
			end = true;
		}

		if (key >= 32 and key <= 126) { // if alnum() or special char

			output += key;

			mvwprintw(window_.get_window(), position.y, position.x + count, std::string(1, key).c_str());

			this->select();

			++count;

		}
		else if (key == 127 or key == '\b' or key == KEY_BACKSPACE) { // if backspace

			if (count != 0) {

				mvwprintw(window_.get_window(), position.y, position.x + count - 1, " "); // remove last position

				output.erase(count - 1, 1); // erase from string

				this->select();

				count -= 1; // back by because of ++i in for loop
			}
		}
		/*		else {
					count -= 1;
				}*/

		on_update_();

	}
	return output;
}

template<int col, typename T>
inline std::string editor_list<col, T>::get_int_input(std::string begin, point position, int begin_pos, int length, integer limit)
{
	int interval = limit.interval;

	int current_int = 0;

	if (std::all_of(begin.begin(), begin.end(), isdigit)) {
		current_int = std::stoi(begin);
	}

	bool end = false;

	while (!end) {

		int key = getch();

		if (key == 13) { // quit if user press enter
			end = true;
		}

		if (key == KEY_LEFT && current_int >= interval) { // decrease int
			current_int -= interval;
		}

		else if (key == KEY_RIGHT) { // increase int
			if ((std::trunc(std::log10(current_int + interval)) + 1) <= length) {
				current_int += interval;
			}
		}

		// Update screen

		mvwprintw(window_.get_window(), position.y, position.x, std::string(length, ' ').c_str());
		mvwprintw(window_.get_window(), position.y, position.x, "%d", current_int);
		
		this->select();

		(this->list_->get_rows().at(this->cursor_.y).object.*this->list_->get_function_pointer().at(this->cursor_.x).setter)(std::to_string(current_int));

		on_update_();
	}

	return std::to_string(current_int);

}

template<int col, typename T>
inline std::string editor_list<col, T>::get_char_input(std::string begin, point position, int begin_pos, int length, character limit)
{

	char output = begin[0];

	bool end = false;

	while (!end) {

		int key = getch();

		if (key == 13) { // quit if user press enter
			end = true;
		}

		if (key == KEY_LEFT && output < limit.to) { // decrease int
			output += 1;
		}

		else if (key == KEY_RIGHT && output > limit.from) { // increase int
			output -= 1;
		}

		// Update screen

		mvwprintw(window_.get_window(), position.y, position.x, std::string(length, ' ').c_str());
		mvwprintw(window_.get_window(), position.y, position.x, std::string(1,output).c_str());

		this->select();

		(this->list_->get_rows().at(this->cursor_.y).object.*this->list_->get_function_pointer().at(this->cursor_.x).setter)(std::string(1, output));

		on_update_();


	}
	return std::string(1, output);
}
