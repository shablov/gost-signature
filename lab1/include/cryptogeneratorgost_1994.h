#ifndef CRYPTOGENERATORGOST_1994_H
#define CRYPTOGENERATORGOST_1994_H

#include "cryptogeneratorinterface.h"

#include <QList>

class CryptoGeneratorGost_1994 : public CryptoGeneratorInterface
{
public:
	CryptoGeneratorGost_1994(const unsigned int &multiplier);
	virtual ~CryptoGeneratorGost_1994();
	virtual void generate();

	void setY0(const big_int &y0);
	big_int getY0();

	void setKeyLength(const unsigned int &length);

protected:
	void generateBeginParameters();
	virtual void generatePAndQ() = 0;
	void generateAlpha();


	virtual QList<big_int> calculateY_Array(const unsigned int &arraySize);
	virtual big_int calculateNumber(const big_int &N, const big_int &k, const big_int &prevP) = 0;
	big_int calculateP(big_int pSize, big_int prevP);
	big_int calculateRm(const unsigned int &bitsQuantity);

	virtual bool checkP(const big_int &prevP, const big_int &N, const big_int &k, const big_int &pSize) = 0;

	big_int y0;
	big_int c;
	unsigned int pSize;
	const unsigned int normalBitsQuantity;
	unsigned int bitsMultiplier;
private:
	big_int generateNumber();
	big_int generateOddNumber();

	big_int calculateProbablyP(const big_int &pSize, const big_int &prevP);
	virtual big_int calculateN(const big_int &p, const big_int &t, const big_int &Ym) = 0;
	big_int calculateYm(const QList<big_int> &y_array);

	unsigned int standartGostMultiplier();
};

class CryptoGeneratorGost_1994_MethodA : public CryptoGeneratorGost_1994
{
public:
	CryptoGeneratorGost_1994_MethodA(const unsigned int &multiplier);
	void generateQ();

protected:
	virtual void generatePAndQ();

private:
	QList<big_int> calculateT_Array();
	big_int calculateN(const big_int &p, const big_int &t, const big_int &Ym);
	big_int calculateNumber(const big_int &N, const big_int &k, const big_int &prevP);

	big_int generateMinPrimeWithLength(const unsigned int &bitsQuantity);

	bool checkP(const big_int &prevP, const big_int &N, const big_int &k, const big_int &pSize);
};

class CryptoGeneratorGost_1994_MethodB : public CryptoGeneratorGost_1994
{
public:
	CryptoGeneratorGost_1994_MethodB(const unsigned int &multiplier);

protected:
	virtual void generatePAndQ();

private:
	big_int calculateN(const big_int &q, const big_int &pSize, const big_int &Ym);
	QList<big_int> calculateY_Array(const big_int &);
	big_int calculateNumber(const big_int &N, const big_int &k, const big_int &prevP);

	big_int generateQ(const unsigned int &bitsQuantity);

	bool checkP(const big_int &prevP, const big_int &N, const big_int &k, const big_int &pSize);
};

#endif // CRYPTOGENERATORGOST_1994_H
