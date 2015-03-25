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

#ifndef _VCAP_HPP
#define _VCAP_HPP

/**
 * \file
 * Main header.
 */

#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

extern "C" {
#include <vcap/vcap.h>
}

#include <Vcap/Controls.hpp>
#include <Vcap/Formats.hpp>
#include <Vcap/SmartPtr.hpp>

namespace Vcap {
	/*
	 * Forward declarations
	 */
	class RuntimeError;
	class Size;
	class Format;
	class MenuItem;
	class Control;
	class Camera;
	
	/**
	 * \brief Size smart pointer.
	 */
	typedef SmartPtr<Size> SizePtr;
	
	/**
	 * \brief Format smart pointer.
	 */
	typedef SmartPtr<Format> FormatPtr;
	
	/**
	 * \brief Menu item smart pointer..
	 */
	typedef SmartPtr<MenuItem> MenuItemPtr;
	
	/**
	 * \brief Menu item smart pointer..
	 */
	typedef SmartPtr<Control> ControlPtr;
	
	/**
	 * \brief Camera smart pointer.
	 */
	typedef SmartPtr<Camera> CameraPtr;
}

/**
 * \brief Encapsulates a Vcap runtime error.
 */
class Vcap::RuntimeError : public std::runtime_error {
	public:
		RuntimeError(const std::string& msg) : std::runtime_error(msg) { }
};

/**
 * \brief Encapsulates a frame size.
 */
class Vcap::Size {
	friend class Format;
	
	public:
		std::uint32_t width();
		std::uint32_t height();
	
	private:
		Size(vcap_size_t size);
	
		vcap_size_t _size;
};

/**
 * \brief Encapsulates pixel format information.
 */
class Vcap::Format {
	friend class Camera;
	
	public:
		virtual ~Format();
	
		std::uint32_t code();
		std::string codeString();
		std::string description();
		
		std::vector<SizePtr> sizes();
		
	private:
		Format(vcap_format_t* format);
	
		vcap_format_t* _format;
};

/**
 * \brief Encapsulates a camera control menu item.
 */
class Vcap::MenuItem {
	friend class Control;

	public:
		virtual ~MenuItem();
	
		std::string name();
		std::uint32_t value();
		
	private:
		MenuItem(vcap_menu_item_t* menuItem);
	
		vcap_menu_item_t* _menuItem;
};

/**
 * \brief Encapsulates a camera control.
 */
class Vcap::Control {
	friend class Camera;

	public:
		virtual ~Control();
	
		ControlId id();
		ControlType type();
		
		std::string name();
		
		std::int32_t min();
		std::int32_t max();
		std::int32_t step();
		std::int32_t default_value();
	
		std::vector<MenuItemPtr> menu();
	
	private:
		Control(vcap_control_t* control);
	
		vcap_control_t* _control;
};

/**
 * \brief Encapsulates an image capture device.
 */
class Vcap::Camera {
	public:
		static std::vector<CameraPtr> cameras() throw (RuntimeError);
	
		Camera(const std::string& device);
		virtual ~Camera();
		
		/**
		 * \brief Returns the camera's device path.
		 */
		std::string device();
		
		/**
		 * \brief Returns the camera's internal driver.
		 */
		std::string driver();
		
		/**
		 * \brief Returns the camera's name and bus info.
		 */
		std::string info();
		
		/**
		 * \brief Opens the underlying device.
		 */
		void open() throw (RuntimeError);
		
		/**
		 * \brief Closes the underlying device.
		 */
		void close() throw (RuntimeError);
		
		/**
		 * \brief Returns all formats supported by this device.
		 */
		std::vector<FormatPtr> formats() throw (RuntimeError);
		
		/**
		 * \brief Returns the current format as a tuple.
		 */
		std::tuple<std::uint32_t, std::uint32_t, std::uint32_t> format() throw (RuntimeError);
		
		/**
		 * \brief Sets the current format.
		 */
		void setFormat(std::uint32_t formatCode, std::uint32_t width, std::uint32_t height) throw (RuntimeError);
		
		/**
		 * \brief Automatically sets the camera's format based upon decoder priority.
		 */
		void autoSetFormat() throw (RuntimeError);
		
		/**
		 * \brief Returns all supported frame rates for a given format and frame size.
		 */
		std::vector<std::uint16_t> frameRates(std::uint32_t formatCode, std::uint32_t width, std::uint32_t height) throw (RuntimeError);
		
		/**
		 * \brief Returns the current frame rate.
		 */
		std::uint16_t frameRate() throw (RuntimeError);
		
		/**
		 * \brief Sets the current frame rate.
		 */
		void setFrameRate(std::uint16_t frameRate) throw (RuntimeError);
		
		/**
		 * \brief Returns all supported controls.
		 */
		std::vector<ControlPtr> controls() throw (RuntimeError);
		
		/**
		 * \brief Returns the current value for the specified control.
		 */
		std::int32_t controlValue(ControlId id) throw (RuntimeError);
		
		/**
		 * \brief Sets the current value for the specified control.
		 */
		void setControlValue(ControlId id, std::int32_t value) throw (RuntimeError);
		
		/**
		 * \brief Starts streaming.
		 */
		void start() throw (RuntimeError);
		
		/**
		 * \brief Stops streaming.
		 */
		void stop() throw (RuntimeError);
		
		/**
		 * \brief Allocates a buffer, grabs an image from the camera (optionally decodes it), and stores it in the buffer.
		 */
		std::size_t grab(std::uint8_t** buffer, bool decode = false, bool bgr = false) throw (RuntimeError);
		
	private:
		Camera(vcap_camera_t* camera);
	
		vcap_camera_t* _camera;
};

#endif
