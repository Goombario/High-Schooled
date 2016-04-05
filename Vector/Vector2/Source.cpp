
#include "Vector2.h"
#include <iostream>

using std::cout;
using std::endl;

void testVector3();
void testVector2();

int main()
{
	//testVector3();
	testVector2();

	system("pause");
	return 0;
}

void testVector3()
{
	Vector::Vector3 a(1.0, 2.0, 3.0);
	Vector::Vector3 b(3.4, -4.3, 3.8);
	Vector::Vector3 temp;

	cout << "a: " << a << endl;
	cout << "b: " << b << endl;
	cout << "a + b: " << a + b << endl;
	cout << "a - b: " << a - b << endl;
	cout << "a * 3: " << a * 3 << endl;
	cout << "3 * a: " << 3 * a << endl;
	cout << "a == b: " << (a == b) << endl;

	cout << endl;
	temp = a;
	cout << "temp = a: " << temp << endl;
	temp = Vector::Vector3(b);
	cout << "temp = Vector3(b): " << temp << endl << endl;

	cout << "a dot b: " << a.dot(b) << endl;
	cout << "a cross b: " << a.cross(b) << endl;
	cout << "magnitude of a: " << a.getMagnitude() << endl;
	cout << "normalization of a: " << a.getNormalizedVector() << endl;
	cout << "projection of b on a(?): " << a.getProjection(b) << endl;
}

void testVector2()
{
	Vector::Vector2 a(1, 2);
	Vector::Vector2 b(3, 4);
	Vector::Vector2 temp;

	cout << "a: " << a << endl;
	cout << "b: " << b << endl;
	cout << "a + b: " << a + b << endl;
	cout << "a - b: " << a - b << endl;
	cout << "a * 3: " << a * 3 << endl;
	cout << "3 * a: " << 3 * a << endl;
	cout << "a == b: " << (a == b) << endl;

	cout << endl;
	temp = a;
	cout << "temp = a: " << temp << endl;
	temp = Vector::Vector2(b);
	cout << "temp = Vector2(b): " << temp << endl << endl;

	cout << "a dot b: " << a.dot(b) << endl;
	cout << "magnitude of a: " << a.getMagnitude() << endl;
	cout << "normalization of a: " << a.getNormalizedVector() << endl;
	cout << "projection of b on a(?): " << a.getProjection(b) << endl;
}