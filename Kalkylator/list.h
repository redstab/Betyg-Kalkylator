#pragma once
#include "precompile.h"
#include "ui_element.h"
#include "header.h"

struct string{};
struct integer {
	int interval;
};
struct character {
	char from, to;
};

template<typename T> struct column {
	column(const window& win, point pos, std::string(T::* dat)() const, void (T::* set)(const std::string&), std::variant<string, integer, character> typ) : position(pos), getter{ dat }, setter(set), txt(win, "", pos), type(typ) {}
	point position;
	std::string(T::* getter)() const;
	void (T::* setter)(const std::string&);
	text txt;
	std::variant<string, integer, character> type;
};

template<int n, typename T> struct row {
	row(std::array<column<T>, n> cols, T obj) : columns{ cols }, object{ obj }{}

	std::array<column<T>, n> columns;
	T object;
};

template<int number_of_columns, typename T>
class list : public ui_element {
public:
	list(const window& win, std::vector<T> items, std::array<column<T>, number_of_columns> columns, header<number_of_columns>* head, point position);

	void set_length(int length);

	int get_length() const;

	std::vector<T> get_elements() const;

	std::array<point, number_of_columns> get_column_positions() const;

	std::array<int, number_of_columns> get_column_length() const;

	std::array<column<T>, number_of_columns> get_function_pointer() const;

	std::array<int, number_of_columns> get_data_length() const;

	std::vector<row<number_of_columns, T>>& get_rows();

	int get_max_rows() const;

	void push_item(const T& item);

	void pop_item(const T& item);

	size get_element_size() const;

	void draw_element();

private:

	std::vector<row<number_of_columns, T>> rows;

	std::array<column<T>, number_of_columns> boilerplate_row;

	header<number_of_columns>* headers;

	int list_length;

};

template<int number_of_columns, typename T>
inline list<number_of_columns, T>::list(const window& win, std::vector<T> items, std::array<column<T>, number_of_columns> columns, header<number_of_columns>* head, point position) :
	ui_element(win, position),
	boilerplate_row{ columns },
	headers{ head },
	list_length{ head->get_element_size().x }
{
	for (auto i : items) {
		push_item(i);
	}
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::set_length(int length)
{
	list_length = length;
}

template<int number_of_columns, typename T>
inline int list<number_of_columns, T>::get_length() const
{
	return list_length;
}

template<int number_of_columns, typename T>
inline std::vector<T> list<number_of_columns, T>::get_elements() const
{
	std::vector<T> object_list;

	object_list.resize(rows.size());

	std::generate(object_list.begin(), object_list.end(), [n = 0, this]() mutable {return rows[n++].object; });

	return object_list;
}

template<int number_of_columns, typename T>
inline std::array<point, number_of_columns> list<number_of_columns, T>::get_column_positions() const
{
	std::array<point, number_of_columns> positions;
	
	auto columns = headers->get_header_positions();

	std::generate(positions.begin(), positions.end(), [n = 0, this, columns]() mutable {return point{ columns[n++].x, position_.y }; });

	return positions;
}

template<int number_of_columns, typename T>
inline std::array<int, number_of_columns> list<number_of_columns, T>::get_column_length() const
{
	std::array<int, number_of_columns> col_length;

	auto head = headers->get_headers();

	std::generate(col_length.begin(), col_length.end(), [n = 0, head]() mutable {return head[n++].get_element_size().x; });

	return col_length;
}

template<int number_of_columns, typename T>
inline std::array<column<T>, number_of_columns> list<number_of_columns, T>::get_function_pointer() const
{
	return boilerplate_row;
}

template<int number_of_columns, typename T>
inline std::array<int, number_of_columns> list<number_of_columns, T>::get_data_length() const
{
	std::array<int, number_of_columns> col_length;

	auto head = headers->get_headers();

	std::generate(col_length.begin(), col_length.end(), [n = 0, head]() mutable {return head[n++].get_data_length(); });

	return col_length;
}

template<int number_of_columns, typename T>
inline std::vector<row<number_of_columns, T>>& list<number_of_columns, T>::get_rows()
{
	return rows;
}

template<int number_of_columns, typename T>
inline int list<number_of_columns, T>::get_max_rows() const
{
	return get_element_size().y;
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::push_item(const T& item)
{
	std::array<column<T>, number_of_columns> cols
	{
		column<T>(window_, {0,0}, nullptr, nullptr, boilerplate_row.at(0).type),
		column<T>(window_, {0,0}, nullptr, nullptr, boilerplate_row.at(1).type),
		column<T>(window_, {0,0}, nullptr, nullptr, boilerplate_row.at(2).type),
		column<T>(window_, {0,0}, nullptr, nullptr, boilerplate_row.at(3).type),
		column<T>(window_, {0,0}, nullptr, nullptr, boilerplate_row.at(4).type)
	};

	auto columns = headers->get_header_positions();

	for (auto i = 0; i < static_cast<int>(cols.size()); ++i) {

		column<T> col(window_, { columns.at(i).x, static_cast<int>(rows.size()) + position_.y }, boilerplate_row.at(i).getter, boilerplate_row.at(i).setter, boilerplate_row.at(i).type);
		col.txt.set_text((item.*col.getter)());
		col.txt.set_window(window_);
		cols.at(i) = col;

	}

	rows.push_back(row<number_of_columns, T>(cols, item));
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::pop_item(const T& item)
{
	auto result = std::find_if(rows.begin(), rows.end(), [&](row<number_of_columns, T> rw) {return rw.object == item; });

	if (result != rows.end()) {
		int i = std::distance(rows.begin(), result);

		if (i+1 == static_cast<int>(rows.size())) {
			auto& col = rows.at(i).columns;
			for (auto c = 0; c < number_of_columns; ++c) {
				col[c].txt.clear_element();
			}
		}

		rows.erase(result);

		for (; i < static_cast<int>(rows.size()); ++i) {
			auto& col = rows.at(i).columns;
			for (auto j = 0; j < number_of_columns; ++j) {
				col[j].txt.clear_element();
				col[j].txt.set_position({ col[j].position.x, --col[j].position.y });
			}
		}


	}
}

template<int number_of_columns, typename T>
inline size list<number_of_columns, T>::get_element_size() const
{
	return { list_length, static_cast<int>(rows.size()) };
}

template<int number_of_columns, typename T>
inline void list<number_of_columns, T>::draw_element()
{
	for (auto _row : rows) {
		for (auto col : _row.columns) {
			col.txt.set_text((_row.object.*col.getter)());
			col.txt.draw_element();
		}
	}
}
