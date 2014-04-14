[IEC_IEC_NORM]
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


