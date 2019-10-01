#pragma once

/*Vector3D operator*(const Vector3D& vector, float scalar)
{
	return Vector3D(
		scalar * vector.x,
		scalar * vector.y,
		scalar * vector.z);
}
Vector3D operator*(float scalar, const Vectore3D& vector)
{
	return Vector3D();
}

float Vector::dot(const Vector3D& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::perpCwXy() const
{

}

Vector3D Vector3D::perpCcwXy() const;
