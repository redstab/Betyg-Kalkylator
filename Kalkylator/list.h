#pragma once
#include "precompile.h"
#include "ui_element.h"
#include "header.h"
template<typename T> struct column {
	column() : data(nullptr) {}
	column(point pos, std::string(T::* dat)() const) : position(pos), data{ dat }, txt() {}
	point position;
	std::string(T::*data)() const;
	text txt;
};

template<int n,typename T> struct row {
	row(std::array<column<T>, n> cols, T obj) : columns{ cols }, object{ obj }{}
	std::array<column<T>, n> columns;
	T object;
};

template<int number_of_columns, typename T>
class list : public ui_element {
public:
	list(const window& win, std::vector<T> items, std::array<column<T>, number_of_columns> columns, header<number_of_columns> head, point position);
	
	void push_item(const T& item);

	void pop_item(const T& item);

	size get_element_size() const;

	void draw_element();

private:

	std::vector<row<number_of_columns, T>> rows;

	std::array<column<T>, number_of_columns> boilerplate_row;

	int list_length;

};

template<int number_of_columns, typename T>
inline list<number_of_columns, T>::list(const window& win, std::vector<T> items, std::array<column<T>, number_of_columns> columns, header<number_of_columns> head, point position) :
	ui_element(win, position),
	boilerplate_row{columns},
	list_length{head.get_element_size().x}
{
	for (auto i : items) {
		push_item(i);
	}
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::push_item(const T& item)
{
	std::array<column<T>, number_of_columns> cols;

	for (auto i = 0; i < static_cast<int>(cols.size()); ++i) {
		column<T> col({ boilerplate_row.at(i).position.x, static_cast<int>(rows.size()) + position_.y }, boilerplate_row.at(i).data);
		col.txt.set_text((item.*col.data)());
		col.txt.set_window(window_);
		col.txt.set_position(col.position);

		cols.at(i) = col;
	}

	row<number_of_columns, T> rw(cols, item);

	rows.push_back(rw);
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::pop_item(const T& item)
{
	auto result = std::find_if(rows.begin(), rows.end(), [&](row<number_of_columns, T> rw) {return rw.object == item; });

	if (result != rows.end()) {
		int i = std::distance(rows.begin(), result);

		rows.erase(result);

		for (; i < static_cast<int>(rows.size()); ++i) {
			auto& col = rows.at(i).columns;
			for (auto j = 0; j < number_of_columns; ++j) {
				col[j].txt.clear_element();
				col[j].position.y--;
				col[j].txt.set_position(col[j].position);
			}
		}
	}
}

template<int number_of_columns, typename T>
inline size list<number_of_columns, T>::get_element_size() const
{
	return {list_length, static_cast<int>(rows.size())};
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::draw_element()
{
	for (auto _row : rows) {
		for (auto col : _row.columns) {
			col.txt.set_text((_row.object.*col.data)());
			col.txt.draw_element();
		}
	}
}
