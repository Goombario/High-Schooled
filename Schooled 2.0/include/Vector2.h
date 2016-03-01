#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

namespace Vector
{

	class Vector2
	{
	public:
		Vector2();
		Vector2(const Vector2&);
		Vector2(double const& x, double const& y);

		// Vector specific functions
		inline double const dot(Vector2 const& b) const;
		inline double const getMagnitude() const;
		inline Vector2 const getNormalizedVector() const;
		inline Vector2 const getProjection(Vector2 const& b) const;
		inline Vector2 const rotate(Vector2 const& rotationVector) const;	// INCOMPLETE

		// Setters
		inline void setX(const double& newX) { x = newX; }
		inline void setY(const double& newY) { y = newY; }

		// Getters
		inline double const getX() const { return x; }
		inline double const getY() const { return y; }

		// Operators
		Vector2& operator =(Vector2 const&);

		inline friend std::ostream& operator <<(std::ostream& stream, Vector2 const&);
		inline bool const operator ==(Vector2 const&) const;
		inline Vector2& operator +=(Vector2 const&);
		inline Vector2 const operator +(Vector2 const&) const;
		inline Vector2 const operator -(Vector2 const&) const;
		inline Vector2 const operator *(const double&) const;
		inline friend Vector2 const operator *(const double&, Vector2 const&);

	private:
		double x, y;
	};
}

// Vector 3
/*
namespace Vector
{
	enum class Axes
	{
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
	};

	class Vector3
	{
	public:
		// Constructors
		Vector3();
		Vector3(Vector3 const&);
		Vector3(double const& x, double const& y, double const& z);

		// Vector specific functions
		inline double const dot(Vector3 const& b) const;
		inline Vector3 const cross(Vector3 const& b) const;
		inline double const getMagnitude() const;
		inline Vector3 const getNormalizedVector() const;
		inline Vector3 const getProjection(Vector3 const& b) const;
		inline Vector3 const rotate(Vector3 const& rotationVector) const;	// INCOMPLETE

		// Setters
		inline void setX(double const& newX) { x = newX; }
		inline void setY(double const& newY) { y = newY; }
		inline void setZ(double const& newZ) { z = newZ; }

		// Getters
		inline double const getX() const { return x; }
		inline double const getY() const { return y; }
		inline double const getZ() const { return z; }

		// Operators
		Vector3& operator =(Vector3 const&);

		inline friend std::ostream& operator <<(std::ostream& stream, Vector3 const&);
		inline bool const operator ==(Vector3 const&) const;
		inline Vector3 const operator +(Vector3 const&) const;
		inline Vector3 const operator -(Vector3 const&) const;
		inline Vector3 const operator *(const double&) const;
		inline friend Vector3 const operator *(const double&, Vector3 const&);

	private:
		bool const getRotationInformation(Vector3 const&, double&, Axes&);
		double x, y, z;

	};
}
*/

#include "Vector2.inl"

#endif