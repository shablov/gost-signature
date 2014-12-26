#include "signaturefactory.h"
#include "signatureinterface.h"
#include "signaturegost_1994.h"
#include "signaturegost_2012.h"

SignatureInterface *SignatureFactory::signatureByYear(int year)
{
	switch (year)
	{
		case 2012:
		{
			return new SignatureGost_2012();
		}
		case 1994:
		default:
		{
			return new SignatureGost_1994();
		}
	}
}
