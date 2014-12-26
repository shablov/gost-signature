#include "hashgost_2012.h"
#include "datagost_2012.h"

HashGost_2012::HashGost_2012() : HashInterface()
{
}

QByteArray HashGost_2012::hash(QIODevice *inputDevice, const HashInterface::HashSize &size)
{
	HashInterface::hash(inputDevice, size);
	switch (size)
	{
		case Base:
		{
			return hash_256(data);
			break;
		}
		case DoubleBase:
		default:
		{
			return hash_512(data);
			break;
		}
	}
}

QByteArray HashGost_2012::hash_256(const QByteArray &data)
{
	QByteArray beginVector(64, char(0x01));

	return hash_x(data, beginVector).mid(0, 32);
}

QByteArray HashGost_2012::hash_512(const QByteArray &data)
{
	QByteArray beginVector(64, char(0x00));

	return hash_x(data, beginVector);
}

QByteArray HashGost_2012::hash_x(const QByteArray &data, const QByteArray &beginVector)
{
	QByteArray N(64, 0x00);
	QByteArray Summ(64, 0x00);
	QByteArray n512(64, 0x00);
	n512[62] = 0x02;

	QByteArray h = beginVector;

	QByteArray m(64, 0x00);

	quint64 size = (data.length()) * 8;

	while (size >= 512)
	{
		memcpy(m.data(), data.data() + size/8 - 63 - ( (size & 0x7) == 0 ), 64);

		compression(N, h, m);
		N = addVectorMod(N, n512);
		Summ = addVectorMod(Summ, m);

		size -= 512;
	}

	memset(m.data(),0,64);
	memcpy(m.data() + 63 - size/8 + ( (size & 0x7) == 0 ), data.data(), size/8 + 1 - ( (size & 0x7) == 0 ));
	m.data()[ 63 - size/8 ] |= (1 << (size & 0x7));

	compression(N, h, m);
	n512[63] = size & 0xFF;
	n512[62] = size >> 8;
	N = addVectorMod(N, n512);
	Summ = addVectorMod(Summ, m);

	compression(QByteArray(64, 0x00), h, N);
	compression(QByteArray(64, 0x00), h, Summ);
	return h;
}

void HashGost_2012::compression(const QByteArray &N, QByteArray &h, const QByteArray &partMessage)
{
	QByteArray K(64, 0x00);
	QByteArray t(64, 0x00);
	K = xorVectorMod(N,h);
	S(K);
	P(K);
	L(K);
	t = E(K, partMessage);
	t = xorVectorMod(t,h);
	h = xorVectorMod(t,partMessage);
}


QByteArray HashGost_2012::addVectorMod(const QByteArray &first, const QByteArray &second)
{
	QByteArray result(64, 0x00);
	int t = 0;
	for(int i = 63; i >= 0; i--)
	{
		t = quint8(first[i]) + quint8(second[i]) + (t >> 8);
		result[i] = t & 0xFF;
	}
	return result;
}

QByteArray HashGost_2012::xorVectorMod(const QByteArray &first, const QByteArray &second)
{
	QByteArray result(64, 0x00);
	const long long *A;
	A = (long long*)(first.constData());
	const long long *B;
	B = (long long*)(second.constData());
	unsigned long long *C;
	C = (unsigned long long*)(result.data());
	for(int i = 0; i < 8; i++)
	{
		C[i] = A[i] ^ B[i];
	}
	return result;
}

void HashGost_2012::S(QByteArray &K)
{
	unsigned char *resultData = (unsigned char *)(K.data());
	for(int i = 0; i < 64; i++)
	{
		resultData[i] = Sbox[(quint8)(K[i])];
	}
}

void HashGost_2012::P(QByteArray &K)
{
	int i = 0;
	unsigned char t[64] = {};
	for(i=0;i<64;i++)
	{
		t[i] = quint8(K[Tau[i]]);
	}
	memcpy(K.data(),t,64);
}

void HashGost_2012::L(QByteArray &K)
{
	unsigned char * resultData = (unsigned char *)(K.data());
	unsigned long long v = 0;
	int i = 0, j = 0, k = 0;
	for(i=0;i<8;i++)
	{
		v=0;
		for(k=0;k<8;k++)
		{
			for(j=0;j<8;j++)
			{
				if ((K[i*8+k] & (1<<(7-j))) != 0)
					v ^= A[k*8+j];
			}
		}
		for(k=0;k<8;k++)
		{
			resultData[i*8+k] = (v & ((unsigned long long)0xFF << (7-k)*8)) >> (7-k)*8;
		}
	}
}

QByteArray HashGost_2012::E(QByteArray &K, const QByteArray &partMessage)
{
	int i = 0;
	QByteArray result(64, 0x00);
	result = xorVectorMod(partMessage, K);
	for(i=0;i<12;i++)
	{
		S(result);
		P(result);
		L(result);
		KeySchedule(K, i);
		result = xorVectorMod(result, K);
	}
	return result;
}

void HashGost_2012::KeySchedule(QByteArray &K, const int &iteration)
{
	K = xorVectorMod(K, QByteArray((char*)(C[iteration]), 64));
	S(K);
	P(K);
	L(K);
}
