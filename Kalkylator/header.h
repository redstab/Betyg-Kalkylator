#pragma once
#include "precompile.h"
#include "ui_element.h"
#include "table_header.h"

template <int number_of_headers> class header :
	public ui_element
{
public:
	header(const window& win, std::array<table_header, number_of_headers> headers, point position, int head_length, int seperation);

	std::array<table_header, number_of_headers> get_headers();
	void set_headers(const std::array<table_header, number_of_headers> headers);

	void update_header(int header_index, std::string titel);

	std::array<point, number_of_headers> get_header_positions() const;

	int get_max_size() const;

	void set_max_size(int head_length);

	size get_element_size() const;

	void set_seperation(int seperation);

	int get_seperation() const;

	void draw_element();
private:
	int head_length_;
	int seperation_;

	const int padding = 4;

	std::array<table_header, number_of_headers> headers_;

	point calculate_position(int index, int& tracker);

	void calculate_positions();

	void set_positions(std::array<point, number_of_headers> input);
};

template<int number_of_headers>
header<number_of_headers>::header(const window& win, std::array<table_header, number_of_headers> headers, point position, int head_length, int seperation) : ui_element(win, position), head_length_{ head_length }, seperation_(seperation), headers_{ headers } {calculate_positions(); }

template<int number_of_headers>
inline std::array<table_header, number_of_headers> header<number_of_headers>::get_headers()
{
	return headers_;
}

template<int number_of_headers>
inline void header<number_of_headers>::set_headers(const std::array<table_header, number_of_headers> headers)
{
	headers_ = headers;
	calculate_positions();
}

template<int number_of_headers>
inline void header<number_of_headers>::update_header(int index, std::string titel)
{
	headers_.at(index).set_text(titel);
	calculate_positions();
}

template<int number_of_headers>
inline std::array<point, number_of_headers> header<number_of_headers>::get_header_positions() const
{
	std::array<point, number_of_headers> positions;
	std::generate(positions.begin(), positions.end(), [n = 0, this]() mutable {return headers_.at(n++).get_position(); });
	return positions;
}

template<int number_of_headers>
inline int header<number_of_headers>::get_max_size() const
{
	return head_length_;
}

template<int number_of_headers>
inline void header<number_of_headers>::set_max_size(int head_length)
{
	head_length_ = head_length;
	calculate_positions();
}

template<int number_of_headers>
inline size header<number_of_headers>::get_element_size() const
{
	return {head_length_ - 1, 1};
}

template<int number_of_headers>
inline void header<number_of_headers>::set_seperation(int seperation)
{
	seperation_ = seperation;
	calculate_positions();
}

template<int number_of_headers>
inline int header<number_of_headers>::get_seperation() const
{
	return seperation_;
}

template<int number_of_headers>
inline void header<number_of_headers>::draw_element()
{
	for (auto& header_ : headers_) {
		header_.draw_element();
	}
}

template<int number_of_headers>
inline point header<number_of_headers>::calculate_position(int index, int& tracker)
{

	int header_size = headers_.at(index).get_element_size().x;

	point position;

	if (index >= seperation_) {
		tracker -= header_size + padding;
	}

	position = { tracker, position_.y };

	if (index < seperation_) {
		tracker += header_size + padding;
	}

	return position;

}

template<int number_of_headers>
inline void header<number_of_headers>::calculate_positions()
{

	std::array<point, number_of_headers> positions;

	int tracker = position_.x + padding; // + 1 because of size vs 0 indexing

	int reverse_tracker = head_length_; // + 1 because of size vs 0 indexing

	for (int i = 0; i < number_of_headers; ++i) {

		positions.at(i) = calculate_position(i, (i < seperation_) ? tracker : reverse_tracker);
		// select tracker when it has not reached separation else choose reverse_tracker

	}

	std::reverse(positions.begin() + seperation_, positions.end());

	set_positions(positions);
	
}

template<int number_of_headers>
inline void header<number_of_headers>::set_positions(std::array<point, number_of_headers> input)
{
	for (auto i = 0; i < number_of_headers; ++i) {
		headers_.at(i).set_position(input.at(i));
	}
}
