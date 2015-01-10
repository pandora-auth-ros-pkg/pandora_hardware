[IEC_IEC_NORM]
TYPE MAIN_STATE :(
	(*initialize all controllers to power on - stand still*)
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
	
	(*halts all motors*)
	MAIN_StateHalt0,			(* 16 *)
	MAIN_StateHalt1,			(* 17 *)
	
	(*attempts an error recovery if there is CAN error*)
	(*uses function block FB_CanRecover*)
	MAIN_CanError,				(* 18 *)
	MAIN_CanError_loop,			(* 19 *)
	
	(*Intercepts non-CAN errors coming from other FBs to MAIN_Init*)
	(*uses FB_ErrorWarning to write error codes to EPOS Object Dictionary*)
	MAIN_Error,					(* 20 *)
	MAIN_Error_loop				(* 21 *)
	) := MAIN_Init;
END_TYPE

TYPE INITIALIZE_STATE :(
	(*Initialize both axis*)
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
	(*stop all axis*)
	INITIALIZE_Stop,		(* 15 *)
	(*loop until all axis have stoped*)
	INITIALIZE_Stop_loop,	(* 16 *)
	INITIALIZE_StateHalt0,	(* 17 *)
	INITIALIZE_CanError,    (* 18 *)
	INITIALIZE_CanError_loop(* 19 *)
) := INITIALIZE_Init;
END_TYPE


