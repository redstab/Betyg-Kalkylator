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

};

template<int col, typename T>
inline editor_list<col, T>::editor_list(const window& win, list<col, T>* lst, selection_type type) : list_traverser<col, T>(win, lst, type) {}

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

		//if ((this->cursor_.y == this->list_->get_max_rows()) and this->cursor_.y != 0) {
		//	this->move_cursor(direction::up);
		//}
		//else if (this->cursor_.y == 0) {
		//	//add_empty_entry();
		//}

		this->select();

	}
}
