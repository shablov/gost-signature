#ifndef CRYPTOGENERATORGOST_2012_H
#define CRYPTOGENERATORGOST_2012_H

#include "cryptogeneratorinterface.h"

class CryptoGeneratorGost_2012 : public CryptoGeneratorInterface
{
public:
	CryptoGeneratorGost_2012();
	virtual ~CryptoGeneratorGost_2012();
	virtual void generate();

protected:
};

#endif // CRYPTOGENERATORGOST_2012_H
