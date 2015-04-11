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

#ifndef _VCAP_CONTROLS_HPP
#define _VCAP_CONTROLS_HPP

/**
 * \file
 * Defines control IDs and control types.
 */

namespace Vcap {
	typedef enum {
		CTRL_BRIGHTNESS,
		CTRL_CONTRAST,
		CTRL_SATURATION,
		CTRL_HUE,
		CTRL_AUTO_WHITE_BALANCE,
		CTRL_DO_WHITE_BALANCE,
		CTRL_RED_BALANCE,
		CTRL_BLUE_BALANCE,
		CTRL_GAMMA,
		CTRL_EXPOSURE,
		CTRL_AUTOGAIN,
		CTRL_GAIN,
		CTRL_HFLIP,
		CTRL_VFLIP,
		CTRL_EXPOSURE_AUTO,
		CTRL_EXPOSURE_ABSOLUTE,
		CTRL_EXPOSURE_AUTO_PRIORITY,
		CTRL_FOCUS_ABSOLUTE,
		CTRL_FOCUS_RELATIVE,
		CTRL_FOCUS_AUTO,
		CTRL_ZOOM_ABSOLUTE,
		CTRL_ZOOM_RELATIVE,
		CTRL_WHITE_BALANCE_TEMPERATURE,
		CTRL_INVALID
	} ControlId;
	
	typedef enum {
		CTRL_TYPE_RANGE,
		CTRL_TYPE_BOOLEAN,
		CTRL_TYPE_MENU,
		CTRL_TYPE_BUTTON,
		CTRL_TYPE_INVALID
	} ControlType;
}

#endif
