#ifndef HASHGOST_2012_H
#define HASHGOST_2012_H

#include <hashinterface.h>

class HashGost_2012 : public HashInterface
{
public:
	HashGost_2012();

	virtual QByteArray hash(QIODevice *inputDevice, const HashSize &size);

private:
	QByteArray hash_x(const QByteArray &data, const QByteArray &beginVector);
	QByteArray hash_256(const QByteArray &data);
	QByteArray hash_512(const QByteArray &data);

	void compression(const QByteArray &N, QByteArray &h, const QByteArray &partMessage);
	QByteArray addVectorMod(const QByteArray &first, const QByteArray &second);
	QByteArray xorVectorMod(const QByteArray &first, const QByteArray &second);

	void S(QByteArray &K);
	void P(QByteArray &K);
	void L(QByteArray &K);

	QByteArray E(QByteArray &K, const QByteArray &partMessage);

	void KeySchedule(QByteArray &K, const int &iteration);
};

#endif // HASHGOST_2012_H
