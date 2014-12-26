#ifndef ELLIPTICCURVEPOINT_H
#define ELLIPTICCURVEPOINT_H

#include <big_int.hpp>

using namespace Arageli;

class ECPoint
{
public:
	ECPoint();
	ECPoint(const ECPoint &point);
	ECPoint(const big_int &a, const big_int &b, const big_int &x, const big_int &y, const big_int &field);

	friend const ECPoint operator+(const ECPoint &left, const ECPoint &right);
	friend ECPoint& operator+=(ECPoint &left, const ECPoint &right);

	friend const ECPoint operator*(big_int x, ECPoint p);

	friend bool operator==(const ECPoint& left, const ECPoint& right);

	static big_int compressionCoordinate(const ECPoint &p);
	static ECPoint decompressionCoordinate(const big_int &Gx, const big_int &a, const big_int &b, const big_int &field);

	big_int a;
	big_int b;
	big_int x;
	big_int y;
	big_int field;
};

#endif // ELLIPTICCURVEPOINT_H
