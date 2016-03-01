// Implementation of Vector3
/*
namespace Vector
{
	inline double const Vector3::dot(Vector3 const& b) const
	{
		return (x*b.x + y*b.y + z*b.z);
	}

	inline Vector3 const Vector3::cross(Vector3 const& b) const
	{
		double newX = y*b.z - z*b.y;
		double newY = z*b.x - x*b.z;
		double newZ = x*b.y - y*b.x;
		return Vector3(newX, newY, newZ);
	}

	inline double const Vector3::getMagnitude() const
	{
		double result = dot(*this);
		result = sqrt(result);
		return result;
	}

	inline Vector3 const Vector3::getNormalizedVector() const
	{
		double magnitude = getMagnitude();
		double newX = x / magnitude;
		double newY = y / magnitude;
		double newZ = z / magnitude;
		return Vector3(newX, newY, newZ);
	}

	inline Vector3 const Vector3::getProjection(Vector3 const& b) const
	{
		double numerator = this->dot(b);
		double denominator = b.dot(b);
		return Vector3((numerator / denominator) * b);
	}

	inline Vector3 const Vector3::rotate(Vector3 const& rotationVector) const
	{
		// If it is rotating around more than one axis, return invalid.
		if (!(!rotationVector.x != !rotationVector.y != !rotationVector.z))
		{
			std::cerr << "ERROR: Attempt to rotate multiple axes in Vector3::rotate" << std::endl;
			return Vector3(0, 0, 0);
		}

		double rotateValue = 0;
		Axes axis;
		//getRotationInformation(rotationVector, rotateValue, axis);
		if (rotationVector.x != 0)
		{
			rotateValue = rotationVector.x;
			axis = Axes::X_AXIS;
		}
		else if (rotationVector.y != 0)
		{
			rotateValue = rotationVector.y;
			axis = Axes::Y_AXIS;
		}
		else if (rotationVector.z != 0)
		{
			rotateValue = rotationVector.z;
			axis = Axes::Z_AXIS;
		}



		//return result;
	}

	inline bool const Vector3::operator ==(Vector3 const& b) const
	{
		return (this->x == b.x && this->y == b.y && this->z == b.z);
	}

	inline Vector3 const Vector3::operator +(Vector3 const& b) const
	{
		return Vector3(this->x + b.x, this->y + b.y, this->z + b.z);
	}

	inline Vector3 const Vector3::operator -(Vector3 const& b) const
	{
		return Vector3(this->x - b.x, this->y - b.y, this->z - b.z);
	}

	inline Vector3 const Vector3::operator *(const double& s) const
	{
		return Vector3(this->x*s, this->y*s, this->z*s);
	}

	inline std::ostream& operator <<(std::ostream& stream, Vector3 const& a)
	{
		stream << "(" << a.x << ", " << a.y << ", " << a.z << ")";
		return stream;
	}

	inline Vector3 const operator *(const double& s, Vector3 const& a)
	{
		return Vector3(a.x*s, a.y*s, a.z*s);
	}
}
*/

// Implementation of Vector2
namespace Vector
{
	inline std::ostream& operator <<(std::ostream& stream, Vector2 const& a)
	{
		stream << "(" << a.x << ", " << a.y << ")";
		return stream;
	}

	inline double const Vector2::dot(Vector2 const& b) const
	{
		return (x*b.x + y*b.y);
	}

	inline double const Vector2::getMagnitude() const
	{
		double result = dot(*this);
		result = sqrt(result);
		return result;
	}

	inline Vector2 const Vector2::getNormalizedVector() const
	{
		double magnitude = getMagnitude();
		double newX = x / magnitude;
		double newY = y / magnitude;
		return Vector2(newX, newY);
	}

	inline Vector2 const Vector2::getProjection(Vector2 const& b) const
	{
		double numerator = this->dot(b);
		double denominator = b.dot(b);
		return Vector2((numerator / denominator) * b);
	}

	inline bool const Vector2::operator ==(Vector2 const& b) const
	{
		return (this->x == b.x && this->y == b.y);
	}

	inline Vector2& Vector2::operator +=(Vector2 const& b)
	{
		this->x += b.x;
		this->y += b.y;
		return (*this);
	}

	inline Vector2 const Vector2::operator +(Vector2 const& b) const
	{
		return (Vector2(*this) += b);
	}

	inline Vector2 const Vector2::operator -(Vector2 const& b) const
	{
		return Vector2(this->x - b.x, this->y - b.y);
	}

	inline Vector2 const Vector2::operator *(const double& s) const
	{
		return Vector2(this->x*s, this->y*s);
	}

	inline Vector2 const operator *(const double& s, Vector2 const& a)
	{
		return Vector2(a.x*s, a.y*s);
	}

}