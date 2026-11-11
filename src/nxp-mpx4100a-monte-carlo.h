/*
 *	Copyright (c) 2024-2026, Signaloid.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#pragma once

#include <stddef.h>

/**
 *	@brief	Run `numberOfMonteCarloIterations` iterations of the sensor
 *		calibration. Each iteration draws new input samples and stores the
 *		calibrated output in `monteCarloOutputSamples`.
 *
 *	@param	numberOfMonteCarloIterations	: Number of iterations to run.
 *	@param	inputVariables			: Scratch array of input variables, of size `kNXPMPX4100AInputVariableIndexMax`.
 *	@param	outputVariables			: Array of output variables to fill.
 *	@param	monteCarloOutputSamples		: Array of `numberOfMonteCarloIterations` doubles, filled with samples.
 *	@return	double				: Returns the last calibrated output sample.
 */
double
NXPMPX4100A_calculateOutputMonteCarloSamples(
	size_t      numberOfMonteCarloIterations,
	double *    inputVariables,
	double *    outputVariables,
	double *    monteCarloOutputSamples);
