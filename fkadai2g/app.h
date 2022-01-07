/*
 *���\�z�m�F�p�@�T���v���v���O����(00_sample_tone)
 */
/*
 *�^�[�Q�b�g�ˑ��̒�`
 */
#include "target_test.h"
/*
 *�^�[�Q�b�g�Ɉˑ�����\���̂���萔�̒�`
 */
#ifndef STACK_SIZE
#define STACK_SIZE 4096 /*�^�X�N�̃X�^�b�N�T�C�Y*/
#endif					/*STACK_SIZE*/
/*
 *�֐��̃v���g�^�C�v�錾
 */
#ifndef TOPPERS_MACRO_ONLY
extern void main_task(intptr_t exinf);
extern void run_task(intptr_t exinf);
#endif
