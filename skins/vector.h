#ifndef __VECTOR_H
#define __VECTOR_H

//Vector3D.h
//made by [exa]

#include <math.h>

class Vector3D
{
public:
	union {
		struct{
			float x, y, z;
		};
		float v[3];
	};


	Vector3D (float a = 0, float b = 0, float c = 0) : x (a), y (b), z (c)
	{}
	Vector3D (const Vector3D &vec) : x (vec.x), y (vec.y), z (vec.z)
	{}

	const Vector3D &operator= (const Vector3D &vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	const bool operator== (const Vector3D &vec) const
	{
		return ( (x == vec.x) && (y == vec.y) && (z == vec.z) );
	}

	const bool operator!= (const Vector3D &vec) const
	{
		return ! (*this == vec);
	}

	const Vector3D operator+ (const Vector3D &vec) const
	{
		return Vector3D (x + vec.x, y + vec.y, z + vec.z);
	}

	const Vector3D operator+() const
	{
		return Vector3D (*this);
	}

	const Vector3D& operator+= (const Vector3D vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	const Vector3D operator- (const Vector3D& vec) const
	{
		return Vector3D (x - vec.x, y - vec.y, z - vec.z);
	}

	const Vector3D operator-() const
	{
		return Vector3D (-x, -y, -z);
	}

	const Vector3D &operator-= (const Vector3D vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;

		return *this;
	}

	const Vector3D &operator*= (const float &s)
	{
		x *= s;
		y *= s;
		z *= s;

		return *this;
	}

	const Vector3D &operator/= (const float &s)
	{
		const float t = 1 / s;

		x *= t;
		y *= t;
		z *= t;

		return *this;
	}

	const Vector3D operator* (const float &s) const
	{
		return Vector3D (x*s, y*s, z*s);
	}

	friend inline const Vector3D operator* (const float &s, const Vector3D &vec)
	{
		return vec*s;
	}

	const Vector3D operator/ (float s) const
	{
		s = 1 / s;

		return Vector3D (s*x, s*y, s*z);
	}

	const Vector3D crossproduct (const Vector3D &vec) const
	{
		return Vector3D (y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	}

	const Vector3D operator^ (const Vector3D &vec) const
	{
		return Vector3D (y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
	}

	const float dotproduct (const Vector3D &vec) const
	{
		return x*vec.x + y*vec.y + z*vec.z;
	}

	const float operator% (const Vector3D &vec) const
	{
		return x*vec.x + y*vec.y + z*vec.z;
	}


	const float length() const
	{
		return (float) sqrt ( (double) (x*x + y*y + z*z) );
	}

	const Vector3D unitVector3D() const
	{
		return (*this) / length();
	}

	void normalize()
	{
		(*this) /= length();
	}

	const float operator!() const
	{
		return sqrtf (x*x + y*y + z*z);
	}

	const Vector3D operator | (const float length) const
	{
		if (length == 0) return Vector3D (0, 0, 0);
		register float x;
		if ( (x = ! (*this) ) == 0) return Vector3D (0, 0, 0);
		return *this * (length / ! (*this) );
	}

	const Vector3D& operator |= (const float length)
	{
		return *this = *this | length;
	}

	inline const float angle (const Vector3D& normal) const
	{
		return acosf (*this % normal);
	}

	inline const Vector3D reflection (const Vector3D& normal) const
	{
		const Vector3D vec (*this | 1);
		return (vec - normal * 2.0 * (vec % normal) ) * !*this;
	}

};

#endif
