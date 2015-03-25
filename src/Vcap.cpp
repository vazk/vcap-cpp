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

#include <Vcap/Vcap.hpp>

extern "C" {
#include <vcap/vcap_decode.h>
}

#include <cstdint>
#include <string>

/*
 * Size class definition
 */
Vcap::Size::Size(vcap_size_t size) {
	_size = size;
}

std::uint32_t Vcap::Size::width() {
	return _size.width;
}

std::uint32_t Vcap::Size::height() {
	return _size.height;
}

/*
 * Format class definition
 */
Vcap::Format::Format(vcap_format_t* format) {
	_format = new vcap_format_t;	
	vcap_copy_format(format, _format);
}

Vcap::Format::~Format() {
	vcap_destroy_format(_format);
}

std::uint32_t Vcap::Format::code() {
	return _format->code;
}

std::string Vcap::Format::codeString() {
	return std::string(_format->code_str);
}

std::string Vcap::Format::description() {
	return std::string(_format->desc);
}

std::vector<Vcap::SizePtr> Vcap::Format::sizes() {
	std::vector<SizePtr> sizes;
	
	for (int i = 0; i < _format->num_sizes; i++) {
		sizes.push_back(SizePtr(new Size(_format->sizes[i])));
	}
	
	return sizes;
}

/*
 * Control menu item class definition
 */
Vcap::MenuItem::MenuItem(vcap_menu_item_t* menuItem) {
	_menuItem = new vcap_menu_item_t;
	vcap_copy_menu_item(menuItem, _menuItem);
}

Vcap::MenuItem::~MenuItem() {
	delete _menuItem;
}

std::string Vcap::MenuItem::name() {
	return std::string(_menuItem->name);
}

std::uint32_t Vcap::MenuItem::value() {
	return _menuItem->value;
}

/*
 * Control class definition
 */
Vcap::Control::Control(vcap_control_t* control) {
	_control = new vcap_control_t;
	vcap_copy_control(control, _control);
}

Vcap::Control::~Control() {
	vcap_destroy_control(_control);
}

Vcap::ControlId Vcap::Control::id() {
	return (ControlId)_control->id;
}

Vcap::ControlType Vcap::Control::type() {
	return (ControlType)_control->type;
}

std::string Vcap::Control::name() {
	return std::string(_control->name);
}

std::int32_t Vcap::Control::min() {
	return _control->min;
}

std::int32_t Vcap::Control::max() {
	return _control->max;
}

std::int32_t Vcap::Control::step() {
	return _control->step;
}

std::int32_t Vcap::Control::default_value() {
	return _control->default_value;
}

std::vector<Vcap::MenuItemPtr> Vcap::Control::menu() {
	std::vector<MenuItemPtr> menu;
	
	for (int i = 0; i < _control->menu_length; i++) {
		vcap_menu_item_t* menuItem = new vcap_menu_item_t;
		
		vcap_copy_menu_item(&_control->menu[i], menuItem);
		
		menu.push_back(MenuItemPtr(new MenuItem(menuItem)));
		
		delete menuItem;
	}
	
	return menu;
}

/*
 * Camera class definition
 */
std::vector<Vcap::CameraPtr> Vcap::Camera::cameras() throw (RuntimeError) {
	std::vector<CameraPtr> cameras;
	
	vcap_camera_t* cams;
	
	int numCameras = vcap_cameras(&cams);
	
	if (-1 == numCameras)
		throw RuntimeError(std::string(vcap_error()));
		
	for (int i = 0; i < numCameras; i++) {
		cameras.push_back(CameraPtr(new Camera(&cams[i])));
	}
	
	if (-1 == vcap_destroy_cameras(cams, numCameras))
		throw RuntimeError(std::string(vcap_error()));
	
	return cameras;
}

Vcap::Camera::Camera(const std::string& device) {
	_camera = vcap_create_camera(device.c_str());
	
	if (!_camera)
		throw RuntimeError(std::string(vcap_error()));
}

Vcap::Camera::Camera(vcap_camera_t* camera) {
	_camera = new vcap_camera_t;
	
	if (-1 == vcap_copy_camera(camera, _camera))
		throw RuntimeError(std::string(vcap_error()));
}

Vcap::Camera::~Camera() {
	vcap_destroy_camera(_camera);
}

std::string Vcap::Camera::device() {
	return std::string(_camera->device);
}

std::string Vcap::Camera::driver() {
	return std::string(_camera->driver);
}

std::string Vcap::Camera::info() {
	return std::string(_camera->info);
}

void Vcap::Camera::open() throw (RuntimeError) {
	if (-1 == vcap_open_camera(_camera))
		throw RuntimeError(std::string(vcap_error()));
}

void Vcap::Camera::close() throw (RuntimeError) {
	if (-1 == vcap_close_camera(_camera))
		throw RuntimeError(std::string(vcap_error()));
}

std::vector<Vcap::FormatPtr> Vcap::Camera::formats() throw (RuntimeError) {
	std::vector<FormatPtr> formats;

	vcap_format_t* fmts;
	
	int numFormats = vcap_get_formats(_camera, &fmts);
	
	if (-1 == numFormats)
		throw RuntimeError(std::string(vcap_error()));
		
	for (int i = 0; i < numFormats; i++) {
		formats.push_back(FormatPtr(new Format(&fmts[i])));
	}

	vcap_destroy_formats(fmts, numFormats);

	return formats;
}

std::tuple<std::uint32_t, std::uint32_t, std::uint32_t> Vcap::Camera::format() throw (RuntimeError) {
	std::uint32_t formatCode, width, height;
	
	if (-1 == vcap_get_format(_camera, &formatCode, &width, &height))
		throw RuntimeError(std::string(vcap_error()));
	
	return std::make_tuple(formatCode, width, height);
}

void Vcap::Camera::setFormat(std::uint32_t formatCode, std::uint32_t width, std::uint32_t height) throw (RuntimeError) {
	if (-1 == vcap_set_format(_camera, formatCode, width, height))
		throw RuntimeError(std::string(vcap_error()));
}

void Vcap::Camera::autoSetFormat() throw (RuntimeError) {
	if (-1 == vcap_auto_set_format(_camera))
		throw RuntimeError(std::string(vcap_error()));
}

std::vector<std::uint16_t> Vcap::Camera::frameRates(std::uint32_t formatCode, std::uint32_t width, std::uint32_t height) throw (RuntimeError) {
	std::vector<std::uint16_t> frameRates;
	
	std::uint16_t* rates;
	
	int numFrameRates = vcap_get_frame_rates(_camera, formatCode, width, height, &rates);
	
	if (-1 == numFrameRates)
		throw RuntimeError(std::string(vcap_error()));
	
	for (int i = 0; i < numFrameRates; i++) {
		frameRates.push_back(rates[i]);
	}
	
	delete [] rates;
	
	return frameRates;
}

std::uint16_t Vcap::Camera::frameRate() throw (RuntimeError) {
	uint16_t frameRate;
	
	if (-1 == vcap_get_frame_rate(_camera, &frameRate))
		throw RuntimeError(std::string(vcap_error()));
	
	return frameRate;
}

void Vcap::Camera::setFrameRate(std::uint16_t frameRate) throw (RuntimeError) {
	if (-1 == vcap_set_frame_rate(_camera, frameRate))
		throw RuntimeError(std::string(vcap_error()));
}

std::vector<Vcap::ControlPtr> Vcap::Camera::controls() throw (RuntimeError) {	
	std::vector<ControlPtr> controls;
	
	vcap_control_t* ctrls;
	
	int numControls = vcap_get_controls(_camera, &ctrls);
	
	if (-1 == numControls)
		throw RuntimeError(std::string(vcap_error()));
		
	for (int i = 0; i < numControls; i++) {
		controls.push_back(ControlPtr(new Control(&ctrls[i])));
	}
	
	vcap_destroy_controls(ctrls, numControls);
	
	return controls;
}

std::int32_t Vcap::Camera::controlValue(ControlId id) throw (RuntimeError) {
	std::int32_t value;
	
	if (-1 == vcap_get_control_value(_camera, (vcap_control_id_t)id, &value))
		throw RuntimeError(std::string(vcap_error()));
	
	return value;
}

void Vcap::Camera::setControlValue(ControlId id, std::int32_t value) throw (RuntimeError) {
	if (-1 == vcap_set_control_value(_camera, (vcap_control_id_t)id, value))
		throw RuntimeError(std::string(vcap_error()));
}

void Vcap::Camera::start() throw (RuntimeError) {
	if (-1 == vcap_start_capture(_camera))
		throw RuntimeError(std::string(vcap_error()));
}

void Vcap::Camera::stop() throw (RuntimeError) {
	if (-1 == vcap_stop_capture(_camera))
		throw RuntimeError(std::string(vcap_error()));
}	

std::size_t Vcap::Camera::grab(std::uint8_t** buffer, bool decode, bool bgr) throw (RuntimeError) {
	int bufferSize;
	
	if (!decode) {
		bufferSize = vcap_grab_frame(_camera, buffer);
		
		if (-1 == bufferSize)
			throw RuntimeError(std::string(vcap_error()));
			
		return (std::size_t)bufferSize;
	} else {
		std::uint32_t formatCode, width, height;
		
		std::tie(formatCode, width, height) = format();
		
		std::uint8_t* rawBuffer;
		std::uint8_t* rgbBuffer = new std::uint8_t[3 * width * height];
		
		bufferSize = vcap_grab_frame(_camera, &rawBuffer); 
		
		if (-1 == bufferSize)
			throw RuntimeError(std::string(vcap_error()));
		
		if (-1 == vcap_decode(rawBuffer, rgbBuffer, formatCode, width, height, bgr))
			throw RuntimeError(std::string(vcap_error()));
			
		delete [] rawBuffer;
		
		*buffer = rgbBuffer;
		
		return (std::size_t)(3 * width * height);
	}
}
