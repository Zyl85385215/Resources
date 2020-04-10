#ifndef __MMX_FUNC_H__
#define __MMX_FUNC_H__


extern	int isMMX;

void MMX_Check(void);
inline void MMX_Memcpy(void * desc, void * src, int copySize)
{
	__asm
	{
		mov		esi, src
		mov		edi, desc
		mov		ecx, copySize
		mov		ebx, ecx
		shr		ecx, 4
		or		ecx, ecx
		jz		REMAINDER
		mov		eax, 16
ALIGN 4
MMX_LOOP:
		movq	mm0,[esi]
		movq	mm1,[esi+8]
		movq	[edi],mm0
		movq	[edi+8],mm1
		add		esi,eax
		add		edi,eax
		dec		ecx
		jnz		MMX_LOOP
		emms
ALIGN 4
REMAINDER:
		mov		ecx, ebx
		and		ecx, 0xF
		rep		movsb
	}
}


#endif
