#include "signatureinterface.h"

#include "cryptogeneratorinterface.h"
#include "cryptogeneratorfactory.h"

#include "hashfactory.h"

#include "function.h"

#include <QBuffer>

#include <QDebug>

SignatureInterface::SignatureInterface()
	: inputDevice(0), cryptoGenerator(0),
	  secretKey(0), publicKey(0),
	  signature("")
{
}

SignatureInterface::~SignatureInterface()
{
	delete cryptoGenerator;
	cryptoGenerator = 0;
}

void SignatureInterface::setInputDevice(QIODevice *input)
{
	inputDevice = input;
	tryOpenInputDevice();
}

void SignatureInterface::setMessage(const QByteArray &message)
{
	QBuffer *buffer = new QBuffer;
	buffer->setData(message);
	inputDevice = buffer;
	tryOpenInputDevice();
}

bool SignatureInterface::openInputDevice()
{
	if (!inputDevice->isOpen())
	{
		inputDevice->open(QIODevice::ReadOnly);
	}
	return inputDevice->isOpen();
}

void SignatureInterface::tryOpenInputDevice()
{
	if (!openInputDevice())
	{
		qDebug() << Q_FUNC_INFO << "Input device doesn't open.";
		return;
	}
	calculateHash();
}

void SignatureInterface::calculateHash()
{
	QByteArray h = HashFactory::hash(gostYear(), inputDevice, HashInterface::Base);
	hash = Function::big_intFromByteArray(h);
}

big_int SignatureInterface::getParameter(const QByteArray &key)
{
	return cryptoGenerator->getParameter(key);
}

CryptoGeneratorInterface *SignatureInterface::getParameters()
{
	return cryptoGenerator;
}

void SignatureInterface::setParameter(const QByteArray &key, const big_int &value)
{
	if (!cryptoGenerator)
	{
		cryptoGenerator = CryptoGeneratorFactory::cryptoGeneratorByYear(gostYear());
	}
	cryptoGenerator->setParameter(key, value);
}

void SignatureInterface::setParameters(CryptoGeneratorInterface *parameters)
{
	if (cryptoGenerator)
	{
		delete cryptoGenerator;
	}
	cryptoGenerator = parameters;
}

big_int SignatureInterface::getSecretKey()
{
	return secretKey;
}

big_int SignatureInterface::getPublicKey()
{
	return publicKey;
}

void SignatureInterface::setSecretKey(const big_int &key)
{
	secretKey = key;
}

void SignatureInterface::setPublicKey(const big_int &key)
{
	publicKey = key;
}

void SignatureInterface::setSign(const QByteArray &sign)
{
	signature = sign;
}

QByteArray SignatureInterface::getSign()
{
	return signature;
}

bool SignatureInterface::calculateSign()
{
	if (!mayBeSigned())
	{
		return false;
	}
	signature = QByteArray();

	QPair<big_int, big_int> signParts = calculateSignParts();

	QByteArray first = Function::big_intToByteArray(signParts.first);
	QByteArray second = Function::big_intToByteArray(signParts.second);

	quint8 tmp = (modul.length() % 8 == 0) ? 0 : 1;
	signature += first.rightJustified(modul.length() / 8 + tmp, 0x00);
	signature += second.rightJustified(modul.length() / 8 + tmp, 0x00);

	return true;
}

QPair<big_int, big_int> SignatureInterface::calculateSignParts()
{
	big_int s = 0;
	big_int r = 0;
	do
	{
		big_int k = calculateK();
		r = calculateR(k);
		s = calculateS(k, r);
	} while (s.is_null());
	return qMakePair(r, s);
}

big_int SignatureInterface::calculateK()
{
	return big_int::random_in_range(modul);
}

big_int SignatureInterface::calculateS(const big_int &k, const big_int &r)
{
	if (r.is_null())
	{
		return 0;
	}

	if (mod(hash, modul).is_null())
	{
		hash = 1;
	}

	big_int tmp = secretKey * r + k * hash;
	return mod(tmp, modul);
}

bool SignatureInterface::verifySign()
{
	if (!mayBeVerified())
	{
		return false;
	}
	hash = (mod(hash, modul) == 0) ? big_int(1) : hash;
	big_int v = calculateV();
	big_int s = calculateSFromSign();
	big_int r = calculateRFromSign();
	big_int z1 = calculateZ1(v, s);
	big_int z2 = calculateZ2(v, r);
	big_int u = calculateU(z1, z2);
	return u == r;
}

big_int SignatureInterface::calculateSFromSign()
{
	quint8 tmp = (modul.length() % 8 == 0) ? 0 : 1;
	return Function::big_intFromByteArray(signature.right(modul.length() / 8 + tmp));
}

big_int SignatureInterface::calculateRFromSign()
{
	quint8 tmp = (modul.length() % 8 == 0) ? 0 : 1;
	return Function::big_intFromByteArray(signature.left(modul.length() / 8 + tmp));
}

big_int SignatureInterface::calculateZ1(const big_int &v, const big_int &s)
{
	return mod(s * v, modul);
}

big_int SignatureInterface::calculateZ2(const big_int &v, const big_int &r)
{
	return mod((modul - r) * v, modul);
}

void SignatureInterface::generateNewParameters()
{
	if (!cryptoGenerator)
	{
		cryptoGenerator = CryptoGeneratorFactory::cryptoGeneratorByYear(gostYear());
	}
	cryptoGenerator->generate();
	generateNewKeys();
}

bool SignatureInterface::mayBeSigned()
{
	return  (inputDevice->isOpen()) &&
			(cryptoGenerator != 0) &&
			(secretKeyIsCorrect());
}

bool SignatureInterface::mayBeVerified()
{
	return  (!signature.isEmpty()) &&
			(inputDevice->isOpen()) &&
			(cryptoGenerator != 0);
}
