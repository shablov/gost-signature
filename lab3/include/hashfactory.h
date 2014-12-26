#ifndef HASHFACTORY_H
#define HASHFACTORY_H

#include <QByteArray>
#include <hashinterface.h>

class QIODevice;

class HashFactory
{
public:
	static QByteArray hash(const int &year, QIODevice *inputDevice,
						   const HashInterface::HashSize &size = HashInterface::Base);
	static QByteArray hash(const int &year, QByteArray data,
							const HashInterface::HashSize &size = HashInterface::Base);
};
#endif // HASHFACTORY_H
