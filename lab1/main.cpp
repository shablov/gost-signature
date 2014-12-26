#include <QCoreApplication>
#include <QDebug>

#include <iostream>
#include <time.h>

#include "cryptogeneratorinterface.h"
#include "cryptogeneratorfactory.h"
#include "cryptogeneratorgost_1994.h"
#include "prime.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	QCoreApplication(argc, argv);

	srand(time(NULL));

	CryptoGeneratorInterface *cp = CryptoGeneratorFactory::cryptoGeneratorByYear(1994);
	cp->generate();

	big_int p = cp->getParameter("p");
	big_int a = cp->getParameter("q");
	big_int b = cp->getParameter("alpha");
	delete cp;
	cp = 0;

	qDebug() << p.length();

	cout << p << "\n";
	cout << pow2<big_int>(521) - 1 << "\n\n";
	cout << a << "\n\n";
	cout << b << "\n\n";

	cout << is_pseudoprime_miller_rabin(p, 2) << "\n";

	return 0;
}
