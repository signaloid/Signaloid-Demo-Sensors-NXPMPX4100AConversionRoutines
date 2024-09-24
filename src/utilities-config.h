/*
 *	Copyright (c) 2024, Signaloid.
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

/*
 *	These values of the MPX4100's Constant1 and Constant2
 *	are taken from the Transfer Function on page 6 of MPX4100A.pdf, 2024-07-03
 */
#define kSensorCalibrationConstant1				(0.1518)
#define kSensorCalibrationConstant2				(0.01059)

#define kDefaultInputDistributionVsensorUniformDistLow		(2.3)
#define kDefaultInputDistributionVsensorUniformDistHigh		(2.7)
#define kDefaultInputDistributionVsupplyUniformDistLow		(4.8)
#define kDefaultInputDistributionVsupplyUniformDistHigh		(5.4)

/*
 *	Input Distributions:
 *		kInputDistributionIndexVsensorADC	: Ratiometric Analog Voltage (in Volt)
 *		kInputDistributionIndexVsupplyADC	: Supply Voltage (in Volt)
 */
typedef enum
{
	kInputDistributionIndexVsensorADC			= 0,
	kInputDistributionIndexVsupplyADC			= 1,
	kInputDistributionIndexMax,
} InputDistributionIndex;

/*
 *	Output Distribution:
 *		kOutputDistributionIndexCalibratedSensorOutput	: Calibrated Pressure Output (in kPa)
 */
typedef enum
{
	kOutputDistributionIndexCalibratedSensorOutput		= 0,
	kOutputDistributionIndexMax,
} OutputDistributionIndex;

