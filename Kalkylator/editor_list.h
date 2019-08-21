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
	std::string get_input(std::string begin, point position, int begin_pos, int length);

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
	auto col_length = this->list_->get_data_length().at(this->cursor_.x);

	std::vector<row<col, T>>& rows = this->list_->get_rows();

	row<col, T>& row = rows.at(this->cursor_.y);

	point col_pos = row.columns.at(this->cursor_.x).position;

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
	}
	else {
		this->selection_length_ = col_length;
		col_text = get_col_txt();
		begin_cursor_pos = col_text.length();
	}

	// redraw and reselect

	this->list_->redraw_element();
	this->set_selection_type(selection_type::column_selection);

	this->select();

	set_col_txt(get_input(col_text, col_pos, begin_cursor_pos, col_length));

	this->list_->redraw_element();

	this->reselect();



	//// clear column if default else edit 
	//if (this->list_->get_elements().at(this->cursor_.y) == T()) {
	//	

	//	this->list_->redraw_element();
	//	this->set_selection_type(selection_type::column_selection);

	//	this->reselect();
	//	
	//	set_col_txt(get_input("", col_pos, 0, col_length));
	//	
	//	this->list_->redraw_element();

	//	this->reselect();


	//}

	//else {

	//	std::string padding = std::string((col_length - static_cast<int>(get_col_txt().length())), ' ');

	//	int begin_txt_pos = get_col_txt().length();

	//	set_col_txt(std::string(get_col_txt() + padding));
	//	this->list_->redraw_element();
	//	this->set_selection_type(selection_type::column_selection);
	//	this->reselect();

	//	set_col_txt(get_input(get_col_txt(), col_pos, begin_txt_pos, col_length));

	//	this->list_->redraw_element();

	//	this->reselect();

	//}


}

template<int col, typename T>
inline std::string editor_list<col, T>::get_input(std::string begin, point position, int begin_pos, int length)
{

	//highlight column

	std::string output = begin;
	auto count = begin_pos;

	bool end = false;

	while (!end) {

		int key = getch();

		// if at max length and the user does not hit bs break, because it would overwrite or when user hits enter break

		if ((count >= length and !(key == 127 or key == '\b' or key == KEY_BACKSPACE)) or key == 13) { 
			break;
		}

		if (key >= 32 and key <= 126) { // if alnum() or special char
			
			output += key;
			
			mvwprintw(window_.get_window(), position.y, position.x+count, std::string(1, key).c_str());
			
			this->select();

			++count;

		}
		else if (key == 127 or key == '\b' or key == KEY_BACKSPACE) { // if backspace
			
			if (count != 0) { 
				
				mvwprintw(window_.get_window(), position.y, position.x + count - 1, " "); // remove last position
				
				output.erase(count - 1, 1); // erase from string
				
				count -= 1; // back by because of ++i in for loop
				this->select();
			}
		}
/*		else {
			count -= 1;
		}*/

	}
	return output;
}
