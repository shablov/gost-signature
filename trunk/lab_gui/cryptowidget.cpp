#include "cryptowidget.h"

#include "function.h"
#include "signatureinterface.h"

#include <QLineEdit>

CryptoWidget::CryptoWidget(QWidget *parent) :
	QWidget(parent), sign(0)
{
	connect(this, SIGNAL(generatedParameters()), SLOT(onGeneratedParameters()));
	connect(this, SIGNAL(generatedKeys()), SLOT(onGeneratedKeys()));
}

CryptoWidget::~CryptoWidget()
{
	delete sign;
}

void CryptoWidget::initializeGui()
{
	publicKey = new QLineEdit(this);
	secretKey = new QLineEdit(this);
}

void CryptoWidget::onGeneratedKeys()
{
	publicKey->setText(keyToHex(sign->getPublicKey()));
	secretKey->setText(keyToHex(sign->getSecretKey()));
}

QByteArray CryptoWidget::keyToHex(const big_int &key)
{
	QByteArray keyBA = Function::big_intToByteArray(key);
	return keyBA.toHex();
}

void CryptoWidget::generateNewParameters()
{
	sign->generateNewParameters();
	emit generatedParameters();
	emit generatedKeys();
}

void CryptoWidget::generateNewKeys()
{
	if (sign->getParameters())
	{
		sign->generateNewKeys();
	}
	emit generatedKeys();
}

bool CryptoWidget::saveSecretKey(QIODevice *file)
{
	return save(file, sign->getSecretKey());
}

bool CryptoWidget::save(QIODevice *file, const big_int &key)
{
	if (!file->isWritable())
	{
		return false;
	}
	QDataStream out(file);
	return saveCryptoParameters(out) && saveKey(out, key);
}

bool CryptoWidget::saveKey(QDataStream &out, const big_int &key)
{
	QByteArray bytes = Function::big_intToByteArray(key);
	if (key.sign() == -1)
	{
		bytes.prepend("-");
	}
	out << (quint32)(bytes.length());
	out.writeRawData(bytes.constData(), bytes.length());
	return true;
}

bool CryptoWidget::savePublicKey(QIODevice *file)
{
	return save(file, sign->getPublicKey());
}

bool CryptoWidget::loadSecretKey(QIODevice *file)
{
	QDataStream in(file);
	if (!loadIfPossible(in))
	{
		return false;
	}
	return loadSecretKey(in);
}

bool CryptoWidget::loadIfPossible(QDataStream &in)
{
	if (!(in.device()->isReadable()))
	{
		return false;
	}
	return loadCryptoParameters(in);
}

bool CryptoWidget::loadSecretKey(QDataStream &in)
{
	big_int key = loadKey(in);
	sign->setSecretKey(key);
	return key != 0;
}

big_int CryptoWidget::loadKey(QDataStream &in)
{
	quint32 length = 0;
	in >> length;
	QByteArray bytes;
	bytes.resize(length);
	in.readRawData(bytes.data(), length);
	int tmpSign = 1;
	if (bytes.at(0) == '-')
	{
		tmpSign = -1;
		bytes = bytes.remove(0, 1);
	}
	return tmpSign * Function::big_intFromByteArray(bytes);
}

bool CryptoWidget::loadPublicKey(QIODevice *file)
{
	QDataStream in(file);
	if (!loadIfPossible(in))
	{
		return false;
	}
	return loadPublicKey(in);
}

bool CryptoWidget::loadPublicKey(QDataStream &in)
{
	big_int key = loadKey(in);
	sign->setPublicKey(key);
	return key != 0;
}

SignatureInterface *CryptoWidget::signature()
{
	return sign;
}

void CryptoWidget::saveChangesFromWidget()
{
	saveParametersChanges();
	saveKeysChanges();
}

void CryptoWidget::saveKeysChanges()
{
	sign->setPublicKey(keyFromHex(publicKey->text()));
	sign->setSecretKey(keyFromHex(secretKey->text()));
}

big_int CryptoWidget::keyFromHex(const QByteArray &keyBA)
{
	int tmpSign = 1;
	QByteArray tmpKeyBA = keyBA;
	if (keyBA.at(0) == '-')
	{
		tmpKeyBA = tmpKeyBA.remove(0, 1);
		tmpSign = -1;
	}
	return tmpSign * Function::big_intFromByteArray(QByteArray::fromHex(tmpKeyBA));
}

big_int CryptoWidget::keyFromHex(const QString &keyS)
{
	if (keyS.isEmpty())
	{
		return big_int();
	}
	return keyFromHex(keyS.toUtf8());
}


void CryptoWidget::updateGui()
{
	emit generatedParameters();
	emit generatedKeys();
}
