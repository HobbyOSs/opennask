unsigned uni_up(unsigned u)
{
	u8 b = u;
	switch(u>>7) {
	case 0: /* 0000 */
		if(b <= 0x60) break;
		if(b <= 0x7A) {
			u -= 32;
			break;
		}
		break;
	case 1: /* 0080 */
		if(b <= 0xDF) {
			if(b == 0xB5) u += 743;
			break;
		}
		if(b == 0xF7) break;
		u -= 32;
		if(b == 0xFF) u += 153;
		break;
	case 2: /* 0100 */
		if(b <= 0x38) {
			if(b != 0x31) goto ret_p;
			u -= 232;
			break;
		}
		if(b <= 0x49) goto ret_o;
		if(b <= 0x78) goto ret_p;
		if(b != 0x7F) goto ret_o;
		u -= 300;
		break;
	case 3: /* 0180 */
		if(b <= 0x82) break;
		if(b <= 0x86) goto ret_p;
		if(b <= 0xA0) {
			u--;
			if(b == 0x88) break;
			if(b == 0x8C) break;
			if(b == 0x92) break;
			if(b == 0x99) break;
			u += 98;
			if(b == 0x95) break;
			u -= 97;
			if(b == 0x9E) u += 130;
			break;
		}
		if(b <= 0xA5) goto ret_p;
		if(b <= 0xCB) {
			u--;
			if(b == 0xA8) break;
			if(b == 0xAD) break;
			if(b == 0xB0) break;
			if(b == 0xB4) break;
			if(b == 0xB6) break;
			if(b == 0xB9) break;
			if(b == 0xBD) break;
			if(b == 0xC6) break;
			if(b == 0xC9) break;
			u++;
			if(b == 0xBF) u += 56;
			break;
		}
		if(b <= 0xDD) {
			if(b != 0xDD) goto ret_o;
			u -= 79;
			break;
		}
		if(b == 0xF1) break;
		if(b != 0xF7) goto ret_p;
		break;
	case 4: /* 0200 */
		if(b <= 0x33) {
			if(b != 0x21) goto ret_p;
			break;
		}
		u -= 214;
		if(b == 0x75) break;
		u++;
		if(b == 0x72) break;
		u += 2;
		if(b == 0x69) break;
		if(b == 0x6F) break;
		u++;
		if(b == 0x53) break;
		u++;
		if(b == 0x68) break;
		u += 2;
		if(b == 0x63) break;
		u++;
		if(b == 0x54) break;
		u++;
		if(b == 0x56) break;
		if(b == 0x57) break;
		if(b == 0x60) break;
		u += 2;
		if(b == 0x5B) break;
		u += 203;
		if(b == 0x59) u -= 202;
		break;
	case 5: /* 0280 */
		u -= 219;
		if(b == 0x92) break;
		u++;
		if(b == 0x80) break;
		if(b == 0x83) break;
		if(b == 0x88) break;
		b &= 0xFE;
		u += 218;
		if(b == 0x8A) u -= 217;
		break;
	case 7: /* 0380 */
		if(b <= 0xAC) {
			if(b == 0xAC) u -= 38;
			break;
		}
		if(b <= 0xAF) {
			u -= 37;
			break;
		}
		if(b <= 0xB0) break;
		if(b <= 0xCB) {
			u -= 32;
			if(b == 0xC2) u++;
			break;
		}
		if(b <= 0xD9) {
			u -= 64;
			if(b == 0xCC) break;
			u++;
			if(b == 0xCD) break;
			if(b == 0xCE) break;
			u++;
			if(b == 0xD0) break;
			u += 5;
			if(b == 0xD1) break;
			u += 3;
			if(b == 0xD6) break;
			u += 7;
			if(b == 0xD5) break;
			u += 47;
			if(b == 0xD9) u--;
			break;
		}
		if(b <= 0xEF) goto ret_p;
		u -= 96;
		if(b == 0xF5) break;
		u += 10;
		if(b == 0xF0) break;
		u += 6;
		if(b == 0xF1) break;
		u += 79;
		if(b == 0xF8) break;
		if(b == 0xFB) break;
		u++;
		if(b == 0xF2) u += 7;
		break;
	case 8: /* 0400 */
		if(b <= 0x2F) break;
		if(b <= 0x4F) {
			u -= 32;
			break;
		}
		if(b <= 0x5F) {
			u -= 80;
			break;
		}
		goto ret_p;
	case 9: /* 0480 */
		if(b <= 0x8A) {
			if(b == 0x81) u--;
			break;
		}
		if(b <= 0xC0) goto ret_p;
		if(b <= 0xCF) goto ret_o;
		if(b <= 0xF5) goto ret_p;
		if(b == 0xF9) u--;
		break;
	case 10: /* 0500 */
		if(b <= 0x0F) goto ret_p;
		if(b <= 0x60) break;
		u -= 48;
		break;
	case 11: /* 0580 */
		if(b <= 0x86) {
			u -= 48;
			break;
		}
		break;
	case 60: /* 1E00 */
		goto ret_p;
	case 61: /* 1E80 */
		if(b <= 0x96) goto ret_p;
		if(b <= 0xA0) {
			if(b == 0x9B) u -= 59;
			break;
		}
		if(b <= 0xF9) goto ret_p;
		break;
	case 62: /* 1F00 */
		if(b <= 0x6F) {
			b &= 0xF9;
			if(b != 0x50) return u | 8;
			break;
		}
		if(b <= 0x71) {
			b &= 0xFE;
			if(b == 0x70) u += 74;
			break;
		}
		if(b <= 0x75) {
			u += 86;
			break;
		}
		b &= 0xFE;
		u += 100;
		if(b == 0x76) break;
		u += 12;
		if(b == 0x7A) break;
		u += 14;
		if(b == 0x7C) break;
		u -= 126;
		if(b == 0x78) u += 128;
		break;
	case 63: /* 1F80 */
		if(b <= 0xB1) return u | 8;
		u -= 7205;
		if(b == 0xBE) break;
		b &= 0xBF;
		u += 7212;
		if(b == 0xA5) break;
		u++;
		if(b == 0x90) break;
		if(b == 0x91) break;
		if(b == 0xA0) break;
		if(b == 0xA1) break;
		u++;
		if(b == 0x83) break;
		if(b == 0xB3) break;
		u -= 9;
		break;
	case 510: /* FF00 */
		if(b <= 0x40) break;
		if(b <= 0x5A) {
			u -= 32;
			break;
		}
		break;
	}
	return u;
ret_p:
	return u & ~1;
ret_o:
	return u-1 + (u&1);
}
