#include "hashgost_1994.h"

HashGost_1994::HashGost_1994() : HashInterface()
{
}

QByteArray HashGost_1994::hash(QIODevice *inputDevice, const HashSize &size)
{
	HashInterface::hash(inputDevice, size);
	char result[size / 8];
	memset(result, 0x00, size / 8);

	qint64 tmp = data.length();
	unsigned char additional[size / 8];
	if (tmp >= size / 8)
	{
		while (tmp >= (size / 8))
		{
			memcpy(additional, data.data() + tmp - size / 8, size / 8);
			for (int i = 0; i < size / 8; i++)
			{
				result[i] = result[i] ^ additional[i];
			}
			tmp -= size / 8;
		}
	}
	memset(additional, 0x00, size / 8);
	memcpy(additional, data.data(), tmp);
	for (int i = 0; i < size / 8; i++)
	{
		result[i] = result[i] ^ additional[i];
	}
	return QByteArray(result, size / 8);
}
