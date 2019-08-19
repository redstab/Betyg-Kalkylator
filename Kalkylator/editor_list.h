#pragma once
#include "precompile.h"
#include "list_traverser.h"



template<int col, typename T> class editor_list : public list_traverser<col, T> {
public:
	editor_list(const window& win, list<col, T>* lst, selection_type type);

	void enter_editing_mode();
	void exit_editing_mode();
	void swap_mode();

	void add_empty_entry();

	void remove_entry();

	void edit_entry();

	void edit_column();

private:
	window window_;
	std::string get_input(point position, int length);

};

template<int col, typename T>
inline editor_list<col, T>::editor_list(const window& win, list<col, T>* lst, selection_type type) : list_traverser<col, T>(win, lst, type), window_(win) {}

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
inline void editor_list<col, T>::edit_column()
{
	auto col_length = this->list_->get_data_length()[this->cursor_.x];

	std::vector<row<col, T>>& rows = this->list_->get_rows();

	row<col, T>& row = rows.at(this->cursor_.y);

	point col_pos = row.columns.at(this->cursor_.x).position;

	std::string(T:: * str_get)() const = this->list_->get_function_pointer().at(this->cursor_.x).getter;

	void (T:: * str_set)(const std::string&) = this->list_->get_function_pointer().at(this->cursor_.x).setter;

	auto set_col_txt = [&](const std::string& str) {(row.object.*str_set)(str); };


	// clear column if default else edit 
	if (this->list_->get_elements().at(this->cursor_.y) == T()) {
		set_col_txt(std::string(col_length, ' '));

		this->list_->redraw_element();
		this->set_selection_type(selection_type::column_selection);
		this->update_length();
		this->select();

		set_col_txt(get_input(col_pos, col_length));
	}
	else {

	}


}

template<int col, typename T>
inline std::string editor_list<col, T>::get_input(point position, int length)
{
	std::string output;
	for (auto i = 0; i < length; ++i) {

		int key = getch();

		if (key >= 32 and key <= 126) {
			
			output += key;
			
			mvwprintw(window_.get_window(), position.y, position.x+i, std::string(1, key).c_str());
			
			this->select();
			wrefresh(window_.get_window());

		}
	}
	return output;
}
