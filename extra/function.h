#ifndef FUNCTION_H
#define FUNCTION_H

#include <big_int.hpp>

using namespace Arageli;

#include <QByteArray>

class Function
{

public:
	static big_int big_intFromByteArray(const QByteArray &ba);
	static QByteArray big_intToByteArray(const big_int &number);
	static QByteArray big_intToByteArrayBlock(big_int number, quint32 blockSize);
	static big_int bin_pow(big_int a, big_int n, big_int m);

	static big_int modSqrt(const big_int &a, const big_int &p);
	static big_int legendre(const big_int &a, const big_int &p);
};

#endif // FUNCTION_H
