FUNCTION_BLOCK FB_INITIALIZE
VAR_EXTERNAL
    Axis0 				                    : Axis_REF;                    (* Axis definitions, Node 2, Right_Back Wheel, EPOS controller *)
	Axis1 				                    : Axis_REF;	                   (* Axis definitions, Node 1, Right_Front Wheel, EPOS P controller *)
	Axis2                                   : Axis_REF;                    (* Axis definitions, Node 3, Left_Back Wheel, EPOS controller *)
	Axis3                                   : Axis_REF;	                   (* Axis definitions, Node 4, Left_Front Wheel, EPOS controller *)			   	
END_VAR

VAR_INPUT
	Execute : BOOL;
END_VAR

VAR_OUTPUT
    Done                      : BOOL;
    Error                     : BOOL;
	ErrorID                   : DINT;
END_VAR

VAR
	lastErrorId : DINT :=0;
	internalErrorCounter : UDINT := 0;
	initializationState	: INITIALIZE_STATE;  
	
	fbReadStatus0	: MC_ReadStatus;
	fbReadStatus1	: MC_ReadStatus;
	fbReadStatus2   : MC_ReadStatus;
	fbReadStatus3   : MC_ReadStatus;
	
	
	fbPower0		: MC_Power;
	fbPower1		: MC_Power;
	fbPower2		: MC_Power;
	fbPower3		: MC_Power;
	
	fbAxisStop0		: MC_Stop;
	fbAxisStop1		: MC_Stop;
	fbAxisStop2		: MC_Stop;
	fbAxisStop3		: MC_Stop;
	
	fbReset0		: MC_Reset;
	fbReset1		: MC_Reset;
	fbReset2		: MC_Reset;
	fbReset3		: MC_Reset;
	
	fbCanRecover 	: FB_CanRecover;
	
END_VAR
IF Execute = FALSE THEN

	initializationState := INITIALIZE_Init;
	Done := FALSE;
	Error := FALSE;
	ErrorId := 0;
	internalErrorCounter := 0;

ELSE

	IF initializationState = INITIALIZE_Init THEN
	
		IF internalErrorCounter >= 5 THEN
		
			Error := TRUE;
			ErrorId := lastErrorId;
			Done := FALSE;
		
		ELSE

			(*initialize all controllers to velocity mode, stand still*)

			fbReadStatus0(Axis := Axis0 , Enable := TRUE);
			fbReadStatus1(Axis := Axis1 , Enable := TRUE);
			fbReadStatus2(Axis := Axis2 , Enable := TRUE);
			fbReadStatus3(Axis := Axis3 , Enable := TRUE);
	
			IF fbReadStatus0.Error OR fbReadStatus1.Error OR fbReadStatus2.Error OR fbReadStatus3.Error THEN

				(*cannot read the state of 1 or 2 or 3 or 4 controllers*)
				(*go to state canError to check for errors*)

				(*initializationState := MAIN_CanError;*)
				initializationState := INITIALIZE_CanError;
	
			ELSIF fbReadStatus0.Valid AND fbReadStatus1.Valid AND fbReadStatus2.Valid AND fbReadStatus3.Valid THEN
	
				(*function blocks fbReadStatus0 or 1 or 2 or 3 have updated their values*)
				IF fbReadStatus0.Disabled AND fbReadStatus1.Disabled AND fbReadStatus2.Disabled AND fbReadStatus3.Disabled THEN
			
					(*all controllers are disabled*)
					(*switch to state INITIALIZE_PowerOn to enable them*)
					initializationState := INITIALIZE_PowerOn;
			
				ELSIF NOT fbReadStatus0.Disabled THEN
		
					(*axis 0 - EPOS controller - Node 2 - right_back motor is not disabled*)
					(*switch to state INITIALIZE_CheckAxis0 to disable it*)
		
					initializationState := INITIALIZE_CheckAxis0;

				ELSIF NOT fbReadStatus1.Disabled THEN

					(*axis 1 - EPOS P controller - Node 1 - right_front motor is not disabled*)
					(*switch to state INITIALIZE_CheckAxis1 to disable it*)
	
					initializationState := INITIALIZE_CheckAxis1;
					
				ELSIF NOT fbReadStatus2.Disabled THEN
		
					(*axis 2 - EPOS controller - Node 3 - left_back motor is not disabled*)
					(*switch to state INITIALIZE_CheckAxis2 to disable it*)
		
					initializationState := INITIALIZE_CheckAxis2;

				ELSIF NOT fbReadStatus3.Disabled THEN

					(*axis 3 - EPOS controller - Node 4 - left_front motor is not disabled*)
					(*switch to state INITIALIZE_CheckAxis3 to disable it*)
	
					initializationState := INITIALIZE_CheckAxis3;

				END_IF;
		
			ELSE
	
			    (*the fbReadStatus0 or1 or 2 or 3 blocks have not updated their values yet*)
				(*continue calling them with enable:=TRUE until they do*)
				(*no need to change state*)
				;

			END_IF;

		END_IF;

	ELSIF initializationState = INITIALIZE_CheckAxis0 THEN

		(*check the state of Axis0*)
		(*after the sequence completes the axis will be in the disabled state*)
		(*control is transfered to state INITIALIZE_Init that will start the enable sequence*)

	    fbReadStatus0(Axis := Axis0 , Enable := TRUE);

		IF fbReadStatus0.Error THEN
	
			(*cannot read the status of axis0*)
			(*axis0 is an EPOS controller connected on the CAN bus*)
			(*maybe there is a CAN communication error, switch to state MAIN_CanError*)
			(*initializationState := MAIN_CanError;*)
			initializationState := INITIALIZE_CanError;
		
		ELSIF fbReadStatus0.Valid THEN
	
			(*we have read the status of axis0*)
	
			IF fbReadStatus0.Errorstop THEN
				(*the motor is in an error state*)
				(*call the MC_Reset function block to reset it*)
				initializationState := INITIALIZE_Reset0;
			ELSIF fbReadStatus0.Disabled THEN
				(*the motor is in the disabled state, nothing to do*)
				(*switch to MAIN_Init*)
				initializationState := INITIALIZE_Init;
			ELSIF fbReadStatus0.Stopping THEN
				(*the motor is stopping*)
				(*although we could simply wait, force the stop*)
				(*i.e. greater decceleration*)
				initializationState := INITIALIZE_Reset0;
			ELSIF fbReadStatus0.StandStill THEN
				(*the controller is not in an error state but it is enabled*)
				(*power off all controllers*)
				initializationState := INITIALIZE_PowerOff;
			ELSIF fbReadStatus0.DiscreteMotion THEN
				(*i.e. position control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;
			ELSIF fbReadStatus0.ContinuousMotion THEN
				(*i.e. velocity control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;		
			ELSIF fbReadStatus0.Homing THEN
				(*stop homing on all controllers*)
				initializationState := INITIALIZE_Stop;
			END_IF;
		ELSE
			(*status not updated yet*)
			;
		END_IF;

	ELSIF initializationState = INITIALIZE_CheckAxis1 THEN

		(*check the state of Axis1*)
		(*after the sequence completes the axis will be in the disabled state*)
		(*control is transfered to state MAIN_Init that will start the enable sequence*)

	    fbReadStatus1(Axis := Axis1 , Enable := TRUE);

		IF fbReadStatus1.Error THEN
	
			(*this is wierd*)
			(*there is no reason why we cannot read our own status*)
			(*axis1 is the EPOS P controller*)
			(*simply increment the internal error counter and try again*)
			lastErrorId := fbReadStatus1.ErrorId;
			internalErrorCounter := internalErrorCounter + 1;
			initializationState := INITIALIZE_Init;
		
		ELSIF fbReadStatus1.Valid THEN
			(*we have read the status of axis1*)
	
			IF fbReadStatus1.Errorstop THEN
				(*the motor is in an error state*)
				(*call the MC_Reset function block to reset it*)
				initializationState := INITIALIZE_Reset1;
			ELSIF fbReadStatus1.Disabled THEN
				(*the motor is in the disabled state, nothing to do*)
				(*switch to MAIN_Init*)
				initializationState := INITIALIZE_Init;
			ELSIF fbReadStatus1.Stopping THEN
				(*the motor is stopping*)
				(*although we could simply wait, force the stop*)
				(*i.e. greater decceleration*)
				initializationState := INITIALIZE_Reset1;
			ELSIF fbReadStatus1.StandStill THEN
				(*the controller is not in an error state but it is enabled*)
				(*power off all controllers*)
				initializationState := INITIALIZE_PowerOff;
			ELSIF fbReadStatus1.DiscreteMotion THEN
				(*i.e. position control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;
			ELSIF fbReadStatus1.ContinuousMotion THEN
				(*i.e. velocity control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;		
			ELSIF fbReadStatus1.Homing THEN
				(*stop homing on all controllers*)
				initializationState := INITIALIZE_Stop;
			END_IF;
		ELSE
			(*status not updated yet*)
			;
		END_IF;
		
	ELSIF initializationState = INITIALIZE_CheckAxis2 THEN

		(*check the state of Axis2*)
		(*after the sequence completes the axis will be in the disabled state*)
		(*control is transfered to state INITIALIZE_Init that will start the enable sequence*)

	    fbReadStatus0(Axis := Axis2 , Enable := TRUE);

		IF fbReadStatus2.Error THEN
	
			(*cannot read the status of axis2*)
			(*axis2 is an EPOS controller connected on the CAN bus*)
			(*maybe there is a CAN communication error, switch to state MAIN_CanError*)
			(*initializationState := MAIN_CanError;*)
			initializationState := INITIALIZE_CanError;
		
		ELSIF fbReadStatus2.Valid THEN
	
			(*we have read the status of axis2*)
	
			IF fbReadStatus2.Errorstop THEN
				(*the motor is in an error state*)
				(*call the MC_Reset function block to reset it*)
				initializationState := INITIALIZE_Reset2;
			ELSIF fbReadStatus2.Disabled THEN
				(*the motor is in the disabled state, nothing to do*)
				(*switch to MAIN_Init*)
				initializationState := INITIALIZE_Init;
			ELSIF fbReadStatus2.Stopping THEN
				(*the motor is stopping*)
				(*although we could simply wait, force the stop*)
				(*i.e. greater decceleration*)
				initializationState := INITIALIZE_Reset0;
			ELSIF fbReadStatus2.StandStill THEN
				(*the controller is not in an error state but it is enabled*)
				(*power off all controllers*)
				initializationState := INITIALIZE_PowerOff;
			ELSIF fbReadStatus2.DiscreteMotion THEN
				(*i.e. position control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;
			ELSIF fbReadStatus2.ContinuousMotion THEN
				(*i.e. velocity control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;		
			ELSIF fbReadStatus2.Homing THEN
				(*stop homing on all controllers*)
				initializationState := INITIALIZE_Stop;
			END_IF;
		ELSE
			(*status not updated yet*)
			;
		END_IF;
		
	ELSIF initializationState = INITIALIZE_CheckAxis3 THEN

		(*check the state of Axis3*)
		(*after the sequence completes the axis will be in the disabled state*)
		(*control is transfered to state INITIALIZE_Init that will start the enable sequence*)

	    fbReadStatus3(Axis := Axis3 , Enable := TRUE);

		IF fbReadStatus3.Error THEN
	
			(*cannot read the status of axis3*)
			(*axis3 is an EPOS controller connected on the CAN bus*)
			(*maybe there is a CAN communication error, switch to state MAIN_CanError*)
			(*initializationState := MAIN_CanError;*)
			initializationState := INITIALIZE_CanError;
		
		ELSIF fbReadStatus3.Valid THEN
	
			(*we have read the status of axis3*)
	
			IF fbReadStatus3.Errorstop THEN
				(*the motor is in an error state*)
				(*call the MC_Reset function block to reset it*)
				initializationState := INITIALIZE_Reset3;
			ELSIF fbReadStatus3.Disabled THEN
				(*the motor is in the disabled state, nothing to do*)
				(*switch to MAIN_Init*)
				initializationState := INITIALIZE_Init;
			ELSIF fbReadStatus3.Stopping THEN
				(*the motor is stopping*)
				(*although we could simply wait, force the stop*)
				(*i.e. greater decceleration*)
				initializationState := INITIALIZE_Reset3;
			ELSIF fbReadStatus3.StandStill THEN
				(*the controller is not in an error state but it is enabled*)
				(*power off all controllers*)
				initializationState := INITIALIZE_PowerOff;
			ELSIF fbReadStatus3.DiscreteMotion THEN
				(*i.e. position control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;
			ELSIF fbReadStatus3.ContinuousMotion THEN
				(*i.e. velocity control*)
				(*stop motion on all controllers*)
				initializationState := INITIALIZE_Stop;		
			ELSIF fbReadStatus3.Homing THEN
				(*stop homing on all controllers*)
				initializationState := INITIALIZE_Stop;
			END_IF;
		ELSE
			(*status not updated yet*)
			;
		END_IF;

	ELSIF initializationState = INITIALIZE_PowerOff THEN

		(*Disable power on all controllers*)
		fbPower0(Axis := Axis0 , Enable := FALSE);
		fbPower1(Axis := Axis1 , Enable := FALSE);
		fbPower2(Axis := Axis2 , Enable := FALSE);
		fbPower3(Axis := Axis3 , Enable := FALSE);

		(*Switch state when all controllers are disabled*)
		IF fbPower0.Error OR fbPower1.Error OR fbPower2.Error OR fbPower3.Error THEN
	
			initializationState := INITIALIZE_Init;
	
		ELSIF ( NOT fbPower0.Status ) AND ( NOT fbPower1.Status ) AND ( NOT fbPower2.Status ) AND ( NOT fbPower3.Status ) THEN
			(*all controllers are disabled, switch to the next state*)
			initializationState := INITIALIZE_Init;
		END_IF;

	ELSIF initializationState = INITIALIZE_Reset0 THEN

		(*reset axis0*)

		fbReset0(Axis := Axis0 , Execute := FALSE);
		initializationState := INITIALIZE_Reset0_loop;

	ELSIF initializationState = INITIALIZE_Reset0_loop THEN

		fbReset0(Axis := Axis0 , Execute := TRUE);
	
		IF fbReset0.ERROR THEN
			(*check for can communication errors*)
			(*initializationState := MAIN_CanError;*)
			initializationState := INITIALIZE_CanError;
			;
		ELSIF fbReset0.DONE THEN
			(*done reseting*)
			initializationState := INITIALIZE_Init;
		END_IF;

	ELSIF initializationState = INITIALIZE_Reset1 THEN

		(*reset axis1*)

		fbReset1(Axis := Axis1 , Execute := FALSE);
		initializationState := INITIALIZE_Reset1_loop;

	ELSIF initializationState = INITIALIZE_Reset1_loop THEN

		fbReset1(Axis := Axis1 , Execute := TRUE);
	
		IF fbReset1.ERROR THEN
	
			(*this is wierd*)
			(*there is no reason why we cannot reset our controller*)
			(*axis1 is the EPOS P controller*)
			(*simply increment the internal error counter and try again*)
			internalErrorCounter := internalErrorCounter + 1;
			initializationState := INITIALIZE_Init;
		
		ELSIF fbReset1.DONE THEN
			(*done reseting*)
			initializationState := INITIALIZE_Init;
		END_IF;
		
		ELSIF initializationState = INITIALIZE_Reset2 THEN

		(*reset axis2*)

		fbReset2(Axis := Axis2 , Execute := FALSE);
		initializationState := INITIALIZE_Reset2_loop;

	ELSIF initializationState = INITIALIZE_Reset2_loop THEN

		fbReset2(Axis := Axis2 , Execute := TRUE);
	
		IF fbReset2.ERROR THEN
			(*check for can communication errors*)
			(*initializationState := MAIN_CanError;*)
			initializationState := INITIALIZE_CanError;
			;
		ELSIF fbReset2.DONE THEN
			(*done reseting*)
			initializationState := INITIALIZE_Init;
		END_IF;
	
	ELSIF initializationState = INITIALIZE_Reset3 THEN

		(*reset axis3*)

		fbReset3(Axis := Axis3 , Execute := FALSE);
		initializationState := INITIALIZE_Reset3_loop;
			
	ELSIF initializationState = INITIALIZE_Reset3_loop THEN

		fbReset3(Axis := Axis3 , Execute := TRUE);
	
		IF fbReset3.ERROR THEN
			(*check for can communication errors*)
			(*initializationState := MAIN_CanError;*)
			initializationState := INITIALIZE_CanError;
			;
		ELSIF fbReset3.DONE THEN
			(*done reseting*)
			initializationState := INITIALIZE_Init;
		END_IF;


	ELSIF initializationState = INITIALIZE_Stop THEN

		(*stop all axis*)

		fbAxisStop0(Axis := Axis0, Execute := FALSE, Deceleration := 10000);
		fbAxisStop1(Axis := Axis1, Execute := FALSE, Deceleration := 10000);
		fbAxisStop2(Axis := Axis2, Execute := FALSE, Deceleration := 10000);
		fbAxisStop3(Axis := Axis3, Execute := FALSE, Deceleration := 10000);
		initializationState := INITIALIZE_Stop_loop;

	ELSIF initializationState = INITIALIZE_Stop_loop THEN

		fbAxisStop0(Axis := Axis0, Execute := TRUE, Deceleration := 10000);
		fbAxisStop1(Axis := Axis1, Execute := TRUE, Deceleration := 10000);
		fbAxisStop2(Axis := Axis2, Execute := TRUE, Deceleration := 10000);
		fbAxisStop3(Axis := Axis3, Execute := TRUE, Deceleration := 10000);


		IF fbAxisStop0.Error OR fbAxisStop1.Error OR fbAxisStop2.Error OR fbAxisStop3.Error THEN

			(*there is an error while attempting to stop 1 or 2 or 3 or 4 of the controllers*)
			(*re-initialize the controllers*)
			initializationState:=INITIALIZE_Init;

		ELSIF fbAxisStop0.Done AND fbAxisStop1.DONE AND fbAxisStop2.Done AND fbAxisStop3.DONE THEN
			(*all controllers have stopped*)
			(*switch to state init*)
			initializationState := INITIALIZE_Init;

		END_IF;

	ELSIF initializationState = INITIALIZE_PowerOn THEN

		(*power on all controllers*)

		fbPower0(Axis := Axis0, Enable := TRUE);
		fbPower1(Axis := Axis1, Enable := TRUE);
		fbPower2(Axis := Axis2, Enable := TRUE);
		fbPower3(Axis := Axis3, Enable := TRUE);
	
		IF fbPower0.Error OR fbPower1.Error OR fbPower2.Error OR fbPower3.Error THEN
			(*there is an error while attempting to power on 1 or 2 or 3 or 4 of the controllers*)
			(*re-initialize the controllers*)
			initializationState:=INITIALIZE_Init;
		ELSIF fbPower0.Status AND fbPower1.Status AND fbPower2.Status AND fbPower3.Status THEN
		    (*all controllers are powered on*)
			(*go start the control sequence*)
			Done := TRUE;
		END_IF;
	ELSIF initializationState = INITIALIZE_CanError THEN
	
		fbCanRecover(Execute:=FALSE);
		initializationState := INITIALIZE_CanError_loop;
		
	ELSIF initializationState = INITIALIZE_CanError_loop THEN
		fbCanRecover(Execute:=TRUE);
		IF fbCanRecover.Error THEN
		
			lastErrorId:=fbCanRecover.ErrorId;
			internalErrorCounter := internalErrorCounter + 1;
			initializationState := INITIALIZE_Init;
		
		ELSIF fbCanRecover.Done THEN
			initializationState := INITIALIZE_Init;
		END_IF;
	END_IF; 
END_IF;






END_FUNCTION_BLOCK