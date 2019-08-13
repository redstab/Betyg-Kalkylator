#pragma once
#include "precompile.h"
#include "ui_element.h"

template<int number_of_columns, typename T>
class list : ui_element {
public:
	list(const window& win, std::array<point, number_of_columns> data_positions, std::vector<T> items, std::array<std::string (T::*)() const, number_of_columns> points, point position);
	
	size get_element_size() const;

	void draw_element() const;
private:
	std::array<std::string(T::*)() const, number_of_columns> data_points; // Access points in structure to the separate columns
	std::vector<T> rows;
	std::vector<std::array<text, number_of_columns>> text_rows;
	std::array<point, number_of_columns> data_positions_;

	int list_length;

};

template<int number_of_columns, typename T>
inline list<number_of_columns, T>::list(const window& win, std::array<point, number_of_columns> data_positions, std::vector<T> items, std::array<std::string(T::*)() const, number_of_columns> points, point position) :
	ui_element(win, position),
	data_points{points},
	rows{items},
	data_positions_{ data_positions }
{

	text_rows.resize(rows.size());

	for (auto i = 0; i < rows.size(); ++i) {
		T& element = rows.at(i);
		std::array<text, number_of_columns>& row = text_rows.at(i);
		for (auto j = 0; j < number_of_columns; ++j) {
			auto& column = row.at(j);
			std::string(T::* getter)() const = data_points.at(j);
			column.set_position({data_positions_.at(j).x, position_.y + i});
			column.set_text((element.*getter)());
			column.set_window(window_);

			column.draw_element();
			wrefresh(window_.get_window());
		}
	}
}

template<int number_of_columns, typename T>
inline size list<number_of_columns, T>::get_element_size() const
{
	return {list_length, static_cast<int>(rows.size())};
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::draw_element() const
{

}
