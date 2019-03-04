#pragma once
class PNG_IMAGE {
private:
	unsigned char* data;
	int data_w, data_h, data_n;
	bool foreign;
public:

	PNG_IMAGE(unsigned char* data, int w, int h, int n);
	PNG_IMAGE(const char* filename);

	PNG_IMAGE(PNG_IMAGE& other) = delete;
	PNG_IMAGE& operator=(PNG_IMAGE& other) = delete;

	unsigned char& getNum(int row, int col, int rgba);

	int weight() { return data_w; }
	int height() { return data_h; }
	int channel() { return data_n; }

	void write(const char* filename);
	~PNG_IMAGE();
};