/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifdef BUILD_LK
#include <string.h>
#include <platform/mt_gpio.h>
#include <platform/mt_pmic.h>
#include <platform/upmu_common.h>
#include <platform/mt_i2c.h>
#include <platform/mt_pmic.h>
#elif defined(BUILD_UBOOT)
#include <asm/arch/mt_gpio.h>
#else
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <asm-generic/gpio.h>
#include <linux/string.h>
#include <linux/gpio.h>
#include <linux/pinctrl/consumer.h>
#endif

#ifdef CONFIG_OF
#include <linux/regulator/consumer.h>
#endif

#include "lcm_drv.h"

#if 1
#ifndef BUILD_LK
//static struct regulator *lcm_vgp;
static unsigned int LCD_RST_PIN;
static unsigned int LCD_PWR_BIAS_ENN_PIN;
static unsigned int LCD_PWR_BIAS_ENP_PIN;
#if 0
/* get LDO supply */
static int lcm_get_vgp_supply(struct device *dev)
{
	int ret;
	struct regulator *lcm_vgp_ldo;

	pr_debug("LCM: lcm_get_vgp_supply is going\n");

	lcm_vgp_ldo = devm_regulator_get(dev, "reg-lcm");
	if (IS_ERR(lcm_vgp_ldo)) {
		ret = PTR_ERR(lcm_vgp_ldo);
		pr_debug("failed to get reg-lcm LDO\n");
		return ret;
	}

	pr_debug("LCM: lcm get supply ok.\n");

	/* get current voltage settings */
	ret = regulator_get_voltage(lcm_vgp_ldo);
	pr_debug("lcm LDO voltage = %d in LK stage\n", ret);

	lcm_vgp = lcm_vgp_ldo;

	return ret;
}

int lcm_vgp_supply_enable(void)
{
	int ret;
	unsigned int volt;

	pr_debug("LCM: lcm_vgp_supply_enable\n");

	if (lcm_vgp == NULL)
		return 0;

	pr_debug("LCM: set regulator voltage lcm_vgp voltage to 1.8V\n");
	/* set voltage to 1.8V */
	ret = regulator_set_voltage(lcm_vgp, 1800000, 1800000);
	if (ret != 0) {
		pr_debug("LCM: lcm failed to set lcm_vgp voltage\n");
		return ret;
	}

	/* get voltage settings again */
	volt = regulator_get_voltage(lcm_vgp);
	if (volt == 1800000)
		pr_debug("LCM: check regulator voltage=1800000 pass!\n");
	else
		pr_debug("LCM: check regulator voltage=1800000 fail! (voltage: %d)\n", volt);

	ret = regulator_enable(lcm_vgp);
	if (ret != 0) {
		pr_debug("LCM: Failed to enable lcm_vgp\n");
		return ret;
	}

	return ret;
}

int lcm_vgp_supply_disable(void)
{
	int ret = 0;
	unsigned int isenable;

	if (lcm_vgp == NULL)
		return 0;

	/* disable regulator */
	isenable = regulator_is_enabled(lcm_vgp);

	pr_debug("LCM: lcm query regulator enable status[%d]\n", isenable);

	if (isenable) {
		ret = regulator_disable(lcm_vgp);
		if (ret != 0) {
			pr_debug("LCM: lcm failed to disable lcm_vgp\n");
			return ret;
		}
		/* verify */
		isenable = regulator_is_enabled(lcm_vgp);
		if (!isenable)
			pr_debug("LCM: lcm regulator disable pass\n");
	}

	return ret;
}
#endif
static void lcm_request_gpio_control(struct device *dev)
{
	LCD_RST_PIN = of_get_named_gpio(dev->of_node, "lcd_rst_pin", 0);
	gpio_request(LCD_RST_PIN, "LCD_RST_PIN");

	LCD_PWR_BIAS_ENN_PIN = of_get_named_gpio(dev->of_node, "lcd_pwr_bias_enn_pin", 0);
	gpio_request(LCD_PWR_BIAS_ENN_PIN, "LCD_PWR_BIAS_ENN_PIN");

	LCD_PWR_BIAS_ENP_PIN = of_get_named_gpio(dev->of_node, "lcd_pwr_bias_enp_pin", 0);
	gpio_request(LCD_PWR_BIAS_ENP_PIN, "LCD_PWR_BIAS_ENP_PIN");
}

static int lcm_driver_probe(struct device *dev, void const *data)
{
	lcm_request_gpio_control(dev);
	//lcm_get_vgp_supply(dev);
	//lcm_vgp_supply_enable();

	return 0;
}

static const struct of_device_id lcm_platform_of_match[] = {
	{
	 .compatible = "rm,rm68200_pb_dsi_vdo",
	 .data = 0,
	 }, {
	     /* sentinel */
	     }
};

MODULE_DEVICE_TABLE(of, platform_of_match);

static int lcm_platform_probe(struct platform_device *pdev)
{
	const struct of_device_id *id;

	id = of_match_node(lcm_platform_of_match, pdev->dev.of_node);
	if (!id)
		return -ENODEV;

	return lcm_driver_probe(&pdev->dev, id->data);
}

static struct platform_driver lcm_driver = {
	.probe = lcm_platform_probe,
	.driver = {
		   .name = "rm68200_pb_dsi_vdo",
		   .owner = THIS_MODULE,
		   .of_match_table = lcm_platform_of_match,
		   },
};

static int __init lcm_init(void)
{
	if (platform_driver_register(&lcm_driver)) {
		pr_debug("LCM: failed to register this driver!\n");
		return -ENODEV;
	}

	return 0;
}

static void __exit lcm_exit(void)
{
	platform_driver_unregister(&lcm_driver);
}

late_initcall(lcm_init);
module_exit(lcm_exit);
MODULE_AUTHOR("mediatek");
MODULE_DESCRIPTION("LCM display subsystem driver");
MODULE_LICENSE("GPL");
#endif
#endif
/* --------------------------------------------------------------------------- */
/* Local Constants */
/* --------------------------------------------------------------------------- */
#define LCM_DSI_CMD_MODE								0
#define FRAME_WIDTH                                     (800)
#define FRAME_HEIGHT                                    (1280)

#define GPIO_OUT_ONE 1
#define GPIO_OUT_ZERO 0


static LCM_UTIL_FUNCS lcm_util;

#define SET_RESET_PIN(v) (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

/* --------------------------------------------------------------------------- */
/* Local Functions */
/* --------------------------------------------------------------------------- */
#define dsi_set_cmdq_V2(cmd, count, ppara, force_update) lcm_util.dsi_set_cmdq_V2(cmd, count, ppara, force_update)
#define dsi_set_cmdq(pdata, queue_size, force_update) lcm_util.dsi_set_cmdq(pdata, queue_size, force_update)
#define wrtie_cmd(cmd)                          lcm_util.dsi_write_cmd(cmd)
#define write_regs(addr, pdata, byte_nums)      lcm_util.dsi_write_regs(addr, pdata, byte_nums)
#define read_reg(cmd)                           lcm_util.dsi_dcs_read_lcm_reg(cmd)
#define read_reg_v2(cmd, buffer, buffer_size)   lcm_util.dsi_dcs_read_lcm_reg_v2(cmd, buffer, buffer_size)
/* --------------------------------------------------------------------------- */
/* Local Constants */
/* --------------------------------------------------------------------------- */
#define REGFLAG_DELAY                               0xFC
#define REGFLAG_END_OF_TABLE                        0xFD	/* END OF REGISTERS MARKER */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif
extern void lcd_init_power_sequence(unsigned int enable);

/* --------------------------------------------------------------------------- */
/* Local Variables */
/* --------------------------------------------------------------------------- */
static void lcm_set_gpio_output(unsigned int GPIO, unsigned int output)
{

#ifdef BUILD_LK
	mt_set_gpio_mode(GPIO, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO, (output > 0) ? GPIO_OUT_ONE : GPIO_OUT_ZERO);
#else
	gpio_direction_output(GPIO, output);
	gpio_set_value(GPIO, output);
#endif
}

struct LCM_setting_table {
	unsigned char cmd;
	unsigned char count;
	unsigned char para_list[64];
};

#ifndef BUILD_LK
static struct LCM_setting_table lcm_suspend_setting[] = {
	{0x28, 0, {} },
	{REGFLAG_DELAY, 20, {} },
	{0x10, 0, {} },
	{REGFLAG_DELAY, 130, {} },
	{REGFLAG_END_OF_TABLE, 0x00, {} }
};
#endif

static struct LCM_setting_table lcm_initialization_setting[] = {
	{0xfe,1,{0x0e}},
	{0x1b,1,{0x00}},
	{0x1c,1,{0x00}},
	{0xfe,1,{0x00}},
	{0x11,0, {} },
	{REGFLAG_DELAY, 120, {} },
	{0x29,0, {} },
	{REGFLAG_DELAY, 50, {} },

	{REGFLAG_END_OF_TABLE, 0x00, {} }
};

static void dsi_send_cmdq_tinno(unsigned cmd, unsigned char count, unsigned char *para_list,
				unsigned char force_update)
{
	unsigned int item[16];
	unsigned char dsi_cmd = (unsigned char)cmd;
	unsigned char dc;
	int index = 0, length = 0;

	memset(item, 0, sizeof(item));
	if (count + 1 > 60)
		return;

	if (count == 0) {
		item[0] = 0x0500 | (dsi_cmd << 16);
		length = 1;
	} else if (count == 1) {
		item[0] = 0x1500 | (dsi_cmd << 16) | (para_list[0] << 24);
		length = 1;
	} else {
		item[0] = 0x3902 | ((count + 1) << 16);	/* Count include command. */
		++length;
		while (1) {
			if (index == count + 1)
				break;
			if (index == 0)
				dc = cmd;
			else
				dc = para_list[index - 1];
			/* an item make up of 4data. */
			item[index / 4 + 1] |= (dc << (8 * (index % 4)));
			if (index % 4 == 0)
				++length;
			++index;
		}
	}

	dsi_set_cmdq(item, length, force_update);
}


static void push_table(struct LCM_setting_table *table, unsigned int count,
		       unsigned char force_update)
{
	unsigned int i;
	unsigned cmd;

	for (i = 0; i < count; i++) {
		cmd = table[i].cmd;
		switch (cmd) {
		case REGFLAG_DELAY:
			MDELAY(table[i].count);
			break;

		case REGFLAG_END_OF_TABLE:
			break;

		default:
			dsi_send_cmdq_tinno(cmd, table[i].count, table[i].para_list, force_update);
			/* dsi_set_cmdq_V2(cmd, table[i].count, table[i].para_list, force_update); */
		}
	}
}

static void init_lcm_registers(void)
{
	push_table(lcm_initialization_setting,
		   sizeof(lcm_initialization_setting) / sizeof(struct LCM_setting_table), 1);
}

/* --------------------------------------------------------------------------- */
/* LCM Driver Implementations */
/* --------------------------------------------------------------------------- */
static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}

static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(LCM_PARAMS));

	params->type = LCM_TYPE_DSI;

	params->width = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;
	//params->density = 213;
#if (LCM_DSI_CMD_MODE)
	params->dsi.mode = CMD_MODE;
#else
	params->dsi.mode = SYNC_PULSE_VDO_MODE;
#endif

	/* DSI */
	/* Command mode setting */
	params->dsi.LANE_NUM = LCM_FOUR_LANE;
	/* The following defined the fomat for data coming from LCD engine. */
	params->dsi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dsi.data_format.trans_seq = LCM_DSI_TRANS_SEQ_MSB_FIRST;
	params->dsi.data_format.padding = LCM_DSI_PADDING_ON_LSB;
	params->dsi.data_format.format = LCM_DSI_FORMAT_RGB888;

	params->dsi.intermediat_buffer_num = 2;
	params->dsi.PS = LCM_PACKED_PS_24BIT_RGB888;
#if 1
	params->dsi.esd_check_enable = 1;
	params->dsi.customization_esd_check_enable = 1;
	params->dsi.lcm_esd_check_table[0].cmd = 0x0A;
	params->dsi.lcm_esd_check_table[0].count = 1;
	params->dsi.lcm_esd_check_table[0].para_list[0] = 0x9c;
#endif
	params->dsi.vertical_sync_active = 4;
	params->dsi.vertical_backporch = 8;
	params->dsi.vertical_frontporch = 8;
	params->dsi.vertical_active_line = FRAME_HEIGHT;

	params->dsi.horizontal_sync_active = 4;
	params->dsi.horizontal_backporch = 40; //modify by RF required,ke.huang 2018.5.29
	params->dsi.horizontal_frontporch = 24;
	params->dsi.horizontal_active_pixel = FRAME_WIDTH;
	params->dsi.cont_clock	= 0;//noncontinue mode for dsi clk

	/* video mode timing */
	params->dsi.word_count = FRAME_WIDTH * 3;

	params->dsi.PLL_CLOCK = 210;//modify by yusen.ke.sz@tcl.com at 20180717 for fix cts test
}

static void lcm_init_lcm(void)
{
	printk("[LK/LCM] lcm_init() enter\n");
	/*power on*/
	lcm_set_gpio_output(LCD_PWR_BIAS_ENP_PIN, GPIO_OUT_ONE);
	MDELAY(5);
	lcm_set_gpio_output(LCD_PWR_BIAS_ENN_PIN, GPIO_OUT_ONE);
	MDELAY(10);
/*reset */
	//lcm_set_gpio_output(GPIO_LCD_RST, GPIO_OUT_ZERO);
	//MDELAY(10);
	lcm_set_gpio_output(LCD_RST_PIN, GPIO_OUT_ONE);
	MDELAY(20);

	init_lcm_registers();
}

static void lcm_suspend(void)
{
	printk("enter rm68200 pb suspend\n");
	push_table(lcm_suspend_setting,sizeof(lcm_suspend_setting) / sizeof(struct LCM_setting_table), 1);
	/*power off*/
	lcm_set_gpio_output(LCD_RST_PIN, GPIO_OUT_ZERO);
	MDELAY(15);
	lcm_set_gpio_output(LCD_PWR_BIAS_ENN_PIN, GPIO_OUT_ZERO);
	MDELAY(1);
	lcm_set_gpio_output(LCD_PWR_BIAS_ENP_PIN, GPIO_OUT_ZERO);
	MDELAY(5);
}

static void lcm_resume(void)
{
	printk("enter rm68200 pb resume\n");
	lcm_init_lcm();
}

#if (LCM_DSI_CMD_MODE)
static void lcm_update(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	unsigned char x0_MSB = ((x0 >> 8) & 0xFF);
	unsigned char x0_LSB = (x0 & 0xFF);
	unsigned char x1_MSB = ((x1 >> 8) & 0xFF);
	unsigned char x1_LSB = (x1 & 0xFF);
	unsigned char y0_MSB = ((y0 >> 8) & 0xFF);
	unsigned char y0_LSB = (y0 & 0xFF);
	unsigned char y1_MSB = ((y1 >> 8) & 0xFF);
	unsigned char y1_LSB = (y1 & 0xFF);

	unsigned int data_array[16];

	data_array[0] = 0x00053902;
	data_array[1] = (x1_MSB << 24) | (x0_LSB << 16) | (x0_MSB << 8) | 0x2a;
	data_array[2] = (x1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x00053902;
	data_array[1] = (y1_MSB << 24) | (y0_LSB << 16) | (y0_MSB << 8) | 0x2b;
	data_array[2] = (y1_LSB);
	dsi_set_cmdq(data_array, 3, 1);

	data_array[0] = 0x002c3909;
	dsi_set_cmdq(data_array, 1, 0);
}
#endif

LCM_DRIVER rm68200_pb_dsi_lcm_drv = {
	.name = "rm68200_pb_dsi",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init_lcm,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
};
