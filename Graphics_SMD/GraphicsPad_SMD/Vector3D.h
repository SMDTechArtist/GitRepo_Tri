#pragma once
#ifndef ENGINE_VectorMath_3D_H
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
	struct vec3//Vector3D
	{
		float x;
		float y;
		float z;
		explicit vec3(
			float x = 0.0f,
			float y = 0.0f,
			float z = 0.0f) : x(x), y(y), z(z) {}
		inline vec3& operator+=(const vec3& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			return *this;
		};
		inline float dot(const vec3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		};
		inline vec3 perpCwXy() const
		{
			return vec3(y, -x);
		};
		inline vec3 perpCcwXy() const
		{
			return vec3(-y, x);
		};
	};
	inline vec3 operator+(const vec3& left, const vec3& right)
	{
		return vec3(
			left.x + right.x,
			left.y + right.y,
			left.z + right.z);

	};
	inline vec3 operator-(const vec3& left, const vec3& right)
	{
		return vec3(
			left.x - right.x,
			left.y - right.y,
			left.z - right.z);
	}

	inline vec3 operator*(float scalar, const vec3& vector)
	{
		return vec3(
			scalar * vector.x,
			scalar * vector.y,
			scalar * vector.z);
	};



	//#include <VectorMath.inl>


};

/*class vec3 {
private:
	double x, y, z;
public:
	vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {
	}
	friend ostream& operator << (ostream& s, vec3 v) {
		return s << v.x << "," << v.y << "," << v.z;
	}

	double dot(const vec3& a, const vec3& b)
	{
		return (b.x, b.y, b.z);
	}

};*/
#endif