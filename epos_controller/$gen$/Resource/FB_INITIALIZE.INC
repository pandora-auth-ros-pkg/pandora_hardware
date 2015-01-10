$INCLUDE_FILE


(* GLOBALE TYPEN *)

TYPE AXIS_REF:
    STRUCT
      AxisNo : USINT;
    END_STRUCT;
END_TYPE



(* PROJEKT TYPEN *)

TYPE INITIALIZE_STATE :(
	(*Initialize all axis*)
	INITIALIZE_Init,		(* 0 *)
	(*check the status of axis0*)
	INITIALIZE_CheckAxis0,	(* 1 *)
	(*check the status of axis1*)
	INITIALIZE_CheckAxis1,	(* 2 *)
	(*check the status of axis2*)
	INITIALIZE_CheckAxis2,  (* 3 *)
	(*check the status of axis3*)
	INITIALIZE_CheckAxis3,  (* 4 *)
	(*power of both axis*)
	INITIALIZE_PowerOff,	(* 5 *)
	(*reset axis0*)
	INITIALIZE_Reset0,		(* 6 *)
	(*loop until axis0 is reset*)
	INITIALIZE_Reset0_loop,	(* 7 *)
	(*reset axis1*)
	INITIALIZE_Reset1,		(* 8 *)
	(*wait until axis1 is reset*)
	INITIALIZE_Reset1_loop,	(* 9 *)
	(*reset axis2*)
	INITIALIZE_Reset2,      (* 10 *)
	(*loop until axis2 is reset*)
	INITIALIZE_Reset2_loop, (* 11 *)
	(*reset axis3*)
	INITIALIZE_Reset3,      (* 12 *)
	(*loop until axis3 is reset*)
	INITIALIZE_Reset3_loop, (* 13 *)
	(*power on all axis*)
	INITIALIZE_PowerOn,		(* 14 *)
	(*stop both axis*)
	INITIALIZE_Stop,		(* 15 *)
	(*loop until all axis have stoped*)
	INITIALIZE_Stop_loop,	(* 16 *)
	INITIALIZE_StateHalt0,	(* 17 *)
	INITIALIZE_CanError,    (* 18 *)
	INITIALIZE_CanError_loop(* 19 *)
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

$INCLUDE "K:\DOCUMENTS AND SETTINGS\EPOS\DESKTOP\ANNA\ANNAST\$GEN$\FB_CANRECOVER.PTT"


(* EXTERNE POEs *)



$END_INCLUDE_FILE
