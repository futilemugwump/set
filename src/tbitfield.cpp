// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int _Bitlen)
{
	if (_Bitlen < 0) throw _Bitlen;
	BitLen = _Bitlen;
	MemLen = (BitLen / (8 * sizeof(TELEM))) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}
/* TBitField::TBitField()
{
	
	BitLen = 15;
	MemLen = (BitLen / (8 * sizeof(TELEM))) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
} */
TBitField::TBitField(const TBitField &a) // конструктор копирования
{
	BitLen = a.BitLen;
	MemLen = a.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = a.pMem[i];
	}
} 

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int v;
	v = n % 32;
	TELEM tmp = 1;
	tmp = tmp << v;
	return tmp;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n > BitLen) throw n;
	if (n < 0) throw 21;
	int i = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[i] = pMem[i] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n > BitLen) throw n;
	if (n < 0) throw 12;
	int i = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	mask = ~mask;
	pMem[i] &= mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n > BitLen) throw n;
	if (n < 0) throw 45;
	int i = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return pMem[i] & mask;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	/* if (BitLen != bf.BitLen)
	{
		BitLen = bf.BitLen;
		delete[]pMem;
		pMem = new TELEM[MemLen];
	}
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[]pMem;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen - 1; i++)
	{
		pMem[i] = bf.GetBit(i);
	}
		
	return *this; */
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		if (pMem != NULL) delete pMem;
		pMem = new TELEM[MemLen];
	}
	if (pMem != NULL)
	{
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;
	if (BitLen != bf.BitLen) res = 0;
	for (int i = 0; i < MemLen - 1; i++)
	{
		if (pMem[i] != bf.pMem[i])
			res =0;
	}
	for (int i = (MemLen - 1) * 32; i < BitLen; i++)
	{
		if (GetBit(i) != bf.GetBit(i))
			res = 0;
	}
	return res;
}

int TBitField::operator!=(const TBitField& bf) const
{
	if (BitLen != bf.BitLen) return 1;
	else
	{
		for (int i = 0; i < MemLen - 1; i++)
		{
			if (pMem[i] != bf.pMem[i])
				return 1;
		}
		for (int i = (MemLen - 1) * 32; i < BitLen; i++)
		{
			if (GetBit(i) != bf.GetBit(i)) return 1;
		}
		return 0;
	}
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxBitLen = bf.BitLen;
	if (BitLen > bf.BitLen)
	{
		maxBitLen = BitLen;
	}
	TBitField res(maxBitLen);
	if (BitLen < bf.BitLen)
	{
		for (int i = 0; i < MemLen - 1; i++)
		{
			res.pMem[i] = pMem[i];
		}
		for (int i = (MemLen - 1) * 32; i < BitLen; i++)
		{
			if (GetBit(i))
			{
				res.SetBit(i);

			}
		}
		for (int i = 0; i < res.MemLen; i++)
		{
			res.pMem[i] = res.pMem[i] | bf.pMem[i];
		}
	}
	else
	{
		for (int i = 0; i < bf.MemLen - 1; i++)
		{
			res.pMem[i] = bf.pMem[i];
		}
		for (int i = (bf.MemLen - 1) * 32; i < bf.BitLen; i++)
		{
			if (bf.GetBit(i))
			{
				res.SetBit(i);

			}
		}
		for (int i = 0; i < res.MemLen; i++)
		{
			res.pMem[i] = res.pMem[i] | pMem[i];
		}
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxBitLen = bf.BitLen;
	if (BitLen > bf.BitLen)
		maxBitLen = BitLen;
	TBitField temp(maxBitLen);
	if (BitLen < bf.BitLen)
	{
		for (int i = 0; i < MemLen - 1; i++)
			temp.pMem[i] = pMem[i];
		for (int i = (MemLen - 1) * 32; i < BitLen; i++)
		{
			if (GetBit(i))
				temp.SetBit(i);
		}
		for (int i = 0; i < temp.MemLen; i++)
			temp.pMem[i] = temp.pMem[i] & bf.pMem[i];
	}
	else 
	{
		for (int i = 0; i < bf.MemLen - 1; i++)
			temp.pMem[i] = bf.pMem[i];
		for (int i = (bf.MemLen - 1) * 32; i < bf.BitLen; i++)
		{
			if (bf.GetBit(i))
				temp.SetBit(i);
		}
		for (int i = 0; i < temp.MemLen; i++)
			temp.pMem[i] = temp.pMem[i] & pMem[i];
	}
	return temp;
}

TBitField TBitField::operator~(void) const // отрицание
{
	TBitField b(BitLen);
	for (int i = 0; i < MemLen; i++)
	{
		b.pMem[i] = ~pMem[i];
	}
	return b;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char c; int i = 0;
	do {

		istr >> c;
		if (c == '1') bf.SetBit(i);
		if (c == '0') bf.ClrBit(i);
		i++;
	} while ((c == '0') || (c == '1'));
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i) != 0)
			ostr << 1;
		else ostr << 0;
		
	}
	return ostr;
}
