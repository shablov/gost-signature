#ifndef CRYPTOWIDGETFACTORY_H
#define CRYPTOWIDGETFACTORY_H

class CryptoWidget;

class CryptoWidgetFactory
{
public:
	static CryptoWidget *getCryptoWidget(int year);
};

#endif // CRYPTOWIDGETFACTORY_H
