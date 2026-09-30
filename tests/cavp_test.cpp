#include <cstddef>

#include <iostream>
#include <fstream>

#include <string>
#include <regex>

#include <vector>
#include <map>

#include <AES/AES.hpp>


struct test_vector_t
{
	std::size_t count;
	
	std::vector<byte> key;
	std::vector<byte> iv;
	std::vector<byte> plaintext;
	std::vector<byte> ciphertext;
	
};


std::vector<byte> stobv(const std::string& str)
{
	std::vector<byte> word;
	
	for (std::size_t i = 0; i + 1 < str.size(); i += 2)
	{
		word.push_back(static_cast<byte>(std::stoi(str.substr(i,2), nullptr, 16)));
	}
	
	return word;
}


int main(int argc, char *argv[])
{
	AES::Variant variant;
	
	std::map<std::string, std::vector<test_vector_t>> test_vectors {{"ENCRYPT", {}}, {"DECRYPT", {}}};
	
	
	{
		std::ifstream file;
		if (argc > 1)
		{
			file.open(argv[1]);
		}
		std::istream& in = (argc > 1) ? static_cast<std::istream&>(file) : std::cin;
		
		
		
		std::regex header_key_length("# Key Length : (128|192|256)");
		std::regex testvector_mode("\\[(ENCRYPT|DECRYPT)\\]");
		std::regex testvector_keyvalue_pair("(COUNT|KEY|IV|PLAINTEXT|CIPHERTEXT) = ([0-9a-fA-F]+)");
		
		std::smatch regex_match;
		
		bool IN_HEADER = true;
		bool IN_TESTVECTOR = false;
		
		
		std::string mode = "";
		test_vector_t test_vector;
		
		std::string line;
		
		while (std::getline(in, line))
		{
			if (!line.empty() && line.back() == '\r')
			{
				line.pop_back();
			}
			
			if (!line.empty() && line.front() == '#' && IN_HEADER)
			{
				if (std::regex_match(line, regex_match, header_key_length))
				{
					int key_length = std::stoi(regex_match[1]);
					
					switch (key_length)
					{
						case 128:
							variant = AES::Variant::AES_128;
							break;
						case 192:
							variant = AES::Variant::AES_192;
							break;
						case 256:
							variant = AES::Variant::AES_256;
							break;
						default:
							std::cerr << "FAIL invalid key length (" << key_length << ")" << std::endl;
							return 2;
							break;
					}
					
					continue;
				}
			}
			else
			{
				IN_HEADER = false;
			}
			
			
			if (std::regex_match(line, regex_match, testvector_mode))
			{
				mode = regex_match[1];
				
				if (test_vectors.count(mode) > 0)
				{
					continue;
				}
				else
				{
					std::cerr << "FAIL invalid mode ([" << mode << "])" << std::endl;
					return 2;
				}
			}
			
			
			if (std::regex_match(line, regex_match, testvector_keyvalue_pair))
			{
				if (regex_match[1] == "COUNT")
				{
					IN_TESTVECTOR = true;
					
					test_vector = test_vector_t();
					test_vector.count = std::stoi(regex_match[2]);
				}
				else if (regex_match[1] == "KEY" && IN_TESTVECTOR)
				{
					test_vector.key = stobv(regex_match[2]);
				}
				else if (regex_match[1] == "IV" && IN_TESTVECTOR)
				{
					test_vector.iv = stobv(regex_match[2]);
				}
				else if (regex_match[1] == "PLAINTEXT" && IN_TESTVECTOR)
				{
					test_vector.plaintext = stobv(regex_match[2]);
				}
				else if (regex_match[1] == "CIPHERTEXT" && IN_TESTVECTOR)
				{
					test_vector.ciphertext = stobv(regex_match[2]);
				}
				
				continue;
			}
			
			if (line.empty())
			{
				if (IN_TESTVECTOR)
				{
					IN_TESTVECTOR = false;
					
					test_vectors[mode].push_back(test_vector);
					
					continue;
				}
			}
		}
		
		if (IN_TESTVECTOR)
		{
			IN_TESTVECTOR = false;
			
			test_vectors[mode].push_back(test_vector);
		}
	}
	
	
	

	
	AES AES(variant);
	
	bool success = true;
	
	
	// [ENCRYPT]
	{
		std::vector<byte> ciphertext;
		
		for (const test_vector_t &test_vector : test_vectors["ENCRYPT"])
		{
			AES.set_key(test_vector.key);
			ciphertext = AES.encrypt(test_vector.plaintext);
			
			if (ciphertext != test_vector.ciphertext)
			{
				std::cerr << "FAIL [ENCRYPT] on test vector COUNT = " << test_vector.count << std::endl;
				
				success = false;
			}
		}
	}
	
	// [DECRYPT]
	{
		std::vector<byte> plaintext;
		
		for (const test_vector_t &test_vector : test_vectors["DECRYPT"])
		{
			AES.set_key(test_vector.key);
			plaintext = AES.decrypt(test_vector.ciphertext);
			
			if (plaintext != test_vector.plaintext)
			{
				std::cerr << "FAIL [DECRYPT] on test vector COUNT = " << test_vector.count << std::endl;
				
				success = false;
			}
		}
	}
	
	
	return success ? 0 : 1;
}
