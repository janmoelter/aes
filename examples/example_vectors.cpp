#include <AES/AES.hpp>

int main(void)
{

	// PRINT THE EXAMPLES VECTORS INCLUDING INTERMEDIATE VALUES FOR ALL 3 KEY LENGTHS OF THE AES STANDARD (cf. NIST FIPS 197, Appendix C)

	// AES-128 (NIST FIPS 197, Appendix C.1)
	AES AES128 = AES(AES::Variant::AES_128);
	AES128.compute_example_vectors();
	
	// AES-128 (NIST FIPS 197, Appendix C.2)
	AES AES192 = AES(AES::Variant::AES_192);
	AES192.compute_example_vectors();
	
	// AES-128 (NIST FIPS 197, Appendix C.3)
	AES AES256 = AES(AES::Variant::AES_256);
	AES256.compute_example_vectors();

}
