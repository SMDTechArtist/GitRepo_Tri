#pragma once
/*ifndef ENGINE_VectorMath_3D_H
#define ENGINE_VectorMath_3D_H
#include <gl\glew.h>
#include <iostream>
#include <MeGlWindow.h>
#include <fstream>
#include <glfw3.h>
#include <glm\glm.hpp>
#include <QtGui/qkeysequence.h>
#include <Qt\qevent.h>
#include <cassert>
#include <Qt\qtimer.h>
#include <ctime>

using namespace std;

using glm::vec3;

namespace Math
{
	struct vector3D
	{
		float x;
		float y;
		float z;
		explicit Vector3D(
			float x = 0.0f,
			float y = 0.0f,
			float z = 0.0f) : x(x), y(y), z(z) {}
		inline Vector3D& operator+=(const Vector3D& right);
	};
	//inline Vector3D operator+(const Vector3D& left, const Vector3D& vector);
	//inline Vector3D operator-(const Vector3D& left, const Vector3D& vector);
	//inline Vector3D operator*(float scalar, const Vector3D& vector);
	//inline Vector3D operator*(const Vector3D& vector, float scalar);
	inline float dot(const Vector3D& other) const;
	inline Vector3D perpCwXy() const;
	inline Vector3D perpCcwXy() const;
	#include <VectorMath.inl>


}

/*class Vector3D {
private:
	double x, y, z;
public:
	Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {
	}
	friend ostream& operator << (ostream& s, Vector3D v) {
		return s << v.x << "," << v.y << "," << v.z;
	}

	double dot(const Vector3D& a, const Vector3D& b)
	{
		return (b.x, b.y, b.z);
	}

};*/
#endif