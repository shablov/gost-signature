#include "cryptogeneratorfactory.h"
#include "cryptogeneratorinterface.h"
#include "cryptogeneratorgost_1994.h"
#include "cryptogeneratorgost_2012.h"

CryptoGeneratorInterface *CryptoGeneratorFactory::cryptoGeneratorByYear(int year)
{
	switch (year) {
		case 2012:
		{
			return new CryptoGeneratorGost_2012();
		}
		case 1994:
		default:
		{
			return new CryptoGeneratorGost_1994_MethodB(1);
		}
	}
}
