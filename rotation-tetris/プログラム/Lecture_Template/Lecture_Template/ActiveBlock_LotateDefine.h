#pragma once

/*
	アクティブブロックの回転処理用。4×7=28通りに場合分けをしている
	アクティブブロックの回転状態:4種
	アクティブブロックの種類:7種
	左回り:0→1→2→3→0→…
	右回り:0→3→2→1→0→…

	新座標のx座標は、base_array平面上では28種それぞれの場合において固定なので、
	整数値をそのまま代入している。
	新座標のy座標は、現在のactive_block_index[0][0]から見た時に[0][0]以外のブロックが、
	そこから何ブロック(何段)上下にあるかということは28種それぞれにおいて固定なので、
	その差のブロック数(段数)とactive_block_index[0][0]を足したものを格納している。
	ただし、差が0の場合は「+0」を省略している。
*/
extern int active_block_index[4][2];
extern int ActiveBlockIndex_Tmp[4][2];

void ActiveBlock_LotateDefine_00() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 0;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 1;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 11;
}
void ActiveBlock_LotateDefine_01() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_02() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 0;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 11;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 1;
}
void ActiveBlock_LotateDefine_03() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_10() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 1;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 11;
}
void ActiveBlock_LotateDefine_11() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_12() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 11;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 1;
}
void ActiveBlock_LotateDefine_13() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_20() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 1;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 11;
}
void ActiveBlock_LotateDefine_21() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_22() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 11;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 1;
}
void ActiveBlock_LotateDefine_23() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_30() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 0;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-2;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_31() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 11;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 10;
}
void ActiveBlock_LotateDefine_32() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 0;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+2;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_33() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 1;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 2;
}
void ActiveBlock_LotateDefine_40() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[3][1] = 11;
}
void ActiveBlock_LotateDefine_41() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 0;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 11;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[3][1] = 11;
}
void ActiveBlock_LotateDefine_42() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[3][1] = 1;
}
void ActiveBlock_LotateDefine_43() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 0;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 1;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[3][1] = 1;
}
void ActiveBlock_LotateDefine_50() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 11;
}
void ActiveBlock_LotateDefine_51() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 1;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_52() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 1;
}
void ActiveBlock_LotateDefine_53() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 11;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_60() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 1;
}
void ActiveBlock_LotateDefine_61() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 1;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
void ActiveBlock_LotateDefine_62() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 1;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[2][1] = 0;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[3][1] = 11;
}
void ActiveBlock_LotateDefine_63() {
	ActiveBlockIndex_Tmp[1][0] = active_block_index[0][0]-1;
	ActiveBlockIndex_Tmp[1][1] = 11;
	ActiveBlockIndex_Tmp[2][0] = active_block_index[0][0];
	ActiveBlockIndex_Tmp[2][1] = 11;
	ActiveBlockIndex_Tmp[3][0] = active_block_index[0][0]+1;
	ActiveBlockIndex_Tmp[3][1] = 0;
}
