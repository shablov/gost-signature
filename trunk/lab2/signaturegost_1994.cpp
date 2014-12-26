#include "signaturegost_1994.h"

#include "function.h"

#include <QDebug>

SignatureGost_1994::SignatureGost_1994() : SignatureInterface()
{
}

void SignatureGost_1994::setParameter(const QByteArray &key, const big_int &value)
{
	SignatureInterface::setParameter(key, value);
	if (key == "q")
	{
		modul = value;
	}
}

void SignatureGost_1994::setParameters(CryptoGeneratorInterface *parameters)
{
	SignatureInterface::setParameters(parameters);
	modul = getParameter("q");
}

big_int SignatureGost_1994::calculateR(const big_int &k)
{
	big_int r = power_mod(getParameter("alpha"), k, getParameter("p"));
	return mod(r, getParameter("q"));
}

big_int SignatureGost_1994::calculateV()
{
	return power_mod(hash, modul - 2, modul);
}

big_int SignatureGost_1994::calculateU(const big_int &z1, const big_int &z2)
{
	big_int u = power_mod( getParameter("alpha"), z1,  getParameter("p"));
	u *= power_mod(publicKey, z2, getParameter("p"));
	u = mod(u, getParameter("p"));
	return mod(u, modul);
}

void SignatureGost_1994::generateNewKeys()
{
	if (cryptoGenerator == 0)
	{
		qDebug() << Q_FUNC_INFO << "CryptoGenerator doesn't exist";
		return;
	}
	modul = getParameter("q");
	generateSecretKey();
	publicKey = power_mod(getParameter("alpha"), secretKey, getParameter("p"));
}

void SignatureGost_1994::generateSecretKey()
{
	secretKey = big_int::random_in_range(getParameter("q"));
}

int SignatureGost_1994::gostYear()
{
	return 1994;
}

bool SignatureGost_1994::secretKeyIsCorrect()
{
	return (secretKey < getParameter("q"));
}
