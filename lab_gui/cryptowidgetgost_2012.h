#ifndef CRYPTOWIDGETGOST_2012_H
#define CRYPTOWIDGETGOST_2012_H

#include "cryptowidget.h"

class CryptoWidgetGost_2012 : public CryptoWidget
{
public:
	CryptoWidgetGost_2012(QWidget *parent = 0);

protected:
	virtual bool saveCryptoParameters(QDataStream &out);
	virtual bool loadCryptoParameters(QDataStream &in);

	virtual void initializeGui();
	virtual void layoutManagment();

	virtual void saveParametersChanges();

protected slots:
	virtual void onGeneratedParameters();

private:
	QLineEdit *pKey;
	QLineEdit *nKey;
	QLineEdit *aKey;
	QLineEdit *bKey;
	QLineEdit *GxKey;
	QLineEdit *GyKey;
	QLineEdit *hKey;
};

#endif // CRYPTOWIDGETGOST_2012_H
