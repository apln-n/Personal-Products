#pragma once

/*
	�A�N�e�B�u�u���b�N�̉�]�����p�B4�~7=28�ʂ�ɏꍇ���������Ă���
	�A�N�e�B�u�u���b�N�̉�]���:4��
	�A�N�e�B�u�u���b�N�̎��:7��
	�����:0��1��2��3��0���c
	�E���:0��3��2��1��0���c

	�V���W��x���W�́Abase_array���ʏ�ł�28�킻�ꂼ��̏ꍇ�ɂ����ČŒ�Ȃ̂ŁA
	�����l�����̂܂ܑ�����Ă���B
	�V���W��y���W�́A���݂�active_block_index[0][0]���猩������[0][0]�ȊO�̃u���b�N���A
	�������牽�u���b�N(���i)�㉺�ɂ��邩�Ƃ������Ƃ�28�킻�ꂼ��ɂ����ČŒ�Ȃ̂ŁA
	���̍��̃u���b�N��(�i��)��active_block_index[0][0]�𑫂������̂��i�[���Ă���B
	�������A����0�̏ꍇ�́u+0�v���ȗ����Ă���B
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
