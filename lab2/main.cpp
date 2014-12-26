#include <QCoreApplication>
#include <QByteArray>
#include <QDebug>

#include <iostream>
#include <time.h>

#include "signaturefactory.h"
#include "signaturegost_1994.h"
#include "function.h"

#include "prime.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QCoreApplication(argc, argv);

	srand(time(NULL));

	SignatureInterface *cp = SignatureFactory::signatureByYear(2012);
	cp->generateNewParameters();
	cp->setMessage("Hello World");
	if (!cp->calculateSign())
	{
		return 0;
	}
	qDebug() << cp->getSign().toHex();
	qDebug() << cp->getSign().toHex().length();

	SignatureInterface *cp1 = SignatureFactory::signatureByYear(2012);
	cp1->setMessage("Hello World");
	cp1->setParameters(cp->getParameters());
	cp1->setPublicKey(cp->getPublicKey());
	cp1->setSign(cp->getSign());

	qDebug() << cp->verifySign();
	qDebug() << cp1->verifySign();

	delete cp;
	cp = 0;

	return 0;
}
