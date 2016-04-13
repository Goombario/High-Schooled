#ifndef VECTOR_H
#define VECTOR_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>

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
		inline Vector2 const getRotation(double angle) const;	// INCOMPLETE

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

#include "Vector2.inl"

#endif