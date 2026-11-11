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


/*
 *	These values of the MPX4100's Constant1 and Constant2
 *	are taken from the Transfer Function on page 6 of MPX4100A.pdf, 2024-07-03
 */
#define kNXPMPX4100ASensorCalibrationConstant1  (0.1518)
#define kNXPMPX4100ASensorCalibrationConstant2  (0.01059)

#define kNXPMPX4100ADefaultInputVariableVsensorUniformDistLow   (2.3)
#define kNXPMPX4100ADefaultInputVariableVsensorUniformDistHigh  (2.7)
#define kNXPMPX4100ADefaultInputVariableVsupplyUniformDistLow   (4.8)
#define kNXPMPX4100ADefaultInputVariableVsupplyUniformDistHigh  (5.4)

/*
 *	Input Variables:
 *		kNXPMPX4100AInputVariableIndexVsensorADC	: Ratiometric Analog Voltage (in Volt)
 *		kNXPMPX4100AInputVariableIndexVsupplyADC	: Supply Voltage (in Volt)
 */
typedef enum
{
	kNXPMPX4100AInputVariableIndexVsensorADC    = 0,
	kNXPMPX4100AInputVariableIndexVsupplyADC    = 1,
	kNXPMPX4100AInputVariableIndexMax,
} NXPMPX4100AInputVariableIndex;

/*
 *	Output Variable:
 *		kOutputVariableIndexCalibratedSensorOutput	: Calibrated Pressure Output (in kPa)
 */
typedef enum
{
	kNXPMPX4100AOutputVariableIndexCalibratedSensorOutput = 0,
	kNXPMPX4100AOutputVariableIndexMax,
} NXPMPX4100AOutputVariableIndex;

/**
 *	@brief	Sensor calibration routine taken from the Transfer Function on page 6 of
 *		MPX4100A-3139452.pdf, 2024-07-03.
 *
 *	@param	inputVariables	: The array of input variables used in the calculation.
 *	@param	outputVariables	: An array of output variables. Writes the result to
 *					  `outputVariables[kOutputVariableIndexCalibratedSensorOutput]`.
 *	@return	double			: Returns the distributional value calculated.
 */
double
NXPMPX4100A_calculateOutput(double * inputVariables, double *  outputVariables);

/**
 *	@brief	Sets the input variables via calls to the UxHw uniform distribution function.
 *		In UxHw mode this gives distributions. In native Monte Carlo mode each call gives one sample.
 *
 *	@param	inputVariables	: An array of double values, where the function writes the distributional data.
 */
void
NXPMPX4100ASetInputVariablesViaUxHwCall(double * inputVariables);

/**
 *	@brief	UxHw-mode calculation kernel. Evaluates the sensor calibration once
 *		using distributional arithmetic.
 *
 *	@param	outputVariables		: Array of size `kNXPMPX4100AOutputVariableIndexMax` to fill.
 *	@param	monteCarloOutputSamples	: Single-element array for the distributional result.
 *	@return	double			: Returns the distributional calibrated output.
 */
double
NXPMPX4100ACalculateOutputUxHw(double * outputVariables, double * monteCarloOutputSamples);

/**
 *	@brief	Monte Carlo calculation kernel. Runs
 *		`numberOfMonteCarloIterations` iterations of the
 *		sensor calibration into `monteCarloOutputSamples`.
 *
 *	@param	numberOfMonteCarloIterations	: Number of Monte Carlo iterations to run.
 *	@param	outputVariables		: Array of size `kNXPMPX4100AOutputVariableIndexMax` to fill.
 *	@param	monteCarloOutputSamples	: Array of `numberOfMonteCarloIterations` doubles, filled with samples.
 *	@return	double			: Returns the last calibrated output sample.
 */
double
NXPMPX4100ACalculateOutputMonteCarlo(
	size_t      numberOfMonteCarloIterations,
	double *    outputVariables,
	double *    monteCarloOutputSamples);
