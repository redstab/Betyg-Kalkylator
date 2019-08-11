#pragma once
#include "precompile.h"
#include "text.h"
#include "window.h"
class table_header : public text {
public:
	table_header(const window& win, const std::string& header_title, int data_length);

	size get_element_size() const;

	void set_data_length(int header_length);

private:
	int data_length_;
};