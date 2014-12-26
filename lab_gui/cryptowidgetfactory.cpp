#include "cryptowidgetfactory.h"
#include "cryptowidgetgost_1994.h"
#include "cryptowidgetgost_2012.h"

CryptoWidget *CryptoWidgetFactory::getCryptoWidget(int year)
{
	switch (year)
	{
		case 2012:
		{
			return new CryptoWidgetGost_2012();
		}
		case 1994:
		default:
		{
			return new CryptoWidgetGost_1994();
		}
	}
}
