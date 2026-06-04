/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Tustin_Filter.c
 *
 * Code generated for Simulink model 'Tustin_Filter'.
 *
 * Model version                  : 1.8
 * Simulink Coder version         : 25.2 (R2025b) 28-Jul-2025
 * C/C++ source code generated on : Sun Apr  5 10:31:33 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Tustin_Filter.h"
#include "rtwtypes.h"

/* Block states (default storage) */
DW_Tustin_Filter_T Tustin_Filter_DW;

/* External inputs (root inport signals with default storage) */
ExtU_Tustin_Filter_T Tustin_Filter_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_Tustin_Filter_T Tustin_Filter_Y;

/* Real-time model */
static RT_MODEL_Tustin_Filter_T Tustin_Filter_M_;
RT_MODEL_Tustin_Filter_T *const Tustin_Filter_M = &Tustin_Filter_M_;

/* Model step function */
void Tustin_Filter_step(void)
{
  real_T DiscreteTransferFcn_tmp;

  /* DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' incorporates:
   *  Inport: '<Root>/RAW'
   */
  DiscreteTransferFcn_tmp = (Tustin_Filter_U.RAW -
    Tustin_Filter_P.DiscreteTransferFcn_DenCoef[1] *
    Tustin_Filter_DW.DiscreteTransferFcn_states) /
    Tustin_Filter_P.DiscreteTransferFcn_DenCoef[0];

  /* Outport: '<Root>/Filtered' incorporates:
   *  DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
   */
  Tustin_Filter_Y.Filtered = Tustin_Filter_P.DiscreteTransferFcn_NumCoef[0] *
    DiscreteTransferFcn_tmp + Tustin_Filter_P.DiscreteTransferFcn_NumCoef[1] *
    Tustin_Filter_DW.DiscreteTransferFcn_states;

  /* Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' */
  Tustin_Filter_DW.DiscreteTransferFcn_states = DiscreteTransferFcn_tmp;
}

/* Model initialize function */
void Tustin_Filter_initialize(void)
{
  /* InitializeConditions for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' */
  Tustin_Filter_DW.DiscreteTransferFcn_states =
    Tustin_Filter_P.DiscreteTransferFcn_InitialStat;
}

/* Model terminate function */
void Tustin_Filter_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
