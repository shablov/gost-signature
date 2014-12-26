#include "cryptowidgetgost_1994.h"

#include "signaturefactory.h"
#include "signatureinterface.h"

#include "function.h"

#include <QLineEdit>

#include <QFormLayout>

CryptoWidgetGost_1994::CryptoWidgetGost_1994(QWidget *parent) :
	CryptoWidget(parent)
{
	initializeGui();
	layoutManagment();
	sign = SignatureFactory::signatureByYear(1994);
}

bool CryptoWidgetGost_1994::saveCryptoParameters(QDataStream &out)
{
	bool result = saveKey(out, sign->getParameter("p"));
	result &= saveKey(out, sign->getParameter("q"));
	result &= saveKey(out, sign->getParameter("alpha"));
	return result;
}

bool CryptoWidgetGost_1994::loadCryptoParameters(QDataStream &in)
{
	sign->setParameter("p", loadKey(in));
	sign->setParameter("q", loadKey(in));
	sign->setParameter("alpha", loadKey(in));

	return true;
}

void CryptoWidgetGost_1994::initializeGui()
{
	CryptoWidget::initializeGui();
	pKey = new QLineEdit(this);
	qKey = new QLineEdit(this);
	alphaKey = new QLineEdit(this);
}

void CryptoWidgetGost_1994::layoutManagment()
{
	QFormLayout *mainLayout = new QFormLayout(this);
	mainLayout->addRow("p = ", pKey);
	mainLayout->addRow("q = ", qKey);
	mainLayout->addRow("alpha = ", alphaKey);
	mainLayout->addRow("public key = ", publicKey);
	mainLayout->addRow("secret key = ", secretKey);
}

void CryptoWidgetGost_1994::saveParametersChanges()
{
	sign->setParameter("p", keyFromHex(pKey->text()));
	sign->setParameter("q",keyFromHex(qKey->text()));
	sign->setParameter("alpha",keyFromHex(alphaKey->text()));
}

void CryptoWidgetGost_1994::onGeneratedParameters()
{
	pKey->setText(keyToHex(sign->getParameter("p")));
	qKey->setText(keyToHex(sign->getParameter("q")));
	alphaKey->setText(keyToHex(sign->getParameter("alpha")));
}
