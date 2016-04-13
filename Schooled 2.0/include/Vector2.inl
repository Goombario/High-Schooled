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

	inline Vector2 const Vector2::getRotation(double angle) const
	{
		Vector::Vector2 newVector;
		double radians = angle * M_PI / 180.0;

		newVector.setX(getX() * cos(radians) - getY() * sin(radians));
		newVector.setY(getX() * sin(radians) + getY() * cos(radians));

		return newVector;
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