#include "PNG_IMAGE.h"
#include <exception>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
PNG_IMAGE::PNG_IMAGE(unsigned char* data, int w, int h, int n) {
	this->data = data;
	data_w = w;
	data_h = h;
	data_n = n;
	foreign = false;
}

PNG_IMAGE::PNG_IMAGE(const char* filename) {
	data = stbi_load("out.png", &data_w, &data_h, &data_n, 0);
	foreign = true;
}

unsigned char& PNG_IMAGE::getNum(int row, int col, int rgba)
{
	if (data_w == 0 || data_h == 0 || data_n == 0) {
		throw std::exception("Õº∆¨Œ¥∂¡»Î");
	}
	return *(data + (row*data_w + col)*data_n + rgba);
}

void PNG_IMAGE::write(const char* filename) {
	stbi_write_png(filename, data_w, data_h, data_n, data, data_w * 4);
}

PNG_IMAGE::~PNG_IMAGE() {
	if (foreign) {
		stbi_image_free(data);
	}
	else {
		delete[] data;
	}
}