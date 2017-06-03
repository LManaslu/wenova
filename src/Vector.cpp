#include "Vector.h"
#include <cmath>

Vector::Vector(float mx, float my) : x(mx), y(my){}

void Vector::rotate(Vector origin, float angle){
	x = origin.x + 200 * cos(angle);
	y = origin.y + 200 * sin(angle);
}

void Vector::transform(float module, float angle){
	x = module * cos(angle);
	y = module * sin(angle);
}

Vector Vector::operator+(const Vector& rhs) const {
	return Vector(x + rhs.x, y + rhs.y);
}

Vector Vector::operator-(const Vector& rhs) const {
	return Vector(x - rhs.x, y - rhs.y);
}

Vector Vector::operator*(const float rhs) const {
	return Vector(x * rhs, y * rhs);
}
