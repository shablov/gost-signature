#include "function.h"

big_int Function::big_intFromByteArray(const QByteArray &ba)
{
	big_int result = 0;
	for (int i = 0; i < ba.length(); i++)
	{
		quint8 ch = ba.at(i);
		for (int k = 0; k < 8; k++)
		{
			result <<= 1;
			result += ((ch >> (7-k)) & 0x01);
		}
	}
	return result;
}

QByteArray Function::big_intToByteArray(const big_int &number)
{
	QByteArray result = QByteArray();
	unsigned char byte = 0x00;
	for (int i = number.length() - 1; i >=0 ; i--)
	{
		byte <<= 1;
		byte += (number.bit(i) ? 1: 0);
		if ((i) % 8 == 0)
		{
			result.append(byte);
			byte = 0x00;
		}
	}

	return result;
}

QByteArray Function::big_intToByteArrayBlock(Arageli::big_int number, quint32 blockSize)
{
	if (number.length() > blockSize)
	{
		number = mod(number, pow2<big_int>(blockSize));
	}
	quint8 quantityBytes = (number.length() / 8) + ((number.length() % 8 != 0) ? 1 : 0);
	QByteArray byteArrayBlock(blockSize / 8 - quantityBytes, 0x00);
	unsigned char byte = 0x00;
	for (int i = number.length() - 1; i >=0 ; i--)
	{
		byte <<= 1;
		byte += (number.bit(i) ? 1: 0);
		if ((i) % 8 == 0)
		{
			byteArrayBlock.append(byte);
			byte = 0x00;
		}
	}

	return byteArrayBlock;
}

big_int Function::bin_pow(big_int a, big_int n, big_int m)
{
	big_int result = 1;
	while (n != 0)
	{
		if (n % 2 == 1)
		{
			(result *= a) %= m;
		}
		(a *= a) %=m;
		n >>= 1;
	}
	return result;
}

big_int Function::modSqrt(const big_int &a, const big_int &p)
{
	big_int b;
	do
	{
		b = big_int::random_with_length(255);
	} while (Function::legendre(b, p) == 1);

	big_int s = 0;
	big_int t = p - 1;
	while (!t.is_odd())
	{
		s++;
		t = t >> 1;
	}
	big_int inv_a = inverse_mod(a, p);
	big_int c = power_mod(b, t, p);
	big_int r = power_mod(a, (t + 1) / 2, p);
	big_int d = 0;
	for (int i = 1; i < s; i++)
	{
		big_int tmp = 2;
		tmp = power_mod(tmp, s - i - 1, p);
		d = power_mod(power_mod(r, 2, p) * inv_a, tmp, p);
		if (d == (p - 1))
		{
			r = (r * c) % p;
		}
		c = power_mod(c, 2, p);
	}
	return r;
}

big_int Function::legendre(const big_int &a, const big_int &p)
{
	return power_mod(a, (p - 1) / 2, p);
}


