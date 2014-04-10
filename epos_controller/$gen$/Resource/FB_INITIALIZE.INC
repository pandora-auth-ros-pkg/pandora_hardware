$INCLUDE_FILE


(* GLOBALE TYPEN *)

TYPE AXIS_REF:
    STRUCT
      AxisNo : USINT;
    END_STRUCT;
END_TYPE



(* PROJEKT TYPEN *)

TYPE INITIALIZE_STATE :(
	(*Initialize both axis*)
	INITIALIZE_Init,		(* 0 *)
	(*check the status of axis0*)
	INITIALIZE_CheckAxis0,	(* 1 *)
	(*check the status of axis1*)
	INITIALIZE_CheckAxis1,	(* 2 *)
	(*power of both axis*)
	INITIALIZE_PowerOff,	(* 3 *)
	(*reset axis0*)
	INITIALIZE_Reset0,		(* 4 *)
	(*loop until axis0 is reset*)
	INITIALIZE_Reset0_loop,	(* 5 *)
	(*reset axis1*)
	INITIALIZE_Reset1,		(* 6 *)
	(*wait until axis1 is reset*)
	INITIALIZE_Reset1_loop,	(* 7 *)
	(*power on both axis*)
	INITIALIZE_PowerOn,		(* 8 *)
	(*stop both axis*)
	INITIALIZE_Stop,		(* 9 *)
	(*loop until both axis have stoped*)
	INITIALIZE_Stop_loop,	(* 10 *)
	INITIALIZE_StateHalt0,	(* 11 *)
	INITIALIZE_CanError,
	INITIALIZE_CanError_loop
) := INITIALIZE_Init;
END_TYPE



(* GLOBALE PROTOTYPEN *)

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



(* PROJEKT PROTOTYPEN *)

$INCLUDE "K:\DOCUMENTS AND SETTINGS\EPOS\DESKTOP\HOPE\$GEN$\FB_CANRECOVER.PTT"


(* EXTERNE POEs *)



$END_INCLUDE_FILE
