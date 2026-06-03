/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: Tustin_Filter.h
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

#ifndef Tustin_Filter_h_
#define Tustin_Filter_h_
#ifndef Tustin_Filter_COMMON_INCLUDES_
#define Tustin_Filter_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "math.h"
#endif                                 /* Tustin_Filter_COMMON_INCLUDES_ */

#include "Tustin_Filter_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T DiscreteTransferFcn_states;   /* '<S1>/Discrete Transfer Fcn' */
} DW_Tustin_Filter_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T RAW;                          /* '<Root>/RAW' */
} ExtU_Tustin_Filter_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T Filtered;                     /* '<Root>/Filtered' */
} ExtY_Tustin_Filter_T;

/* Parameters (default storage) */
struct P_Tustin_Filter_T_ {
  real_T DiscreteTransferFcn_NumCoef[2];/* Expression: [C0 C0]
                                         * Referenced by: '<S1>/Discrete Transfer Fcn'
                                         */
  real_T DiscreteTransferFcn_DenCoef[2];/* Expression: [1 C1]
                                         * Referenced by: '<S1>/Discrete Transfer Fcn'
                                         */
  real_T DiscreteTransferFcn_InitialStat;/* Expression: 0
                                          * Referenced by: '<S1>/Discrete Transfer Fcn'
                                          */
};

/* Real-time Model Data Structure */
struct tag_RTM_Tustin_Filter_T {
  const char_T * volatile errorStatus;
};

/* Block parameters (default storage) */
extern P_Tustin_Filter_T Tustin_Filter_P;

/* Block states (default storage) */
extern DW_Tustin_Filter_T Tustin_Filter_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_Tustin_Filter_T Tustin_Filter_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_Tustin_Filter_T Tustin_Filter_Y;

/* Model entry point functions */
extern void Tustin_Filter_initialize(void);
extern void Tustin_Filter_step(void);
extern void Tustin_Filter_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Tustin_Filter_T *const Tustin_Filter_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('Disc_Filter/Tustin_Filter')    - opens subsystem Disc_Filter/Tustin_Filter
 * hilite_system('Disc_Filter/Tustin_Filter/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Disc_Filter'
 * '<S1>'   : 'Disc_Filter/Tustin_Filter'
 */
#endif                                 /* Tustin_Filter_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
