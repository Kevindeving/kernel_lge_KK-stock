/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* LGE_CHANGE_S, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */
#include "msm.h"
#include "msm_ispif.h"
/* LGE_CHANGE_E, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */
#include "msm_sensor.h"
#define SENSOR_NAME "imx091"
#define PLATFORM_DRIVER_NAME "msm_camera_imx091"
#define imx091_obj imx091_##obj

DEFINE_MUTEX(imx091_mut);
static struct msm_sensor_ctrl_t imx091_s_ctrl;

static struct msm_camera_i2c_reg_conf imx091_start_settings[] = {
	{0x0100, 0x01},
};

static struct msm_camera_i2c_reg_conf imx091_stop_settings[] = {
	{0x0100, 0x00},
};

static struct msm_camera_i2c_reg_conf imx091_groupon_settings[] = {
	{0x0104, 0x01},
};

static struct msm_camera_i2c_reg_conf imx091_groupoff_settings[] = {
	{0x0104, 0x00},
};

static struct msm_camera_i2c_reg_conf imx091_prev_settings[] = {
	//reg_C Preview (1/2H, 1/2V), 2104x1560
	/* PLL setting */
	{0x0305, 0x04},
	{0x0307, 0x5E},
	{0x30A4, 0x02},
	{0x303C, 0x4B},
	/* mode setting */
	{0x0112, 0x0A},
	{0x0113, 0x0A},
	{0x0340, 0x06},
	{0x0341, 0x46},
	{0x0342, 0x12},
	{0x0343, 0x0C},
	{0x0344, 0x00},
	{0x0345, 0x08},
	{0x0346, 0x00},
	{0x0347, 0x30},
	{0x0348, 0x10},
	{0x0349, 0x77},
	{0x034A, 0x0C},
	{0x034B, 0x5F},
	{0x034C, 0x08},
	{0x034D, 0x38},
	{0x034E, 0x06},
	{0x034F, 0x18},
	{0x0381, 0x01},
	{0x0383, 0x03},
	{0x0385, 0x01},
	{0x0387, 0x03},
	{0x3033, 0x00},
	{0x303D, 0x10},
	{0x303E, 0xD0},
	{0x3040, 0x08},
	{0x3041, 0x97},
	{0x3048, 0x01},
	{0x304C, 0x7F},
	{0x304D, 0x04},
	{0x3064, 0x12},
	{0x309B, 0x28},
	{0x309E, 0x00},
	{0x30D5, 0x09},
	{0x30D6, 0x01},
	{0x30D7, 0x01},
	{0x30D8, 0x64},
	{0x30D9, 0x89},
	{0x30DE, 0x02},
	{0x3102, 0x10},
	{0x3103, 0x44},
	{0x3104, 0x40},
	{0x3105, 0x00},
	{0x3106, 0x0D},
	{0x3107, 0x01},
	{0x315C, 0x99},
	{0x315D, 0x98},
	{0x316E, 0x9A},
	{0x316F, 0x99},
	{0x3301, 0x03},
	{0x3304, 0x05},
	{0x3305, 0x04},
	{0x3306, 0x12},
	{0x3307, 0x03},
	{0x3308, 0x0D},
	{0x330A, 0x09},
	{0x330C, 0x08},
	{0x330D, 0x05},
	{0x330E, 0x03},
	{0x3318, 0x73},
	{0x3322, 0x02},
	{0x3342, 0x0F},
	{0x3348, 0xE0},
};

static struct msm_camera_i2c_reg_conf imx091_video_settings[] = {
// register map F, HD1080/30fps
	/* PLL setting */
	{0x0305,	0x04},
	{0x0307,	0x5E},
	{0x30A4,	0x02},
	{0x303C,	0x4B},
	/* mode setting */
	{0x0112,	0x0A},
	{0x0113,	0x0A},
	{0x0340,	0x06},
	{0x0341,	0x46},
	{0x0342,	0x12},
	{0x0343,	0x0C},
	{0x0344,	0x00},
	{0x0345,	0xA4},
	{0x0346,	0x02},
	{0x0347,	0x00},
	{0x0348,	0x0F},
	{0x0349,	0xDB},
	{0x034A,	0x0A},
	{0x034B,	0x8F},
	{0x034C,	0x07},
	{0x034D,	0x9C},
	{0x034E,	0x04},
	{0x034F,	0x48},
	{0x0381,	0x01},
	{0x0383,	0x03},
	{0x0385,	0x01},
	{0x0387,	0x03},
	{0x3033,	0x00},
	{0x303D,	0x10},
	{0x303E,	0xD0},
	{0x3040,	0x08},
	{0x3041,	0x97},
	{0x3048,	0x01},
	{0x304C,	0x7F},
	{0x304D,	0x04},
	{0x3064,	0x12},
	{0x309B,	0x28},
	{0x309E,	0x00},
	{0x30D5,	0x09},
	{0x30D6,	0x01},
	{0x30D7,	0x01},
	{0x30D8,	0x64},
	{0x30D9,	0x89},
	{0x30DE,	0x02},
	{0x3102,	0x10},
	{0x3103,	0x44},
	{0x3104,	0x40},
	{0x3105,	0x00},
	{0x3106,	0x0D},
	{0x3107,	0x01},
	{0x315C,	0x99},
	{0x315D,	0x98},
	{0x316E,	0x9A},
	{0x316F,	0x99},
	{0x3301,	0x03},
	{0x3304,	0x05},
	{0x3305,	0x04},
	{0x3306,	0x12},
	{0x3307,	0x03},
	{0x3308,	0x0D},
	{0x330A,	0x09},
	{0x330C,	0x08},
	{0x330D,	0x05},
	{0x330E,	0x03},
	{0x3318,	0x73},
	{0x3322,	0x02},
	{0x3342,	0x0F},
	{0x3348,	0xE0},
};

static struct msm_camera_i2c_reg_conf imx091_snap_settings[] = {
	/* full size */
	/* PLL setting */
	/* reg_A Still full resolution MAX fps */
	/* PLL setting */
	{0x0305,	0x04},
	{0x0307,	0x5E},
	{0x30A4,	0x02},
	{0x303C,	0x4B},
	/* mode setting */
	{0x0112,	0x0A},
	{0x0113,	0x0A},
	{0x0340,	0x0C},
	{0x0341,	0x8C},	// reg_A setting
//	{0x0341,	0x4A},	// reg_B setting
	{0x0342,	0x12},
	{0x0343,	0x0C},
	{0x0344,	0x00},
	{0x0345,	0x08},
	{0x0346,	0x00},
	{0x0347,	0x30},
	{0x0348,	0x10},
	{0x0349,	0x77},
	{0x034A,	0x0C},
	{0x034B,	0x5F},
	{0x034C,	0x10},
	{0x034D,	0x70},
	{0x034E,	0x0C},
	{0x034F,	0x30},
	{0x0381,	0x01},
	{0x0383,	0x01},
	{0x0385,	0x01},
	{0x0387,	0x01},
	{0x3033,	0x00},
	{0x303D,	0x10},
	{0x303E,	0xD0},
	{0x3040,	0x08},
	{0x3041,	0x97},
	{0x3048,	0x00},
	{0x304C,	0x7F},
	{0x304D,	0x04},
	{0x3064,	0x12},
	{0x309B,	0x20},
	{0x309E,	0x00},
	{0x30D5,	0x00},
	{0x30D6,	0x85},
	{0x30D7,	0x2A},
	{0x30D8,	0x64},
	{0x30D9,	0x89},
	{0x30DE,	0x00},
	{0x3102,	0x10},
	{0x3103,	0x44},
	{0x3104,	0x40},
	{0x3105,	0x00},
	{0x3106,	0x0D},
	{0x3107,	0x01},
	{0x315C,	0x99},
	{0x315D,	0x98},
	{0x316E,	0x9A},
	{0x316F,	0x99},
	{0x3301,	0x03},
	{0x3304,	0x05},
	{0x3305,	0x04},
	{0x3306,	0x12},
	{0x3307,	0x03},
	{0x3308,	0x0D},
	{0x330A,	0x09},
	{0x330C,	0x08},
	{0x330D,	0x05},
	{0x330E,	0x03},
	{0x3318,	0x64},
	{0x3322,	0x02},
	{0x3342,	0x0F},
	{0x3348,	0xE0},
};

static struct msm_camera_i2c_reg_conf imx091_recommend_settings[] = {
	/* global setting */
	{0x3087,	0x53},
	{0x309D,	0x94},
	{0x30A1,	0x08},
	{0x30AA,	0x04},
	{0x30B1,	0x00},
	{0x30C7,	0x00},
	{0x3115,	0x0E},
	{0x3118,	0x42},
	{0x311D,	0x34},
	{0x3121,	0x0D},
	{0x3212,	0xF2},
	{0x3213,	0x0F},
	{0x3215,	0x0F},
	{0x3217,	0x0B},
	{0x3219,	0x0B},
	{0x321B,	0x0D},
	{0x321D,	0x0D},
	/* black level setting */
	{0x3032,	0x40},
};

static struct msm_camera_i2c_reg_conf imx091_comm1_settings[] = {
// Start LGE_BSP_CAMERA::john.park@lge.com 2012-08-09 shutter lag shortening
#if 0 // for cropping EOF mode
	{0x3035,	0x10},
	{0x303B,	0x14},
	{0x3312,	0x45},
	{0x3313,	0xC0},
	{0x3310,	0x20},
	{0x3310,	0x00},
	{0x303B,	0x04},
	{0x303D,	0x00},
	{0x0100,	0x10},
	{0x3035,	0x00},
#else // for guaranteed EOF mode
	{0x303D,	0x00},
	{0x0100,	0x10},
#endif	
// End LGE_BSP_CAMERA::john.park@lge.com 2012-08-09 shutter lag shortening	
};

static struct msm_camera_i2c_reg_conf imx091_comm2_part1_settings[] = {
	{0x0340,	0x0C},
	{0x0341,	0x8C},
	{0x034C,	0x10},
	{0x034D,	0x70},
	{0x034E,	0x0C},
	{0x034F,	0x30},
	{0x0383,	0x01},
	{0x0387,	0x01},
	{0x3048,	0x00},
	{0x309B,	0x20},
	{0x30D5,	0x00},
	{0x30D6,	0x85},
	{0x30D7,	0x2A},
	{0x30DE,	0x00},
	{0x3318,	0x64},
};	

static struct msm_camera_i2c_reg_conf imx091_comm2_part2_settings[] = {
	{0x30B1, 0x43},
	{0x3311, 0x80}, 
	{0x3311, 0x00},
};

static struct msm_camera_i2c_conf_array imx091_comm_confs[] = {
	{&imx091_comm1_settings[0],
	ARRAY_SIZE(imx091_comm1_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx091_comm2_part1_settings[0],
	ARRAY_SIZE(imx091_comm2_part1_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx091_comm2_part2_settings[0],
	ARRAY_SIZE(imx091_comm2_part2_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
};

static struct v4l2_subdev_info imx091_subdev_info[] = {
	{
	.code   = V4L2_MBUS_FMT_SBGGR10_1X10,
	.colorspace = V4L2_COLORSPACE_JPEG,
	.fmt    = 1,
	.order    = 0,
	},
	/* more can be supported, to be added later */
};

static struct msm_camera_i2c_conf_array imx091_init_conf[] = {
	{&imx091_recommend_settings[0],
	ARRAY_SIZE(imx091_recommend_settings), 0, MSM_CAMERA_I2C_BYTE_DATA}
};

static struct msm_camera_i2c_conf_array imx091_confs[] = {
	{&imx091_snap_settings[0],
	ARRAY_SIZE(imx091_snap_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx091_prev_settings[0],
	ARRAY_SIZE(imx091_prev_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx091_video_settings[0],
	ARRAY_SIZE(imx091_video_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
	{&imx091_snap_settings[0],
	ARRAY_SIZE(imx091_snap_settings), 0, MSM_CAMERA_I2C_BYTE_DATA},
};	

static struct msm_sensor_output_info_t imx091_dimensions[] = {
	{
	/* full size  15fps*/
		.x_output = 0x1070, /* 4208 */
		.y_output = 0x0C30, /* 3120 */
		.line_length_pclk = 0x120C, /* 4620 */
		.frame_length_lines = 0x0C8C, /* 3212 */ // reg_A setting
//		.frame_length_lines = 0x0C4A, /* 3146 */ // reg_B setting
		.vt_pixel_clk = 225600000,
		.op_pixel_clk = 225600000,
		.binning_factor = 1,
	},
	{
	/* 30 fps 1/2 * 1/2 */
		.x_output = 0x0838, /* 2104 */
		.y_output = 0x0618, /* 1560 */
		.line_length_pclk = 0x120C, /* 4620 */
		.frame_length_lines = 0x0646, //1606,
		.vt_pixel_clk = 225600000,
		.op_pixel_clk = 225600000,
		.binning_factor = 1,
	},
	{
	/* 30 fps video */
		.x_output = 0x079C, /* 1948 */
		.y_output = 0x0448, /* 1096 */
		.line_length_pclk = 0x120C, /* 4620 */
		.frame_length_lines = 0x0646, //1606,
		.vt_pixel_clk = 112800000,
		.op_pixel_clk = 112800000,
		.binning_factor = 1,
	},
	{
	/* full size  15fps*/
		.x_output = 0x1070, /* 4208 */
		.y_output = 0x0C30, /* 3120 */
		.line_length_pclk = 0x120C, /* 4620 */
		.frame_length_lines = 0x0C8C, /* 3212 */ // reg_A setting
//		.frame_length_lines = 0x0C4A, /* 3146 */ // reg_B setting
		.vt_pixel_clk = 225600000,
		.op_pixel_clk = 225600000,
		.binning_factor = 1,
	},
};

#if 0 // Move to Userspace
static struct msm_camera_csid_vc_cfg imx091_cid_cfg[] = {
	{0, CSI_RAW10, CSI_DECODE_10BIT},
	{1, CSI_EMBED_DATA, CSI_DECODE_8BIT},
	{2, CSI_RESERVED_DATA_0, CSI_DECODE_8BIT},
};

static struct msm_camera_csi2_params imx091_csi_params = {
	.csid_params = {
		.lane_cnt = 4,
		.lut_params = {
			.num_cid = ARRAY_SIZE(imx091_cid_cfg),
			.vc_cfg = imx091_cid_cfg,
		},
	},
	.csiphy_params = {
		.lane_cnt = 4,
		.settle_cnt = 0x12,
	},
};

static struct msm_camera_csi2_params *imx091_csi_params_array[] = {
	&imx091_csi_params,
	&imx091_csi_params,
	&imx091_csi_params,
	&imx091_csi_params,
};
#endif

static struct msm_sensor_output_reg_addr_t imx091_reg_addr = {
	.x_output = 0x034C,
	.y_output = 0x034E,
	.line_length_pclk = 0x0342,
	.frame_length_lines = 0x0340,
};

/* LGE_CHANGE_S, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */

static struct msm_sensor_id_info_t imx091_id_info = {
	.sensor_id_reg_addr = 0x0000,
	.sensor_id = 0x0091,
};

static enum msm_camera_vreg_name_t imx091_veg_seq[] = {
	CAM_VANA,
	CAM_VDIG,
	CAM_VIO,
	CAM_VAF,
};

static struct msm_camera_power_seq_t imx091_power_seq[] = {
	{REQUEST_GPIO, 0},
	{REQUEST_VREG, 0},
	{ENABLE_VREG, 0},
	{ENABLE_GPIO, 0},
	{CONFIG_CLK, 1},
	{CONFIG_I2C_MUX, 0},
};
/* LGE_CHANGE_E, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */

static struct msm_sensor_exp_gain_info_t imx091_exp_gain_info = {
	.coarse_int_time_addr = 0x0202,
	.global_gain_addr = 0x0204,
	.vert_offset = 5,
};

static const struct i2c_device_id imx091_i2c_id[] = {
	{SENSOR_NAME, (kernel_ulong_t)&imx091_s_ctrl},
	{ }
};

static struct i2c_driver imx091_i2c_driver = {
	.id_table = imx091_i2c_id,
	.probe  = msm_sensor_i2c_probe,
	.driver = {
		.name = SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client imx091_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};


/* LGE_CHANGE_S, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */
static int __init msm_sensor_init_module(void)
{
	pr_err("__jrchoi: %s: E\n", __func__);
	return i2c_add_driver(&imx091_i2c_driver);
}
/* LGE_CHANGE_E, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */

static struct v4l2_subdev_core_ops imx091_subdev_core_ops = {
	.ioctl = msm_sensor_subdev_ioctl,
	.s_power = msm_sensor_power,
};

static struct v4l2_subdev_video_ops imx091_subdev_video_ops = {
	.enum_mbus_fmt = msm_sensor_v4l2_enum_fmt,
};

static struct v4l2_subdev_ops imx091_subdev_ops = {
	.core = &imx091_subdev_core_ops,
	.video  = &imx091_subdev_video_ops,
};

/* LGE_CHANGE_S, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */
int32_t imx091_sensor_setting(struct msm_sensor_ctrl_t *s_ctrl,
			int update_type, int res)
{
	int32_t rc = 0;

//	s_ctrl->func_tbl->sensor_stop_stream(s_ctrl);
//	msleep(30);

	pr_err("[rafal47] %s: update: %d, res : %d : Change camera mode\n", __func__, update_type, res);
	if (update_type == MSM_SENSOR_REG_INIT) {
//		s_ctrl->curr_csi_params = NULL;
		msm_sensor_enable_debugfs(s_ctrl);
		msm_sensor_write_init_settings(s_ctrl);
	} else if (update_type == MSM_SENSOR_UPDATE_PERIODIC) {
		if (res == 0) {
			msm_camera_i2c_write_tbl(s_ctrl->sensor_i2c_client,
				(struct msm_camera_i2c_reg_conf *)
				imx091_comm_confs[0].conf,
				imx091_comm_confs[0].size,
				imx091_comm_confs[0].data_type);
		} else {
			msm_sensor_write_res_settings(s_ctrl, res);
/*			if (s_ctrl->curr_csi_params != s_ctrl->csi_params[res]) {
				s_ctrl->curr_csi_params = s_ctrl->csi_params[res];
				s_ctrl->curr_csi_params->csid_params.lane_assign =
					s_ctrl->sensordata->sensor_platform_info->
					csi_lane_params->csi_lane_assign;
				s_ctrl->curr_csi_params->csiphy_params.lane_mask =
					s_ctrl->sensordata->sensor_platform_info->
					csi_lane_params->csi_lane_mask;
				v4l2_subdev_notify(&s_ctrl->sensor_v4l2_subdev,
						NOTIFY_CSID_CFG,
						&s_ctrl->curr_csi_params->csid_params);
				mb();
				v4l2_subdev_notify(&s_ctrl->sensor_v4l2_subdev,
						NOTIFY_CSIPHY_CFG,
						&s_ctrl->curr_csi_params->csiphy_params);
				mb();
				msleep(20);
			}
*/
			v4l2_subdev_notify(&s_ctrl->sensor_v4l2_subdev,
				NOTIFY_PCLK_CHANGE, &s_ctrl->msm_sensor_reg->
				output_settings[res].op_pixel_clk);
//			s_ctrl->func_tbl->sensor_start_stream(s_ctrl);
//			msleep(30);
		}
	}
	pr_err("%s: X", __func__);
	return rc;
}

int32_t imx091_sensor_set_fps(struct msm_sensor_ctrl_t *s_ctrl,
						struct fps_cfg *fps)
{
	uint16_t total_lines_per_frame;
	int32_t rc = 0;
	s_ctrl->fps_divider = fps->fps_div;

	if (s_ctrl->curr_res != MSM_SENSOR_INVALID_RES) {
		uint16_t fl_read = 0;
		total_lines_per_frame = (uint16_t)
			((s_ctrl->curr_frame_length_lines) *
			s_ctrl->fps_divider/Q10);

		rc = msm_camera_i2c_read(s_ctrl->sensor_i2c_client,
			s_ctrl->sensor_output_reg_addr->frame_length_lines,
			&fl_read, MSM_CAMERA_I2C_WORD_DATA);

		CDBG("%s: before_fl = %d, new_fl = %d", __func__, fl_read, total_lines_per_frame);

		if(fl_read < total_lines_per_frame) {
			pr_err("%s: Write new_fl (before_fl = %d, new_fl = %d)", __func__, fl_read, total_lines_per_frame);
			rc = msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
				s_ctrl->sensor_output_reg_addr->frame_length_lines,
				total_lines_per_frame, MSM_CAMERA_I2C_WORD_DATA);
		}
	}
	return rc;
}

int32_t imx091_sensor_write_exp_gain1(struct msm_sensor_ctrl_t *s_ctrl,
		uint16_t gain, uint32_t line, int32_t luma_avg, uint16_t fgain)
{
	uint32_t fl_lines;
	uint8_t offset;
	fl_lines = s_ctrl->curr_frame_length_lines;
	fl_lines = (fl_lines * s_ctrl->fps_divider) / Q10;
	offset = s_ctrl->sensor_exp_gain_info->vert_offset;
	if (line > (fl_lines - offset))
		fl_lines = line + offset;

	CDBG("\n%s:Gain:%d, Linecount:%d\n", __func__, gain, line);
	if (s_ctrl->curr_res == 0) {
		msm_camera_i2c_write_tbl(s_ctrl->sensor_i2c_client,
			(struct msm_camera_i2c_reg_conf *)
			imx091_comm_confs[1].conf,
			imx091_comm_confs[1].size,
			imx091_comm_confs[1].data_type);

		msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
			s_ctrl->sensor_output_reg_addr->frame_length_lines,
			fl_lines, MSM_CAMERA_I2C_WORD_DATA);
		msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
			s_ctrl->sensor_exp_gain_info->coarse_int_time_addr,
			line, MSM_CAMERA_I2C_WORD_DATA);
		msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
			s_ctrl->sensor_exp_gain_info->global_gain_addr, gain,
			MSM_CAMERA_I2C_WORD_DATA);

		msm_camera_i2c_write_tbl(s_ctrl->sensor_i2c_client,
			(struct msm_camera_i2c_reg_conf *)
			imx091_comm_confs[2].conf,
			imx091_comm_confs[2].size,
			imx091_comm_confs[2].data_type);

/*		if (s_ctrl->curr_csi_params !=
			s_ctrl->csi_params[s_ctrl->curr_res]) {
			s_ctrl->curr_csi_params =
				s_ctrl->csi_params[s_ctrl->curr_res];
			v4l2_subdev_notify(&s_ctrl->sensor_v4l2_subdev,
					NOTIFY_CSID_CFG,
					&s_ctrl->curr_csi_params->csid_params);
			mb();
			v4l2_subdev_notify(&s_ctrl->sensor_v4l2_subdev,
					NOTIFY_CSIPHY_CFG,
					&s_ctrl->curr_csi_params->csiphy_params);
			mb();
			msleep(20);
		}
*/
		v4l2_subdev_notify(&s_ctrl->sensor_v4l2_subdev,
			NOTIFY_PCLK_CHANGE, &s_ctrl->msm_sensor_reg->
			output_settings[s_ctrl->curr_res].op_pixel_clk);
		//s_ctrl->func_tbl->sensor_start_stream(s_ctrl);
	} else {
		s_ctrl->func_tbl->sensor_group_hold_on(s_ctrl);
		msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
			s_ctrl->sensor_output_reg_addr->frame_length_lines,
			fl_lines, MSM_CAMERA_I2C_WORD_DATA);
		msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
			s_ctrl->sensor_exp_gain_info->coarse_int_time_addr,
			line, MSM_CAMERA_I2C_WORD_DATA);
		msm_camera_i2c_write(s_ctrl->sensor_i2c_client,
			s_ctrl->sensor_exp_gain_info->global_gain_addr, gain,
			MSM_CAMERA_I2C_WORD_DATA);
		s_ctrl->func_tbl->sensor_group_hold_off(s_ctrl);
	}

	return 0;
}
/* LGE_CHANGE_E, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */

static struct msm_sensor_fn_t imx091_func_tbl = {
	.sensor_start_stream = msm_sensor_start_stream,
	.sensor_stop_stream = msm_sensor_stop_stream,
	.sensor_group_hold_on = msm_sensor_group_hold_on,
	.sensor_group_hold_off = msm_sensor_group_hold_off,
/* LGE_CHANGE_S, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */
#if 0  // QCT
	.sensor_set_fps = msm_sensor_set_fps,
	.sensor_write_exp_gain = msm_sensor_write_exp_gain1,
	.sensor_write_snapshot_exp_gain = msm_sensor_write_exp_gain1,
	.sensor_setting = msm_sensor_setting,
#else
	.sensor_set_fps = imx091_sensor_set_fps,
	.sensor_write_exp_gain = imx091_sensor_write_exp_gain1,
	.sensor_write_snapshot_exp_gain = imx091_sensor_write_exp_gain1,
	.sensor_setting = imx091_sensor_setting,
#endif
/* LGE_CHANGE_E, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */
	.sensor_set_sensor_mode = msm_sensor_set_sensor_mode,
	.sensor_mode_init = msm_sensor_mode_init,
	.sensor_get_output_info = msm_sensor_get_output_info,
	.sensor_config = msm_sensor_config,
	.sensor_power_up = msm_sensor_power_up,
	.sensor_power_down = msm_sensor_power_down,
/* LGE_CHANGE_S, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */
#if 0 // QCT
	.sensor_adjust_frame_lines = msm_sensor_adjust_frame_lines1,
#else
	//.sensor_adjust_frame_lines = msm_sensor_adjust_frame_lines,
#endif
/* LGE_CHANGE_E, change for shutter lag shortening, 2012.06.29, kyounghoon.noh@lge.com */
	.sensor_get_csi_params = msm_sensor_get_csi_params,
};

static struct msm_sensor_reg_t imx091_regs = {
	.default_data_type = MSM_CAMERA_I2C_BYTE_DATA,
	.start_stream_conf = imx091_start_settings,
	.start_stream_conf_size = ARRAY_SIZE(imx091_start_settings),
	.stop_stream_conf = imx091_stop_settings,
	.stop_stream_conf_size = ARRAY_SIZE(imx091_stop_settings),
	.group_hold_on_conf = imx091_groupon_settings,
	.group_hold_on_conf_size = ARRAY_SIZE(imx091_groupon_settings),
	.group_hold_off_conf = imx091_groupoff_settings,
	.group_hold_off_conf_size = ARRAY_SIZE(imx091_groupoff_settings),
	.init_settings = &imx091_init_conf[0],
	.init_size = ARRAY_SIZE(imx091_init_conf),
	.mode_settings = &imx091_confs[0],
	.output_settings = &imx091_dimensions[0],
	.num_conf = ARRAY_SIZE(imx091_confs),
};

static struct msm_sensor_ctrl_t imx091_s_ctrl = {
	.msm_sensor_reg = &imx091_regs,
	.sensor_i2c_client = &imx091_sensor_i2c_client,
	.sensor_i2c_addr = 0x34,
	.vreg_seq = imx091_veg_seq,
	.num_vreg_seq = ARRAY_SIZE(imx091_veg_seq),
	.sensor_output_reg_addr = &imx091_reg_addr,
/* LGE_CHANGE_S, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */
	.power_seq = &imx091_power_seq[0],
	.num_power_seq = ARRAY_SIZE(imx091_power_seq),
/* LGE_CHANGE_E, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */
	.sensor_id_info = &imx091_id_info,
	.sensor_exp_gain_info = &imx091_exp_gain_info,
	.cam_mode = MSM_SENSOR_MODE_INVALID,
//	.csi_params = &imx091_csi_params_array[0],
	.msm_sensor_mutex = &imx091_mut,
	.sensor_i2c_driver = &imx091_i2c_driver,
	.sensor_v4l2_subdev_info = imx091_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(imx091_subdev_info),
	.sensor_v4l2_subdev_ops = &imx091_subdev_ops,
	.func_tbl = &imx091_func_tbl,
//	.clk_rate = MSM_SENSOR_MCLK_24HZ,
};

/* LGE_CHANGE_S, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */
module_init(msm_sensor_init_module);
/* LGE_CHANGE_E, Code refining for Jelly bean from G-MR ver. , 2012.11.03 youngwook.song */
MODULE_DESCRIPTION("Sony 13MP Bayer sensor driver");
MODULE_LICENSE("GPL v2");
