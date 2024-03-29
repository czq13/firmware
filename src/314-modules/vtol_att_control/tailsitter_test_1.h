/****************************************************************************
 *
 *   Copyright (c) 2015 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
* @file tailsitter.h
*
* @author Roman Bapst 		<bapstroman@gmail.com>
* @author David Vorsin     <davidvorsin@gmail.com>
*
*/

#ifndef TAILSITTER_H
#define TAILSITTER_H

#include "vtol_type.h"
#include <perf/perf_counter.h>  /** is it necsacery? **/
#include <parameters/param.h>
#include <drivers/drv_hrt.h>

class Tailsitter : public VtolType
{

public:
	Tailsitter(VtolAttitudeControl *_att_controller);
	~Tailsitter() = default;

	virtual void update_vtol_state();
	virtual void update_transition_state();
	virtual void update_mc_state();
	virtual void update_fw_state();
	virtual void fill_actuator_outputs();
	virtual void waiting_on_tecs();

private:

	struct {
		float front_trans_dur_p2;
		float trans_thr_min;
		float trans_thr_max;
		float fw_pitch_trim;
		int32_t motors_off_test;
		float GROUND_SPEED2_TRANSITION_FRONT_P1;
		float manual_pitch_max;
		float manual_roll_max;
		float manual_yaw_max;
		float trans_f_p2_dur;
		float trans_p3_dur;
		float trans_p3_f_pitch;
		int32_t yaw_control_flag;
		float height_p;
	} _params_tailsitter;

	struct {
		param_t front_trans_dur_p2;
		param_t trans_thr_min;
		param_t trans_thr_max;
		param_t	motors_off_test;
		param_t fw_pitch_trim;
		param_t GROUND_SPEED2_TRANSITION_FRONT_P1;
		param_t manual_pitch_max;
		param_t manual_roll_max;
		param_t manual_yaw_max;
		param_t trans_f_p2_dur;
		param_t trans_p3_dur;
		param_t trans_p3_f_pitch;
		param_t yaw_control_flag;
		param_t height_p;
	} _params_handles_tailsitter;

	enum vtol_mode {
		MC_MODE = 0,			/**< vtol is in multicopter mode */
		TRANSITION_FRONT_P1,	/**< vtol is in front transition part 1 mode *///xj-zhang
		TRANSITION_FRONT_P2,	/**<vtol is in front transition part 2mode */
		TRANSITION_FRONT_P3,
		TRANSITION_BACK,		/**< vtol is in back transition mode part 1*/
		TRANSITION_BACK_P3,
		FW_MODE					/**< vtol is in fixed wing mode */
	};

	struct {
		vtol_mode flight_mode;			/**< vtol flight mode, defined by enum vtol_mode */
		hrt_abstime transition_start;	/**< absoulte time at which front transition started */
	} _vtol_schedule;

	float _thrust_transition_start; // throttle value when we start the front transition
	float _yaw_transition;	// yaw angle in which transition will take place
	float _pitch_transition_start;  // pitch angle at the start of transition (tailsitter)
	float _munual_thr_start;
	float _z_start;
	//xj-zhang
	float _pitch_transition_start_p2{0.0f};// pitch angle at the start of transition P2 (tailsitter)
	hrt_abstime _time_transition_start_p2{0};
	hrt_abstime _time_transition_start_p3{0};
	struct manual_control_setpoint_s *_manual;
	math::LowPassFilter2p _filter_manual_pitch=math::LowPassFilter2p(50,10);
	math::LowPassFilter2p _filter_manual_roll=math::LowPassFilter2p(50,10);
	math::LowPassFilter2p _filter_manual_yaw=math::LowPassFilter2p(50,10);

	/**
	 * Update parameters.
	 */
	virtual void parameters_update();

};
#endif
