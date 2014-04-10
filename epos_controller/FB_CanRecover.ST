FUNCTION_BLOCK FB_CanRecover
VAR_EXTERNAL
	Axis0 				                    : Axis_REF;                    (* Axis definitions, Node 2, Right Wheel, EPOS controller *)
	Axis1 				                    : Axis_REF;					   (* Axis definitions, Node 1, Left Wheel *)	
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
	canState	: CAN_RECOVER_STATE; 
	
	fbPower0 : MC_Power;
	fbPower1 : MC_Power; 
	
	fbTimer : TON;
	
	fbReset0 : MC_Reset;
	fbReset1 : MC_Reset;

END_VAR
IF Execute = FALSE THEN

	Done:=FALSE;
	lastErrorId:=0;
	internalErrorCounter:=0;
	canState:=CAN_CanError;
	
ELSE

	IF canState = CAN_CanError THEN

		IF internalErrorCounter > 5 THEN
		
			Error := TRUE;
			ErrorId := LastErrorId;
		ELSE
		

			canState := CAN_PowerOffGracefully;
		END_IF;

	ELSIF canState = CAN_PowerOffGracefully THEN
		(*attempt to stop both controllers*)


		(*Disable power on both controllers*)
		fbPower0(Axis := Axis0 , Enable := FALSE);
		fbPower1(Axis := Axis1 , Enable := FALSE);

		(*Switch state when both controllers are disabled*)
		(*of have an error*)
		IF ( ( NOT fbPower0.Status ) OR fbPower0.Error ) AND
	       ( ( NOT fbPower1.Status ) OR fbPower1.Error ) THEN
			(*both controllers are disabled or in an error state, switch to the next state*)
			canState := CAN_Sleep;
		END_IF;

	ELSIF canState = CAN_Sleep THEN

		fbTimer(In := false , Pt := t#1s);
		canState := CAN_SleepWait;
	
	ELSIF canState = CAN_SleepWait THEN

		fbTimer(In := true , Pt := t#1s);

		IF fbTimer.Q THEN
			canState := CAN_CanReset;
		END_IF;

	ELSIF canState = CAN_CanReset THEN

		fbReset0(Axis := Axis0 , Execute := FALSE);
		fbReset1(Axis := Axis1 , Execute := FALSE);
		canState := CAN_CanReset_loop;

	ELSIF canState = CAN_CanReset_loop THEN

		fbReset0(Axis := Axis0 , Execute := TRUE);
		fbReset1(Axis := Axis1 , Execute := TRUE);
	
		IF fbReset0.Error OR fbReset1.Error THEN
			
			IF fbReset0.Error THEN
				lastErrorId := fbReset0.ErrorId;
			ELSE
				lastErrorId := fbReset1.ErrorId;
			END_IF;
			
			internalErrorCounter := internalErrorCounter +1;
			canState :=CAN_CanError;
	
		ELSIF fbReset0.DONE AND fbReset1.DONE THEN
			Done := TRUE;
			(*canState := CAN_Init;*)
		END_IF;
	END_IF;
	
END_IF;
END_FUNCTION_BLOCK