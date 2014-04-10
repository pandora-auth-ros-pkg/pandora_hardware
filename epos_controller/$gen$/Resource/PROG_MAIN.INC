$INCLUDE_FILE


(* GLOBALE TYPEN *)

TYPE AXIS_REF:
    STRUCT
      AxisNo : USINT;
    END_STRUCT;
END_TYPE

TYPE MC_Direction: ( MCpositive,    (*0*)
					MCshortestway, (*1*)
					MCnegative,    (*2*)
					MCcurrent      (*3*)
					);
END_TYPE



(* PROJEKT TYPEN *)

TYPE MAIN_STATE :(
	(*initialize both controllers to power on - stand still*)
	(*uses function block FB_INITIALIZE*)
	MAIN_Init,					(* 0 *)
	MAIN_Init_loop,				(* 1 *)
	
	(*Resets the 3s timeout timer to zero*)
	MAIN_ResetTimer,			(* 2 *)
	
	(*Reads the control word from EPOS*)
	MAIN_Read,					(* 3 *)
	MAIN_Read_loop,				(* 4 *)
	
	(*Reads the actual velocity of EPOS P and stores it to a local variable*)
	MAIN_Read_Velocity,			(* 5 *)
	MAIN_Read_Velocity_loop,	(* 6 *)
	
	(*Reads the actual current of EPOS P and stores it to a local variable*)
	MAIN_Read_Current,			(* 7 *)
	MAIN_Read_Current_loop,		(* 8 *)

	(*Writes the actual velocity of EPOS P from the local variable to EPOS object dictionary*)
	MAIN_Write_Velocity,		(* 9 *)
	MAIN_Write_Velocity_loop,	(* 10 *)
	
	(*Writes the actual current of EPOS P from the local variable to EPOS object dictionary*)
	MAIN_Write_Current,			(* 11 *)
	MAIN_Write_Current_loop,	(* 12 *)

	(*updatess the timeout timer*)
	(*if the timer reaches 3s switches to state halt*)
	MAIN_UpdateTimer,			(* 13 *)
	
	(*Executes the move specified by the control word*)
	MAIN_StateAxis_Move,		(* 14 *)
	
	(*Resets the control word in order to accept a new command*)
	MAIN_StateWrite,			(* 15 *)
	
	(*halts both motors*)
	MAIN_StateHalt0,			(* 16 *)
	MAIN_StateHalt1,			(* 17 *)
	
	(*attempts an error recovery if there is CAN error*)
	(*uses function block FB_CanRecover*)
	MAIN_CanError,				(* 18 *)
	MAIN_CanError_loop			(* 19 *)
	
) := MAIN_Init;
END_TYPE



(* GLOBALE PROTOTYPEN *)

FUNCTION_BLOCK MC_Stop 
    VAR_INPUT
      Execute : BOOL;
      Deceleration : UDINT;
    END_VAR 
    VAR_IN_OUT
      Axis : AXIS_REF;
    END_VAR 
    VAR_OUTPUT
      Done : BOOL;
      Error : BOOL;
      ErrorID : DINT;
    END_VAR 
  END_FUNCTION_BLOCK

FUNCTION_BLOCK MC_MoveVelocity 
    VAR_INPUT
      Execute : BOOL;
      Velocity : UDINT;
      Acceleration : UDINT;
      Deceleration : UDINT;
      Direction : MC_Direction;
    END_VAR 
    VAR_IN_OUT
      Axis : AXIS_REF;
    END_VAR 
    VAR_OUTPUT
      InVelocity : BOOL;
      Abort : BOOL;
      Error : BOOL;
      ErrorID : DINT;
    END_VAR 
  END_FUNCTION_BLOCK

FUNCTION_BLOCK MC_ReadActualVelocity 
    VAR_INPUT
      Enable : BOOL;
    END_VAR 
    VAR_IN_OUT
      Axis : AXIS_REF;
    END_VAR 
    VAR_OUTPUT
      Done : BOOL;
      Error : BOOL;
      ErrorID : DINT;
      Velocity : DINT;
    END_VAR 
  END_FUNCTION_BLOCK

FUNCTION_BLOCK MC_ReadStatus 
    VAR_INPUT
      Enable : BOOL;
    END_VAR 
    VAR_IN_OUT
      Axis : AXIS_REF;
    END_VAR 
    VAR_OUTPUT
      Valid : BOOL;
      Error : BOOL;
      ErrorID : DINT;
      Errorstop : BOOL;
      Disabled : BOOL;
      Stopping : BOOL;
      StandStill : BOOL;
      DiscreteMotion : BOOL;
      ContinuousMotion : BOOL;
      Homing : BOOL;
    END_VAR 
  END_FUNCTION_BLOCK

FUNCTION_BLOCK MC_Reset 
    VAR_INPUT
      Execute : BOOL;
    END_VAR 
    VAR_IN_OUT
      Axis : AXIS_REF;
    END_VAR 
    VAR_OUTPUT
      Done : BOOL;
      Error : BOOL;
      ErrorID : DINT;
    END_VAR 
  END_FUNCTION_BLOCK

FUNCTION_BLOCK MC_Power 
    VAR_INPUT
      Enable : BOOL;
    END_VAR 
    VAR_IN_OUT
      Axis : AXIS_REF;
    END_VAR 
    VAR_OUTPUT
      Status : BOOL;
      Error : BOOL;
      ErrorID : DINT;
    END_VAR 
  END_FUNCTION_BLOCK



(* PROJEKT PROTOTYPEN *)

$INCLUDE "K:\DOCUMENTS AND SETTINGS\EPOS\DESKTOP\HOPE\$GEN$\FB_CANRECOVER.PTT"
$INCLUDE "K:\DOCUMENTS AND SETTINGS\EPOS\DESKTOP\HOPE\$GEN$\FB_INITIALIZE.PTT"
$INCLUDE "K:\DOCUMENTS AND SETTINGS\ALL USERS\APPLICATION DATA\INFOTEAM SOFTWARE\OPENPCS2008\OPENPCS.520\LIB\MAXONUTILITYLIB\$GEN$\MU_SETOBJECT.PTT"
$INCLUDE "K:\DOCUMENTS AND SETTINGS\ALL USERS\APPLICATION DATA\INFOTEAM SOFTWARE\OPENPCS2008\OPENPCS.520\LIB\MAXONUTILITYLIB\$GEN$\MU_GETOBJECT.PTT"
$INCLUDE "K:\DOCUMENTS AND SETTINGS\EPOS\DESKTOP\HOPE\$GEN$\FB_MAIN_STATEWRITE.PTT"
$INCLUDE "K:\DOCUMENTS AND SETTINGS\EPOS\DESKTOP\HOPE\$GEN$\FB_MAIN_STATEREAD.PTT"


(* EXTERNE POEs *)



$END_INCLUDE_FILE
