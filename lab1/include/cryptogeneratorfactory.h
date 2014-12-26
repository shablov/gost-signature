#ifndef CRYPTOPAGENERATORFACTORY_H
#define CRYPTOPAGENERATORFACTORY_H

class CryptoGeneratorInterface;

class CryptoGeneratorFactory
{
public:
	static CryptoGeneratorInterface *cryptoGeneratorByYear(int year);
};

#endif // CRYPTOPAGENERATORFACTORY_H
