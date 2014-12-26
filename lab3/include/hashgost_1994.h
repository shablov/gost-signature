#ifndef HASHGOST_1994_H
#define HASHGOST_1994_H

#include <hashinterface.h>

class HashGost_1994 : public HashInterface
{
public:
	HashGost_1994();

	virtual QByteArray hash(QIODevice *inputDevice, const HashSize &size);
};

#endif // HASHGOST_1994_H
