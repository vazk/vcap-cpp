/*
 * Vcap C++ Bindings
 * 
 * Copyright (C) 2014 James McLean
 * 
 * This library is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _VCAP_FORMATS_HPP
#define _VCAP_FORMATS_HPP

/**
 * \file
 * List of known pixel format (FOURCC) character codes.
 */
#include <Vcap/Vcap.hpp> 

extern "C" {
#include <vcap/vcap_formats.h>
}

namespace Vcap {
	/* RGB formats */
	const uint FMT_RGB332		= VCAP_FMT_RGB332;			//  8  RGB-3-3-2 
	const uint FMT_RGB444		= VCAP_FMT_RGB444;			// 16  xxxxrrrr ggggbbbb
	const uint FMT_RGB555		= VCAP_FMT_RGB555;			// 16  RGB-5-5-5
	const uint FMT_RGB565		= VCAP_FMT_RGB565;			// 16  RGB-5-6-5
	const uint FMT_RGB555X		= VCAP_FMT_RGB555X;			// 16  RGB-5-5-5 BE
	const uint FMT_RGB565X		= VCAP_FMT_RGB565X;			// 16  RGB-5-6-5 BE
	const uint FMT_BGR666		= VCAP_FMT_BGR666;			// 18  BGR-6-6-6
	const uint FMT_BGR24		= VCAP_FMT_BGR24;			// 24  BGR-8-8-8
	const uint FMT_RGB24		= VCAP_FMT_RGB24;			// 24  RGB-8-8-8
	const uint FMT_BGR32		= VCAP_FMT_BGR32;			// 32  BGR-8-8-8-8
	const uint FMT_RGB32		= VCAP_FMT_RGB32;			// 32  RGB-8-8-8-8
	
	/* Grey formats */
	const uint FMT_GREY			= VCAP_FMT_GREY;			//  8  Greyscale
	const uint FMT_Y4			= VCAP_FMT_Y4;				//  4  Greyscale
	const uint FMT_Y6			= VCAP_FMT_Y6;				//  6  Greyscale
	const uint FMT_Y10			= VCAP_FMT_Y10;				// 10  Greyscale
	const uint FMT_Y12			= VCAP_FMT_Y12;				// 12  Greyscale
	const uint FMT_Y16			= VCAP_FMT_Y16;				// 16  Greyscale
	
	/* Grey bit-packed formats */
	const uint FMT_Y10BPACK		= VCAP_FMT_Y16;				// 10  Greyscale bit-packed
	
	/* Palette formats */
	const uint FMT_PAL8			= VCAP_FMT_PAL8;			//  8  8-bit palette
	
	/* Chrominance formats */
	const uint FMT_UV8			= VCAP_FMT_UV8;				//  8  UV 4:4
	
	/* Luminance+Chrominance formats */
	const uint FMT_YVU410 		= VCAP_FMT_YVU410;			//  9  YVU 4:1:0
	const uint FMT_YVU420 		= VCAP_FMT_YVU420;			// 12  YVU 4:2:0
	const uint FMT_YUYV			= VCAP_FMT_YUYV;			// 16  YUV 4:2:2
	const uint FMT_YYUV			= VCAP_FMT_YYUV;			// 16  YUV 4:2:2
	const uint FMT_YVYU			= VCAP_FMT_YVYU;			// 16  YVU 4:2:2
	const uint FMT_UYVY    		= VCAP_FMT_UYVY;			// 16  YUV 4:2:2
	const uint FMT_VYUY			= VCAP_FMT_VYUY;			// 16  YUV 4:2:2
	const uint FMT_YUV422P		= VCAP_FMT_YUV422P;			// 16  YVU422 planar
	const uint FMT_YUV411P		= VCAP_FMT_YUV411P;			// 16  YVU411 planar
	const uint FMT_Y41P			= VCAP_FMT_Y41P;			// 12  YUV 4:1:1
	const uint FMT_YUV444		= VCAP_FMT_YUV444;			// 16  xxxxyyyy uuuuvvvv
	const uint FMT_YUV555		= VCAP_FMT_YUV555;			// 16  YUV-5-5-5
	const uint FMT_YUV565		= VCAP_FMT_YUV565;			// 16  YUV-5-6-5
	const uint FMT_YUV32		= VCAP_FMT_YUV32;			// 32  YUV-8-8-8-8
	const uint FMT_YUV410		= VCAP_FMT_YUV410;			//  9  YUV 4:1:0
	const uint FMT_YUV420		= VCAP_FMT_YUV420;			// 12  YUV 4:2:0
	const uint FMT_HI240		= VCAP_FMT_HI240;			//  8  8-bit color
	const uint FMT_HM12			= VCAP_FMT_HM12;			//  8  YUV 4:2:0 16x16 macroblocks
	const uint FMT_M420			= VCAP_FMT_M420;			// 12  YUV 4:2:0 2 lines y, 1 line uv interleaved
	
	/* two planes -- one Y, one Cr + Cb interleaved  */
	const uint FMT_NV12			= VCAP_FMT_NV12;			// 12  Y/CbCr 4:2:0
	const uint FMT_NV21			= VCAP_FMT_NV21;			// 12  Y/CrCb 4:2:0
	const uint FMT_NV16			= VCAP_FMT_NV16;			// 16  Y/CbCr 4:2:2
	const uint FMT_NV61			= VCAP_FMT_NV61;			// 16  Y/CrCb 4:2:2
	const uint FMT_NV24			= VCAP_FMT_NV24;			// 24  Y/CbCr 4:4:4
	const uint FMT_NV42			= VCAP_FMT_NV42;			// 24  Y/CrCb 4:4:4
	
	/* two non contiguous planes - one Y, one Cr + Cb interleaved  */
	const uint FMT_NV12M		= VCAP_FMT_NV12M;			// 12  Y/CbCr 4:2:0
	const uint FMT_NV21M		= VCAP_FMT_NV21M;			// 21  Y/CrCb 4:2:0
	const uint FMT_NV16M		= VCAP_FMT_NV16M;			// 16  Y/CbCr 4:2:2
	const uint FMT_NV61M		= VCAP_FMT_NV61M;			// 16  Y/CrCb 4:2:2
	const uint FMT_NV12MT		= VCAP_FMT_NV12MT;			// 12  Y/CbCr 4:2:0 64x32 macroblocks
	const uint FMT_NV12MT_16X16	= VCAP_FMT_NV12MT_16X16;	// 12  Y/CbCr 4:2:0 16x16 macroblocks
	
	/* three non contiguous planes - Y, Cb, Cr */
	const uint FMT_YUV420M		= VCAP_FMT_YUV420M;			// 12  YUV420 planar
	const uint FMT_YVU420M		= VCAP_FMT_YVU420M;			// 12  YVU420 planar
	
	/* Bayer formats - see http://www.siliconimaging.com/RGB%20Bayer.htm */
	const uint FMT_SBGGR8		= VCAP_FMT_SBGGR8;			//  8  BGBG.. GRGR..
	const uint FMT_SGBRG8		= VCAP_FMT_SGBRG8;			//  8  GBGB.. RGRG..
	const uint FMT_SGRBG8		= VCAP_FMT_SGRBG8;			//  8  GRGR.. BGBG..
	const uint FMT_SRGGB8		= VCAP_FMT_SRGGB8;			//  8  RGRG.. GBGB..
	const uint FMT_SBGGR10		= VCAP_FMT_SBGGR10;			// 10  BGBG.. GRGR..
	const uint FMT_SGBRG10		= VCAP_FMT_SGBRG10;			// 10  GBGB.. RGRG..
	const uint FMT_SGRBG10		= VCAP_FMT_SGRBG10;			// 10  GRGR.. BGBG..
	const uint FMT_SRGGB10		= VCAP_FMT_SRGGB10; 		// 10  RGRG.. GBGB..
	const uint FMT_SBGGR12		= VCAP_FMT_SBGGR12;			// 12  BGBG.. GRGR..
	const uint FMT_SGBRG12		= VCAP_FMT_SGBRG12;			// 12  GBGB.. RGRG..
	const uint FMT_SGRBG12		= VCAP_FMT_SGRBG12;			// 12  GRGR.. BGBG..
	const uint FMT_SRGGB12		= VCAP_FMT_SRGGB12;			// 12  RGRG.. GBGB..
		
	/* 10bit raw bayer a-law compressed to 8 bits */
	const uint FMT_SBGGR10ALAW8 = VCAP_FMT_SBGGR10ALAW8;
	const uint FMT_SGBRG10ALAW8 = VCAP_FMT_SGBRG10ALAW8;
	const uint FMT_SGRBG10ALAW8 = VCAP_FMT_SGRBG10ALAW8;
	const uint FMT_SRGGB10ALAW8 = VCAP_FMT_SRGGB10ALAW8;
	
	/* 10bit raw bayer DPCM compressed to 8 bits */
	const uint FMT_SBGGR10DPCM8 = VCAP_FMT_SBGGR10DPCM8;
	const uint FMT_SGBRG10DPCM8 = VCAP_FMT_SGBRG10DPCM8;
	const uint FMT_SGRBG10DPCM8 = VCAP_FMT_SGRBG10DPCM8;
	const uint FMT_SRGGB10DPCM8 = VCAP_FMT_SRGGB10DPCM8;
	
	/*
	 * 10bit raw bayer, expanded to 16 bits
	 * xxxxrrrrrrrrrrxxxxgggggggggg xxxxggggggggggxxxxbbbbbbbbbb...
	 */
	const uint FMT_SBGGR16		= VCAP_FMT_SBGGR16;			// 16  BGBG.. GRGR..
	
	/* Compressed formats */
	const uint FMT_MJPEG		= VCAP_FMT_MJPEG;			// Motion-JPEG   */
	const uint FMT_JPEG			= VCAP_FMT_JPEG;			// JFIF JPEG     */
	const uint FMT_DV			= VCAP_FMT_DV;				// 1394          */
	const uint FMT_MPEG			= VCAP_FMT_MPEG;			// MPEG-1/2/4 Multiplexed */
	const uint FMT_H264			= VCAP_FMT_H264;			// H264 with start codes */
	const uint FMT_H264_NO_SC	= VCAP_FMT_H264_NO_SC;		// H264 without start codes */
	const uint FMT_H264_MVC		= VCAP_FMT_H264_MVC;		// H264 MVC */
	const uint FMT_H263			= VCAP_FMT_H263;			// H263          */
	const uint FMT_MPEG1		= VCAP_FMT_MPEG1;			// MPEG-1 ES     */
	const uint FMT_MPEG2		= VCAP_FMT_MPEG2;			// MPEG-2 ES     */
	const uint FMT_MPEG4		= VCAP_FMT_MPEG4;			// MPEG-4 part 2 ES */
	const uint FMT_XVID			= VCAP_FMT_XVID;			// Xvid           */
	const uint FMT_VC1_ANNEX_G	= VCAP_FMT_VC1_ANNEX_G;		// SMPTE 421M Annex G compliant stream */
	const uint FMT_VC1_ANNEX_L	= VCAP_FMT_VC1_ANNEX_L;		// SMPTE 421M Annex L compliant stream */
	const uint FMT_VP8			= VCAP_FMT_VP8;				// VP8 */
	
	/*  Vendor-specific formats   */
	const uint FMT_CPIA1		= VCAP_FMT_CPIA1;			// cpia1 YUV */
	const uint FMT_WNVA			= VCAP_FMT_WNVA;			// Winnov hw compress */
	const uint FMT_SN9C10X		= VCAP_FMT_SN9C10X;			// SN9C10x compression */
	const uint FMT_SN9C20X_I420 = VCAP_FMT_SN9C20X_I420;	// SN9C20x YUV 4:2:0 */
	const uint FMT_PWC1			= VCAP_FMT_PWC1;			// pwc older webcam */
	const uint FMT_PWC2			= VCAP_FMT_PWC2;			// pwc newer webcam */
	const uint FMT_ET61X251		= VCAP_FMT_ET61X251;		// ET61X251 compression */
	const uint FMT_SPCA501		= VCAP_FMT_SPCA501;			// YUYV per line */
	const uint FMT_SPCA505		= VCAP_FMT_SPCA505;			// YYUV per line */
	const uint FMT_SPCA508		= VCAP_FMT_SPCA508;			// YUVY per line */
	const uint FMT_SPCA561		= VCAP_FMT_SPCA561;			// compressed GBRG bayer */
	const uint FMT_PAC207		= VCAP_FMT_PAC207;			// compressed BGGR bayer */
	const uint FMT_MR97310A		= VCAP_FMT_MR97310A;		// compressed BGGR bayer */
	const uint FMT_JL2005BCD	= VCAP_FMT_JL2005BCD;		// compressed RGGB bayer */
	const uint FMT_SN9C2028		= VCAP_FMT_SN9C2028;		// compressed GBRG bayer */
	const uint FMT_SQ905C		= VCAP_FMT_SQ905C;			// compressed RGGB bayer */
	const uint FMT_PJPG			= VCAP_FMT_PJPG;			// Pixart 73xx JPEG */
	const uint FMT_OV511		= VCAP_FMT_OV511;			// ov511 JPEG */
	const uint FMT_OV518		= VCAP_FMT_OV518;			// ov518 JPEG */
	const uint FMT_STV0680		= VCAP_FMT_STV0680;			// stv0680 bayer */
	const uint FMT_TM6000		= VCAP_FMT_TM6000; 			// tm5600/tm60x0 */
	const uint FMT_CIT_YYVYUY	= VCAP_FMT_CIT_YYVYUY;		// one line of Y then 1 line of VYUY */
	const uint FMT_KONICA420	= VCAP_FMT_KONICA420;		// YUV420 planar in blocks of 256 pixels */
	const uint FMT_JPGL			= VCAP_FMT_JPGL;			// JPEG-Lite */
	const uint FMT_SE401		= VCAP_FMT_SE401;			// se401 janggu compressed rgb */
	const uint FMT_S5C_UYVY_JPG = VCAP_FMT_S5C_UYVY_JPG;	// S5C73M3 interleaved UYVY/JPEG */
}

#endif
