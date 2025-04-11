/*
 * File: rlSimplePendulumModel.c
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

#include "rlSimplePendulumModel.h"
#include <math.h>
#include "rlSimplePendulumModel_private.h"
#include "rtwtypes.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"

/* Block signals (default storage) */
B_rlSimplePendulumModel_T rlSimplePendulumModel_B;

/* Continuous states */
X_rlSimplePendulumModel_T rlSimplePendulumModel_X;

/* Disabled State Vector */
XDis_rlSimplePendulumModel_T rlSimplePendulumModel_XDis;

/* Block states (default storage) */
DW_rlSimplePendulumModel_T rlSimplePendulumModel_DW;

/* External inputs (root inport signals with default storage) */
ExtU_rlSimplePendulumModel_T rlSimplePendulumModel_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_rlSimplePendulumModel_T rlSimplePendulumModel_Y;

/* Real-time model */
static RT_MODEL_rlSimplePendulumMode_T rlSimplePendulumModel_M_;
RT_MODEL_rlSimplePendulumMode_T *const rlSimplePendulumModel_M =
  &rlSimplePendulumModel_M_;

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 2;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  rlSimplePendulumModel_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  rlSimplePendulumModel_step();
  rlSimplePendulumModel_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  rlSimplePendulumModel_step();
  rlSimplePendulumModel_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  rlSimplePendulumModel_step();
  rlSimplePendulumModel_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u1 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u0 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(tmp_0, tmp);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model step function */
void rlSimplePendulumModel_step(void)
{
  real_T TrigonometricFunction1_tmp;
  real_T u0;
  if (rtmIsMajorTimeStep(rlSimplePendulumModel_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&rlSimplePendulumModel_M->solverInfo,
                          ((rlSimplePendulumModel_M->Timing.clockTick0+1)*
      rlSimplePendulumModel_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(rlSimplePendulumModel_M)) {
    rlSimplePendulumModel_M->Timing.t[0] = rtsiGetT
      (&rlSimplePendulumModel_M->solverInfo);
  }

  /* SecondOrderIntegrator: '<S2>/Integrator, Second-Order' */
  rlSimplePendulumModel_B.theta = rlSimplePendulumModel_X.theta[0];

  /* SecondOrderIntegrator: '<S2>/Integrator, Second-Order' */
  rlSimplePendulumModel_B.theta_dot = rlSimplePendulumModel_X.theta[1];

  /* Trigonometry: '<S1>/Trigonometric Function1' incorporates:
   *  Trigonometry: '<S2>/Trigonometric Function'
   */
  TrigonometricFunction1_tmp = sin(rlSimplePendulumModel_B.theta);

  /* Trigonometry: '<S1>/Trigonometric Function1' */
  rlSimplePendulumModel_B.TrigonometricFunction1 = TrigonometricFunction1_tmp;

  /* Trigonometry: '<S1>/Trigonometric Function2' */
  rlSimplePendulumModel_B.TrigonometricFunction2 = cos
    (rlSimplePendulumModel_B.theta);

  /* Trigonometry: '<S1>/Trigonometric Function' */
  rlSimplePendulumModel_B.TrigonometricFunction = rt_atan2d_snf
    (rlSimplePendulumModel_B.TrigonometricFunction1,
     rlSimplePendulumModel_B.TrigonometricFunction2);

  /* Outport: '<Root>/theta' */
  rlSimplePendulumModel_Y.theta = rlSimplePendulumModel_B.TrigonometricFunction;

  /* Outport: '<Root>/theta_dot' */
  rlSimplePendulumModel_Y.theta_dot = rlSimplePendulumModel_B.theta_dot;

  /* Saturate: '<Root>/Saturation' incorporates:
   *  Inport: '<Root>/action'
   */
  u0 = rlSimplePendulumModel_U.action;
  if (u0 > 2.0) {
    /* Saturate: '<Root>/Saturation' */
    rlSimplePendulumModel_B.tau_limited = 2.0;
  } else if (u0 < -2.0) {
    /* Saturate: '<Root>/Saturation' */
    rlSimplePendulumModel_B.tau_limited = -2.0;
  } else {
    /* Saturate: '<Root>/Saturation' */
    rlSimplePendulumModel_B.tau_limited = u0;
  }

  /* End of Saturate: '<Root>/Saturation' */

  /* Trigonometry: '<S2>/Trigonometric Function' */
  rlSimplePendulumModel_B.TrigonometricFunction_l = TrigonometricFunction1_tmp;

  /* Gain: '<S2>/mgl' */
  rlSimplePendulumModel_B.mgl = 9.81 *
    rlSimplePendulumModel_B.TrigonometricFunction_l;

  /* Gain: '<S2>/c' */
  rlSimplePendulumModel_B.c = 0.0 * rlSimplePendulumModel_B.theta_dot;

  /* Sum: '<S2>/Sum' */
  rlSimplePendulumModel_B.Sum = (rlSimplePendulumModel_B.mgl +
    rlSimplePendulumModel_B.tau_limited) - rlSimplePendulumModel_B.c;

  /* Gain: '<S2>/I^-1' */
  rlSimplePendulumModel_B.theta_ddot = rlSimplePendulumModel_B.Sum;
  if (rtmIsMajorTimeStep(rlSimplePendulumModel_M)) {
    rt_ertODEUpdateContinuousStates(&rlSimplePendulumModel_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++rlSimplePendulumModel_M->Timing.clockTick0;
    rlSimplePendulumModel_M->Timing.t[0] = rtsiGetSolverStopTime
      (&rlSimplePendulumModel_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      rlSimplePendulumModel_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void rlSimplePendulumModel_derivatives(void)
{
  XDot_rlSimplePendulumModel_T *_rtXdot;
  _rtXdot = ((XDot_rlSimplePendulumModel_T *) rlSimplePendulumModel_M->derivs);

  /* Derivatives for SecondOrderIntegrator: '<S2>/Integrator, Second-Order' */
  if (rlSimplePendulumModel_DW.IntegratorSecondOrder_MODE == 0) {
    _rtXdot->theta[0] = rlSimplePendulumModel_X.theta[1];
    _rtXdot->theta[1] = rlSimplePendulumModel_B.theta_ddot;
  }

  /* End of Derivatives for SecondOrderIntegrator: '<S2>/Integrator, Second-Order' */
}

/* Model initialize function */
void rlSimplePendulumModel_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&rlSimplePendulumModel_M->solverInfo,
                          &rlSimplePendulumModel_M->Timing.simTimeStep);
    rtsiSetTPtr(&rlSimplePendulumModel_M->solverInfo, &rtmGetTPtr
                (rlSimplePendulumModel_M));
    rtsiSetStepSizePtr(&rlSimplePendulumModel_M->solverInfo,
                       &rlSimplePendulumModel_M->Timing.stepSize0);
    rtsiSetdXPtr(&rlSimplePendulumModel_M->solverInfo,
                 &rlSimplePendulumModel_M->derivs);
    rtsiSetContStatesPtr(&rlSimplePendulumModel_M->solverInfo, (real_T **)
                         &rlSimplePendulumModel_M->contStates);
    rtsiSetNumContStatesPtr(&rlSimplePendulumModel_M->solverInfo,
      &rlSimplePendulumModel_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&rlSimplePendulumModel_M->solverInfo,
      &rlSimplePendulumModel_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&rlSimplePendulumModel_M->solverInfo,
      &rlSimplePendulumModel_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&rlSimplePendulumModel_M->solverInfo,
      &rlSimplePendulumModel_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&rlSimplePendulumModel_M->solverInfo, (boolean_T**)
      &rlSimplePendulumModel_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&rlSimplePendulumModel_M->solverInfo,
                          (&rtmGetErrorStatus(rlSimplePendulumModel_M)));
    rtsiSetRTModelPtr(&rlSimplePendulumModel_M->solverInfo,
                      rlSimplePendulumModel_M);
  }

  rtsiSetSimTimeStep(&rlSimplePendulumModel_M->solverInfo, MAJOR_TIME_STEP);
  rlSimplePendulumModel_M->intgData.y = rlSimplePendulumModel_M->odeY;
  rlSimplePendulumModel_M->intgData.f[0] = rlSimplePendulumModel_M->odeF[0];
  rlSimplePendulumModel_M->intgData.f[1] = rlSimplePendulumModel_M->odeF[1];
  rlSimplePendulumModel_M->intgData.f[2] = rlSimplePendulumModel_M->odeF[2];
  rlSimplePendulumModel_M->intgData.f[3] = rlSimplePendulumModel_M->odeF[3];
  rlSimplePendulumModel_M->contStates = ((X_rlSimplePendulumModel_T *)
    &rlSimplePendulumModel_X);
  rlSimplePendulumModel_M->contStateDisabled = ((XDis_rlSimplePendulumModel_T *)
    &rlSimplePendulumModel_XDis);
  rlSimplePendulumModel_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&rlSimplePendulumModel_M->solverInfo, (void *)
                    &rlSimplePendulumModel_M->intgData);
  rtsiSetIsMinorTimeStepWithModeChange(&rlSimplePendulumModel_M->solverInfo,
    false);
  rtsiSetSolverName(&rlSimplePendulumModel_M->solverInfo,"ode4");
  rtmSetTPtr(rlSimplePendulumModel_M, &rlSimplePendulumModel_M->Timing.tArray[0]);
  rlSimplePendulumModel_M->Timing.stepSize0 = 0.01;

  /* InitializeConditions for SecondOrderIntegrator: '<S2>/Integrator, Second-Order' */
  rlSimplePendulumModel_X.theta[0] = 0.785;
  rlSimplePendulumModel_X.theta[1] = 0.0;
}

/* Model terminate function */
void rlSimplePendulumModel_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
