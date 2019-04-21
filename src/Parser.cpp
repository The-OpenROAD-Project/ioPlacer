////////////////////////////////////////////////////////////////////////////////////
// Authors: Vitor Bandeira, Mateus Fogaça, Eder Matheus Monteiro e Isadora Oliveira
//          (Advisor: Ricardo Reis)
//
// BSD 3-Clause License
//
// Copyright (c) 2018, The Regents of the University of California
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <istream>
#include <fstream>
#include "Parser.h"

Parser::Parser(Parameters &parms) : _parms(&parms) {
}

void Parser::readDieArea() {
	std::ifstream infile;
	infile.open(_parms->getFloorplanFile());
	
	if(!infile.is_open())
		std::cout << "File" << _parms->getFloorplanFile() << "not found!!!\n";
	
	std::string line;

	std::string trash_buffer;
	int lowerX, lowerY, upperX, upperY;

	while (std::getline(infile, line)) {
		std::istringstream iss(line);
		if (iss >> trash_buffer >> lowerX >> lowerY >> upperX >> upperY) {
			point lowerBound = point(lowerX, lowerY);
			point upperBound = point(upperX, upperY);
			_dieArea = box(lowerBound, upperBound);
		} // end if
	} // end while
	infile.close();
} // end method

// -----------------------------------------------------------------------------

void Parser::readConnections() {
	std::ifstream infile;
	infile.open(_parms->getNetlistFile());
	
	if(!infile.is_open())
		std::cout << "File" << _parms->getNetlistFile() << "not found!!!\n";
	
	std::string line;

	int ioCounter = -1;
	std::string pinName;
	std::string direction;
	int lowerX, lowerY, upperX, upperY;
	double x, y;

	while (std::getline(infile, line)) {
		if (line[0] == '\t') {
			line.erase(0, 1);
		} // end if
		
		std::istringstream iss(line);
		
		if (iss >> pinName >> lowerX >> lowerY >> upperX >> upperY >> direction) {
			ioPin pin;
			pin.name = pinName;
			pin.bounds = box(point(lowerX, lowerY), point(upperX, upperY));
			pin.direction = direction;
			_ioPins.push_back(pin);
			ioCounter++;
			continue;
		} // end if
		
		std::istringstream iss2(line);
		if (iss2 >> pinName >> x >> y) {
			cellPin cPin;
			cPin.name = pinName;
			cPin.position = point(x, y);
			_ioPins[ioCounter].connections.push_back(cPin);
		} // end if
	} // end while
	infile.close();
} // end method

// -----------------------------------------------------------------------------

std::vector<Parser::ioPin>& Parser::getIOPins() {
	return _ioPins;
} // end method
