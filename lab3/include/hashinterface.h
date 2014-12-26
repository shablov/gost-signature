#ifndef HASHINTERFACE_H
#define HASHINTERFACE_H

#include <QByteArray>

class QIODevice;

class HashInterface
{
public:
	HashInterface();
	virtual ~HashInterface();

	enum HashSize {
		Base = 256,
		DoubleBase = 512
	};

	virtual QByteArray hash(QIODevice *inputDevice, const HashSize &size);

protected:
	QByteArray data;
};

#endif // HASHINTERFACE_H
