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

#include <uxhw.h>
#include "kernel.h"
#include "nxp-mpx4100a-monte-carlo.h"


double
NXPMPX4100A_calculateOutput(double * inputVariables, double *  outputVariables)
{
	double  vSupplyADC;
	double  vSensorADC;
	double  calibratedValue;

	vSupplyADC  = inputVariables[kNXPMPX4100AInputVariableIndexVsupplyADC];
	vSensorADC  = inputVariables[kNXPMPX4100AInputVariableIndexVsensorADC];

	calibratedValue = ((vSensorADC / vSupplyADC) + kNXPMPX4100ASensorCalibrationConstant1) / kNXPMPX4100ASensorCalibrationConstant2;
	outputVariables[kNXPMPX4100AOutputVariableIndexCalibratedSensorOutput] = calibratedValue;

	return calibratedValue;
}

void
NXPMPX4100ASetInputVariablesViaUxHwCall(double * inputVariables)
{
	inputVariables[kNXPMPX4100AInputVariableIndexVsensorADC] = UxHwDoubleUniformDist(
		kNXPMPX4100ADefaultInputVariableVsensorUniformDistLow,
		kNXPMPX4100ADefaultInputVariableVsensorUniformDistHigh
	);

	inputVariables[kNXPMPX4100AInputVariableIndexVsupplyADC] = UxHwDoubleUniformDist(
		kNXPMPX4100ADefaultInputVariableVsupplyUniformDistLow,
		kNXPMPX4100ADefaultInputVariableVsupplyUniformDistHigh
	);

	return;
}

double
NXPMPX4100ACalculateOutputUxHw(double * outputVariables, double * monteCarloOutputSamples)
{
	double  inputVariables[kNXPMPX4100AInputVariableIndexMax];
	double  calibratedSensorOutput;

	NXPMPX4100ASetInputVariablesViaUxHwCall(inputVariables);
	calibratedSensorOutput      = NXPMPX4100A_calculateOutput(inputVariables, outputVariables);
	monteCarloOutputSamples[0]  = calibratedSensorOutput;

	return calibratedSensorOutput;
}

double
NXPMPX4100ACalculateOutputMonteCarlo(
	size_t      numberOfMonteCarloIterations,
	double *    outputVariables,
	double *    monteCarloOutputSamples)
{
	double inputVariables[kNXPMPX4100AInputVariableIndexMax];

	return NXPMPX4100A_calculateOutputMonteCarloSamples(
		numberOfMonteCarloIterations,
		inputVariables,
		outputVariables,
		monteCarloOutputSamples
	);
}
