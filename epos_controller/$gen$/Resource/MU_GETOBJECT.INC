$INCLUDE_FILE


(* GLOBALE TYPEN *)

TYPE AXIS_REF:
    STRUCT
      AxisNo : USINT;
    END_STRUCT;
END_TYPE



(* PROJEKT TYPEN *)



(* GLOBALE PROTOTYPEN *)

FUNCTION_BLOCK MC_ReadParameter 
    VAR_INPUT
      Enable : BOOL;
      ParameterNumber : UDINT;
    END_VAR 
    VAR_IN_OUT
      Axis : AXIS_REF;
    END_VAR 
    VAR_OUTPUT
      Done : BOOL;
      Error : BOOL;
      ErrorID : DINT;
      Value : UDINT;
    END_VAR 
  END_FUNCTION_BLOCK



(* PROJEKT PROTOTYPEN *)



(* EXTERNE POEs *)



$END_INCLUDE_FILE
