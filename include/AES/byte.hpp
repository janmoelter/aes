#pragma once

#include <iostream>
#include <iomanip>

#include <cstdint>
#include <vector>
#include <array>

#include <bitset>

class byte
{

public:
	byte(void);
	byte(uint8_t);
	byte(std::bitset<8>);
	~byte(void);

private:
	void initialize(const uint8_t &);

	uint8_t _byte;

	uint8_t xtime(const uint8_t &);
	void xtime(void);

	// static const uint8_t _g = 0x03;
	static std::vector<uint8_t> _exp_table;
	static std::vector<uint8_t> _log_table;

public:
	bool operator==(const byte &);
	bool operator!=(const byte &);

	byte operator+(const byte &);
	byte & operator+=(const byte &);

	byte operator*(const byte &);
	byte & operator*=(const byte &);

	byte inverse(void);

	uint8_t to_uint8_t(void) const;
	std::bitset<8> to_bitset(void) const;

	friend std::ostream& operator<< (std::ostream &, byte &);
	
};
