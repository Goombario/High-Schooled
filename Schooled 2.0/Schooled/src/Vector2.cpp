#include "Vector2.h"
#include "Schooled.h"
#include <math.h>
#include <iostream>

namespace Vector
{
	Vector3::Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	
	Vector3::Vector3(const Vector3& b)
	{
		x = b.x;
		y = b.y;
		z = b.z;
	}
	Vector3::Vector3(const double& X, const double& Y, const double& Z) : x(X), y(Y), z(Z)
	{

	}

	Vector3& Vector3::operator =(const Vector3& source)
	{
		if (this == &source)
		{
			return *this;
		}

		x = source.x;
		y = source.y;
		z = source.z;

		return *this;
	}

	bool const Vector3::getRotationInformation(const Vector3& rotationVector,
		double& rotateValue, int& axis)
	{
		if (rotationVector.x != 0)
		{
			rotateValue = rotationVector.x;
			axis = X_AXIS;
		}
		else if (rotationVector.y != 0)
		{
			rotateValue = rotationVector.y;
			axis = Y_AXIS;
		}
		else if (rotationVector.z != 0)
		{
			rotateValue = rotationVector.z;
			axis = Z_AXIS;
		}

		if (rotateValue != 0)
		{
			return true;
		}
		return false;
	}

}


namespace Vector
{
	Vector2::Vector2() : x(0), y(0) {}

	Vector2::Vector2(Vector2 const& b) : x(b.x), y(b.y) {}

	Vector2::Vector2(double const& X, double const& Y) : x(X), y(Y) {}

	Vector2& Vector2::operator =(const Vector2& source)
	{
		if (this == &source)
		{
			return *this;
		}

		x = source.x;
		y = source.y;

		return *this;
	}
}
