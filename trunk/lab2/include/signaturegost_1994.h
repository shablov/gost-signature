#ifndef SIGNATUREGOST_1994_H
#define SIGNATUREGOST_1994_H

#include <signatureinterface.h>

#include <QPair>

class SignatureGost_1994 : public SignatureInterface
{
public:
	SignatureGost_1994();

	virtual void setParameter(const QByteArray &key, const big_int &value);
	virtual void setParameters(CryptoGeneratorInterface *parameters);

	virtual void generateNewKeys();
protected:
	virtual int gostYear();

	virtual bool secretKeyIsCorrect();

	virtual big_int calculateR(const big_int &k);

private:
	void generateSecretKey();

	virtual big_int calculateV();
	virtual big_int calculateU(const big_int &z1, const big_int &z2);
};

#endif // SIGNATUREGOST_1994_H
