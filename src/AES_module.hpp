#pragma once

#include <iostream>
#include <iomanip>

#include <cstdint>

#include <string>
#include <vector>
#include <array>

#include <algorithm>

#include <bitset>

#include "byte.hpp"

#include <stdexcept>

enum class AES_standard { AES128, AES192, AES256 };

class AES_module
{

public:
	AES_module(AES_standard);
	~AES_module();

private:
	void initialize(void);

	AES_standard _standard;

	uint8_t Nk;
	uint8_t Nb;
	uint8_t Nr;

	static std::vector<uint8_t> _sbox;
	static std::vector<uint8_t> _invsbox;

	static bool verbose;

	std::vector<std::vector<byte>> _state;

	std::vector<std::vector<byte>> _rcon;

	std::vector<byte> _key;
	std::vector<std::vector<byte>> _keyschedule;
	std::vector<std::vector<byte>> _altkeyschedule;

	std::vector<std::vector<byte>> to_state(const std::vector<byte>&) const;
	std::vector<byte> to_word(const std::vector<std::vector<byte>>&) const;


	void SubBytes(std::vector<std::vector<byte>>&);
	void ShiftRows(std::vector<std::vector<byte>>&);
	void MixColumns(std::vector<std::vector<byte>>&);

	void SubBytes(void);
	void ShiftRows(void);
	void MixColumns(void);

	void AddRoundKey(const std::vector<std::vector<byte>>&,std::vector<std::vector<byte>>&,uint8_t);

	void AddRoundKey(uint8_t);

	void KeyExpansion(void);
	void KeyExpansionAddendum(void);

	void InvSubBytes(std::vector<std::vector<byte>>&);
	void InvShiftRows(std::vector<std::vector<byte>>&);
	void InvMixColumns(std::vector<std::vector<byte>>&);

	void InvSubBytes(void);
	void InvShiftRows(void);
	void InvMixColumns(void);

	std::vector<byte> SubWord(const std::vector<byte>&) const;
	std::vector<byte> RotWord(const std::vector<byte>&) const;

	std::vector<byte> XorWord(const std::vector<byte>&,const std::vector<byte>&) const;

	void Cipher(void);
	void InvCipher(void);
	void EqInvCipher(void);
	
	void print_state(const std::vector<std::vector<byte>>&) const;
	void print_word(const std::vector<byte>&) const;

	void print_state(void) const;

	void print_keyschedule_string(const std::vector<std::vector<byte>>&,const uint8_t&) const;
	void print_keyschedule(const std::vector<std::vector<byte>>&) const;

	void print_keyschedule_string(const uint8_t&) const;
	void print_keyschedule(void) const;

	void print_bytetable(const std::vector<uint8_t>&) const;

public:
	std::vector<byte> encrypt(const std::vector<byte>&);
	std::vector<byte> decrypt(const std::vector<byte>&);

	void set_key(const std::vector<byte>&);

	void test_standard(void);

};
