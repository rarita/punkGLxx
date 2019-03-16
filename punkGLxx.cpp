#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <vector>
#include <string>

typedef unsigned int uint;

using namespace std;

class RGB {
public:
	RGB(uint r, uint g, uint b);
	uint const getR();
	uint const getG();
	uint const getB();
	string const toString();
	string const toHEX();
private:
	uint R;
	uint G;
	uint B;
};

RGB::RGB(uint r, uint g, uint b) : R(r), G(g), B(b) {}

uint const RGB::getR() {
	return R;
}

uint const RGB::getG() {
	return G;
}

uint const RGB::getB() {
	return B;
}

const string RGB::toString() {
	return "(R = " + to_string(R) + ";G = " + to_string(G) + ";B = " + to_string(B) + ")";
}

const string RGB::toHEX() {
	stringstream stream;
	stream << setfill('0') << setw(2) << std::hex << R; // lil bydlo
	stream << setfill('0') << setw(2) << std::hex << G;
	stream << setfill('0') << setw(2) << std::hex << B;
	return stream.str();
}

class Vector2D {
public:
	Vector2D(double x, double y);
	double const getX();
	double const getY();
	string const toString();
private:
	const double m_x;
	const double m_y;
};

Vector2D::Vector2D(double x, double y) : m_x(x), m_y(y) {}

double const Vector2D::getX() {
	return m_x;
}

double const Vector2D::getY() {
	return m_y;
}

string const Vector2D::toString() {
	return "Vector3D(x = "  + to_string(this->getX()) +
					";y = " + to_string(this->getY()) + ")";
}

class Vector3D : Vector2D {
public:
	Vector3D(double x, double y, double z);
	double const dotProduct(Vector3D other);
	Vector3D operator *(Vector3D const other); // Product as operator Чекнуть передачу референса сюда
	Vector3D operator -(Vector3D const other); // subtract as operator   И сюда
	double const getZ();
	string const toString();
private:
	const double m_z;
};

Vector3D::Vector3D(double x, double y, double z) : Vector2D(x, y), m_z(z) {}

double const Vector3D::getZ() {
	return m_z;
}

double const Vector3D::dotProduct(Vector3D other) {
	return this->getX() * other.getX() + 
		   this->getY() * other.getY() + 
		   this->getZ() * other.getZ();
}

Vector3D Vector3D::operator*(Vector3D other) {
	return Vector3D(this->getX() * other.getX(), 
					this->getY() * other.getY(), 
					this->getZ() * other.getZ());
}

Vector3D Vector3D::operator-(Vector3D other) {
	return Vector3D(this->getX() - other.getX(), 
					this->getY() - other.getY(), 
					this->getZ() - other.getZ());
}

const string Vector3D::toString() {
	return "Vector3D(x = "  + to_string(this->getX()) +
					";y = " + to_string(this->getY()) +
					";z = " + to_string(this->getZ()) + ";)";
}

class Sphere {
public:
	Sphere(Vector3D center, double radius, RGB color);
	Vector2D const intersectWithRay(Vector3D &origin, Vector3D &direction);
	Vector3D const getCenter();
	RGB const getColor();
	double const getRadius();
	string toString();
private:
	const Vector3D m_center;
	const double m_radius;
	const RGB m_color;
};

Sphere::Sphere(const Vector3D center, double radius, RGB color) : m_center(center), m_radius(radius), m_color(color) {}

Vector3D const Sphere::getCenter() {
	return m_center;
}

RGB const Sphere::getColor() {
	return m_color;
}

double const Sphere::getRadius() {
	return m_radius;
}

Vector2D const Sphere::intersectWithRay(Vector3D &origin, Vector3D &direction) {
	Vector3D oc = origin - this->getCenter();

	const double k1 = direction.dotProduct(direction);
	const double k2 = 2 * oc.dotProduct(direction);
	const double k3 = oc.dotProduct(oc) - pow(this->getRadius(), 2);

	const double discriminant = pow(k2, 2) - 4 * k1 * k3;
	if (discriminant < 0) 
		return Vector2D(numeric_limits<double>::infinity(), numeric_limits<double>::infinity());

	const double plot1 = (-k2 + sqrt(discriminant)) / (2 * k1);
	const double plot2 = (-k2 - sqrt(discriminant)) / (2 * k1);
	return Vector2D(plot1, plot2);
}

string Sphere::toString() {
	RGB m_clr = this->getColor(); Vector3D m_cnt = this->getCenter();
	return "Sphere(center = " + m_cnt.toString()
		+ ";radius = " + to_string(m_radius)
		+ ";color = " + m_clr.toString() + ")";
}

class Canvas {
public:
	Canvas(uint x, uint y, RGB &bg_color); // Конструктор
	void put_point(uint x, uint y, RGB &color); // Красим точку
	void dump(); // Сливаем весь холст в нужный поток
	void dumpHEX(string filename); // То же самое но в HEX для экономии места
	uint const getWidth();
	uint const getHeight();
private:
	vector<vector<RGB>> holder;
	const uint x_dim;
	const uint y_dim;
};

Canvas::Canvas(uint x, uint y, RGB &bg_color) : x_dim(x), y_dim(y) {
	holder.resize(x, std::vector<RGB>(y, bg_color));
}

void Canvas::put_point(uint x, uint y, RGB &color) {
	// Кидаем если точка за границами холста
	// Точки считаются от 0
	if (x > x_dim - 1 || y > y_dim - 1) throw overflow_error("Index bigger than canvas bounds");
	holder[x][y] = color;
}

uint const Canvas::getWidth() {
	return x_dim;
}

uint const Canvas::getHeight() {
	return y_dim;
}

// Временно вываливаем писюн на стол, сделать потом редирект в файл или пайп в BMP. 
void Canvas::dump() {
	for (uint x = 0; x < x_dim; x++) {
		for (uint y = 0; y < y_dim; y++) {
			RGB color = holder[x][y];
			cout << color.toString() << " ";
		}
		cout << endl;
	}
}

void Canvas::dumpHEX(string filename) {
	ofstream outfile;
	outfile.open(filename);
	// Пишем ширь и высь в первой строке
	outfile << this->getWidth() << ";" << this->getHeight() << endl;
	// Пишем все пикселы в след строке, не разделяем ибо все по 6 знаков
	for (uint x = 0; x < this->getWidth(); x++)
		for (uint y = 0; y < this->getHeight(); y++)
			outfile << holder[x][y].toHEX();
	outfile.close();
}

// Renderer
// TODO поместить это зло в нужное место.
// Настройки
const uint IMAGE_WIDTH = 64;
const uint IMAGE_HEIGHT = 64;
const uint viewport_size = 1;
const uint projection_plane_z = 1;
const Vector3D cameraPosition(0, 0, 0);
const RGB backgroundColor(255, 255, 255);
vector<Sphere> spheres {
	Sphere(Vector3D(0, -1, 3), 1, RGB(255, 0, 0)),
	Sphere(Vector3D(2, 0, 4), 1, RGB(0, 255, 0)),
	Sphere(Vector3D(-2, 0, 4), 1, RGB(0, 0, 255))
};
// Конец настроек

const Vector3D canvasToViewport(uint x, uint y, Canvas &canvas) {
	return Vector3D(x * double(viewport_size) / canvas.getHeight(),
					y * double(viewport_size) / canvas.getWidth(),
					projection_plane_z);
}

RGB traceRay(Vector3D &origin, Vector3D &direction, double min_t, double max_t) {
	double closest_t = numeric_limits<double>::infinity();
	Sphere* closest_sphere = NULL;

	for (auto &sphere : spheres) {
		Vector2D ts = sphere.intersectWithRay(origin, direction);
		if (ts.getX() < closest_t && min_t < ts.getX() && ts.getX() < max_t) {
			closest_t = ts.getX();
			closest_sphere = &sphere;
		}
		if (ts.getY() < closest_t && min_t < ts.getY() && ts.getY() < max_t) {
			closest_t = ts.getY();
			closest_sphere = &sphere;
		}
	}

	if (closest_sphere == NULL)
		return backgroundColor;
	else
		return closest_sphere->getColor();
}

Canvas render() {
	RGB bgc = backgroundColor;
	Canvas canvas(IMAGE_WIDTH, IMAGE_HEIGHT, bgc);
	for (uint x = 0; x < IMAGE_WIDTH; x++) {
		for (uint y = 0; y < IMAGE_WIDTH; y++) {
			Vector3D direction = canvasToViewport(x, y, canvas);
			Vector3D cameraPos = cameraPosition; // Придумать как задоджить такое разыменование const 
			RGB pointColor = traceRay(cameraPos, direction, 1, numeric_limits<double>::infinity());
			canvas.put_point(x, y, pointColor);
		}
	}
	return canvas;
}

int main()
{
	Canvas test = render();
	test.dumpHEX("image.rcf");
}