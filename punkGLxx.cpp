#include "pch.h"
#include <iostream>
#include <algorithm>
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
	RGB operator*(double multiplier);
	string const toString();
	string const toHEX();
private:
	uint R;
	uint G;
	uint B;
};

RGB::RGB(uint r, uint g, uint b) {
	R = max(0U, min(r, 255U));
	G = max(0U, min(g, 255U));
	B = max(0U, min(b, 255U));
}

uint const RGB::getR() {
	return R;
}

uint const RGB::getG() {
	return G;
}

uint const RGB::getB() {
	return B;
}

RGB RGB::operator*(double multiplier) {
	return RGB(
		this->getR() * multiplier,
		this->getG() * multiplier,
		this->getB() * multiplier
		);
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
	return "Vector3D(x = " + to_string(this->getX()) +
		";y = " + to_string(this->getY()) + ")";
}

class Vector3D : Vector2D {
public:
	Vector3D(double x, double y, double z);
	double const dotProduct(Vector3D other);
	double const length();
	Vector3D operator *(Vector3D &other); // Product as operator
	Vector3D operator *(double multiplier);
	Vector3D operator +(Vector3D &other);
	Vector3D operator -(Vector3D &other); // subtract as operator
	Vector3D clamp();
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
	return 
		this->getX() * other.getX() +
		this->getY() * other.getY() +
		this->getZ() * other.getZ();
}

double const Vector3D::length() {
	return sqrt(this->dotProduct(*this));
}

Vector3D Vector3D::operator*(Vector3D &other) {
	return Vector3D(
		this->getX() * other.getX(),
		this->getY() * other.getY(),
		this->getZ() * other.getZ()
	);
}

Vector3D Vector3D::operator*(double multiplier) {
	return Vector3D(
		this->getX() * multiplier,
		this->getY() * multiplier,
		this->getZ() * multiplier
	);
}

Vector3D Vector3D::operator+(Vector3D &other) {
	return Vector3D(
		this->getX() + other.getX(),
		this->getY() + other.getY(),
		this->getZ() + other.getZ()
	);
}

Vector3D Vector3D::operator-(Vector3D &other) { // Можно это переписать как this + other * (-1), но как-то похуй.
	return Vector3D(
		this->getX() - other.getX(),
		this->getY() - other.getY(),
		this->getZ() - other.getZ()
	);
}

const string Vector3D::toString() {
	return "Vector3D(x = " + to_string(this->getX()) +
		";y = " + to_string(this->getY()) +
		";z = " + to_string(this->getZ()) + ";)";
}

class Light {
public:
	Light(double intensity, Vector3D position);
	double const calculateReflections(Vector3D &normal, Vector3D &view, Vector3D &vec_l, double specular);
	double getIntensity();
	Vector3D getPosition();
private:
	const double m_intensity;
	const Vector3D m_position;
};

Light::Light(double intensity, Vector3D position) : m_intensity(intensity), m_position(position) {}

double Light::getIntensity() {
	return m_intensity;
}

Vector3D Light::getPosition() {
	return m_position;
}

double const Light::calculateReflections(Vector3D &normal, Vector3D &view, Vector3D &vec_l, double specular) {
	double intensity = 0.0;
	double length_normal = normal.length();
	double length_view = view.length();
	// Diffuse
	double n_dot_l = normal.dotProduct(vec_l);
	if (n_dot_l > 0) {
		intensity += this->getIntensity() * n_dot_l / (length_normal * vec_l.length());
	}
	// Specular
	if (specular > 0) {
		double duo = 2.0;
		Vector3D vec_r = (normal * (normal.dotProduct(vec_l) * 2.0)) - vec_l; //Subtract(Multiply(2.0*DotProduct(normal, vec_l), normal), vec_l); 
		double r_dot_v = vec_r.dotProduct(view);
		if (r_dot_v > 0) {
			intensity += this->getIntensity() * pow(r_dot_v / (vec_r.length() * length_view), specular);
		}
	}
	return intensity;
}

class Sphere {
public:
	Sphere(Vector3D center, double radius, RGB color, double specular);
	Vector2D const intersectWithRay(Vector3D &origin, Vector3D &direction);
	Vector3D const getCenter();
	RGB const getColor();
	double const getRadius();
	double const getSpecular();
	string toString();
private:
	const Vector3D m_center;
	const double m_radius;
	const RGB m_color;
	const double m_specular;
};

Sphere::Sphere(const Vector3D center, double radius, RGB color, double specular) 
	: m_center(center), m_radius(radius), m_color(color), m_specular(specular) {}

Vector3D const Sphere::getCenter() {
	return m_center;
}

RGB const Sphere::getColor() {
	return m_color;
}

double const Sphere::getRadius() {
	return m_radius;
}

double const Sphere::getSpecular() {
	return m_specular;
}

Vector2D const Sphere::intersectWithRay(Vector3D &origin, Vector3D &direction) {
	Vector3D center = this->getCenter();
	Vector3D oc = origin - center;

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
	void put_point(int x, int y, RGB &color); // Красим точку
	void dump(); // Сливаем весь холст в нужный поток
	void dumpHEX(string filename); // То же самое но в HEX для экономии места
	void dumpBMP(string filename);
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

void Canvas::put_point(int x, int y, RGB &color) {
	// Кидаем если точка за границами холста
	// Точки считаются от 0
	uint real_x = this->getWidth() / 2 + x;
	uint real_y = this->getHeight() / 2 - y - 1;
	if (real_x > x_dim - 1 || real_y > y_dim - 1) throw overflow_error("Index bigger than canvas bounds");
	holder[real_x][real_y] = color;
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

//Теперь из настроек пуллится image_width и image_height для записи в файл

//многострадальная хуйня с кучей комментов, чтобы не забыть как это вообще работает
void Canvas::dumpBMP(string filename) {

	struct Header
	{

		//file header
		//char b - прикручено руками после открытия файла
		long size;
		short reserved_field_1;
		short reserved_field_2;
		long offset;

		//DIB header
		long numberDIB;
		long width;
		long height;
		short planenumber;
		short depth;
		long comp;
		long rawsize;
		long reshor;
		long resver;
		long palette;
		long important;
	};

	int padding; // считаем отступ, то есть количество байтов, которые нам надо прибавить, чтобы получить строку кратную 4
	padding = 4 - (3 * this->getWidth()) % 4;
	if (padding == 4)
	{
		padding = 0; // я курю и мне похуй
	}

	Header x;
	//x.b = 1; большинство из этих штук можно не трогать, размер подаётся на вход, вся хуйня в байтах
	x.size = sizeof(x) + this->getWidth() * this->getHeight() * 3 + padding * this->getHeight() + 2; //+2 байта за 2 прикрученные буквы
	x.reserved_field_1 = 0;
	x.reserved_field_2 = 0;
	x.offset = sizeof(x) + 2;

	x.numberDIB = 40;
	x.width = this->getWidth();
	x.height = this->getHeight();
	x.planenumber = 1;
	x.depth = 24;
	x.comp = 0;
	x.rawsize = this->getWidth() * this->getHeight() * 3 + padding * this->getHeight();
	x.reshor = 0x0b13;
	x.resver = 0x0b13;
	x.palette = 0;
	x.important = 0;

	//открываем файл на добавление
	ofstream output_file(filename);
	output_file.write("BM", 2); // а вот и прикручивание 2 буквы хэдэра
	output_file.write((char*)&x, sizeof(x)); // прописываем структуру в бинарном формате

	//прописываем массив		
	char dummy{ 0 }; // записываем это значение в качестве паддинга

	//ofstream output_file(filename, ios::app); //открыли на добавление

	//дрочим
	for (int i = this->getHeight() - 1; i >= 0; --i)
	{
		for (int j = 0; j <= this->getWidth(); ++j)
		{
			if (j < this->getWidth())
			{

				output_file << static_cast<unsigned char>(holder[j][i].getG());
				output_file << static_cast<unsigned char>(holder[j][i].getR());
				output_file << static_cast<unsigned char>(holder[j][i].getB());
			}
			else
			{
				for (long long n = 0; n < padding; ++n)
				{
					output_file << dummy; //когда цикл выходит за пределы "холста" добавляем чаров в количестве, которое посчитали заранее
				}
			}
		}
	}
	output_file.close();
}

// Renderer
// TODO поместить это зло в нужное место.
// Настройки теперь тут, чтобы писать в хэдэр
const uint IMAGE_WIDTH = 16;
const uint IMAGE_HEIGHT = 16;
const uint viewport_size = 1;
const uint projection_plane_z = 1;
const Vector3D cameraPosition(0, 0, 0);
const RGB backgroundColor(255, 255, 255);
vector<Sphere> spheres {
	Sphere(Vector3D(0, -1, 3), 1, RGB(255, 50, 100), 500),
	//Sphere(Vector3D(2, 0, 4), 1, RGB(130, 250, 50), 500),
	//Sphere(Vector3D(-2, 0, 4), 1, RGB(100, 200, 255), 10),
	//Sphere(Vector3D(-2, 2, 4), 1.5, RGB(128, 128, 128), 1000) // Цвет Петербурга
};
Light ambient_light(0.2, Vector3D(0.0, 0.0, 0.0)); // Эмбиент всегда один на сцене
vector<Light> point_lights {
	Light(0.6, Vector3D(2.0, 1.0, 0.0))
};
vector<Light> directional_lights {
	Light(0.2, Vector3D(1.0, 4.0, 4.0))
};

const Vector3D canvasToViewport(int x, int y, Canvas &canvas) {
	return Vector3D(
		x * double(viewport_size) / canvas.getWidth(),
		y * double(viewport_size) / canvas.getHeight(),
		projection_plane_z
	);
}

double calculateLightning(Vector3D &point, Vector3D &normal, Vector3D &view, double specular) {
	double intensity = 0.0;

	// Ambient
	intensity += ambient_light.getIntensity();
	// Point & Directional
	for (auto &point_l : point_lights) {
		Vector3D light_vector = (point_l.getPosition() - point);
		intensity += point_l.calculateReflections(normal, view, light_vector, specular);
	} 
	for (auto &direct_l : directional_lights) {
		Vector3D light_pos_pointer = (direct_l.getPosition()); // Сэйв памяти по максимуму. В функцию не пихается, т.к. замкнутый указатель.
		intensity += direct_l.calculateReflections(normal, view, light_pos_pointer, specular);
	}
	return intensity;
}

RGB traceRay(Vector3D &origin, Vector3D &direction, double min_t, double max_t, bool withLight) {
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

	if (withLight) {
		Vector3D point = (direction * closest_t) + origin;
		Vector3D closest_sphere_center = closest_sphere->getCenter(); // Сука const_cast так и не заработал ни разу.
		Vector3D normal_t = point - closest_sphere_center; // Тоже простор для оптимизации по памяти
		Vector3D normal = normal_t * (1.0 / normal_t.length());

		Vector3D view = direction * -1.0;
		double lightning = calculateLightning(point, normal, view, closest_sphere->getSpecular());
		RGB sphere_color = closest_sphere->getColor(); // const_cast zaebal :(
		return sphere_color * lightning;
	}
	else
		return closest_sphere->getColor();
}

Canvas render(bool withLight) {
	RGB bgc = backgroundColor;
	Canvas canvas(IMAGE_WIDTH, IMAGE_HEIGHT, bgc);
	const int w = IMAGE_WIDTH; const int h = IMAGE_HEIGHT;
	for (int x = -w / 2; x < w / 2; x++) {
		for (int y = -h / 2; y < h / 2; y++) {
			Vector3D direction = canvasToViewport(x, y, canvas);
			Vector3D cameraPos = cameraPosition; // Придумать как задоджить такое разыменование const 
			RGB pointColor = traceRay(cameraPos, direction, 1, numeric_limits<double>::infinity(), withLight);
			canvas.put_point(x, y, pointColor);
		}
	}
	return canvas;
}

int main()
{
	Canvas light = render(true);
	light.dumpHEX("dump.rcf");
}