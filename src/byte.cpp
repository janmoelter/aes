#include "byte.hpp"

std::vector<uint8_t> byte::_exp_table = std::vector<uint8_t>(0x00);
std::vector<uint8_t> byte::_log_table = std::vector<uint8_t>(0x00);


void byte::initialize(const uint8_t &value)
{
	this->_byte = value;

	if (this->_exp_table.size() == 0)
	{
		this->_exp_table = std::vector<uint8_t>(0xff + 1);
		this->_log_table = std::vector<uint8_t>(0xff + 1);

		this->_exp_table.at(0) = 0x01;
		this->_log_table.at(0) = 0x00;

		for (uint16_t j = 0x01; j <= 0xff; j++)
		{
			this->_exp_table.at(j) = xtime(this->_exp_table.at(j - 1)) ^ this->_exp_table.at(j - 1);
			this->_log_table.at(this->_exp_table.at(j)) = (uint8_t)(j % 0xff);
		}

		// for (uint16_t j = 0x00; j <= 0xff; j++)
		// {
			// std::cout << "exp( " << "x" << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)( j ) << " )" << " = " << "x" << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)(this->_exp_table[j]) << std::endl;

			// std::cout << "log( " << "x" << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)( j ) << " )" << " = " << "x" << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)(this->_log_table[j]) << std::endl;
		//}
	}
}

byte::byte(void)
{
	byte::initialize(0x00);
}

byte::byte(uint8_t value)
{
	byte::initialize(value);
}

byte::byte(std::bitset<8> value)
{
	byte::initialize((uint8_t)value.to_ulong());
}

byte::~byte(void)
{

}

uint8_t byte::xtime(const uint8_t &value)
{
	return (value >> 7 & 0x01) ? (value << 1 ^ 0x1b) : value << 1;
}

void byte::xtime(void)
{
	// this->_byte = (this->_byte >> 7 & 0x01) ? (this->_byte << 1 ^ 0x1b) : this->_byte = this->_byte << 1;
	this->_byte = xtime(this->_byte);
}

byte byte::operator+(const byte &X)
{
	return byte(this->_byte ^ X._byte);
}

byte & byte::operator+=(const byte &X)
{
	*this = *this + X;
	return *this;
}

byte byte::operator*(const byte &X)
{
	// byte _X = X;
	// byte _Y = byte();
	//
	// for (uint16_t n = 0x00; n < 0x08; n++)
	// {
	// 	if (this->_byte >> n & 0x01) { _Y = _Y + _X; }
	// 	_X.xtime();
	// }
	//
	// return _Y;

	return (!!this->_byte && !!X._byte) ? byte(_exp_table.at((_log_table.at(this->_byte)+_log_table.at(X._byte))%0xff)) : byte(0x00);
}

byte & byte::operator*=(const byte &X)
{
	*this = *this * X;
	return *this;
}

bool byte::operator==(const byte &X)
{
	return (this->_byte == X._byte);
}

bool byte::operator!=(const byte &X)
{
	return !(*this == X);
}

byte byte::inverse(void)
{
	// return this->_byte != 0 ? byte( _exp_table[ ( 0xff ^ _log_table[this->_byte] ) % 0xff ] ) : byte(0x00); 
	return this->_byte != 0 ? byte(_exp_table.at(0xff ^ _log_table.at(this->_byte))) : byte(0x00);
}

uint8_t byte::to_uint8_t(void) const
{
	return this->_byte;
}

std::bitset<8> byte::to_bitset(void) const
{
	//std::array<bool, 8> _bits = std::array<bool, 8>();
	//for (uint8_t j = 0; j < 8; j++)
	//{
	//	_bits.at(0) = this->_byte >> j & 0x01;
	//}
	//return _bits;

	return std::bitset<8>(this->_byte);
}

std::ostream& operator<< (std::ostream &out, byte &X)
{
	out << (int)(X._byte);
	return out;
}
