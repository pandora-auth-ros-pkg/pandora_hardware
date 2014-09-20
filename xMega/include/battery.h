/*! 
 * \file battery.h
 * \brief Battery voltage level sense Header file
 *
 * Created: 6/20/2014 6:33:27 PM
 * Author: Panayiotou Konstantinos
 * Email: klpanagi@gmail.com
 */ 

 /***********************************************************************
  * Software License Agreement (BSD License)
  *
  *  Copyright (c) 2014, P.A.N.D.O.R.A. Team.
  *  All rights reserved.
  *
  *  Redistribution and use in source and binary forms, with or without
  *  modification, are permitted provided that the following conditions
  *  are met:
  *
  *   * Redistributions of source code must retain the above copyright
  *     notice, this list of conditions and the following disclaimer.
  *   * Redistributions in binary form must reproduce the above
  *     copyright notice, this list of conditions and the following
  *     disclaimer in the documentation and/or other materials provided
  *     with the distribution.
  *   * Neither the name of the P.A.N.D.O.R.A. Team nor the names of its
  *     contributors may be used to endorse or promote products derived
  *     from this software without specific prior written permission.
  *
  *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  *  POSSIBILITY OF SUCH DAMAGE.
  *
  * *********************************************************************/

#ifndef BATTERY_H_
#define BATTERY_H_

#include "adc_driver.h"

/*!  \def ADC_OFFSET
 *   \brief Precalculated ADC offset to be value 153 + 7 =160 = 1.086Volt
 */ 		
#define ADC_OFFSET 160	/*<1.086V>*/	 

/*!  \struct batteries_struct
 *   \brief Batteries voltage level measurements structure.
 *   Vars for voltage levels measurements of Electronics-PSU battery and Motor-Servo battery.
 */
typedef struct{
	uint16_t batteryVoltage_PSU;
	uint16_t batteryVoltage_MOTOR;
}batteries_struct;

/*!  \fn void init_ADC(void)
 *   \brief Initialize two ADC inputs ( Battery Voltage Level Measuments )
 */
void init_ADC(void);

#endif /* BATTERY_H_ */
