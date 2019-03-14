// punkGLxx.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

typedef unsigned int uint;

using namespace std;

class RGB {
public:
	RGB(uint r, uint g, uint b);
	string const toString();
private:
	const uint R;
	const uint G;
	const uint B;
};

RGB::RGB(uint r, uint g, uint b) : R(r), G(g), B(b) {}

const string RGB::toString()
{
	return "(R: " + to_string(R) + " G: " + to_string(G) + " B: " + to_string(B) + ")";
}

class Vector3D {
public:
	Vector3D(int x, int y, int z);
	int const getX();
	int const getY();
	int const getZ();
private:
	const int m_x;
	const int m_y;
	const int m_z;
};

Vector3D::Vector3D(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {}

int const Vector3D::getX() {
	return m_x;
}

int const Vector3D::getY() {
	return m_y;
}

int const Vector3D::getZ() {
	return m_z;
}

class Canvas {
public:
	Canvas(uint x, uint y, RGB* bg_color); // Конструктор
	void put_point(uint x, uint y, RGB* color); // Красим точку
	void const dump(); // Сливаем весь холст в нужный поток
private:
	vector<vector<RGB*>> holder;
	const uint x_dim;
	const uint y_dim;
};

Canvas::Canvas(uint x, uint y, RGB* bg_color) : x_dim(x), y_dim(y) {
	holder.resize(x, std::vector<RGB*>(y, bg_color));
} 

void Canvas::put_point(uint x, uint y, RGB* color) {
	// Кидаем если точка за границами холста
	// Точки считаются от 0
	if (x > x_dim - 1 || y > y_dim - 1) throw overflow_error("Index bigger than canvas bounds");
	holder[x][y] = color;
}

// Временно вываливаем писюн на стол, сделать потом редирект в файл или пайп в BMP. 
void const Canvas::dump() {
	for (uint x = 0; x < x_dim; x++) {
		for (uint y = 0; y < y_dim; y++) {
			RGB color = *holder[x][y];
			cout << color.toString() << " ";
		}
		cout << endl;
	}
}

int main()
{
	RGB white(255, 255, 255);
	Canvas canvas(10, 10, &white);
	canvas.put_point(0, 0, &RGB(4, 5, 6));
	canvas.put_point(0, 1, &RGB(7, 8, 9));
	canvas.dump();
}