#include "cryptowidgetgost_2012.h"

#include "signaturefactory.h"
#include "signatureinterface.h"

#include "function.h"

#include <QFormLayout>

#include <QLineEdit>

CryptoWidgetGost_2012::CryptoWidgetGost_2012(QWidget *parent) :
	CryptoWidget(parent)
{
	initializeGui();
	layoutManagment();
	sign = SignatureFactory::signatureByYear(2012);
}

bool CryptoWidgetGost_2012::saveCryptoParameters(QDataStream &out)
{
	bool result = saveKey(out, sign->getParameter("p"));
	result &= saveKey(out, sign->getParameter("n"));
	result &= saveKey(out, sign->getParameter("a"));
	result &= saveKey(out, sign->getParameter("b"));
	result &= saveKey(out, sign->getParameter("Gx"));
	result &= saveKey(out, sign->getParameter("Gy"));
	result &= saveKey(out, sign->getParameter("h"));
	return result;
}

bool CryptoWidgetGost_2012::loadCryptoParameters(QDataStream &in)
{
	sign->setParameter("p", loadKey(in));
	sign->setParameter("n", loadKey(in));
	sign->setParameter("a", loadKey(in));
	sign->setParameter("b", loadKey(in));
	sign->setParameter("Gx", loadKey(in));
	sign->setParameter("Gy", loadKey(in));
	sign->setParameter("h", loadKey(in));

	return true;
}

void CryptoWidgetGost_2012::initializeGui()
{
	CryptoWidget::initializeGui();
	pKey = new QLineEdit(this);
	nKey = new QLineEdit(this);
	aKey = new QLineEdit(this);
	bKey = new QLineEdit(this);
	GxKey = new QLineEdit(this);
	GyKey = new QLineEdit(this);
	hKey = new QLineEdit(this);
}

void CryptoWidgetGost_2012::layoutManagment()
{
	QFormLayout *mainLayout = new QFormLayout(this);
	mainLayout->addRow("p = ", pKey);
	mainLayout->addRow("n = ", nKey);
	mainLayout->addRow("a = ", aKey);
	mainLayout->addRow("b = ", bKey);
	mainLayout->addRow("Gx = ", GxKey);
	mainLayout->addRow("Gy = ", GyKey);
	mainLayout->addRow("h = ", hKey);
	mainLayout->addRow("public key = ", publicKey);
	mainLayout->addRow("secret key = ", secretKey);
}

void CryptoWidgetGost_2012::saveParametersChanges()
{
	sign->setParameter("p", keyFromHex(pKey->text()));
	sign->setParameter("n",keyFromHex(nKey->text()));
	sign->setParameter("a",keyFromHex(aKey->text()));
	sign->setParameter("b", keyFromHex(bKey->text()));
	sign->setParameter("Gx",keyFromHex(GxKey->text()));
	sign->setParameter("Gy",keyFromHex(GyKey->text()));
	sign->setParameter("h",keyFromHex(hKey->text()));
}

void CryptoWidgetGost_2012::onGeneratedParameters()
{
	pKey->setText(keyToHex(sign->getParameter("p")));
	nKey->setText(keyToHex(sign->getParameter("n")));
	QString tmp = "";
	if (sign->getParameter("a").sign() == -1)
	{
		tmp = "-";
	}
	aKey->setText(tmp + keyToHex(sign->getParameter("a")));
	tmp = "";
	if (sign->getParameter("b").sign() == -1)
	{
		tmp = "-";
	}
	bKey->setText(tmp + keyToHex(sign->getParameter("b")));
	GxKey->setText(keyToHex(sign->getParameter("Gx")));
	GyKey->setText(keyToHex(sign->getParameter("Gy")));
	hKey->setText(keyToHex(sign->getParameter("h")));
}
