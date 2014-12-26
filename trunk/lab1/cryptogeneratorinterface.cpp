#include "cryptogeneratorinterface.h"

CryptoGeneratorInterface::~CryptoGeneratorInterface()
{
}

big_int CryptoGeneratorInterface::getParameter(const QByteArray &key)
{
	return parameters.value(key, big_int(-1));
}

void CryptoGeneratorInterface::setParameters(QMap<QByteArray, big_int> newParameters)
{
	QListIterator<QByteArray> i(newParameters.keys());
	while(i.hasNext())
	{
		const QByteArray &key = i.next();
		if (!parameters.contains(key))
		{
			return;
		}
	}
	parameters = newParameters;
}

void CryptoGeneratorInterface::setParameter(const QByteArray &key, const big_int &value)
{
	if (parameters.contains(key))
	{
		parameters[key] = value;
	}
}
