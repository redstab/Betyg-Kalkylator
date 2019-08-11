#pragma once
#include "precompile.h"
#include "text.h"
#include "window.h"
class text_header : public text {
public:
	text_header(const window& win, const std::string& text, point position, int header_length);

	size get_element_size() const;

	void set_header_length(int header_length);

private:
	int header_length_;
};