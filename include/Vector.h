#ifndef VECTOR_H
#define VECTOR_H

class Vector{
public:
	float x;
	float y;
	Vector(float mx = 0, float my = 0);
	void rotate(Vector origin, float angle);
	void transform(float module, float angle);

	Vector operator+(const Vector& rhs) const;
	Vector operator-(const Vector& rhs) const;
	Vector operator*(const float rhs) const;
};

#endif
