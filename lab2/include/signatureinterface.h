#ifndef SIGNATUREINTERFACE_H
#define SIGNATUREINTERFACE_H

#include <big_int.hpp>

using namespace Arageli;

#include <QByteArray>
#include <QPair>

class QIODevice;
class CryptoGeneratorInterface;


class SignatureInterface
{
public:
	SignatureInterface();
	virtual ~SignatureInterface();
	void setInputDevice(QIODevice *inputDevice);
	void setMessage(const QByteArray &message);

	big_int getParameter(const QByteArray &key);
	CryptoGeneratorInterface *getParameters();
	virtual void setParameter(const QByteArray &key, const big_int &value);
	virtual void setParameters(CryptoGeneratorInterface *parameters);

	big_int getSecretKey();
	big_int getPublicKey();

	void setSecretKey(const big_int &key);
	virtual void setPublicKey(const big_int &key);

	void setSign(const QByteArray &sign);
	QByteArray getSign();

	bool calculateSign();
	virtual bool verifySign();

	void generateNewParameters();
	virtual void generateNewKeys() = 0;

protected:
	virtual int gostYear() = 0;
	bool mayBeSigned();
	bool mayBeVerified();
	virtual bool secretKeyIsCorrect() = 0;

	QPair<big_int, big_int> calculateSignParts();
	virtual big_int calculateK();
	virtual big_int calculateR(const big_int &k) = 0;
	virtual big_int calculateS(const big_int &k, const big_int &r);

	virtual big_int calculateV() = 0;
	virtual big_int calculateSFromSign();
	virtual big_int calculateRFromSign();
	virtual big_int calculateZ1(const big_int &v, const big_int &s);
	virtual big_int calculateZ2(const big_int &v, const big_int &r);
	virtual big_int calculateU(const big_int &z1, const big_int &z2) = 0;

	bool openInputDevice();
	void tryOpenInputDevice();
	QIODevice *inputDevice;

	void calculateHash();
	big_int hash;

	CryptoGeneratorInterface *cryptoGenerator;

	big_int secretKey;
	big_int publicKey;

	QByteArray signature;

	big_int modul;
};

#endif // SIGNATUREINTERFACE_H
