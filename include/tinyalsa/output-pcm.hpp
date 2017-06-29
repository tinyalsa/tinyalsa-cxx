/* Copyright (C) 2017 Taylor Holberton
 *
 * This file is part of xmake.
 *
 * xmake is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xmake is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xmake.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TINYALSA_OUTPUT_PCM_HPP
#define TINYALSA_OUTPUT_PCM_HPP

#include <tinyalsa/pcm.hpp>

#include <memory>

namespace tinyalsa {

/// Represents an audio output in the Linux
/// operating system.
class OutputPcm : public Pcm {
public:
	/// Creates a PCM class. This function
	/// does not open any file descriptors.
	/// Use @ref OutputPcm::Open to open a PCM.
	std::shared_ptr<OutputPcm> Create(void);
	virtual ~OutputPcm(void);
	/// Writes a series of continuous frames to the PCM.
	/// @returns The number of  frames actually written to the
	///  PCM.
	virtual unsigned int Write(const void *frame_array, unsigned int frame_count) = 0;
};

} /* namespace tinyalsa */

#endif // TINYALSA_OUTPUT_PCM_HPP
