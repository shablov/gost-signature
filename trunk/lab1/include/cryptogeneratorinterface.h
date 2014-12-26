#ifndef CRYPTOGENERATORINTERFACE_H
#define CRYPTOGENERATORINTERFACE_H

#include <big_int.hpp>

#include <QByteArray>
#include <QMap>

using namespace Arageli;

class CryptoGeneratorInterface
{
public:
	virtual ~CryptoGeneratorInterface();

	virtual void generate() = 0;
	big_int getParameter(const QByteArray &key);
	void setParameters(QMap<QByteArray, big_int> newParameters);
	void setParameter(const QByteArray &key, const big_int &value);

protected:
	QMap<QByteArray, big_int> parameters;
};

#endif // CRYPTOGENERATORINTERFACE_H
