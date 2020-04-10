#pragma once

#define  USE_FLOWER_CODE

// ================= SIMPLE_FLOWER_CODE
#ifdef	USE_FLOWER_CODE
#define  SIMPLE_FLOWER_CODE _asm\
	{\
		_asm push  eax\
		_asm xor   eax,eax\
        _asm test  eax,eax\
		_asm pop   eax\
		_asm jz	   $+22\
		_asm push  eax\
        _asm push  ebx\
		_asm lea   ebx,DWORD PTR[eax]\
		_asm inc   ebx\
		_asm add   ebx,eax\
		_asm pop   ebx\
		_asm pop   eax\
		_asm jnz   $+6\
		_asm _emit 0xe8\
	}
#else
#define  SIMPLE_FLOWER_CODE _asm nop
#endif USE_FLOWER_CODE

// ================= RETN

/* 
// ----------以下内容直接赋值可以使用：每个函数仅限使用一次--
#ifdef USE_FLOWER_CODE
	_asm{
			push ER_FLOWER_CODE1
			retn
			push eax
			push ebx
			nop
			xor   eax,eax
			test  eax,eax
			pop  ebx
			pop  eax
			jnz	 $+7
			sub  eax, 1
			inc  eax
			_emit 0xe8
ER_FLOWER_CODE1:
	}
#endif
*/


// #ifdef CHECK_IS_INT3
// 	DWORD dwAddressCheckCRC		= 0;
// 	DWORD dwAddressCheckBegin	= 0;
// 	DWORD dwAddressCheckEnd		= 0;
// 	_asm
// 	{
// 		mov dwAddressCheckCRC,		offset SEND_PACKET_CHECK_CRC
// 		mov dwAddressCheckBegin,	offset SEND_PACKET_CHECK_BEGIN
// 		mov dwAddressCheckEnd,		offset SEND_PACKET_CHECK_END
// 
// 		jmp   $+9
// SEND_PACKET_CHECK_CRC:
// 		_emit 0x00
// 		_emit 0x00
// 		_emit 0x00
// 		_emit 0x00
// 	}
// 	//*((DWORD*)(dwAddressCheckCRC))=dwAddressCheckBegin;
// SEND_PACKET_CHECK_BEGIN:
// #endif // CHECK_IS_INT3

// #ifdef CHECK_IS_INT3
// SEND_PACKET_CHECK_END:
// #endif // CHECK_IS_INT3
