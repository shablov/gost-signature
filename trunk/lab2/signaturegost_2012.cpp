#include "signaturegost_2012.h"

#include "function.h"

#include <QDebug>

SignatureGost_2012::SignatureGost_2012() : SignatureInterface()
{
}

void SignatureGost_2012::setParameter(const QByteArray &key, const big_int &value)
{
	SignatureInterface::setParameter(key, value);
	if (key == "n")
	{
		modul = value;
	}
	G = ECPoint(getParameter("a"), getParameter("b"),
				getParameter("Gx"), getParameter("Gy"),
				getParameter("p"));

}

void SignatureGost_2012::setParameters(CryptoGeneratorInterface *parameters)
{
	SignatureInterface::setParameters(parameters);
	modul = getParameter("n");
	G = ECPoint(getParameter("a"), getParameter("b"),
				getParameter("Gx"), getParameter("Gy"),
				getParameter("p"));
}

big_int SignatureGost_2012::calculateV()
{
	return inverse_mod(hash, getParameter("n"));
}

big_int SignatureGost_2012::calculateU(const big_int &z1, const big_int &z2)
{
	ECPoint Q = ECPoint::decompressionCoordinate(publicKey,
												 getParameter("a"), getParameter("b"),
												 getParameter("p"));
	ECPoint C = z1 * G + z2 *Q;
	return mod(C.x, modul);
}

void SignatureGost_2012::generateNewKeys()
{
	if (cryptoGenerator == 0)
	{
		qDebug() << Q_FUNC_INFO << "CryptoGenerator doesn't exist";
		return;
	}
	modul = getParameter("n");
	generateSecretKey();
	G = ECPoint(getParameter("a"), getParameter("b"),
				getParameter("Gx"), getParameter("Gy"),
				getParameter("p"));

	ECPoint Q = secretKey * G;
	publicKey = ECPoint::compressionCoordinate(Q);
}

void SignatureGost_2012::generateSecretKey()
{
	secretKey = big_int::random_in_range(getParameter("n"));
}

int SignatureGost_2012::gostYear()
{
	return 2012;
}

bool SignatureGost_2012::secretKeyIsCorrect()
{
	return secretKey < getParameter("n");
}

big_int SignatureGost_2012::calculateR(const big_int &k)
{
	ECPoint C = k * G;
	return mod(C.x, getParameter("n"));
}
