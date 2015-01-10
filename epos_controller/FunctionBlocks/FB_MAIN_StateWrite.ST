FUNCTION_BLOCK FB_MAIN_StateWrite
(********************************************************************************************************
**  maxon motor ag
**  All rights reserved
*********************************************************************************************************
**  Project		:  ObjectDictionary Access
**  File	    :  FB MAIN State Object Writing
**  Description :  Implementation of State 'Object Writing' in Main State Machine
**  Creation    :  15.02.2008, BRE
**  Modification:  15.02.2008, BRE, Initial Version
*********************************************************************************************************)

VAR_EXTERNAL

END_VAR

VAR_INPUT
    Execute                   : BOOL;
	CurrentValue			  : UDINT;
END_VAR                         
                                
VAR_IN_OUT                        
    Axis                      : Axis_REF;
END_VAR                          
                                
VAR_OUTPUT                      
    Done                      : BOOL;
    Error                     : BOOL;
	ErrorID                   : DINT;
END_VAR                         
                                 
VAR                              
    fbSetObjectFresh		  : MU_SetObject;
	
	setValue				  : UDINT;
END_VAR
setValue := CurrentValue MOD 16#80000000;

(* Write object "Fresh" *) 
fbSetObjectFresh(Axis := Axis, Execute := Execute, Index := 16#200C, 
                          Subindex := 16#01, Value := setValue);
						  
(* Output Variables *)
IF fbSetObjectFresh.Error THEN
       Done := FALSE;
	   Error := TRUE;
	   ErrorID := fbSetObjectFresh.ErrorID;	 
ELSE 
       Done := fbSetObjectFresh.Done;	 
	   Error := FALSE;
	   ErrorID := 0;
END_IF;












END_FUNCTION_BLOCK