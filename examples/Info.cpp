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

#include <iostream>
#include <string>
#include <vector>

/*
 * Prints information about all V4L2 devices connected to the system.
 */
int main(int argc, char* argv[]) {
	std::vector<Vcap::CameraPtr> cameras;
	
	try {
		cameras = Vcap::Camera::cameras();
	} catch (Vcap::RuntimeError& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	
	if (0 == cameras.size()) {
		std::cout << "No cameras found!" << std::endl;
		return -1;
	}
	
	std::cout << "------------------------------------------------------------" << std::endl;
	
	/*
	 * Iterate through cameras.
	 */
	for (unsigned int i = 0; i < cameras.size(); i++) {
		std::cout << "Device: " << cameras[i]->device() << std::endl;
		std::cout << "Driver: " << cameras[i]->driver() << std::endl;
		std::cout << "Info: " << cameras[i]->info() << std::endl;

		try {
			cameras[i]->open();
		} catch (Vcap::RuntimeError& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
		
		std::cout << "Formats:" << std::endl;
		
		std::vector<Vcap::FormatInfoPtr> formats;
		
		try {
			formats = cameras[i]->formats();
		} catch (Vcap::RuntimeError& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
		
		/*
		 * Iterate through formats.
		 */
		for (unsigned int j = 0; j < formats.size(); j++) {
			std::cout << formats[j]->codeString() << " (" << formats[j]->description() << "):" << std::endl;
			
			std::vector<Vcap::SizePtr> sizes = formats[j]->sizes();
			
			/*
			 * Iterate through frame sizes.
			 */
			for (unsigned int k = 0; k < sizes.size(); k++) {
				
				std::cout << "\t" << std::to_string(sizes[k]->width()) << "x" << std::to_string(sizes[k]->height());
				
				std::vector<std::uint16_t> frameRates;
				
				try {
					frameRates = cameras[i]->frameRates(Vcap::Format(formats[j]->code(), *sizes[k]));
				} catch (Vcap::RuntimeError& e) {
					std::cout << e.what() << std::endl;
					return -1;
				}
				
				if (frameRates.size() > 0) {
					std::cout << " (FPS: ";
					
					/*
					 * Iterate through frame rates.
					 */
					for (unsigned int l = 0; l < frameRates.size(); l++) {
						std::cout << std::to_string(frameRates[l]);
						
						if (l < frameRates.size() - 1)
							std::cout << ", ";
					}
					
					std::cout << ")";
				}
				
				std::cout << std::endl;
			}
		}
		
		std::cout << "Controls:" << std::endl;
		
		std::vector<Vcap::ControlInfoPtr> controls;
		
		try {
			controls = cameras[i]->controls();
		} catch (Vcap::RuntimeError& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
		
		/*
		 * Iterate through controls.
		 */
		for (unsigned int j = 0; j < controls.size(); j++) {
			std::cout << controls[j]->name();
			std::cout << " (min: " << std::to_string(controls[j]->min());
			std::cout << ", max: " << std::to_string(controls[j]->max());
			std::cout << ", step: " << std::to_string(controls[j]->step());
			std::cout << ", default: " << std::to_string(controls[j]->defaultValue()) << ")";
			
			if (controls[j]->type() == Vcap::CTRL_TYPE_MENU) {
				std::vector<Vcap::MenuItemPtr> menu = controls[j]->menu();
				
				std::cout << " (Menu: ";
				
				for (unsigned int k = 0; k < menu.size(); k++) {
					std::cout << menu[k]->value() << ":" << menu[k]->name();
					
					if (k < menu.size() - 1)
						std::cout << ", ";
				}
				
				std::cout << ")";
			}
			
			std::cout << std::endl;
		}
		
		try {
			cameras[i]->close();
		} catch (Vcap::RuntimeError& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
		
		std::cout << "------------------------------------------------------------" << std::endl;
	}
	
	return 0;
}
