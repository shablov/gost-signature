#ifndef SIGNATUREFACTORY_H
#define SIGNATUREFACTORY_H

class SignatureInterface;

class SignatureFactory
{
public:
	static SignatureInterface *signatureByYear(int year);
};

#endif // SIGNATUREFACTORY_H
