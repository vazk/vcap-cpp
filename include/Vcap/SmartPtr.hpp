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

#ifndef _VCAP_SMART_PTR_HPP
#define _VCAP_SMART_PTR_HPP

/**
 * \file
 * A basic smart pointer implementation.
 */

namespace Vcap {
	template <typename T>
	class SmartPtr;
}

/**
 * \brief Basic smart pointer implementation.
 */
template<typename T>
class Vcap::SmartPtr {
	public:
		SmartPtr();
		SmartPtr(T* value);
		~SmartPtr();
		
		SmartPtr(const SmartPtr<T>& sp);
		SmartPtr<T>& operator = (const SmartPtr<T>& sp);
	
		T& operator * ();
		T* operator -> ();
		
	private:
		unsigned int *_refs;
		T* _value;
};

template <typename T>
Vcap::SmartPtr<T>::SmartPtr() : _value(NULL) {
	_refs = new unsigned int;
	*_refs = 1;
}

template <typename T>
Vcap::SmartPtr<T>::SmartPtr(T* value) : _value(value) {
	_refs = new unsigned int;
	*_refs = 1;
}

template <typename T>
Vcap::SmartPtr<T>::~SmartPtr() {
	if (--(*_refs) == 0) {
		delete _refs;
		
		if (_value)
			delete _value;
	}
}

template <typename T>
Vcap::SmartPtr<T>::SmartPtr(const SmartPtr<T>& sp) : _value(sp._value) {
	_refs = sp._refs;
	(*_refs)++;	
}

template <typename T>
Vcap::SmartPtr<T>& Vcap::SmartPtr<T>::operator = (const SmartPtr<T>& sp) {
	if (this != &sp) {
		if(--(*_refs) == 0) {
			delete _refs;
			
			if (_value)
				delete _value;
		}
	
		_value = sp._value;
		_refs = sp._refs;
		
		(*_refs)++;
	}
	
	return *this;
}

template <typename T>	
T& Vcap::SmartPtr<T>::operator * () {
	return *_value;
}

template <typename T>
T* Vcap::SmartPtr<T>::operator -> () {
	return _value;
}

#endif
