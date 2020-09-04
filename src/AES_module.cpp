#include "AES_module.hpp"

bool AES_module::verbose = false;

std::vector<uint8_t> AES_module::_sbox = std::vector<uint8_t>(0x00);
std::vector<uint8_t> AES_module::_invsbox = std::vector<uint8_t>(0x00);

void AES_module::initialize(void)
{

	if (this->_sbox.size() == 0)
	{
		this->_sbox = std::vector<uint8_t>(0xff + 1);
		this->_invsbox = std::vector<uint8_t>(0xff + 1);

		byte _X;
		std::bitset<8> _x;
		std::bitset<8> _b;

		const std::bitset<8> _c = std::bitset<8>(0x63);

		for (uint16_t j = 0x00; j <= 0xff; j++)
		{
			_X = byte(j);
			_X = _X.inverse();

			_x = _X.to_bitset();

			for (uint8_t i = 0x00; i < 0x08; i++)
			{
				_b[i] = _x[i] ^ _x[(i + 4) % 8] ^ _x[(i + 5) % 8] ^ _x[(i + 6) % 8] ^ _x[(i + 7) % 8] ^ _c[i];
			}

			this->_sbox[j] = (uint8_t)_b.to_ulong();
			this->_invsbox[(int)this->_sbox[j]] = (uint8_t)j;
		}

		// this->print_bytetable(this->_sbox);
		// this->print_bytetable(this->_invsbox);

	}

	{
		this->_rcon = std::vector<std::vector<byte>>((this->Nb*(this->Nr+1)-1)/this->Nk);

		this->_rcon[0] = { byte(0x01) , byte(0x00) , byte(0x00) , byte(0x00) };

		for (uint16_t i = 0x01; i < this->_rcon.size(); i++)
		{
			this->_rcon[i] = { this->_rcon[i-1][0] * byte(0x02) , byte(0x00) , byte(0x00) , byte(0x00) };
		}
		
	}
}

AES_module::AES_module(AES_standard standard)
{
	this->_standard = standard;

	switch (this->_standard)
	{
		case AES_standard::AES128:
			this->Nk = 4; this->Nb = 4; this->Nr = 10;
			break;
		case AES_standard::AES192:
			this->Nk = 6; this->Nb = 4; this->Nr = 12;
			break;
		case AES_standard::AES256:
			this->Nk = 8; this->Nb = 4; this->Nr = 14;
			break;
	}

	AES_module::initialize();
}

AES_module::~AES_module()
{

}

void AES_module::print_state(const std::vector<std::vector<byte>> &_state) const
{
	std::cout << "╓" << std::string(5 * this->Nb + 2, ' ') << "╖" << std::endl;
	
	for (uint8_t r = 0x00; r < 0x04; r++)
	{
		std::cout << "║";

		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			std::cout << std::right << std::setw(3) << std::setfill(' ') << "" << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)(_state[r][c].to_uint8_t());
		}
		std::cout << std::string(2, ' ') << "║" << std::endl;
	}
	std::cout << "╙" << std::string(5 * this->Nb + 2, ' ') << "╜" << std::endl;

	std::cout << std::endl;
}

void AES_module::print_state(void) const { AES_module::print_state(this->_state); }

void AES_module::print_word(const std::vector<byte> &word) const
{
	for (uint8_t i = 0x00; i < word.size(); i++)
	{
		std::cout << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)(word[i].to_uint8_t());
	}
}

void AES_module::print_keyschedule_string(const std::vector<std::vector<byte>> &_keyschedule,const uint8_t &round) const
{	
	for (uint8_t c = 0x00; c < this->Nb; c++)
	{
		for (uint8_t r = 0x00; r < 0x04; r++)
		{
			std::cout << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)(_keyschedule[round*this->Nb+c][r].to_uint8_t());
		}
	}
	std::cout << std::endl;
}

void AES_module::print_keyschedule_string(const uint8_t &round) const { AES_module::print_keyschedule_string(this->_keyschedule,round); }

void AES_module::print_bytetable(const std::vector<uint8_t> &bytemap) const
{
	if(bytemap.size() == 0xff + 1)
	{
		std::cout << std::string(4, ' ');
		for (uint8_t y = 0x00; y <= 0x0f; y++)
		{
			std::cout << std::string(2, ' ') << std::right << std::setw(1) << std::setfill('0') << std::hex << (int)(y);
		}
		std::cout << std::endl;

		std::cout << std::string(3, ' ') << "┌" << "────────────────────────────────────────────────" << std::endl;
		for (uint8_t x = 0x00; x <= 0x0f; x++)
		{
			std::cout << " " << std::right << std::setw(1) << std::setfill('0') << std::hex << (int)(x) << " │";

			for (uint8_t y = 0x00; y <= 0x0f; y++)
			{
				std::cout << " " << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)(bytemap[(x<<4)+y]);
			}
			std::cout << std::endl;
		}
	}
	else
	{
		throw;
	}
}

void AES_module::print_keyschedule(const std::vector<std::vector<byte>> &_keyschedule) const
{
	
	for (uint8_t i = 0x00; i < _keyschedule.size(); i++)
	{
		std::cout << "(" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)i << std::hex << ")";

		std::cout << std::right << std::setw(4) << std::setfill(' ') << "x";
		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			std::cout << std::right << std::setw(2) << std::setfill('0') << std::hex << (int)(_keyschedule[i][c].to_uint8_t());
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void AES_module::print_keyschedule(void) const { AES_module::print_keyschedule(this->_keyschedule); }

std::vector<std::vector<byte>> AES_module::to_state(const std::vector<byte> &word) const
{
	std::vector<std::vector<byte>> state = std::vector<std::vector<byte>>(4);
	for (uint8_t r = 0x00; r < 0x04; r++)
	{
		state[r] = std::vector<byte>(this->Nb);

		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			state[r][c] = word[r + 4 * c];
		}
	}

	return state;
}

std::vector<byte> AES_module::to_word(const std::vector<std::vector<byte>> &state) const
{
	std::vector<byte> word = std::vector<byte>(4 * this->Nb);
	for (uint8_t r = 0x00; r < 0x04; r++)
	{
		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			word[r + 4 * c] = state[r][c];
		}
	}

	return word;
}

void AES_module::Cipher(void)
{
	
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(0) << "].input" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }

	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(0) << "].k_sch" << "\t\t"; this->print_keyschedule_string(0); }
	this->AddRoundKey(0);

	for (uint8_t round = 0x01; round < this->Nr; round++)
	{

		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(round) << "].start" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->SubBytes();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(round) << "].s_box" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->ShiftRows();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(round) << "].s_row" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->MixColumns();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(round) << "].m_col" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(round) << "].k_sch" << "\t\t"; this->print_keyschedule_string(round); }
		this->AddRoundKey(round);
	}

	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].start" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	this->SubBytes();
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].s_box" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	this->ShiftRows();
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].s_row" << "\t\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].k_sch" << "\t\t"; this->print_keyschedule_string(this->Nr); }
	this->AddRoundKey(this->Nr);



	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].output" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
}

void AES_module::InvCipher(void)
{

	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(0) << "].iinput" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }

	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(0) << "].ik_sch" << "\t"; this->print_keyschedule_string(this->Nr); }
	this->AddRoundKey(this->Nr);

	for (uint8_t round = this->Nr - 1; round > 0; round--)
	{
		
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].istart" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->InvShiftRows();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].is_row" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->InvSubBytes();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].is_box" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].ik_sch" << "\t"; this->print_keyschedule_string(round); }
		this->AddRoundKey(round);
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].ik_add" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->InvMixColumns();
	}

	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].istart" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	this->InvShiftRows();
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].is_row" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	this->InvSubBytes();
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].is_box" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].ik_sch" << "\t"; this->print_keyschedule_string(0); }
	this->AddRoundKey(0);



	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].ioutput" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
}

void AES_module::EqInvCipher(void)
{
	
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(0) << "].iinput" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(0) << "].ik_sch" << "\t"; this->print_keyschedule_string(this->_altkeyschedule,this->Nr); }
	this->AddRoundKey(this->_altkeyschedule,this->_state,this->Nr);
	
	for (uint8_t round = this->Nr - 1; round > 0; round--)
	{
		
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].istart" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->InvSubBytes();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].is_box" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->InvShiftRows();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].is_row" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		this->InvMixColumns();
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].im_col" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
		if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr - round) << "].ik_sch" << "\t"; this->print_keyschedule_string(this->_altkeyschedule,round); }
		this->AddRoundKey(this->_altkeyschedule,this->_state,round);
	}
	
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].istart" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	this->InvSubBytes();
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].is_box" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	this->InvShiftRows();
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].is_row" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }
	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].ik_sch" << "\t"; this->print_keyschedule_string(this->_altkeyschedule,0); }
	this->AddRoundKey(this->_altkeyschedule,this->_state,0);



	if( this->verbose ) { std::cout << "round[" << std::right << std::setw(2) << std::setfill(' ') << std::dec << (int)(this->Nr) << "].ioutput" << "\t"; this->print_word(this->to_word(this->_state)); std::cout << std::endl; }

	//std::cout << "> EqInvCipher() is not supported." << std::endl;
}

void AES_module::SubBytes(std::vector<std::vector<byte>> &_state)
{
	for (uint8_t r = 0x00; r < 0x04; r++)
	{
		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			_state[r][c] = byte(_sbox.at(_state[r][c].to_uint8_t()));
		}
	}
}

void AES_module::SubBytes(void) { AES_module::SubBytes(this->_state); }

void AES_module::InvSubBytes(std::vector<std::vector<byte>> &_state)
{
	for (uint8_t r = 0x00; r < 0x04; r++)
	{
		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			_state[r][c] = byte(_invsbox.at(_state[r][c].to_uint8_t()));
		}
	}
}

void AES_module::InvSubBytes(void) { AES_module::InvSubBytes(this->_state); }

void AES_module::ShiftRows(std::vector<std::vector<byte>> &_state)
{
	for (uint8_t r = 0x00; r < 0x04; r++)
	{
		std::rotate(_state[r].begin(),_state[r].begin()+r,_state[r].end());
	}
}

void AES_module::ShiftRows(void) { AES_module::ShiftRows(this->_state); }

void AES_module::InvShiftRows(std::vector<std::vector<byte>> &_state)
{
	for (uint8_t r = 0x00; r < 0x04; r++)
	{
		std::rotate(_state[r].rbegin(),_state[r].rbegin()+r,_state[r].rend());
	}
}

void AES_module::InvShiftRows(void) { AES_module::InvShiftRows(this->_state); }

void AES_module::MixColumns(std::vector<std::vector<byte>> &_state)
{
	std::vector<std::vector<byte>> state = _state;

	std::vector<byte> a = { byte(0x02) , byte(0x03) , byte(0x01) , byte(0x01) };

	for (uint8_t c = 0x00; c < this->Nb; c++)
	{
		for (uint8_t r = 0x00; r < 0x04; r++)
		{
			_state[r][c] = ( a[0] * state[0][c] ) + ( a[1] * state[1][c] ) + ( a[2] * state[2][c] ) + ( a[3] * state[3][c] );
			
			std::rotate(a.rbegin(),a.rbegin()+1,a.rend());
		}
	}
}

void AES_module::MixColumns(void) { AES_module::MixColumns(this->_state); }

void AES_module::InvMixColumns(std::vector<std::vector<byte>> &_state)
{
	std::vector<std::vector<byte>> state = _state;

	std::vector<byte> a = { byte(0x0e) , byte(0x0b) , byte(0x0d) , byte(0x09) };

	for (uint8_t c = 0x00; c < this->Nb; c++)
	{
		for (uint8_t r = 0x00; r < 0x04; r++)
		{
			_state[r][c] = ( a[0] * state[0][c] ) + ( a[1] * state[1][c] ) + ( a[2] * state[2][c] ) + ( a[3] * state[3][c] );
			
			std::rotate(a.rbegin(),a.rbegin()+1,a.rend());
		}
	}
}

void AES_module::InvMixColumns(void) { AES_module::InvMixColumns(this->_state); }

void AES_module::KeyExpansion(void)
{
	this->_keyschedule = std::vector<std::vector<byte>>(this->Nb*(this->Nr+1));

	if(this->_key.size() == 4 * this->Nk)
	{
		for (uint8_t i = 0x00; i < this->Nk; i++)
		{
			this->_keyschedule[i] = { this->_key[4*i] , this->_key[4*i+1] , this->_key[4*i+2] , this->_key[4*i+3] };
		}
		
		std::vector<byte> temp;

		for (uint8_t i = this->Nk; i < this->Nb*(this->Nr+1); i++)
		{
			temp = this->_keyschedule[i-1];

			if (i % this->Nk == 0x00)
			{
				temp = this->XorWord(this->SubWord(this->RotWord(temp)),this->_rcon[(i/this->Nk)-1]);
			}
			else if (this->Nk > 0x06 && i % this->Nk == 0x04)
			{
				temp = this->SubWord(temp);
			}

			this->_keyschedule[i] = this->XorWord(this->_keyschedule[i-this->Nk],temp);

		}

		this->KeyExpansionAddendum();


	}
	else
	{
		throw;
	}
}

void AES_module::KeyExpansionAddendum(void)
{
	this->_altkeyschedule = this->_keyschedule;

	std::vector<std::vector<byte>> keyschedule_state;
	std::vector<byte> keyschedule_word;
	std::vector<byte> word;

	for (uint8_t round = 0x01; round < this->Nr; round++)
	{
		// std::vector<std::vector<byte>>(this->_altkeyschedule.begin()+(round*this->Nb),this->_altkeyschedule.begin()+((round+1)*this->Nb));

		keyschedule_word = {};


		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			word = this->_altkeyschedule[round*this->Nb+c];
			keyschedule_word.insert(keyschedule_word.end(),word.begin(),word.end());
		}

		keyschedule_state = this->to_state(keyschedule_word);

		this->InvMixColumns(keyschedule_state);

		keyschedule_word = this->to_word(keyschedule_state);

		for (uint8_t c = 0x00; c < this->Nb; c++)
		{
			std::copy(keyschedule_word.begin()+(c*4),keyschedule_word.begin()+((c+1)*4),this->_altkeyschedule[round*this->Nb+c].begin());
		}
	}

	//for (uint8_t i = 0x00; i < this->_altkeyschedule.size(); i++)
	//{
	//	this->print_word(this->_altkeyschedule[i]); std::cout << std::endl;
	//}
}

void AES_module::AddRoundKey(const std::vector<std::vector<byte>> &_keyschedule, std::vector<std::vector<byte>> &_state,uint8_t round)
{
	for (uint8_t c = 0x00; c < this->Nb; c++)
	{
		for (uint8_t r = 0x00; r < 0x04; r++)
		{
			_state[r][c] = _state[r][c] + _keyschedule[round*this->Nb+c][r];
		}
	}
}

void AES_module::AddRoundKey(uint8_t round) { AES_module::AddRoundKey(this->_keyschedule,this->_state,round); }

std::vector<byte> AES_module::SubWord(const std::vector<byte> &word) const
{
	std::vector<byte> sub_word = word;

	for (uint8_t i = 0x00; i < word.size(); i++)
	{
		sub_word[i] = byte(_sbox.at(word[i].to_uint8_t()));
	}

	return sub_word;
}

std::vector<byte> AES_module::XorWord(const std::vector<byte> &word1,const std::vector<byte> &word2) const
{
	std::vector<byte> xor_word = word1;

	if (word1.size() == word2.size())
		for (uint8_t i = 0x00; i < xor_word.size(); i++)
		{
			xor_word[i] = xor_word[i] + word2[i];
		}
	else
	{
		throw;
	}

	return xor_word;
}

std::vector<byte> AES_module::RotWord(const std::vector<byte> &word) const
{
	std::vector<byte> rot_word = word;

	std::rotate(rot_word.begin(),rot_word.begin()+1,rot_word.end());

	return rot_word;
}

void AES_module::test_standard(void)
{

	std::vector<byte> keyword;

	std::cout << std::endl << std::string( 80 , '*' ) << std::endl;
	std::cout << " Example Vectors for ";

	switch (this->_standard)
	{
		case AES_standard::AES128:
			std::cout << "AES-128 (NIST FIPS 197, Appendix C.1)" << std::endl;
		
			//keyword = { byte(0x2b) , byte(0x7e) , byte(0x15) , byte(0x16) , byte(0x28) , byte(0xae) , byte(0xd2) , byte(0xa6) , byte(0xab) , byte(0xf7) , byte(0x15) , byte(0x88) , byte(0x09) , byte(0xcf) , byte(0x4f) , byte(0x3c) };
			keyword = { byte(0x00) , byte(0x01) , byte(0x02) , byte(0x03) , byte(0x04) , byte(0x05) , byte(0x06) , byte(0x07) , byte(0x08) , byte(0x09) , byte(0x0a) , byte(0x0b) , byte(0x0c) , byte(0x0d) , byte(0x0e) , byte(0x0f) };

			break;
		case AES_standard::AES192:
			std::cout << "AES-192 (NIST FIPS 197, Appendix C.2)" << std::endl;
		
			//keyword = { byte(0x8e) , byte(0x73) , byte(0xb0) , byte(0xf7) , byte(0xda) , byte(0x0e) , byte(0x64) , byte(0x52) , byte(0xc8) , byte(0x10) , byte(0xf3) , byte(0x2b) , byte(0x80) , byte(0x90) , byte(0x79) , byte(0xe5) , byte(0x62) , byte(0xf8) , byte(0xea) , byte(0xd2) , byte(0x52) , byte(0x2c) , byte(0x6b) , byte(0x7b) };
			keyword = { byte(0x00) , byte(0x01) , byte(0x02) , byte(0x03) , byte(0x04) , byte(0x05) , byte(0x06) , byte(0x07) , byte(0x08) , byte(0x09) , byte(0x0a) , byte(0x0b) , byte(0x0c) , byte(0x0d) , byte(0x0e) , byte(0x0f) , byte(0x10) , byte(0x11) , byte(0x12) , byte(0x13) , byte(0x14) , byte(0x15) , byte(0x16) , byte(0x17) };

			break;
		case AES_standard::AES256:
			std::cout << "AES-256 (NIST FIPS 197, Appendix C.3)" << std::endl;
		
			//keyword = { byte(0x60) , byte(0x3d) , byte(0xeb) , byte(0x10) , byte(0x15) , byte(0xca) , byte(0x71) , byte(0xbe) , byte(0x2b) , byte(0x73) , byte(0xae) , byte(0xf0) , byte(0x85) , byte(0x7d) , byte(0x77) , byte(0x81) , byte(0x1f) , byte(0x35) , byte(0x2c) , byte(0x07) , byte(0x3b) , byte(0x61) , byte(0x08) , byte(0xd7) , byte(0x2d) , byte(0x98) , byte(0x10) , byte(0xa3) , byte(0x09) , byte(0x14) , byte(0xdf) , byte(0xf4) };
			keyword = { byte(0x00) , byte(0x01) , byte(0x02) , byte(0x03) , byte(0x04) , byte(0x05) , byte(0x06) , byte(0x07) , byte(0x08) , byte(0x09) , byte(0x0a) , byte(0x0b) , byte(0x0c) , byte(0x0d) , byte(0x0e) , byte(0x0f) , byte(0x10) , byte(0x11) , byte(0x12) , byte(0x13) , byte(0x14) , byte(0x15) , byte(0x16) , byte(0x17) , byte(0x18) , byte(0x19) , byte(0x1a) , byte(0x1b) , byte(0x1c) , byte(0x1d) , byte(0x1e) , byte(0x1f) };

			break;
	}
	std::cout << std::endl;

	this->_key = keyword;
	this->KeyExpansion();


	//std::vector<byte> word = { byte(0x32) , byte(0x43) , byte(0xf6) , byte(0xa8) , byte(0x88) , byte(0x5a) , byte(0x30) , byte(0x8d) , byte(0x31) , byte(0x31) , byte(0x98) , byte(0xa2) , byte(0xe0) , byte(0x37) , byte(0x07) , byte(0x34) };
	std::vector<byte> word = { byte(0x00) , byte(0x11) , byte(0x22) , byte(0x33) , byte(0x44) , byte(0x55) , byte(0x66) , byte(0x77) , byte(0x88) , byte(0x99) , byte(0xaa) , byte(0xbb) , byte(0xcc) , byte(0xdd) , byte(0xee) , byte(0xff) };

	this->_state = this->to_state(word);


	std::cout << "PLAINTEXT:" << "\t\t"; this->print_word(word); std::cout << std::endl;
	std::cout << "KEY:" << "\t\t\t"; this->print_word(keyword); std::cout << std::endl;
	std::cout << std::endl;


	bool _verbose = this->verbose;
	this->verbose = true;


	std::cout << "CIPHER (ENCRYPT):" << std::endl;
	this->Cipher();
	std::cout << std::endl;

	std::vector<std::vector<byte>> _enc_state = this->_state;

	std::cout << "INVERSE CIPHER (DECRYPT):" << std::endl;
	this->_state = _enc_state;
	this->InvCipher();
	std::cout << std::endl;

	std::cout << "EQUIVALENT INVERSE CIPHER (DECRYPT):" << std::endl;
	this->_state = _enc_state;
	this->EqInvCipher();
	std::cout << std::endl;

	this->verbose = _verbose;
}

void AES_module::set_key(const std::vector<byte> &word)
{
	this->_key = {};

	if (word.size() == 4 * this->Nk)
	{
		this->_key = word;
		this->KeyExpansion();
	}
	else
	{
		throw std::invalid_argument( "Key size does not match AES standard." );
	}
}

std::vector<byte> AES_module::encrypt(const std::vector<byte> &word)
{
	if (word.size() == 4 * this->Nb)
	{
		this->_state = this->to_state(word);
		this->Cipher();

		return this->to_word(this->_state);
	}
	else
	{
		throw std::invalid_argument( "Block length does not match AES standard." );
		return {};
	}
}

std::vector<byte> AES_module::decrypt(const std::vector<byte> &word)
{
	if (word.size() == 4 * this->Nb)
	{
		this->_state = this->to_state(word);
		this->InvCipher();

		return this->to_word(this->_state);
	}
	else
	{
		throw std::invalid_argument( "Block length does not match AES standard." );
		return {};
	}
}
