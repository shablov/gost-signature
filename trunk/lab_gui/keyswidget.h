#ifndef KEYSWIDGET_H
#define KEYSWIDGET_H

#include <QWidget>

class QLineEdit;
class SignatureInterface;

class KeysWidget : public QWidget
{
	Q_OBJECT
public:
	explicit KeysWidget(QWidget *parent = 0);

	bool maybeSave();

	SignatureInterface *signature();

signals:

public slots:
	void openPubKey();
	void openSecKey();
	bool savePubKey();
	bool saveSecKey();
	void generateNewKeys();

signals:
	void generated();

private slots:
	void updateGui();

private:
	void createWidgets();

	bool maybeSaveKey(QLineEdit *keyLE);
	bool cryptoParametersIsModified();
	void setCryptoParametersModified(bool modified);

	void loadKeyFile(const QString fileName, QLineEdit *keyLE);

	void setOverrideWaitCursor();
	void restoreOverrideCursor();

	QByteArray parameterFromStream(QDataStream &in);
	void parameterToStream(QDataStream &out, const QByteArray &parameter);

	bool saveKeyAs(QLineEdit *keyLE);
	bool saveKeyFile(const QString &fileName, QLineEdit *keyLE);
	bool saveSecretKeyFile(const QString &fileName);

	QLineEdit *pLE;
	QLineEdit *qLE;
	QLineEdit *alphaLE;
	QLineEdit *secKeyLE;
	QLineEdit *pubKeyLE;

	SignatureInterface *sign;
};

#endif // KEYSWIDGET_H
