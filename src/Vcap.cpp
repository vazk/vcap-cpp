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
#include <vcap/decode.h>
}

#include <cstdint>
#include <string>

/*
 * Size class definition
 */
Vcap::Size::Size(std::uint32_t width, std::uint32_t height) {
	_size.width = width;
	_size.height = height;
} 
 
Vcap::Size::Size(vcap_size_t size) {
	_size = size;
}

std::uint32_t Vcap::Size::width() const {
	return _size.width;
}

std::uint32_t Vcap::Size::height() const {
	return _size.height;
}

/*
 * Format class definition
 */
Vcap::Format::Format() {
} 
 
Vcap::Format::Format(const std::uint32_t& code, const Size& size) {
	_format.code = code;
	_format.size.width = size.width();
	_format.size.height = size.height();
}

Vcap::Format::Format(vcap_format_t format) {
	_format = format;
}

std::uint32_t Vcap::Format::code() {
	return _format.code;
}

Vcap::Size Vcap::Format::size() {
	return Size(_format.size);
}

/*
 * Format info class definition
 */
Vcap::FormatInfo::FormatInfo(vcap_format_info_t* format) {
	_format = new vcap_format_info_t;	
	vcap_copy_format(format, _format);
}

Vcap::FormatInfo::~FormatInfo() {
	vcap_destroy_format(_format);
}

std::uint32_t Vcap::FormatInfo::code() {
	return _format->code;
}

std::string Vcap::FormatInfo::codeString() {
	return std::string(_format->code_string);
}

std::string Vcap::FormatInfo::description() {
	return std::string(_format->description);
}

std::vector<Vcap::SizePtr> Vcap::FormatInfo::sizes() {
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
Vcap::ControlInfo::ControlInfo(vcap_control_info_t* control) {
	_control = new vcap_control_info_t;
	vcap_copy_control(control, _control);
}

Vcap::ControlInfo::~ControlInfo() {
	vcap_destroy_control(_control);
}

Vcap::ControlId Vcap::ControlInfo::id() {
	return (ControlId)_control->id;
}

Vcap::ControlType Vcap::ControlInfo::type() {
	return (ControlType)_control->type;
}

std::string Vcap::ControlInfo::name() {
	return std::string(_control->name);
}

std::int32_t Vcap::ControlInfo::min() {
	return _control->min;
}

std::int32_t Vcap::ControlInfo::max() {
	return _control->max;
}

std::int32_t Vcap::ControlInfo::step() {
	return _control->step;
}

std::int32_t Vcap::ControlInfo::defaultValue() {
	return _control->default_value;
}

std::vector<Vcap::MenuItemPtr> Vcap::ControlInfo::menu() {
	std::vector<MenuItemPtr> menu;
	
	for (int i = 0; i < _control->menu_length; i++)
		menu.push_back(MenuItemPtr(new MenuItem(&_control->menu[i])));
	
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

bool Vcap::Camera::opened() {
	return _camera->opened;
}

std::vector<Vcap::FormatInfoPtr> Vcap::Camera::formats() throw (RuntimeError) {
	std::vector<FormatInfoPtr> formats;

	vcap_format_info_t* fmts;
	
	int numFormats = vcap_get_formats(_camera, &fmts);
	
	if (-1 == numFormats)
		throw RuntimeError(std::string(vcap_error()));
		
	for (int i = 0; i < numFormats; i++)
		formats.push_back(FormatInfoPtr(new FormatInfo(&fmts[i])));

	vcap_destroy_formats(fmts, numFormats);

	return formats;
}

Vcap::Format Vcap::Camera::format() throw (RuntimeError) {
	vcap_format_t format;
	
	if (-1 == vcap_get_format(_camera, &format))
		throw RuntimeError(std::string(vcap_error()));
	
	return Format(format);
}

void Vcap::Camera::setFormat(const Format& format) throw (RuntimeError) {
	if (-1 == vcap_set_format(_camera, format._format))
		throw RuntimeError(std::string(vcap_error()));
}

void Vcap::Camera::autoSetFormat() throw (RuntimeError) {
	if (-1 == vcap_auto_set_format(_camera))
		throw RuntimeError(std::string(vcap_error()));
}

std::vector<std::uint16_t> Vcap::Camera::frameRates(const Format& format) throw (RuntimeError) {
	std::vector<std::uint16_t> frameRates;
	
	std::uint16_t* rates;
	
	int numFrameRates = vcap_get_frame_rates(_camera, format._format, &rates);
	
	if (-1 == numFrameRates)
		throw RuntimeError(std::string(vcap_error()));
	
	for (int i = 0; i < numFrameRates; i++)
		frameRates.push_back(rates[i]);
	
	delete [] rates;
	
	return frameRates;
}

std::uint16_t Vcap::Camera::frameRate() throw (RuntimeError) {
	uint16_t frameRate;
	
	if (-1 == vcap_get_frame_rate(_camera, &frameRate))
		throw RuntimeError(std::string(vcap_error()));
	
	return frameRate;
}

void Vcap::Camera::setFrameRate(const std::uint16_t& frameRate) throw (RuntimeError) {
	if (-1 == vcap_set_frame_rate(_camera, frameRate))
		throw RuntimeError(std::string(vcap_error()));
}

std::vector<Vcap::ControlInfoPtr> Vcap::Camera::controls() throw (RuntimeError) {	
	std::vector<ControlInfoPtr> controls;
	
	vcap_control_info_t* ctrls;
	
	int numControls = vcap_get_controls(_camera, &ctrls);
	
	if (-1 == numControls)
		throw RuntimeError(std::string(vcap_error()));
		
	for (int i = 0; i < numControls; i++) {
		controls.push_back(ControlInfoPtr(new ControlInfo(&ctrls[i])));
	}
	
	vcap_destroy_controls(ctrls, numControls);
	
	return controls;
}

std::int32_t Vcap::Camera::controlValue(const ControlId& id) throw (RuntimeError) {
	std::int32_t value;
	
	if (-1 == vcap_get_control_value(_camera, (vcap_control_id_t)id, &value))
		throw RuntimeError(std::string(vcap_error()));
	
	return value;
}

void Vcap::Camera::setControlValue(const ControlId& id, const std::int32_t& value) throw (RuntimeError) {
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

bool Vcap::Camera::capturing() {
	return _camera->capturing;
}

std::size_t Vcap::Camera::grab(std::uint8_t** buffer, bool decode, bool bgr) throw (RuntimeError) {
	int bufferSize;
	
	if (!decode) {
		bufferSize = vcap_grab_frame(_camera, buffer);
		
		if (-1 == bufferSize)
			throw RuntimeError(std::string(vcap_error()));
			
		return (std::size_t)bufferSize;
	} else {
		Format fmt = format();
		
		std::uint8_t* rawBuffer;
		std::uint8_t* rgbBuffer = new std::uint8_t[3 * fmt.size().width() * fmt.size().height()];
		
		bufferSize = vcap_grab_frame(_camera, &rawBuffer); 
		
		if (-1 == bufferSize)
			throw RuntimeError(std::string(vcap_error()));
		
		if (-1 == vcap_decode(rawBuffer, rgbBuffer, fmt.code(), fmt.size().width(), fmt.size().height(), bgr))
			throw RuntimeError(std::string(vcap_error()));
			
		delete [] rawBuffer;
		
		*buffer = rgbBuffer;
		
		return (std::size_t)(3 * fmt.size().width() * fmt.size().height());
	}
}
