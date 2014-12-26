#include "hashfactory.h"
#include "hashinterface.h"
#include "hashgost_1994.h"
#include "hashgost_2012.h"

#include <QDataStream>

QByteArray HashFactory::hash(const int &year, QIODevice *inputDevice, const HashInterface::HashSize &size)
{
	HashInterface *alghorithm;
	switch (year)
	{
		case 2012:
		{
			alghorithm = new HashGost_2012();
			break;
		}
		case 1994:
		default:
		{
			alghorithm = new HashGost_1994();
			break;
		}
	}
	return alghorithm->hash(inputDevice, size);
}

QByteArray HashFactory::hash(const int &year, QByteArray data, const HashInterface::HashSize &size)
{
	QDataStream stream(data);
	return hash(year, stream.device(), size);
}
