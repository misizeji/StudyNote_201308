/*
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* This code should work for both the S5PC110
 * as they seem to have the same PLL and clock machinery inside.
 * The different address mapping is handled by the s5pc110.h files below.
 */
#include "s5pv210.h"
#include "speed.h"
#include "uart.h"

#define APLL 0
#define MPLL 1
#define EPLL 2

#define CONFIG_SYS_CLK_FREQ	24000000

static void hang (void)
{
	printf("\ns5pv%d:### ERROR ### Please RESET the board %d###\n",210,3);
	for(;;);
}

/* ------------------------------------------------------------------------- */
/* NOTE: This describes the proper use of this file.
 *
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_FCLK(), get_HCLK(), get_PCLK() and get_UCLK() return the clock of
 * the specified bus in HZ.
 */
/* ------------------------------------------------------------------------- */

static ulong get_PLLCLK(int pllreg)
{
	ulong r, m, p, s;

	if (pllreg == APLL) {
		r = APLL_CON0_REG;
		m = (r>>16) & 0x3ff;
	} else if (pllreg == MPLL) {
		r = MPLL_CON_REG;
		m = (r>>16) & 0x3ff;
	} else if (pllreg == EPLL) {
		r = EPLL_CON0_REG;
		m = (r>>16) & 0x1ff;
	} else
		hang();

	p = (r>>8) & 0x3f;
	s = r & 0x7;

	if (pllreg == APLL) 
		s= s-1;
	
	return (m * (CONFIG_SYS_CLK_FREQ / (p * (1 << s))));
}

/* return ARMCORE frequency */
ulong get_ARMCLK(void)
{
	ulong div,apll_ratio;

	div = CLK_DIV0_REG;
	apll_ratio = ((div>>0) & 0x7);

	return ((get_PLLCLK(APLL)) / (apll_ratio + 1));
}

/* return FCLK frequency */
ulong get_FCLK(void)
{
	ulong fclk;

	fclk = get_PLLCLK(APLL);
	
	return fclk;
}

/* return FCLK frequency */
ulong get_MPLL_CLK(void)
{
	return (get_PLLCLK(MPLL));
}


/* return HCLK frequency */
ulong get_HCLK(void)
{
	ulong fclk;
	uint mux_stat = CLK_MUX_STAT0_REG;
	uint div,hclk_msys_ratio,apll_ratio;

	div = CLK_DIV0_REG;

	apll_ratio = ((div>>0) & 0x7);
	hclk_msys_ratio = ((div>>8)&0x7);

	switch ((mux_stat>>16) & 0x7) 
	{
		case 2: //SCLKMPLL source
			fclk = get_MPLL_CLK();
			break;
		case 1:	//SCLKAPLL source
		default:
			fclk = get_FCLK();
			break;
	}

	return fclk/((apll_ratio+1)*(hclk_msys_ratio+1));
}

/* return PCLK frequency */
ulong get_PCLK(void)
{
	ulong hclk;
	uint div = CLK_DIV0_REG;
	uint pclk_msys_ratio = ((div>>12) & 0x7);/* get bit 8,9,10 of div ;size range is 0~7*/

	hclk = get_HCLK();	

	return hclk/(pclk_msys_ratio+1);/* @page 387  */
}

/* return TCLK frequency */
ulong get_TCLK(void)
{
	uint val_div;
	uint prescaler;
	ulong pclk;
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	prescaler = (timers->TCFG0>>8)&0x0f;	/*get perscaler is 15 */
	val_div = (timers->TCFG1>>15)&0x03;		/*get div value is 2 */
	
	pclk = get_PCLK();

	return ((pclk/(prescaler+1))/val_div);	/* tlck = (pclk/(prescaler+1))*��Ƶֵ */
}

//ulong get_PCLKP(void);


/* return HCLKDSYS frequency */
ulong get_HCLKD(void)
{
	ulong fclk;
	uint mux_stat = CLK_MUX_STAT0_REG;

	uint div,a2m_ratio,hclk_dsys_ratio;

	div = CLK_DIV0_REG;

	a2m_ratio = ((div >>4) & 0x7);
	hclk_dsys_ratio = ((div >>16) & 0xf);
	
	switch ((mux_stat>>20) & 0x7) {
	case 2: //SCLKA2M source
		fclk = get_FCLK()/(a2m_ratio+1);
		break;
	case 1:	//SCLKMPLL source
	default:
		fclk = get_MPLL_CLK();
		break;
	}

	return fclk/(hclk_dsys_ratio+1);
}

/* return PCLKDSYS frequency */
ulong get_PCLKD(void)
{
	ulong fclk;
	uint div = CLK_DIV0_REG;
	uint pclk_dsys_ratio = ((div>>20) & 0x7);

	fclk = get_HCLKD();	

	return fclk/(pclk_dsys_ratio+1);
}

/* return HCLKPSYS frequency */
ulong get_HCLKP(void)
{
	ulong fclk;
	uint mux_stat = CLK_MUX_STAT0_REG;
	uint div,hclk_psys_ratio,a2m_ratio;

	div = CLK_DIV0_REG;
	a2m_ratio = ((div >>4) & 0x7);
	hclk_psys_ratio = ((div>>24)&0xf);
	
	switch ((mux_stat>>20) & 0x7) {
	case 2: //SCLKA2M source
		fclk = get_FCLK()/(a2m_ratio+1);
		break;
	case 1:	//SCLKMPLL source
	default:
		fclk = get_MPLL_CLK();
		break;
	}


	return fclk/(hclk_psys_ratio+1);
}

/* return PCLKPSYS frequency */
ulong get_PCLKP(void)
{
	ulong fclk;
	uint div = CLK_DIV0_REG;
	uint pclk_psys_ratio = ((div>>28) & 0x7);

	fclk = get_HCLKP();	

	return fclk/(pclk_psys_ratio+1);
}

/* return SCLKA2M frequency */
ulong get_SCLKA2M(void)
{
	ulong fclk;
	uint div = CLK_DIV0_REG;
	uint a2m_ratio = ((div>>4) & 0x7);

	fclk = get_FCLK();	

	return fclk/(a2m_ratio+1);
}/* return UCLK frequency */
ulong get_UCLK(void)
{
	return (get_PLLCLK(EPLL));
}

int print_cpuinfo(void)
{
	uint set_speed;
	uint tmp;
	uchar result_set;

#if defined(CONFIG_CLK_533_133_100_100)
	set_speed = 53300;
#elif defined(CONFIG_CLK_667_166_166_133)
	set_speed = 66700;
#elif defined(CONFIG_CLK_800_200_166_133)
	set_speed = 80000;
#elif defined(CONFIG_CLK_1000_200_166_133)
	set_speed = 100000;
#elif defined(CONFIG_CLK_1200_200_166_133)
	set_speed = 120000;
#else
	set_speed = 100000;
	printf("\ns5pv%d:Any CONFIG_CLK_XXX is not enabled\n",210);
#endif
	tmp = (set_speed / (get_ARMCLK()/1000000));

	if((tmp < 105) && (tmp > 95)){
		result_set = 1;
	} else {
		result_set = 0;
	}

#ifdef CONFIG_MCP_SINGLE
	printf("\ns5pv%d:\nCPU:  S5PV210@%ldMHz(%s)\n",210, get_ARMCLK()/1000000, ((result_set == 1) ? "OK" : "FAIL"));
#else
	printf("\ns5pv%d:\nCPU:  S5PC110@%ldMHz(%s)\n",210, get_ARMCLK()/1000000, ((result_set == 1) ? "OK" : "FAIL"));
#endif
	printf(" \ns5pv%d:       APLL = %ldMHz, HclkMsys = %ldMHz, PclkMsys = %ldMHz\n",210,
			get_FCLK()/1000000, get_HCLK()/1000000, get_PCLK()/1000000);
#if 1
	printf("\ns5pv%d:	MPLL = %ldMHz, EPLL = %ldMHz\n",210,
			get_MPLL_CLK()/1000000, get_PLLCLK(EPLL)/1000000);
	printf("\ns5pv%d:		       HclkDsys = %ldMHz, PclkDsys = %ldMHz\n",210,
			get_HCLKD()/1000000, get_PCLKD()/1000000);
	printf("\ns5pv%d:		       HclkPsys = %ldMHz, PclkPsys = %ldMHz\n",210,
			get_HCLKP()/1000000, get_PCLKP()/1000000);
	printf("\ns5pv%d:SCLKA2M  = %ldMHz\n",210, get_SCLKA2M()/1000000);
#endif
	printf("\ns5pv%d:Serial = CLKUART ",210);

	return 0;
}

