/*
 * Copyright (C) 2017 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

#ifndef __IMGSENSOR_PROC_H__
#define __IMGSENSOR_PROC_H__

#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "imgsensor_common.h"

#define PROC_CAMERA_INFO "driver/camera_info"

#define camera_info_size 4096

extern char mtk_ccm_name[camera_info_size];
extern struct IMGSENSOR *pgimgsensor;

#define  OTP_INFO_CONTROL
#ifdef OTP_INFO_CONTROL
#define PROC_OTP_INFO "driver/otp_info"
extern char mtk_main_otp_info[camera_info_size];
extern char mtk_sub_otp_info[camera_info_size];
#endif


enum IMGSENSOR_RETURN imgsensor_proc_init(void);
#endif

