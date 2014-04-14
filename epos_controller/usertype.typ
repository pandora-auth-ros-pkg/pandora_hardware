[IEC_IEC_NORM]
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


