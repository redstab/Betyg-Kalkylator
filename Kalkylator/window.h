#pragma once
#include "precompile.h"
#include "point.h"
class window
{
public:
	
	window(size window_size = {120,30}, std::optional<point> begin_position = std::nullopt);

	WINDOW* get_window() const { return window_; };

	size get_size() const;

	void set_size(size window_size);

	point get_position() const;

	void move_window(point position);

	void show_border() const;
	
	void hide_border() const;

private:
	size window_size_{};
	size screen_size_{};
	point position_{};
	bool center_{};

	WINDOW* window_;
	
	WINDOW* make_window(size window_size);
};

