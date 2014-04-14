$INCLUDE_FILE


(* GLOBALE TYPEN *)

TYPE AXIS_REF:
    STRUCT
      AxisNo : USINT;
    END_STRUCT;
END_TYPE



(* PROJEKT TYPEN *)

TYPE CAN_RECOVER_STATE :(
	CAN_CanError,
	CAN_PowerOffGracefully,
	CAN_Sleep,
	CAN_CanReset,
	CAN_CanReset_loop,
	CAN_SleepWait
) := CAN_CanError;
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



(* EXTERNE POEs *)



$END_INCLUDE_FILE
