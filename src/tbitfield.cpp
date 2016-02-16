// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{ if (len < 0 )
		throw invalid_argument("negative bit field");

	MemLen = (len-1)/(8*sizeof(TELEM)) + 1 ;
	BitLen=len;
	pMem = new TELEM [MemLen]; 
	for (int i=0; i<MemLen; i++) 
	{
		pMem[i]=0; 
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{BitLen = bf.BitLen; 
	MemLen = bf.MemLen; 
	pMem= new TELEM[MemLen]; 

	for (int i=0; i<MemLen; i++) 
	{
		pMem[i]=bf.pMem[i]; 
	}

}

TBitField::~TBitField()
{delete pMem; 
pMem = NULL; 
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{return n/(sizeof(TELEM)*8); 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{ return 1 << n%(sizeof(TELEM)*8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen; 
}

void TBitField::SetBit(const int n) // установить бит
{if (n < 0)
{ throw invalid_argument("Bit<0");    } 

if (n > BitLen)
{ throw invalid_argument("n > BitLen");    }

pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
 if (n < 0) 
{  throw invalid_argument("n < 0"); }
if (n > BitLen)
{  throw invalid_argument("n > BitLen");}
pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);

}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0)
	{ throw invalid_argument("n<0");}

	if (n > BitLen)
	{throw invalid_argument("n > BitLen"); }

	return (pMem[GetMemIndex(n)] & GetMemMask(n)) !=0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{ 
	BitLen=bf.BitLen; 
	if (MemLen != bf.MemLen) 
		MemLen=bf.MemLen; 
	if (pMem != NULL) 
		delete pMem; 
	pMem = new TELEM[MemLen]; 
	if (pMem!=NULL) 
		for (int i=0; i<MemLen; i++) 
			pMem[i]=bf.pMem[i]; 
	return *this; 
}

int TBitField::operator==(const TBitField &bf) const // сравнение 
{ 
	int res = 1; 
	if (BitLen == bf.BitLen) 
	{
		for (int i=0; i<BitLen; i++)
		{
			if (GetBit(i) != bf.GetBit(i))
			{
				return res=0;
			} 
		} 
	}
	else
	{ 
		return res=0;
	}

}
int TBitField::operator!=(const TBitField &bf) const // сравнение 
{ 
	if (*this == bf)
	{
		return 0;
	}
	else 
	{ return 1;}
} 

TBitField TBitField::operator|(const TBitField &bf) // операция "или" 
{ 
	int i, len = BitLen; 
	if (bf.BitLen > len ) 
		len = bf.BitLen; 
	TBitField temp (len) ; 
	for (i=0; i<MemLen; i++) 
		temp.pMem[i] = pMem [i]; 
	for (i=0; i<bf.MemLen; i++ ) 
		temp.pMem[i] |= bf.pMem[i]; 
	return temp; 


} 

TBitField TBitField::operator&(const TBitField &bf) // операция "и" 
{ int len=BitLen; 
if (bf.BitLen>len) 
	len = bf.BitLen; 
TBitField temp(len); 
for (int i=0; i<MemLen; i++) 
	temp.pMem[i] = pMem[i]; 
for (int i=0; i<bf.MemLen; i++) 
	temp.pMem[i] &= bf.pMem[i]; 
return temp; 
} 

TBitField TBitField::operator~(void) // отрицание 
{ 
	
	TBitField temp(BitLen); 
	for (int i=0; i < MemLen; i++) 
	{
		temp.pMem[i] = ~pMem[i]; 
		temp.pMem[MemLen-1] = 0;
		int bitInd = (MemLen - 1) * sizeof(TELEM);
		for (int i=0; i<BitLen - bitInd; i++)
			(GetBit(bitInd + i) == 0 ? temp.SetBit(bitInd + i) : temp.ClrBit(bitInd + i));


	}
	return temp; 
} 

// ввод/вывод 

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0; char c;
	istr >> c ;
	
		while( (c == '0') || (c == '1'))
		{if (c == '1')
		{
			bf.SetBit(i);
		}
		if (c == '0')
		{   bf.ClrBit(i);
		}

		i++;
		
		istr >> c;
	}


	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод 
{  ostr << "Length= " << bf.BitLen <<endl;
for (int i=0; i < bf.BitLen; i++)
{
	ostr << bf.GetBit(i);
}
return ostr;
}