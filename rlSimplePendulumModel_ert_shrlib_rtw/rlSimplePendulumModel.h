/*
 * File: rlSimplePendulumModel.h
 *
 * Code generated for Simulink model 'rlSimplePendulumModel'.
 *
 * Model version                  : 10.2
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Wed Feb 26 20:29:13 2025
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: 32-bit Generic
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_rlSimplePendulumModel_h_
#define RTW_HEADER_rlSimplePendulumModel_h_
#ifndef rlSimplePendulumModel_COMMON_INCLUDES_
#define rlSimplePendulumModel_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                              /* rlSimplePendulumModel_COMMON_INCLUDES_ */

#include "rlSimplePendulumModel_types.h"
#include "rtGetNaN.h"
#include <string.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T theta;                        /* '<S2>/Integrator, Second-Order' */
  real_T theta_dot;                    /* '<S2>/Integrator, Second-Order' */
  real_T TrigonometricFunction1;       /* '<S1>/Trigonometric Function1' */
  real_T TrigonometricFunction2;       /* '<S1>/Trigonometric Function2' */
  real_T TrigonometricFunction;        /* '<S1>/Trigonometric Function' */
  real_T tau_limited;                  /* '<Root>/Saturation' */
  real_T TrigonometricFunction_l;      /* '<S2>/Trigonometric Function' */
  real_T mgl;                          /* '<S2>/mgl' */
  real_T c;                            /* '<S2>/c' */
  real_T Sum;                          /* '<S2>/Sum' */
  real_T theta_ddot;                   /* '<S2>/I^-1' */
} B_rlSimplePendulumModel_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int_T IntegratorSecondOrder_MODE;    /* '<S2>/Integrator, Second-Order' */
  boolean_T IntegratorSecondOrder_DWORK1;/* '<S2>/Integrator, Second-Order' */
} DW_rlSimplePendulumModel_T;

/* Continuous states (default storage) */
typedef struct {
  real_T theta[2];                     /* '<S2>/Integrator, Second-Order' */
} X_rlSimplePendulumModel_T;

/* State derivatives (default storage) */
typedef struct {
  real_T theta[2];                     /* '<S2>/Integrator, Second-Order' */
} XDot_rlSimplePendulumModel_T;

/* State disabled  */
typedef struct {
  boolean_T theta[2];                  /* '<S2>/Integrator, Second-Order' */
} XDis_rlSimplePendulumModel_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T action;                       /* '<Root>/action' */
} ExtU_rlSimplePendulumModel_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T theta;                        /* '<Root>/theta' */
  real_T theta_dot;                    /* '<Root>/theta_dot' */
} ExtY_rlSimplePendulumModel_T;

/* Real-time Model Data Structure */
struct tag_RTM_rlSimplePendulumModel_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_rlSimplePendulumModel_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_rlSimplePendulumModel_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[2];
  real_T odeF[4][2];
  ODE4_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    time_T tStart;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (default storage) */
extern B_rlSimplePendulumModel_T rlSimplePendulumModel_B;

/* Continuous states (default storage) */
extern X_rlSimplePendulumModel_T rlSimplePendulumModel_X;

/* Disabled states (default storage) */
extern XDis_rlSimplePendulumModel_T rlSimplePendulumModel_XDis;

/* Block states (default storage) */
extern DW_rlSimplePendulumModel_T rlSimplePendulumModel_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_rlSimplePendulumModel_T rlSimplePendulumModel_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_rlSimplePendulumModel_T rlSimplePendulumModel_Y;

/* Model entry point functions */
extern void rlSimplePendulumModel_initialize(void);
extern void rlSimplePendulumModel_step(void);
extern void rlSimplePendulumModel_terminate(void);

/* Real-time Model object */
extern RT_MODEL_rlSimplePendulumMode_T *const rlSimplePendulumModel_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'rlSimplePendulumModel'
 * '<S1>'   : 'rlSimplePendulumModel/angle_wrap'
 * '<S2>'   : 'rlSimplePendulumModel/pendulum'
 */
#endif                                 /* RTW_HEADER_rlSimplePendulumModel_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
