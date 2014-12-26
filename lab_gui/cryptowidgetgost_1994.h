#ifndef CRYPTOWIDGETGOST_1994_H
#define CRYPTOWIDGETGOST_1994_H

#include "cryptowidget.h"

class CryptoWidgetGost_1994 : public CryptoWidget
{
public:
	CryptoWidgetGost_1994(QWidget *parent = 0);

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
	QLineEdit *qKey;
	QLineEdit *alphaKey;
};

#endif // CRYPTOWIDGETGOST_1994_H
