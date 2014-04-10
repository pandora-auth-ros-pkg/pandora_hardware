PROGRAM PROG_MAIN
VAR_EXTERNAL
	Axis0 				                    : Axis_REF;                    (* Axis definitions, Node 2, Right Wheel *)
	Axis1 				                    : Axis_REF;					   (* Axis definitions, Node 1, Left Wheel *)	
	eStateMain                              : MAIN_STATE;                  (* Process-State *)
END_VAR                                   
                                           
VAR_GLOBAL                                
                                            
END_VAR                                    
                                             
VAR                                        
	fbPower0							: MC_Power;
	fbPower1							: MC_Power;
	fbReset0							: MC_Reset;
	fbReset1							: MC_Reset;
	
	fbReadStatus0						: MC_ReadStatus;
	fbReadStatus1						: MC_ReadStatus;
	fbReadActualVelocity0				: MC_ReadActualVelocity;
	fbReadActualVelocity1				: MC_ReadActualVelocity;
	
	fbAxisMove0              			: MC_MoveVelocity;
	fbAxisStop0							: MC_Stop;
	fbAxisMove1              			: MC_MoveVelocity;
	fbAxisStop1							: MC_Stop;
	
	fbReadObject						: FB_MAIN_StateRead;
	fbWriteObject						: FB_MAIN_StateWrite;
	
	fbGetObjectEncoder		  			: MU_GetObject;
	fbSetObjectEncoder		  			: MU_SetObject;
	
	fbTimer								: TON;
	
	extractedValue						: UDINT := 0;
	velByte1							: UDINT;
	velByte2							: UDINT;
	leftSign							: UDINT;
	leftVeloc							: UDINT;
	freshByte							: UDINT;
	rightSign							: UDINT;
	rightVeloc							: UDINT;
	
	maxVelocity							: UDINT := 5500; 
	
	
	udEncoderTemp						: UDINT := 0;  
	dEncoderValue						: DINT := 0;       
									
	oExecuteReset0						: BOOL;
	oExecuteReset1						: BOOL;
	oExecutePower0						: BOOL;
	oExecutePower1						: BOOL;
	
	oExecuteTimerStart					: BOOL;
	oExecuteObjectReading				: BOOL;
	oExecuteObjectWriting				: BOOL;
	
	oExecuteObjectReadingEncoder		: BOOL;
	oExecuteObjectWritingEncoder		: BOOL;
	oExecuteAxisMove					: BOOL;
	
	
	fbGetVelocity			: MU_GetObject;
	fbGetCurrent			: MU_GetObject;
    fbWriteVelocity			: MU_SetObject;
	fbWriteCurrent			: MU_SetObject;
	
	velocityActualValue : UDINT;
	currentActualValue : UDINT;
	internalErrorCounter : UDINT;
	
	fbInitialize : FB_INITIALIZE;
	fbCanRecover : FB_CanRecover;
	
END_VAR
IF eStateMain = MAIN_Init THEN

	fbInitialize(Execute:=FALSE);
	eStateMain := MAIN_Init_loop;
ELSIF eStateMain = MAIN_Init_loop THEN

	fbInitialize(Execute:=TRUE);
	IF fbInitialize.Error THEN
		(*+_+*)
		(*Hope, this never happens... *)
		eStateMain := MAIN_Init;
	ELSIF fbInitialize.Done THEN
		eStateMain := MAIN_ResetTimer;
	END_IF;

ELSIF eStateMain = MAIN_ResetTimer THEN

	(*reset the timer back to zero*)

	fbTimer(In := false , Pt := t#3s);
	
	(*read axis1 actual velocity*)
	eStateMain := MAIN_Read_Velocity;

	
ELSIF eStateMain = MAIN_Read_Velocity THEN

	(*set enable to low for triggering*)
	fbGetVelocity(Axis := Axis1, Enable := FALSE, Index := 16#2028, Subindex := 16#00);
	eStateMain := MAIN_Read_Velocity_loop;

ELSIF eStateMain = MAIN_Read_Velocity_loop THEN

	(*trigger function block*)
	fbGetVelocity(Axis := Axis1, Enable := TRUE, Index := 16#2028, Subindex := 16#00);
	IF(fbGetVelocity.Error) THEN
		internalErrorCounter := internalErrorCounter + 1;
		eStateMain := MAIN_Init;
	ELSIF fbGetVelocity.Done THEN 
		(*store velocity actual value to a global variable*)
		velocityActualValue := fbGetVelocity.Value;
		if(velocityActualValue>2147483647)then
			velocityActualValue := velocityActualValue+20000;
		end_if;
		(*go to state write velocity*)
		(*this state will write the velocity value to the EPOS object dictionary*)
		eStateMain := MAIN_Write_Velocity;
	END_IF;

ELSIF eStateMain = MAIN_Write_Velocity THEN

	fbWriteVelocity(Axis := Axis0, Execute := FALSE, Index := 16#206B, 
                          Subindex := 16#00, Value := velocityActualValue);
	eStateMain := MAIN_Write_Velocity_loop;
	
ELSIF eStateMain = MAIN_Write_Velocity_loop THEN

	fbWriteVelocity(Axis := Axis0, Execute := TRUE, Index := 16#206B, 
                          Subindex := 16#00, Value := velocityActualValue);
	IF fbWriteVelocity.Error THEN
		eStateMain := MAIN_CanError;
	ELSIF fbWriteVelocity.Done THEN
		eStateMain := MAIN_Read_Current;
	END_IF;

ELSIF eStateMain = MAIN_Read_Current THEN

	fbGetCurrent(Axis := Axis1, Enable := FALSE, Index := 16#2027, Subindex := 16#00);
	eStateMain := MAIN_Read_Current_loop;

ELSIF eStateMain = MAIN_Read_Current_loop THEN

	fbGetCurrent(Axis := Axis1, Enable := TRUE, Index := 16#2027, Subindex := 16#00);
	if(fbGetCurrent.Error) then
		eStateMain:=MAIN_Init;
	elsif(fbGetCurrent.Done) then
		currentActualValue:=fbGetCurrent.Value;
		if(currentActualValue>2147483647)then
			currentActualValue := currentActualValue+20000;
		end_if;
		eStateMain:=MAIN_Write_Current;
	end_if;
	
ELSIF eStateMain = MAIN_Write_Current THEN

	fbWriteCurrent(Axis := Axis0, Execute := FALSE, Index := 16#2030, 
                          Subindex := 16#00, Value := currentActualValue);
	eStateMain := MAIN_Write_Current_loop;

ELSIF eStateMain = MAIN_Write_Current_loop THEN

	fbWriteCurrent(Axis := Axis0, Execute := TRUE, Index := 16#2030, 
                          Subindex := 16#00, Value := currentActualValue);
	if(fbWriteCurrent.Error) then
		eStateMain:=MAIN_Init;
	elsif fbWriteCurrent.Done then
		eStateMain:=MAIN_Read;
	end_if;

ELSIF eStateMain = MAIN_Read THEN

	(*read the control word from EPOS*)

	fbReadObject(Axis := Axis0 , Execute := FALSE);
	eStateMain := MAIN_Read_loop;

ELSIF eStateMain = MAIN_Read_loop THEN

	fbReadObject(Axis := Axis0 , Execute := TRUE);
	
	IF fbReadObject.Error THEN
		eStateMain := MAIN_Init;
		;
	ELSIF fbReadObject.Done AND fbReadObject.Done1 THEN
		(*Velocity Fresh*)
		
		extractedValue := fbReadObject.velocityWord;

		(* Low Byte *)
		velByte1 := extractedValue MOD 16#00010000;
		leftSign := ( velByte1 / 16#00004000 ) MOD 16#00000002;
		leftVeloc := velByte1 MOD 16#00004000;

		(* High Byte *)
		velByte2 := extractedValue / 16#00010000;
		rightSign := ( velByte2 / 16#00004000 ) MOD 16#00000002;
		rightVeloc := velByte2 MOD 16#00004000; 
		eStateMain := MAIN_StateWrite;
		
		fbTimer(In := false , Pt := t#3s);
		
	ELSIF fbReadObject.Done THEN
		eStateMain := MAIN_UpdateTimer;
	END_IF;

ELSIF eStateMain = MAIN_UpdateTimer THEN

	fbTimer(In := TRUE , Pt := t#3s);

	IF fbTimer.Q THEN
		eStateMain := MAIN_StateHalt0;
	ELSE		
		eStateMain := MAIN_Read_Velocity;
	END_IF;
	  
ELSIF eStateMain = MAIN_StateWrite THEN

	  fbWriteObject(Axis := Axis0, Execute := oExecuteObjectWriting, CurrentValue := extractedValue);                  
	  oExecuteObjectWriting := TRUE;
	 
	  IF fbWriteObject.Done AND (NOT fbWriteObject.Error) THEN				                           
	        oExecuteObjectWriting := FALSE; 
			
			fbAxisMove0(Axis := Axis0, Execute := FALSE, Velocity := 1000 , Acceleration := 100 , Deceleration := 100 , Direction := MCpositive);
        	fbAxisMove1(Axis := Axis1, Execute := FALSE, Velocity := 1000 , Acceleration := 100 , Deceleration := 100 , Direction := MCpositive);     					
			fbAxisStop0(Axis := Axis0, Execute := FALSE);
			fbAxisStop1(Axis := Axis1, Execute := FALSE);				
			fbWriteObject(Axis := Axis0, Execute := FALSE);	
			oExecuteAxisMove := FALSE;
	        eStateMain := MAIN_StateAxis_Move;                                     			
	  END_IF;
	  
ELSIF eStateMain = MAIN_StateHalt0 THEN

	fbAxisStop0(Axis := Axis0 , Execute := FALSE , Deceleration := 10000);
	fbAxisStop1(Axis := Axis1 , Execute := FALSE , Deceleration := 10000);

	eStateMain := MAIN_StateHalt1;

ELSIF eStateMain = MAIN_StateHalt1 THEN


	fbReadStatus0(Axis := Axis0 , Enable := TRUE);
	fbReadStatus1(Axis := Axis1 , Enable := TRUE);
	

	IF fbReadStatus0.Error OR fbReadStatus1.Error THEN

		(*cannot read the state of one or both controllers*)
		(*go to state canError to check for errors*)
		eStateMain := MAIN_CanError;

	ELSIF fbReadStatus0.Valid AND fbReadStatus1.Valid THEN


		IF fbAxisStop0.Error THEN
			eStateMain := MAIN_CanError;
		ELSIF NOT fbReadStatus0.StandStill THEN
			fbAxisStop0(Axis := Axis0 , Execute := TRUE , Deceleration := 10000);
		END_IF;
	
		IF fbAxisStop1.Error THEN
			eStateMain := MAIN_CanError;
		ELSIF NOT fbReadStatus1.StandStill THEN
			fbAxisStop1(Axis := Axis1 , Execute := TRUE , Deceleration := 10000);
		END_IF;
	

		IF fbReadStatus0.StandStill AND fbReadStatus1.StandStill THEN
		
			oExecuteAxisMove := FALSE;
		  	eStateMain := MAIN_ResetTimer; 	
		
		END_IF;

	END_IF;

ELSIF eStateMain = MAIN_StateAxis_Move THEN
	  
	  (* Limiter *)
	  IF (leftVeloc > maxVelocity) THEN
	  	leftVeloc := maxVelocity;
	  END_IF;
	  
	  (* Limiter *)
	  IF (rightVeloc > maxVelocity) THEN
	  	rightVeloc := maxVelocity;
	  END_IF;
		
		
	  IF (leftVeloc = 0) THEN
	  	fbAxisStop0(Axis := Axis0 , Execute := oExecuteAxisMove , Deceleration := 10000);
	  ELSIF (leftSign = 1) THEN
      	fbAxisMove0(Axis := Axis0, Execute := oExecuteAxisMove, Velocity := leftVeloc,              
                Acceleration := 10000, Deceleration := 10000, 
				Direction := MCnegative);
	  ELSE
	  	fbAxisMove0(Axis := Axis0, Execute := oExecuteAxisMove, Velocity := leftVeloc,              
                Acceleration := 10000, Deceleration := 10000, 
				Direction := MCpositive);
	  END_IF;  
	  
	  
	  IF (rightVeloc = 0) THEN
	  	fbAxisStop1(Axis := Axis1 , Execute := oExecuteAxisMove , Deceleration := 10000);
	  ELSIF (rightsign = 1) THEN
	  	fbAxisMove1(Axis := Axis1, Execute := oExecuteAxisMove, Velocity := rightVeloc,              
                Acceleration := 10000, Deceleration := 10000, 
				Direction := MCnegative);
	  ELSE		
	  	fbAxisMove1(Axis := Axis1, Execute := oExecuteAxisMove, Velocity := rightVeloc ,Acceleration := 10000, Deceleration := 10000, Direction := MCpositive);
	  END_IF;
	  
	  oExecuteAxisMove := TRUE;
	  

	  fbReadStatus0(Axis := Axis0 , Enable := TRUE);
	  fbReadStatus1(Axis := Axis1 , Enable := TRUE);
	  
	  fbReadActualVelocity0(Axis := Axis0 , Enable := TRUE);
	  fbReadActualVelocity1(Axis := Axis1 , Enable := TRUE);
			
	  IF fbAxisMove0.Error THEN   
	  	eStateMain := MAIN_Init;  
	  ELSIF fbAxisStop0.Error THEN   
	  	eStateMain := MAIN_Init;     
	  ELSIF fbAxisMove1.Error THEN
	  	eStateMain := MAIN_Init; 
	  ELSIF fbAxisStop1.Error THEN
	  	eStateMain := MAIN_Init;    
	  ELSIF (fbAxisMove0.InVelocity OR fbAxisStop0.Done ) AND (fbAxisMove1.InVelocity OR fbAxisStop1.Done ) AND 
	  			fbReadStatus0.Valid AND (fbReadStatus0.ContinuousMotion OR fbReadStatus0.StandStill) AND 
				fbReadStatus1.Valid AND (fbReadStatus1.ContinuousMotion OR fbReadStatus1.StandStill) THEN
				
		oExecuteAxisMove := FALSE;
	  	eStateMain := MAIN_Read_Velocity;  
	  END_IF; 	

ELSIF eStateMain = MAIN_CanError THEN

	fbCanRecover(Execute:=FALSE);
	eStateMain := MAIN_CanError_loop;
	
ELSIF eStateMain = MAIN_CanError_loop THEN

	fbCanRecover(Execute:=TRUE);
	IF fbCanRecover.Done THEN
		eStateMain:=MAIN_Init;
	END_IF;

END_IF; 








END_PROGRAM