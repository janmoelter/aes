#include "AES_module.hpp"

int main(void)
{

	// PRINT THE EXAMPLES VECTORS INCLUDING INTERMEDIATE VALUES FOR ALL 3 KEY LENGTHS OF THE AES STANDARD (cf. NIST FIPS 197, Appendix C)

	// AES-128 (NIST FIPS 197, Appendix C.1)
	AES_module AES128 = AES_module(AES_standard::AES128);
	AES128.test_standard();
	
	// AES-128 (NIST FIPS 197, Appendix C.2)
	AES_module AES192 = AES_module(AES_standard::AES192);
	AES192.test_standard();
	
	// AES-128 (NIST FIPS 197, Appendix C.3)
	AES_module AES256 = AES_module(AES_standard::AES256);
	AES256.test_standard();

}
