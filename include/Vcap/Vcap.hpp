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
	class FormatInfo;
	class MenuItem;
	class ControlInfo;
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
	 * \brief Format smart pointer.
	 */
	typedef SmartPtr<FormatInfo> FormatInfoPtr;
	
	/**
	 * \brief Menu item smart pointer..
	 */
	typedef SmartPtr<MenuItem> MenuItemPtr;
	
	/**
	 * \brief Menu item smart pointer..
	 */
	typedef SmartPtr<ControlInfo> ControlInfoPtr;
	
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
	friend class FormatInfo;
	
	public:
		Size(std::uint32_t width, std::uint32_t height);
	
		std::uint32_t width() const;
		std::uint32_t height() const;
	
	private:
		Size(vcap_size_t size);
	
		vcap_size_t _size;
};

class Vcap::Format {
	friend class Camera;
	
	public:
		Format();
		Format(const std::uint32_t& code, const Size& size);
	
		std::uint32_t code();
		Size size();
		
	private:
		Format(vcap_format_t format);
	
		vcap_format_t _format;
};

/**
 * \brief Encapsulates pixel format information.
 */
class Vcap::FormatInfo {
	friend class Camera;
	
	public:
		virtual ~FormatInfo();
	
		std::uint32_t code();
		std::string codeString();
		std::string description();
		
		std::vector<SizePtr> sizes();
		
	private:
		FormatInfo(vcap_format_info_t* format);
	
		vcap_format_info_t* _format;
};

/**
 * \brief Encapsulates a camera control menu item.
 */
class Vcap::MenuItem {
	friend class ControlInfo;

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
class Vcap::ControlInfo {
	friend class Camera;

	public:
		virtual ~ControlInfo();
	
		ControlId id();
		ControlType type();
		
		std::string name();
		
		std::int32_t min();
		std::int32_t max();
		std::int32_t step();
		std::int32_t defaultValue();
	
		std::vector<MenuItemPtr> menu();
	
	private:
		ControlInfo(vcap_control_info_t* control);
	
		vcap_control_info_t* _control;
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
		 * \brief Returns true if the camera is open; false otherwise.
		 */
		bool opened();
		
		/**
		 * \brief Returns all formats supported by this device.
		 */
		std::vector<FormatInfoPtr> formats() throw (RuntimeError);
		
		/**
		 * \brief Returns the current format.
		 */
		Format format() throw (RuntimeError);
		
		/**
		 * \brief Sets the current format.
		 */
		void setFormat(const Format& format) throw (RuntimeError);
		
		/**
		 * \brief Automatically sets the camera's format based upon decoder priority.
		 */
		void autoSetFormat() throw (RuntimeError);
		
		/**
		 * \brief Returns all supported frame rates for a given format and frame size.
		 */
		std::vector<std::uint16_t> frameRates(const Format& format) throw (RuntimeError);
		
		/**
		 * \brief Returns the current frame rate.
		 */
		std::uint16_t frameRate() throw (RuntimeError);
		
		/**
		 * \brief Sets the current frame rate.
		 */
		void setFrameRate(const std::uint16_t& frameRate) throw (RuntimeError);
		
		/**
		 * \brief Returns all supported controls.
		 */
		std::vector<ControlInfoPtr> controls() throw (RuntimeError);
		
		/**
		 * \brief Returns the current value for the specified control.
		 */
		std::int32_t controlValue(const ControlId& id) throw (RuntimeError);
		
		/**
		 * \brief Sets the current value for the specified control.
		 */
		void setControlValue(const ControlId& id, const std::int32_t& value) throw (RuntimeError);
		
		/**
		 * \brief Starts streaming.
		 */
		void start() throw (RuntimeError);
		
		/**
		 * \brief Stops streaming.
		 */
		void stop() throw (RuntimeError);
		
		/**
		 * \brief Returns true if the camera is streaming; false otherwise.
		 */
		bool capturing();
		
		/**
		 * \brief Allocates a buffer, grabs an image from the camera (optionally decodes it), and stores it in the buffer.
		 */
		std::size_t grab(std::uint8_t** buffer, bool decode = false, bool bgr = false) throw (RuntimeError);
		
	private:
		Camera(vcap_camera_t* camera);
	
		vcap_camera_t* _camera;
};

#endif
