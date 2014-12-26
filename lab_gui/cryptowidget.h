#ifndef CRYPTOWIDGET_H
#define CRYPTOWIDGET_H

#include <QWidget>

#include <big_int.hpp>

using namespace Arageli;

class SignatureInterface;

class QLineEdit;

class CryptoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit CryptoWidget(QWidget *parent = 0);
	virtual ~CryptoWidget();

	void generateNewParameters();
	void generateNewKeys();

	bool saveSecretKey(QIODevice *file);
	bool savePublicKey(QIODevice *file);

	bool loadSecretKey(QIODevice *file);
	bool loadPublicKey(QIODevice *file);

	void saveChangesFromWidget();

	void updateGui();

	SignatureInterface *signature();

signals:
	void generatedParameters();
	void generatedKeys();

public slots:

protected:
	virtual bool saveCryptoParameters(QDataStream &out) = 0;
	virtual bool loadCryptoParameters(QDataStream &in) = 0;

	bool saveKey(QDataStream &out, const big_int& key);
	big_int loadKey(QDataStream &in);

	virtual void initializeGui();
	virtual void layoutManagment() = 0;

	virtual void saveParametersChanges() = 0;

	QByteArray keyToHex(const big_int &key);
	big_int keyFromHex(const QByteArray &keyBA);
	big_int keyFromHex(const QString &keyS);

	SignatureInterface *sign;

	QLineEdit *publicKey;
	QLineEdit *secretKey;

protected slots:
	void onGeneratedKeys();
	virtual void onGeneratedParameters() = 0;

private:
	bool save(QIODevice *file, const big_int &key);

	bool loadIfPossible(QDataStream &in);
	bool loadPublicKey(QDataStream &in);
	bool loadSecretKey(QDataStream &in);

	void saveKeysChanges();
};

#endif // CRYPTOWIDGET_H
