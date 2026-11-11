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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <uxhw.h>
#include "utilities.h"
#include "kernel.h"
#include "common.h"

#ifdef NO_OS_AVAILABLE

void
returnZeroNoOS(void);
#endif

int
main(int argc, char *  argv[])
{
	CommandLineArguments arguments = { 0 };

	double *                    monteCarloOutputSamples = NULL;
	clock_t                     start = 0;
	clock_t                     end;
	double                      cpuTimeUsedSeconds = 0.0;
	double                      outputVariables[kNXPMPX4100AOutputVariableIndexMax] = { 0 };
	const char *                outputVariableNames[kNXPMPX4100AOutputVariableIndexMax] = {
		[kNXPMPX4100AOutputVariableIndexCalibratedSensorOutput] = "calibratedSensorOutput"
	};
	const char *                outputVariableDescriptions[kNXPMPX4100AOutputVariableIndexMax] = {
		[kNXPMPX4100AOutputVariableIndexCalibratedSensorOutput] = "Calibrated Pressure Output (kPa)"
	};
	kOutputVariableTypeIndex    outputVariableTypes[kNXPMPX4100AOutputVariableIndexMax] = {
		[kNXPMPX4100AOutputVariableIndexCalibratedSensorOutput] = kOutputVariableTypeDistribution
	};
	const char *                applicationDescription = "NXP MPX4100A Conversion Routines";
	MeanAndVariance             meanAndVariance;

	/*
	 *	Get command line arguments.
	 */
	if (getCommandLineArguments(argc, argv, &arguments))
	{
		return kCommonConstantReturnTypeError;
	}

	/*
	 *	MonteCarlo output samples are used even in the UxHw mode to store
	 *	the distributional result (a single entry).
	 */
	monteCarloOutputSamples = (double *) checkedMalloc(
		arguments.common.numberOfMonteCarloIterations * sizeof(double),
		__FILE__,
		__LINE__
	);

	const bool isScalarOutputSelected = isSelectedOutputScalar(
		&arguments.common,
		outputVariableTypes,
		kNXPMPX4100AOutputVariableIndexMax
	);

	/*
	 *	Start timing.
	 */
	if (arguments.common.isTimingEnabled)
	{
		start = clock();
	}

	/*
	 *	Dispatch to the mode-specific kernel. The Monte Carlo loop (when
	 *	applicable) lives inside `NXPMPX4100ACalculateOutputMonteCarlo`. UxHw mode runs a single
	 *	distributional evaluation inside `NXPMPX4100ACalculateOutputUxHw`.
	 */
	if (arguments.common.isMonteCarloMode)
	{
		NXPMPX4100ACalculateOutputMonteCarlo(arguments.common.numberOfMonteCarloIterations, outputVariables, monteCarloOutputSamples);

		/*
		 *	If not doing UxHw version, then approximate the cost of the third phase of
		 *	Monte Carlo (post-processing), by calculating the mean and variance.
		 */
		if (!isScalarOutputSelected)
		{
			meanAndVariance = calculateMeanAndVarianceOfDoubleSamples(
				monteCarloOutputSamples,
				arguments.common.numberOfMonteCarloIterations
			);
			outputVariables[arguments.common.outputSelect] = meanAndVariance.mean;
		}
	}
	else
	{
		NXPMPX4100ACalculateOutputUxHw(outputVariables, monteCarloOutputSamples);
	}

	/*
	 *	Stop timing.
	 */
	if (arguments.common.isTimingEnabled)
	{
		end                 = clock();
		cpuTimeUsedSeconds  = ((double) (end - start)) / CLOCKS_PER_SEC;
	}

	/*
	 *	Print the results (either in JSON or standard output format).
	 */
	if (arguments.common.isOutputJSONMode)
	{
		printJSONFormattedOutput(
			&arguments.common,
			monteCarloOutputSamples,
			outputVariables,
			outputVariableNames,
			kNXPMPX4100AOutputVariableIndexMax,
			applicationDescription,
			outputVariableTypes
		);
	}
	else
	{
		printHumanConsumableOutput(
			&arguments.common,
			kNXPMPX4100AOutputVariableIndexMax,
			outputVariables,
			outputVariableNames,
			outputVariableDescriptions,
			monteCarloOutputSamples,
			outputVariableTypes
		);
	}

	/*
	 *	Print timing result.
	 */
	if (arguments.common.isTimingEnabled)
	{
		printf("\nCPU time used: %lf seconds\n", cpuTimeUsedSeconds);
	}

	/*
	 *	Write output data.
	 */
	if (arguments.common.isWriteToFileEnabled)
	{
		if (writeOutputDoubleDistributionsToCSV(
				arguments.common.outputFilePath,
				outputVariables,
				outputVariableNames,
				kNXPMPX4100AOutputVariableIndexMax
		))
		{
			return kCommonConstantReturnTypeError;
		}
	}

	/*
	 *	Save Monte carlo outputs in an output file.
	 *	Free dynamically-allocated memory.
	 */
	if (arguments.common.isMonteCarloMode)
	{
		saveMonteCarloDoubleDataToDataDotOutFile(
			monteCarloOutputSamples, (uint64_t) (cpuTimeUsedSeconds * 1000000),
			isScalarOutputSelected ? 1 : arguments.common.numberOfMonteCarloIterations
		);
	}
	free(monteCarloOutputSamples);
#ifdef NO_OS_AVAILABLE
	returnZeroNoOS();
#else

	return 0;

#endif
}
