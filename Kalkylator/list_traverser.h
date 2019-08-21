#pragma once
#include "precompile.h"
#include "list.h"

enum class selection_type {
	column_selection,
	row_selection
};

enum class direction {
	up,
	down,
	left,
	right
};

template<int col, typename T> class list_traverser
{
public:
	list_traverser(const window& win, list<col, T>* lst, selection_type type);

	void set_list(list<col, T>* lst);

	void set_cursor_column(int cl);

	void set_cursor_row(int rw);

	void set_selection_type(selection_type type);

	void swap_selection_type();

	list<col, T>* get_list() const;

	direction get_selection_type();

	void select() const;
	void unselect() const;

	void reselect();

	void move_cursor(direction dir);

protected:

	point cursor_{ 0,0 };

	window win_;
	list<col, T>* list_;
	selection_type type_;
	point selection_start_;
	int selection_length_;

	const int padding_ = 4;

	void update_length();

};

template<int col, typename T>
inline list_traverser<col, T>::list_traverser(const window& win, list<col, T>* lst, selection_type type) : win_{ win }, list_{ lst }, type_{ type }, selection_start_{ list_->get_column_positions().at(0).x, list_->get_position().y }, selection_length_{ 0 } {update_length(); }

template<int col, typename T>
inline void list_traverser<col, T>::set_list(list<col, T>* lst)
{
	list_ = lst;
}

template<int col, typename T>
inline void list_traverser<col, T>::set_cursor_column(int cl)
{

	if (cl < 0) {
		cl = list_->get_column_positions().size() - 1;
	}
	else if (cl > static_cast<int>(list_->get_column_positions().size()) - 1) {
		cl = 0;
	}

	cursor_.x = cl;
}

template<int col, typename T>
inline void list_traverser<col, T>::set_cursor_row(int rw)
{

	if (rw < 0) {
		rw = list_->get_max_rows() - 1;
	}
	else if (rw > list_->get_max_rows() - 1) {
		rw = 0;
	}

	cursor_.y = rw;
}

template<int col, typename T>
inline void list_traverser<col, T>::set_selection_type(selection_type type)
{
	if (type != type_) {
		swap_selection_type();
	}
}

template<int col, typename T>
inline void list_traverser<col, T>::swap_selection_type()
{
	if (type_ == selection_type::column_selection) {
		type_ = selection_type::row_selection;
		selection_start_.x = list_->get_column_positions().at(0).x;
		unselect();
	}
	else {
		unselect();
		selection_start_.x = list_->get_column_positions().at(cursor_.x).x;
		type_ = selection_type::column_selection;
	}

	update_length();
	select();
}

template<int col, typename T>
inline list<col, T>* list_traverser<col, T>::get_list() const
{
	return &list_;
}

template<int col, typename T>
inline direction list_traverser<col, T>::get_selection_type()
{
	return type_;
}

template<int col, typename T>
inline void list_traverser<col, T>::select() const
{
	mvwchgat(win_.get_window(), selection_start_.y, selection_start_.x, selection_length_, A_REVERSE, 0, 0);
	wrefresh(win_.get_window());
}

template<int col, typename T>
inline void list_traverser<col, T>::unselect() const
{
	mvwchgat(win_.get_window(), selection_start_.y, selection_start_.x, selection_length_, A_NORMAL, 0, 0);
	wrefresh(win_.get_window());
}

template<int col, typename T>
inline void list_traverser<col, T>::reselect()
{
	unselect();
	update_length();
	select();
}

template<int col, typename T>
inline void list_traverser<col, T>::move_cursor(direction dir)
{
		unselect();

		if (dir == direction::left && type_ == selection_type::column_selection) {
			set_cursor_column(cursor_.x - 1);
		}
		else if (dir == direction::right && type_ == selection_type::column_selection) {
			set_cursor_column(cursor_.x + 1);
		}

		if (dir == direction::up) {
			set_cursor_row(cursor_.y - 1);
		}
		else if (dir == direction::down) {
			set_cursor_row(cursor_.y + 1);
		}

		selection_start_.y = cursor_.y + list_->get_position().y;

		selection_start_.x = list_->get_column_positions().at((type_ == selection_type::row_selection) ? 0 : cursor_.x).x;

		update_length();

		select();
}

template<int col, typename T>
inline void list_traverser<col, T>::update_length()
{
	
	int length_ = 0;

	if (type_ == selection_type::column_selection) {

		auto text_object = list_->get_elements().at(cursor_.y);
		auto strfunc_pointer = list_->get_function_pointer().at(cursor_.x);

		length_ = (text_object.*strfunc_pointer.getter)().length();
	}
	else {
		length_ = list_->get_length() - (padding_ * 2);
	}

	selection_length_ = length_;

}

