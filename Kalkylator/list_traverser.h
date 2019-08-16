#pragma once
#include "precompile.h"
#include "list.h"

enum class selection_type {
	column_selection,
	row_selection
};

template<int col, typename T> class list_traverser
{
public:
	list_traverser(const window& win, list<col, T>* lst, selection_type type);

	void set_list(list<col, T>* lst);

	list<col, T>* get_list() const;

	void select() const;
	void unselect() const;

	void move_row(int row);
	void move_column(int colum);
	void move_row_column(int row, int colum);

private:
	window win_;
	list<col, T>* list_;
	selection_type type_;
	point selection_start_;
	int selection_length_;
};

template<int col, typename T>
inline list_traverser<col, T>::list_traverser(const window& win, list<col, T>* lst, selection_type type) : win_{ win }, list_{ lst }, type_{ type } {}

template<int col, typename T>
inline void list_traverser<col, T>::set_list(list<col, T>* lst)
{
	list_ = lst;
}

template<int col, typename T>
inline list<col, T>* list_traverser<col, T>::get_list() const
{
	return &list_;
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
inline void list_traverser<col, T>::move_row(int row)
{

	unselect();

	// boundschecking

	if (row < 0) {
		row = list_->get_max_rows();
	}
	else if (row > list_->get_max_rows()) {
		row = 0;
	}

	selection_start_.y = row;

	select();

}

template<int col, typename T>
inline void list_traverser<col, T>::move_column(int colum)
{
	if (type_ == selection_type::column_selection) { // can only change column if you are selecting by column

	}
}

