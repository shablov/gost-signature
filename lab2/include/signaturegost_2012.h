#ifndef SIGNATUREGOST_2012_H
#define SIGNATUREGOST_2012_H

#include <signatureinterface.h>

#include <ellipticcurvepoint.h>

#include <QPair>

class SignatureGost_2012 : public SignatureInterface
{
public:
	SignatureGost_2012();


	void setParameter(const QByteArray &key, const big_int &value);
	virtual void setParameters(CryptoGeneratorInterface *parameters);

	virtual void generateNewKeys();
protected:
	virtual int gostYear();

	virtual bool secretKeyIsCorrect();

	virtual big_int calculateR(const big_int &k);

private:
	void generateSecretKey();

	QPair<big_int, big_int> calculateSignParts();

	ECPoint G;

	virtual big_int calculateV();
	virtual big_int calculateU(const big_int &z1, const big_int &z2);
};

#endif // SIGNATUREGOST_2012_H
