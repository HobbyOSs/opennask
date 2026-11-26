; BMP decode routine by I.Tak. 2003

[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "bmp.nasm"]

		GLOBAL	_info_BMP
		GLOBAL	_decode0_BMP
;BMP File Structure (I can't understand MS.)

;	struc BMP
;		;FILE HEADER
;.fType:		resw 1	;BM
;.fSize:		resd 1	;whole file size
;		resd 1	;reserved
;.fOffBits:	resd 1	;offset from file top to image
;		;INFO HEADER
;.iSize:		resd 1	;INFO HEADER size
;.iWidth:	resd 1	;Image Width in pixels
;.iHeight:	resd 1	;Image Height in pixels
;.iPlanes:	resw 1	;must be 1
;.iBitCount:	resw 1	;BitPerPixel 1, 4, 8, 24 (and 15,16 for new OS/2 ?)
;.iCompression:	resd 1	;Compress Type. 0 for none, then SizeImage=0
;.iSizeImage:	resd 1	;Image Size(compressed)
;.iXPPM:		resd 1	;X Pixel Per Meter
;.iYPPM:		resd 1
;.iClrUsed:	resd 1	;Number of used ColorQuad (0 for whole Quad)
;.iClrImportant:	resd 1	;Number of Important ColorQuad.
;	endstruc
BMP_fType	EQU	0
BMP_fSize	EQU	2
BMP_fOffBits	EQU	10
BMP_iSize	EQU	14
BMP_iWidth	EQU	18
BMP_iHeight	EQU	22
BMP_iPlanes	EQU	26
BMP_iBitCount	EQU	28
BMP_iCompression	EQU	30
BMP_iSizeImage	EQU	34
BMP_iXPPM	EQU	38
BMP_iYPPM	EQU	42
BMP_iClrUsed	EQU	46
BMP_iClrImportant	EQU	50
BMP_size	EQU	54


;	struc BMPOS2
;		;FILE HEADER
;.fType:		resw 1	;BM
;.fSize:		resd 1	;whole file size
;		resd 1	;reserved
;.fOffBits:	resd 1	;offset from file top to image
;		;CORE HEADER
;.iSize:		resd 1	;CORE HEADER size
;.iWidth:	resw 1	;Image Width in pixels
;.iHeight:	resw 1	;Image Height in pixels
;.iPlanes:	resw 1	;must be 1
;.iBitCount:	resw 1	;BitPerPixel 1, 4, 8, 24 (and 15,16 for new OS/2 ?)
;	endstruc
BMPOS2_fType	EQU	0
BMPOS2_fSize	EQU	2
BMPOS2_fOffBits	EQU	10
BMPOS2_iSize	EQU	14
BMPOS2_iWidth	EQU	18
BMPOS2_iHeight	EQU	20
BMPOS2_iPlanes	EQU	22
BMPOS2_iBitCount	EQU	24
BMPOS2_size	EQU	26


; B/W bmp can also have palettes. The first for 0, second for 1.

;	struc CQuad
;.b:	resb 1
;.g:	resb 1
;.r:	resb 1
;	resb 1	;reserved
;	endstruc
CQuad_b	EQU	0
CQuad_g	EQU	1
CQuad_r	EQU	2
CQuad_size	EQU	4



;%if 0
;int info_BMP(struct DLL_STRPICENV *env, int *info, int size, UCHAR *fp);
;/* 成功したら1 (非0?), 失敗したら0 */
;int decode0_BMP(struct DLL_STRPICENV *env, int size, UCHAR *fp,
;                int b_type, UCHAR *buf, int skip);
;/* エラーコードを返す。とりあえず非0にしてるだけ */
;envは64KBのワークエリアである。先頭8dwは返り値用になっている。
;揮発性らしく, インスタンス変数的には使えないようだ。JPEG_initで
;base_imgを作るのはdecodeのときだけでいいと思う。
;%endif


;[absolute 0]	;naskなら[section .bss] org 0 かな win32だからダメか
;bmpinfo:
;.regs:		resd 4
;.reteip:	resd 1
;.env:		resd 1
;.info:		resd 1
;.size:		resd 1
;.module:	resd 1
bmpinfo_regs	EQU	0
bmpinfo_reteip	EQU	16
bmpinfo_env	EQU	20
bmpinfo_info	EQU	24
bmpinfo_size	EQU	28
bmpinfo_module	EQU	32

;[absolute 0]
;info:
;.type:		resd 1	;1 for BMP, 2 for JPEG
;		resd 1	;0
;.width:		resd 1
;.height:	resd 1
info_type	EQU	0
info_width	EQU	8
info_height	EQU	12

[SECTION .text]
_info_BMP:
	push ebx
	push ebp
	push esi
	push edi
	mov esi, [esp+bmpinfo_module]
	mov eax, [esp+bmpinfo_size]
	call bmpHeader
	test edi, edi
	jz .ret
	mov esi, [esp+bmpinfo_info]
	mov [esi+info_width], eax
	mov [esi+info_height], ecx
	mov [esi+info_type], edi	;=1
	dec edi
	mov [esi+info_type+4], edi	;=0
	inc edi
.ret:
	mov eax, edi
	pop edi
	pop esi
	pop ebp
	pop ebx
	ret

;in: esi=module, eax=size
;ret:eax=width, ecx=hegiht, edx=paletteSize, ebx=palette
;    ebp=bpp, esi=endOfImage, edi=successFlag
bmpHeader:
	lea edx, [esi+eax]		;moduleEnd
	xor edi, edi
	push edx
	xor edx, edx

	cmp eax, BMP_iSize+4
	jbe .midret			;中継してしまう
	cmp word [esi],0x4d42
	je .notMAC
	sub esi, -128
	add eax, -128
	pop ebx
	push eax
	cmp eax, BMP_iSize+4
.midret:
	jbe .ret
	cmp word [esi], 0x4d42
	jne .ret
.notMAC:
	;;MS,OS/2 フォーマット確認
	mov ecx, [esi +BMP_iSize]
	cmp ecx, 12			;OS/2 format.
	jne .MS
	  cmp eax, BMPOS2_size
	  jbe .ret				;coreヘッダなし
	  lea ebx, [esi+ecx+14]			;palette
	  movzx eax, [esi+BMPOS2_iWidth]	;width
	  movzx ecx, [esi+BMPOS2_iHeight]	;height
	  movzx ebp, [esi+BMPOS2_iBitCount]	;bpp
	  mov dl, 3				;paletteSize
	jmp .endif
.MS:
	cmp eax, BMP_size
	  jbe .ret				;infoヘッダなし
	  lea ebx, [esi+ecx+14]
	  sub ecx, 40
	  jne .ret				;unknownFormat
	  cmp ecx, [esi+BMP_iCompression]
	  jne .ret				;Compressed.
	  mov   eax, [esi+BMP_iWidth]		;width
	  mov   ecx, [esi+BMP_iHeight]		;height
	  movzx ebp, [esi +BMP_iBitCount]	;bpp
	  mov dl, 4				;paletteSize
.endif:
	add esi, [esi +BMP_fOffBits]

	;sizeが小さい場合heightを削ってで 
	;読めると答えるべきだろう。今はエラー
	push edx
	push eax
	mul ebp				;eax=width*bpp
	add eax, 7
	shr eax, 3			;lineSizeWithoutPudding
	mov edx, eax
	add eax, 3			;size<1GBを仮定してedxを無 
	and al, -4			;lineSizeWithPudding
	sub edx, eax			;-puddingSize
	push edx
	mul ecx
	pop edx
	add esi, eax
	add esi, edx			;最終行の最後にはpuddingがないと見るべき
	cmp esi, [esp+8]		;endOfModule
	pop eax
	ja .ret2
	sub esi, edx			;esi=endOfImage
	inc edi				;succeeded!
.ret2:
	pop edx
.ret:
	add esp, 4
	ret

;***************************************************************
; 小ささ優先で作っているが, 速さ優先で作った昔のよりいいかも。
; 品質最低の高速モードのみ作ってい 

;[absolute 0]
;decode:
;.regs:		resd 4
;.reteip:	resd 1
;.env:		resd 1
;.size:		resd 1
;.module:	resd 1
;.outputType:	resd 1
;.dest:		resd 1
;.skip:		resd 1
decode_regs	EQU	0
decode_reteip	EQU	16
decode_env	EQU	20
decode_size	EQU	24
decode_module	EQU	28
decode_outputType	EQU	32
decode_dest	EQU	36
decode_skip	EQU	40

_decode0_BMP:
	push ebx
	push ebp
	push esi
	push edi
	mov esi, [esp+decode_module]
	mov eax, [esp+decode_size]
	call bmpHeader
	;ret:eax=width, ecx=hegiht, edx=paletteSize, ebx=palette
	;    ebp=bpp, esi=endOfImage, edi=successFlag
	test edi,edi
	jz .error
	mov edi, [esp+decode_dest]
	push dword [esp+decode_outputType]
	push dword [esp+4+decode_skip]
	push ecx			;height
	push eax
	push edx
	mul ebp
	add eax, 31
	shr eax, 3
	and al, -4
	push eax
	mov edx, ebp
	mov ebp, esp
	call bmp2beta			;ecx!=0 for error
	add esp, bb_size
	mov eax, ecx
	test ecx, ecx
	jz .ret
.error:
	push 1
	pop eax
.ret:
	pop edi
	pop esi
	pop ebp
	pop ebx
	ret

;[absolute -4*2]
;bb:
;.col0:		resd 1	;bpp1で使う
;.reteip:	resd 1
;.sw:		resd 1	;byte
;.paletteSize:	resd 1	;byte
;.w:		resd 1	;pixel
;.h:		resd 1
;.s:		resd 1
;.t:		resd 1
;.size: equ $-$$
bb_col0	EQU	-8
bb_reteip	EQU	-4
bb_sw	EQU	0
bb_paletteSize	EQU	4
bb_w	EQU	8
bb_h	EQU	12
bb_s	EQU	16
bb_t	EQU	20
bb_size	EQU	24

;eax=?, ecx=height, edx=bpp, ebx=palette
;ebp=bb, esi=endOfImage, edi=dest
bmp2beta:
	mov al, [ebp+bb_t]
	and al, 0x7f
	cmp al, 2
	je buf16
	cmp al, 4
	je buf32
	mov ecx, esp		;!=0
	ret
;===============================================================
;	Buffer mode 4
;===============================================================
buf32:
	dec edx
	je .bpp1
	sub edx, 4-1
	je .bpp4
	sub edx, 8-4
	je .bpp8
	sub edx, 24-8
	je .bpp24
	mov ecx, esp		;!=0
	ret
;---------------------------------------------------
;	24bpp BMP to 4byte bufer
;---------------------------------------------------
.bpp24:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer
	;bb_w=width(pixel), bb_s=skipByte, bb_h=height
	;bb_t=outputType, bb_sw=sourceWidthByte

	.do24_1:
	  sub esi, [ebp+bb_sw]	;esi=startOfLine
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do24_2:
	    mov al, [esi]
	     mov [edi+3], dl
	    mov [edi], al
	     mov al, [esi+1]
	    mov [edi+1], al
	     mov al, [esi+2]
	    mov [edi+2], al
	     add esi, 3
	    add edi, 4
	     dec ecx
	  jnz .do24_2
	  pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do24_1
	ret

;---------------------------------------------------
;	8bpp BMP to 4byte buffer
;---------------------------------------------------
.bpp8:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer

	;pallete変換
	mov dl, 255
	mov eax, [ebp+bb_paletteSize]
	sub ebx, eax
	shl eax, 8
	add ebx, eax		;ebx += paletteSize*255
	.do8_1:
	  mov eax, [ebx]
	  sub ebx, [ebp+bb_paletteSize]
	  and eax, 0x00ffffff
	  dec edx
	  push eax
	jns .do8_1

	.do8_2:
	  sub esi, [ebp+bb_sw]	;esi=firstLineStart
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do8_3:
	    xor eax, eax
	     add edi, 4
	    mov al, [esi]
	     inc esi
	    ;AGI stole
	    mov eax, [esp+eax*4+8]
	     dec ecx
	    mov [edi-4], eax
	  jnz .do8_3
	  pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do8_2
	add esp, 256*4		;palette
	ret

;---------------------------------------------------
;	4bpp BMP to 4byte buffer
;---------------------------------------------------
.bpp4:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer

	;pallete変換
	mov dl, 16
	mov eax, [ebp+bb_paletteSize]
	sub ebx, eax
	shl eax, 4
	add ebx, eax			;ebx+=eax*15
	.do4_1:
	  mov eax, [ebx]
	  sub ebx, [ebp+bb_paletteSize]
	  and eax, 0x00ffffff
	  dec edx
	  push eax
	jnz .do4_1

	.do4_2:
	  sub esi, [ebp+bb_sw]	;esi=firstLineStart
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do4_3:
	    xor edx, edx
	     mov al, [esi]
	    mov dl, al
	     inc esi
	    shr dl, 4
	     and eax, 15
	    add edi, 4
	     dec ecx
	    mov edx, [esp+edx*4+8]
	     mov eax, [esp+eax*4+8]
	    mov [edi-4], edx
	     jz .wend
	    mov [edi], eax
	     add edi, 4
	    dec ecx
	  jnz .do4_3
.wend:
	pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do4_2
	add esp, 16*4		;palette
	ret

;---------------------------------------------------
;	1bpp BMP to 4byte buffer
;---------------------------------------------------
.bpp1:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer

	;pallete変換
	mov eax, [ebx]
	add ebx, [ebp+bb_paletteSize]
	and eax, 0x00ffffff
	mov ebx, [ebx]
	and ebx, 0x00ffffff
	xor ebx, eax
	;push ebx
	push eax

	.do1_1:
	  sub esi, [ebp+bb_sw]	;esi=firstLineStart
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do1_2:
	    mov dl, [esi]
	     inc esi
	    push esi
	     mov esi, 8
	    .do1_3:
	      add edi, 4
	       add dl, dl
	      sbb eax, eax
	      and eax, ebx
	      xor eax, [ebp+bb_col0]
	       dec ecx
	      mov [edi-4], eax
	       jz .wend1bpp
	      dec esi
	    jnz .do1_3
	    pop esi
	  jmp .do1_2
.wend1bpp:
	pop ecx
	  pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do1_1
	pop eax
	ret

;===============================================
;	Buffer mode 2byte
;===============================================
buf16:
	dec edx
	je .bpp1
	sub edx, 4-1
	je .bpp4
	sub edx, 8-4
	je .bpp8
	sub edx, 24-8
	je .bpp24
	mov ecx, esp
	ret
;---------------------------------------------------
;	24bpp BMP to 2byte bufer
;---------------------------------------------------
.bpp24:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer

	.do24_1:
	  sub esi, [ebp+bb_sw]	;esi=startOfLine
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do24_2:
	    mov al, [esi+2]
	    shl eax, 16
	    mov ax, [esi]
	     add esi, 3
	    ;減色処理 eax=24bitColor, edx=work, ecx=counter, ebx=work
	    ;川合さんの趣味で没ったルーチンを持ってくるもよし(ぉ
	    ;誤差拡散ルーチンを持ってくるもよし
	    shr ah, 2		;???????? RRRRRrrr 00GGGGGG BBBBBbbb 
	     inc edi
	    shr eax, 3		;000????? ???RRRRR rrr00GGG GGGBBBBB
	    shl ax, 5		;000????? ???RRRRR GGGGGGBB BBB00000
	     inc edi
	    shr eax, 5		;00000000 ???????? RRRRRGGG GGGBBBBB
	     dec ecx
	    mov [edi-2], ax
	  jnz .do24_2
	  pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do24_1
	ret

;---------------------------------------------------
;	8bpp BMP to 2byte buffer
;---------------------------------------------------
.bpp8:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer

	;pallete変換
	mov dl, 255
	mov eax, [ebp+bb_paletteSize]
	sub ebx, eax
	shl eax, 8
	add ebx, eax		;ebx += paletteSize*255
	.do8_1:
	  mov eax, [ebx]
	  sub ebx, [ebp+bb_paletteSize]
	  call .paletteConv
	  dec edx
	  push eax
	jns .do8_1

	.do8_2:
	  sub esi, [ebp+bb_sw]	;esi=firstLineStart
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do8_3:
	    xor eax, eax
	     add edi, 2
	    mov al, [esi]
	     inc esi
	    ;AGI stole
	    mov eax, [esp+eax*4+8]
	     dec ecx
	    mov [edi-2], ax
	  jnz .do8_3
	  pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do8_2
	add esp, 256*4		;palette
	ret

;---------------------------------------------------
;	4bpp BMP to 2byte buffer
;---------------------------------------------------
.bpp4:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer

	;pallete変換
	mov dl, 16
	mov eax, [ebp+bb_paletteSize]
	sub ebx, eax
	shl eax, 4
	add ebx, eax			;ebx+=eax*15
	.do4_1:
	  mov eax, [ebx]
	  sub ebx, [ebp+bb_paletteSize]
	  call .paletteConv
	  dec edx
	  push eax
	jnz .do4_1

	.do4_2:
	  sub esi, [ebp+bb_sw]	;esi=firstLineStart
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do4_3:
	    xor edx, edx
	     mov al, [esi]
	    mov dl, al
	     inc esi
	    shr dl, 4
	     and eax, 15
	    add edi, 2
	     dec ecx
	    mov edx, [esp+edx*4+8]
	     mov eax, [esp+eax*4+8]
	    mov [edi-2], dx
	     jz .wend
	    mov [edi], ax
	     add edi, 2
	    dec ecx
	  jnz .do4_3
.wend:
	pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do4_2
	add esp, 16*4		;palette
	ret

;---------------------------------------------------
;	1bpp BMP to 2byte buffer
;---------------------------------------------------
.bpp1:
	;ecx=height, edx=0, ebx=palette
	;esi=endOfImage, edi=destinationBuffer

	;pallete変換
	mov eax, [ebx]
	add ebx, [ebp+bb_paletteSize]
	call .paletteConv
	push eax
	mov eax, [ebx]
	call .paletteConv
	pop ebx
	xchg eax, ebx
	xor ebx, eax
	push eax

	.do1_1:
	  sub esi, [ebp+bb_sw]	;esi=firstLineStart
	  push ecx
	  push esi
	  mov ecx, [ebp+bb_w]
	  .do1_2:
	    mov dl, [esi]
	     inc esi
	    push esi
	     mov esi, 8
	    .do1_3:
	      add dl, dl
	       inc edi
	      sbb eax, eax
	       inc edi
	      and eax, ebx
	      xor eax, [ebp+bb_col0]
	       dec ecx
	      mov [edi-2], ax
	       jz .wend1bpp
	      dec esi
	    jnz .do1_3
	    pop esi
	  jmp .do1_2
.wend1bpp:
	  pop ecx
	  pop esi
	  pop ecx
	  add edi, [ebp+bb_s]
	  dec ecx
	jnz .do1_1
	pop eax
	ret

.paletteConv:
	shr ah, 2
	shr eax, 3
	shl ax, 5
	shr eax, 5
	ret
