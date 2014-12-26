#include "ellipticcurvepoint.h"

#include "function.h"

ECPoint::ECPoint()
{

}

ECPoint::ECPoint(const ECPoint &point)
{
	a = point.a;
	b = point.b;
	x = point.x;
	y = point.y;
	field = point.field;
}

ECPoint::ECPoint(const big_int &a, const big_int &b, const big_int &x, const big_int &y, const big_int &field)
	: a(a), b(b), x(x), y(y), field(field)
{
}

big_int ECPoint::compressionCoordinate(const ECPoint &p)
{
	quint8 y = ((p.y).is_odd()) ? (0x03) : 0x02;
	QByteArray tmp;
	tmp.append(y);
	tmp.append(Function::big_intToByteArray(p.x));
	return Function::big_intFromByteArray(tmp);
}

ECPoint ECPoint::decompressionCoordinate(const big_int &Gx, const big_int &a, const big_int &b, const big_int &field)
{
	ECPoint result;
	result.a = a;
	result.b = b;
	result.field = field;

	QByteArray xPoint = Function::big_intToByteArray(Gx);
	quint8 y = xPoint.at(0);
	xPoint = xPoint.remove(0, 1);
	result.x = Function::big_intFromByteArray(xPoint);

	big_int tmp = (Arageli::pow(result.x , 3) + (a * result.x) + b) % field;
	big_int beta = Function::modSqrt(tmp, field);
	result.y = ((beta % 2) == (y % 2)) ? beta : field - beta;

	return result;
}

const ECPoint operator+(const ECPoint &left, const ECPoint &right)
{
	ECPoint result;
	result.a = left.a;
	result.b = left.b;
	result.field = left.field;

	big_int dy;
	big_int dx;

	if (left == right)
	{
		dy = 3 * left.x * left.x + left.a;
		dx = 2 * left.y;
	}
	else
	{
		dy = right.y - left.y;
		dx = right.x - left.x;
	}

	dx += (dx < 0) ? left.field : big_int(0);
	dy += (dy < 0) ? left.field : big_int(0);

	big_int m = (dy * inverse_mod(dx, left.field)) % left.field;
	m += (m < 0) ? left.field : big_int(0);

	if (left == right)
	{
		result.x = (m * m - left.x - left.x) % left.field;
		result.y = (m * (left.x - result.x) - left.y) % left.field;
	}
	else
	{
		result.x = (m * m - left.x - right.x) % left.field;
		result.y = (m * (left.x - result.x) - left.y) % left.field;
	}

	result.x += (result.x < 0) ? left.field : big_int(0);
	result.y += (result.y < 0) ? left.field : big_int(0);

	return result;
}

ECPoint &operator+=(ECPoint &left, const ECPoint &right)
{
	left = left + right;
	return left;
}

const ECPoint operator*(big_int x, ECPoint p)
{
	ECPoint result(p);
	x = x - 1;
	while (!x.is_null())
	{

		if (!((x % 2).is_null()))
		{
			result += p;
			x = x - 1;
		}
		x = x / 2;
		p += p;
	}
	return result;
}

bool operator==(const ECPoint &left, const ECPoint &right)
{
	return (left.x == right.x) || (left.y == right.y);
}

