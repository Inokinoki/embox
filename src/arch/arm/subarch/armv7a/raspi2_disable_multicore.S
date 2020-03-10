/**
 * @file
 * @brief
 *
 * @author  Weixuan XIAO
 * @date    06.10.2014
 */

.global hardware_init_hook
hardware_init_hook:
	// Shut off extra cores
	mrc p15, 0, r5, c0, c0, 5
	and r5, r5, #3
	cmp r5, #0
	bne halt

halt:
	wfe
	b halt
