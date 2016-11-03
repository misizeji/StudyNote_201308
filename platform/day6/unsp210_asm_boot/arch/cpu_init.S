
#define __ASSEMBLY__

#include <smdkv210single.h>
#include <s5pc110.h>


	.word 0x2000(8K)
	.word 0x0
	.word 0x0
	.word 0x0

.globl _start
_start: b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

_undefined_instruction:
	.word reload
_software_interrupt:
	.word reload
_prefetch_abort:
	.word reload
_data_abort:
	.word reload
_not_used:
	.word reload
_irq:
	.word reload
_fiq:
	.word reload


reset:
	/* Disable Watchdog */
	ldr	r0, =ELFIN_WATCHDOG_BASE	/* 0xE2700000 */
	mov	r1, #0
	str	r1, [r0]	

	bl system_clock_init
	bl mem_ctrl_asm_init
	bl uart_asm_init
	bl nand_asm_init

	ldr	sp, =0xd0036000 /* end of sram dedicated to u-boot */
	sub	sp, sp, #12	/* set stack */
	mov	fp, #0

	bl copy_uboot_to_ram

	ldr	pc, _start_main

_start_main:
	.word main




/*
 * system_clock_init: Initialize core clock and bus clock.
 * void system_clock_init(void)
 */
system_clock_init:

	ldr	r0, =ELFIN_CLOCK_POWER_BASE	@0xe0100000

	/* Set Mux to FIN */
	ldr	r1, =0x0
	str	r1, [r0, #CLK_SRC0_OFFSET]

	ldr	r1,	=APLL_LOCKTIME_VAL
	str	r1,	[r0, #APLL_LOCK_OFFSET]

	/* Disable PLL */
#if defined(CONFIG_CHECK_MPLL_LOCK)
retryloop:
#endif
	ldr	r1, =0x0
	str	r1, [r0, #APLL_CON0_OFFSET]
	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =0x0
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr   	r1, [r0, #CLK_DIV0_OFFSET]
	ldr	r2, =CLK_DIV0_MASK
	bic	r1, r1, r2

	ldr	r2, =CLK_DIV0_VAL
	orr	r1, r1, r2
	str	r1, [r0, #CLK_DIV0_OFFSET]

	ldr	r1, =APLL_VAL
	str	r1, [r0, #APLL_CON0_OFFSET]

	ldr	r1, =MPLL_VAL
	str	r1, [r0, #MPLL_CON_OFFSET]

	ldr	r1, =VPLL_VAL
	str	r1, [r0, #VPLL_CON_OFFSET]
#if defined(CONFIG_EVT1)
	ldr	r1, =AFC_ON
	str	r1, [r0, #APLL_CON1_OFFSET]
#endif
	mov	r1, #0x10000
1:	subs	r1, r1, #1
	bne	1b

#if defined(CONFIG_CHECK_MPLL_LOCK)
	/* MPLL software workaround */
	ldr	r1, [r0, #MPLL_CON_OFFSET]
	orr     r1, r1, #(1<<28)
	str	r1, [r0, #MPLL_CON_OFFSET]

	mov	r1, #0x100
1:	subs	r1, r1, #1
	bne	1b

	ldr	r1, [r0, #MPLL_CON_OFFSET]
	and	r1, r1, #(1<<29)
	cmp	r1, #(1<<29)
	bne 	retryloop

	/* H/W lock detect disable */
	ldr	r1, [r0, #MPLL_CON_OFFSET]
	bic     r1, r1, #(1<<28)
	str	r1, [r0, #MPLL_CON_OFFSET]
#endif

	ldr	r1, [r0, #CLK_SRC0_OFFSET]
	ldr	r2, =0x10001111
	orr	r1, r1, r2
	str	r1, [r0, #CLK_SRC0_OFFSET]

#if defined(CONFIG_MCP_AC)

	/* CLK_SRC6[25:24] -> OneDRAM clock sel = MPLL */
	ldr	r1, [r0, #CLK_SRC6_OFFSET]
	bic	r1, r1, #(0x3<<24)
	orr	r1, r1, #0x01000000
	str	r1, [r0, #CLK_SRC6_OFFSET]

	/* CLK_DIV6[31:28] -> 4=1/5, 3=1/4(166MHZ@667MHz), 2=1/3 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0xF<<28)
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	orr	r1, r1, #0x30000000
	str	r1, [r0, #CLK_DIV6_OFFSET]

#elif defined (CONFIG_MCP_N)
	/* CLK_SRC6[25:24] -> OneDRAM clock sel = 00:SCLKA2M, 01:SCLKMPLL */
	ldr	r1, [r0, #CLK_SRC6_OFFSET]
	mov	r1, #0x00000000
	str	r1, [r0, #CLK_SRC6_OFFSET]

	/* CLK_DIV6[31:28] -> 0=1/1 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	mov	r1, #0x00000000
	str	r1, [r0, #CLK_DIV6_OFFSET]


#elif defined (CONFIG_MCP_H)

	/* CLK_SRC6[25:24] -> OneDRAM clock sel = 00:SCLKA2M, 01:SCLKMPLL */
	ldr	r1, [r0, #CLK_SRC6_OFFSET]
	bic	r1, r1, #(0x3<<24)
	orr	r1, r1, #0x00000000
	str	r1, [r0, #CLK_SRC6_OFFSET]

	/* CLK_DIV6[31:28] -> 4=1/5, 3=1/4(166MHZ@667MHz), 2=1/3 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0xF<<28)
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	orr	r1, r1, #0x00000000
	str	r1, [r0, #CLK_DIV6_OFFSET]	

#elif defined (CONFIG_MCP_B) || defined (CONFIG_MCP_D)

	/* CLK_SRC6[25:24] -> OneDRAM clock sel = 00:SCLKA2M, 01:SCLKMPLL */
	ldr	r1, [r0, #CLK_SRC6_OFFSET]
	bic	r1, r1, #(0x3<<24)
	orr	r1, r1, #0x01000000
	str	r1, [r0, #CLK_SRC6_OFFSET]

	/* CLK_DIV6[31:28] -> 4=1/5, 3=1/4(166MHZ@667MHz), 2=1/3 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0xF<<28)
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	orr	r1, r1, #0x30000000
	str	r1, [r0, #CLK_DIV6_OFFSET]

#elif defined (CONFIG_MCP_SINGLE)

	/* CLK_DIV6 */
	ldr	r1, [r0, #CLK_DIV6_OFFSET]
	bic	r1, r1, #(0x7<<12)	@; ONENAND_RATIO: 0
	str	r1, [r0, #CLK_DIV6_OFFSET]

#endif	

	mov	pc, lr


/*
 * uart_asm_init: Initialize UART in asm mode, 115200bps fixed.
 * void uart_asm_init(void)
 */
uart_asm_init:

	/* set GPIO(GPA) to enable UART */
	@ GPIO setting for UART
	ldr	r0, =ELFIN_GPIO_BASE
	ldr	r1, =0x22222222
	str   	r1, [r0, #GPA0CON_OFFSET]

	ldr     r1, =0x2222
	str     r1, [r0, #GPA1CON_OFFSET]

	// HP V210 use. SMDK not use.
#if defined(CONFIG_VOGUES)
	ldr    r1, =0x100
	str    r1, [r0, #GPC0CON_OFFSET]

	ldr    r1, =0x4
	str    r1, [r0, #GPC0DAT_OFFSET]
#endif

	ldr	r0, =ELFIN_UART_CONSOLE_BASE		@0xEC000000
	mov	r1, #0x0
	str	r1, [r0, #UFCON_OFFSET]
	str	r1, [r0, #UMCON_OFFSET]

	mov	r1, #0x3
	str	r1, [r0, #ULCON_OFFSET]

	ldr	r1, =0x3c5
	str	r1, [r0, #UCON_OFFSET]

	ldr	r1, =UART_UBRDIV_VAL
	str	r1, [r0, #UBRDIV_OFFSET]

	ldr	r1, =UART_UDIVSLOT_VAL
	str	r1, [r0, #UDIVSLOT_OFFSET]

	ldr	r1, =0x4f4f4f4f
	str	r1, [r0, #UTXH_OFFSET]		@'O'

	mov	pc, lr





/*
 * Nand Interface Init for SMDKC110
 */
nand_asm_init:

	/* Setting GPIO for NAND */
	/* This setting is NAND initialze code at booting time in iROM. */

	ldr	r0, =ELFIN_GPIO_BASE
	
	ldr	r1, [r0, #MP01CON_OFFSET]
	bic	r1, r1, #(0xf<<8)
	orr	r1, r1, #(0x3<<8)
	str	r1, [r0, #MP01CON_OFFSET]

	ldr	r1, [r0, #MP01PUD_OFFSET]
	bic	r1, r1, #(0x3<<4)
	str	r1, [r0, #MP01PUD_OFFSET]

	ldr	r1, [r0, #MP03CON_OFFSET]
	bic	r1, r1, #0xFFFFFF
	ldr	r2, =0x22222222
	orr	r1, r1, r2
	str	r1, [r0, #MP03CON_OFFSET]

	ldr	r1, [r0, #MP03PUD_OFFSET]
	ldr	r2, =0x3fff
	bic	r1, r1, r2
	str	r1, [r0, #MP03PUD_OFFSET]

	ldr	r0, =ELFIN_NAND_BASE

	ldr	r1, [r0, #NFCONF_OFFSET]
	ldr	r2, =0x777F
	bic	r1, r1, r2
	ldr	r2, =NFCONF_VAL
	orr	r1, r1, r2
	str	r1, [r0, #NFCONF_OFFSET]

	ldr	r1, [r0, #NFCONT_OFFSET]
	ldr	r2, =0x707C7
	bic	r1, r1, r2
	ldr	r2, =NFCONT_VAL
	orr	r1, r1, r2
	str	r1, [r0, #NFCONT_OFFSET]

	ldr	r1, [r0, #NFCONF_OFFSET]
	orr	r1, r1, #0x70
	orr	r1, r1, #0x7700
	str     r1, [r0, #NFCONF_OFFSET]

	ldr	r1, [r0, #NFCONT_OFFSET]
	orr	r1, r1, #0x03
	str     r1, [r0, #NFCONT_OFFSET]

	mov	pc, lr





	.globl mem_ctrl_asm_init
mem_ctrl_asm_init:

	/* DMC0 Drive Strength (Setting 2X) */
	
	ldr	r0, =ELFIN_GPIO_BASE

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_0DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_1DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_2DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_3DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_4DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_5DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_6DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP1_7DRV_SR_OFFSET]

	ldr	r1, =0x00002AAA
	str	r1, [r0, #MP1_8DRV_SR_OFFSET]


	/* DMC1 Drive Strength (Setting 2X) */
	
	ldr	r0, =ELFIN_GPIO_BASE
	
	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_0DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_1DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_2DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_3DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_4DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_5DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_6DRV_SR_OFFSET]

	ldr	r1, =0x0000AAAA
	str	r1, [r0, #MP2_7DRV_SR_OFFSET]

	ldr	r1, =0x00002AAA
	str	r1, [r0, #MP2_8DRV_SR_OFFSET]
	
	/* DMC0 initialization at single Type*/
	ldr	r0, =APB_DMC_0_BASE

	ldr	r1, =0x00101000				@PhyControl0 DLL parameter setting, manual 0x00101000
	str	r1, [r0, #DMC_PHYCONTROL0]

	ldr	r1, =0x00000086				@PhyControl1 DLL parameter setting, LPDDR/LPDDR2 Case
	str	r1, [r0, #DMC_PHYCONTROL1]

	ldr	r1, =0x00101002				@PhyControl0 DLL on
	str	r1, [r0, #DMC_PHYCONTROL0]

	ldr	r1, =0x00101003				@PhyControl0 DLL start
	str	r1, [r0, #DMC_PHYCONTROL0]

find_lock_val:
	ldr	r1, [r0, #DMC_PHYSTATUS]		@Load Phystatus register value
	and	r2, r1, #0x7
	cmp	r2, #0x7				@Loop until DLL is locked
	bne	find_lock_val
	
	and	r1, #0x3fc0 
	mov	r2, r1, LSL #18
	orr	r2, r2, #0x100000
	orr	r2 ,r2, #0x1000	
		
	orr	r1, r2, #0x3				@Force Value locking
	str	r1, [r0, #DMC_PHYCONTROL0]

	/* setting DDR2 */
	ldr	r1, =0x0FFF2010				@ConControl auto refresh off
	str	r1, [r0, #DMC_CONCONTROL]

	ldr	r1, =0x00212400				@MemControl BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
	str	r1, [r0, #DMC_MEMCONTROL]
	
	ldr	r1, =DMC0_MEMCONFIG_0			@MemConfig0 256MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
	str	r1, [r0, #DMC_MEMCONFIG0]

	ldr	r1, =DMC0_MEMCONFIG_1			@MemConfig1
	str	r1, [r0, #DMC_MEMCONFIG1]

	ldr	r1, =0xFF000000				@PrechConfig
	str	r1, [r0, #DMC_PRECHCONFIG]
	
	ldr	r1, =DMC0_TIMINGA_REF			@TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4E)
	str	r1, [r0, #DMC_TIMINGAREF]
	
	ldr	r1, =DMC0_TIMING_ROW			@TimingRow	for @200MHz
	str	r1, [r0, #DMC_TIMINGROW]

	ldr	r1, =DMC0_TIMING_DATA			@TimingData	CL=3
	str	r1, [r0, #DMC_TIMINGDATA]
	
	ldr	r1, =DMC0_TIMING_PWR			@TimingPower
	str	r1, [r0, #DMC_TIMINGPOWER]

	ldr	r1, =0x07000000				@DirectCmd	chip0 Deselect
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x01000000				@DirectCmd	chip0 PALL
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00020000				@DirectCmd	chip0 EMRS2
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00030000				@DirectCmd	chip0 EMRS3
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00010400				@DirectCmd	chip0 EMRS1 (MEM DLL on, DQS# disable)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00000542				@DirectCmd	chip0 MRS (MEM DLL reset) CL=4, BL=4
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x01000000				@DirectCmd	chip0 PALL
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x05000000				@DirectCmd	chip0 REFA
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x05000000				@DirectCmd	chip0 REFA
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00000442				@DirectCmd	chip0 MRS (MEM DLL unreset)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00010780				@DirectCmd	chip0 EMRS1 (OCD default)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00010400				@DirectCmd	chip0 EMRS1 (OCD exit)
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x07100000				@DirectCmd	chip1 Deselect
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x01100000				@DirectCmd	chip1 PALL
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x00120000				@DirectCmd	chip1 EMRS2
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x00130000				@DirectCmd	chip1 EMRS3
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x00110400				@DirectCmd	chip1 EMRS1 (MEM DLL on, DQS# disable)
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x00100542				@DirectCmd	chip1 MRS (MEM DLL reset) CL=4, BL=4
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x01100000				@DirectCmd	chip1 PALL
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x05100000				@DirectCmd	chip1 REFA
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x05100000				@DirectCmd	chip1 REFA
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x00100442				@DirectCmd	chip1 MRS (MEM DLL unreset)
	str	r1, [r0, #DMC_DIRECTCMD]
	
	ldr	r1, =0x00110780				@DirectCmd	chip1 EMRS1 (OCD default)
	str	r1, [r0, #DMC_DIRECTCMD]
		
	ldr	r1, =0x00110400				@DirectCmd	chip1 EMRS1 (OCD exit)
	str	r1, [r0, #DMC_DIRECTCMD]
		
	ldr	r1, =0x0FF02030				@ConControl	auto refresh on
	str	r1, [r0, #DMC_CONCONTROL]
		
	ldr	r1, =0xFFFF00FF				@PwrdnConfig
	str	r1, [r0, #DMC_PWRDNCONFIG]
		
	ldr	r1, =0x00202400				@MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
	str	r1, [r0, #DMC_MEMCONTROL]

	/* DMC1 initialization */
	ldr	r0, =APB_DMC_1_BASE

	ldr	r1, =0x00101000				@Phycontrol0 DLL parameter setting
	str	r1, [r0, #DMC_PHYCONTROL0]

	ldr	r1, =0x00000086				@Phycontrol1 DLL parameter setting
	str	r1, [r0, #DMC_PHYCONTROL1]

	ldr	r1, =0x00101002				@PhyControl0 DLL on
	str	r1, [r0, #DMC_PHYCONTROL0]

	ldr	r1, =0x00101003				@PhyControl0 DLL start
	str	r1, [r0, #DMC_PHYCONTROL0]
find_lock_val1:
	ldr	r1, [r0, #DMC_PHYSTATUS]		@Load Phystatus register value
	and	r2, r1, #0x7
	cmp	r2, #0x7				@Loop until DLL is locked
	bne	find_lock_val1
	
	and	r1, #0x3fc0 
	mov	r2, r1, LSL #18
	orr	r2, r2, #0x100000
	orr	r2, r2, #0x1000
		
	orr	r1, r2, #0x3				@Force Value locking
	str	r1, [r0, #DMC_PHYCONTROL0]

	/* settinf fot DDR2 */
	ldr	r0, =APB_DMC_1_BASE

	ldr	r1, =0x0FFF2010				@auto refresh off
	str	r1, [r0, #DMC_CONCONTROL]

	ldr	r1, =DMC1_MEMCONTROL			@MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
	str	r1, [r0, #DMC_MEMCONTROL]

	ldr	r1, =DMC1_MEMCONFIG_0			@MemConfig0	512MB config, 8 banks,Mapping Method[12:15]0:linear, 1:linterleaved, 2:Mixed
	str	r1, [r0, #DMC_MEMCONFIG0]

	ldr	r1, =DMC1_MEMCONFIG_1			@MemConfig1
	str	r1, [r0, #DMC_MEMCONFIG1]

	ldr	r1, =0xFF000000
	str	r1, [r0, #DMC_PRECHCONFIG]

	ldr	r1, =DMC1_TIMINGA_REF			@TimingAref	7.8us*133MHz=1038(0x40E), 100MHz=780(0x30C), 20MHz=156(0x9C), 10MHz=78(0x4
	str	r1, [r0, #DMC_TIMINGAREF]

	ldr	r1, =DMC1_TIMING_ROW			@TimingRow	for @200MHz
	str	r1, [r0, #DMC_TIMINGROW]

	ldr	r1, =DMC1_TIMING_DATA			@TimingData	CL=3
	str	r1, [r0, #DMC_TIMINGDATA]

	ldr	r1, =DMC1_TIMING_PWR			@TimingPower
	str	r1, [r0, #DMC_TIMINGPOWER]


	ldr	r1, =0x07000000				@DirectCmd	chip0 Deselect
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x01000000				@DirectCmd	chip0 PALL
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00020000				@DirectCmd	chip0 EMRS2
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00030000				@DirectCmd	chip0 EMRS3
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00010400				@DirectCmd	chip0 EMRS1 (MEM DLL on, DQS# disable)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00000542				@DirectCmd	chip0 MRS (MEM DLL reset) CL=4, BL=4
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x01000000				@DirectCmd	chip0 PALL
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x05000000				@DirectCmd	chip0 REFA
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x05000000				@DirectCmd	chip0 REFA
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00000442				@DirectCmd	chip0 MRS (MEM DLL unreset)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00010780				@DirectCmd	chip0 EMRS1 (OCD default)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00010400				@DirectCmd	chip0 EMRS1 (OCD exit)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x07100000				@DirectCmd	chip1 Deselect
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x01100000				@DirectCmd	chip1 PALL
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00120000				@DirectCmd	chip1 EMRS2
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00130000				@DirectCmd	chip1 EMRS3
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00110440				@DirectCmd	chip1 EMRS1 (MEM DLL on, DQS# disable)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00100542				@DirectCmd	chip1 MRS (MEM DLL reset) CL=4, BL=4
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x01100000				@DirectCmd	chip1 PALL
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x05100000				@DirectCmd	chip1 REFA
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x05100000				@DirectCmd	chip1 REFA
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00100442				@DirectCmd	chip1 MRS (MEM DLL unreset)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00110780				@DirectCmd	chip1 EMRS1 (OCD default)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x00110400				@DirectCmd	chip1 EMRS1 (OCD exit)
	str	r1, [r0, #DMC_DIRECTCMD]

	ldr	r1, =0x0FF02030				@ConControl	auto refresh on
	str	r1, [r0, #DMC_CONCONTROL]

	ldr	r1, =0xFFFF00FF				@PwrdnConfig	
	str	r1, [r0, #DMC_PWRDNCONFIG]

	ldr	r1, =DMC1_MEMCONTROL			@MemControl	BL=4, 2 chip, DDR2 type, dynamic self refresh, force precharge, dynamic power down off
	str	r1, [r0, #DMC_MEMCONTROL]
	
	mov	pc, lr



reload:
	mov pc,#0x00
