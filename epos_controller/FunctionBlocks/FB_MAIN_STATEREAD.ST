FUNCTION_BLOCK FB_MAIN_STATEREAD
(********************************************************************************************************
**  maxon motor ag
**  All rights reserved
*********************************************************************************************************
**  Project		:  ObjectDictionary Access
**  File	    :  FB MAIN State Object Reading
**  Description :  Implementation of State 'Object Reading' in Main State Machine
**  Creation    :  15.02.2008, BRE
**  Modification:  15.02.2008, BRE, Initial Version
*********************************************************************************************************)

VAR_EXTERNAL

END_VAR
	
VAR_INPUT
    Execute                   : BOOL;
END_VAR

VAR_IN_OUT
    Axis                      : Axis_REF;
END_VAR

VAR_OUTPUT
    Done                      : BOOL;
	Done1					  : BOOL;
    Error                     : BOOL;
	ErrorID                   : DINT;
	
	velocityWord			  : UDINT;
	
END_VAR                         
                                 
VAR                              
	fbGetObjectVelocity          : MU_GetObject;      (* Create Instance of MU_GetObject *)

END_VAR
(* Read object "Fresh" *)
fbGetObjectVelocity(Axis := Axis, Enable := Execute, Index := 16#200C, Subindex := 16#01);  

IF fbGetObjectVelocity.Done AND (NOT fbGetObjectVelocity.Error) THEN
		
		IF (fbGetObjectVelocity.Value >= 16#8000000) THEN
			velocityWord :=  fbGetObjectVelocity.Value;
		END_IF;                	                        
END_IF;

(* Output Variables *)
IF  fbGetObjectVelocity.Error THEN 
       Done := FALSE;
	   Done1:= FALSE;
	   Error := TRUE;
	   ErrorID := fbGetObjectVelocity.ErrorID;
ELSE
       Done := fbGetObjectVelocity.Done;  
	   Done1:= Done AND (fbGetObjectVelocity.Value >= 16#80000000); 	   
       Error := FALSE;
	   ErrorID := 0;
END_IF;


















END_FUNCTION_BLOCK