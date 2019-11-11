#
# Copyright (c) 2018-2019 CNRS INRIA
#

## In this file, are reported some deprecated functions that are still maintained until the next important future releases ##

from __future__ import print_function

import warnings as _warnings

from . import libpinocchio_pywrap as pin 
from .deprecation import deprecated, DeprecatedWarning

# deprecated StdVect_ (since: 19/04/2019)
StdVect_Frame = deprecated("Please use StdVec_Frame")(pin.StdVec_Frame)
StdVect_GeometryObject = deprecated("Please use StdVec_GeometryObject")(pin.StdVec_GeometryObject)
if pin.WITH_FCL_SUPPORT():
    StdVect_Contact = deprecated("Please use StdVec_Contact")(pin.StdVec_Contact)
    StdVect_CollisionResult = deprecated("Please use StdVec_CollisionResult")(pin.StdVec_CollisionResult)
    StdVect_DistanceResult = deprecated("Please use StdVec_DistanceResult")(pin.StdVec_DistanceResult)
StdVect_SE3 = deprecated("Please use StdVec_SE3")(pin.StdVec_SE3)
StdVect_Force = deprecated("Please use StdVec_Force")(pin.StdVec_Force)
StdVect_Motion = deprecated("Please use StdVec_Motion")(pin.StdVec_Motion)

@deprecated("This function has been renamed to impulseDynamics for consistency with the C++ interface. Please change for impulseDynamics.")
def impulseDynamics(model, data, q, v_before, J, r_coeff=0.0, update_kinematics=True):
  return pin.impulseDynamics(model, data, q, v_before, J, r_coeff, update_kinematics)

@deprecated("This function has been deprecated. Please use buildSampleModelHumanoid or buildSampleModelHumanoidRandom instead.")
def buildSampleModelHumanoidSimple(usingFF=True):
    return pin.buildSampleModelHumanoidRandom(usingFF)

@deprecated("Static method Model.BuildHumanoidSimple has been deprecated. Please use function buildSampleModelHumanoid or buildSampleModelHumanoidRandom instead.")
def _Model__BuildHumanoidSimple(usingFF=True):
    return pin.buildSampleModelHumanoidRandom(usingFF)

pin.Model.BuildHumanoidSimple = staticmethod(_Model__BuildHumanoidSimple)

@deprecated("Static method Model.BuildEmptyModel has been deprecated. Please use the empty Model constructor instead.")
def _Model__BuildEmptyModel():
    return pin.Model()

pin.Model.BuildEmptyModel = staticmethod(_Model__BuildEmptyModel)

@deprecated("This function has been renamed updateFramePlacements when taking two arguments, and framesForwardKinematics when taking three. Please change your code to the appropriate method.")
def framesKinematics(model,data,q=None):
  if q is None:
    pin.updateFramePlacements(model,data)
  else:
    pin.framesForwardKinematics(model,data,q)

# This function is only deprecated when using a specific signature. Therefore, it needs special care
# Marked as deprecated on 19 Feb 2019
def frameJacobian(model, data, q, frameId, *args):
  if len(args)==1:
    message = ("This function signature has been deprecated and will be removed in future releases of Pinocchio. "
               "Please change for the new signature of frameJacobian or use computeJointJacobian + updateFramePlacements + getFrameJacobian.")
    _warnings.warn(message, category=DeprecatedWarning, stacklevel=2)
    rf = args[0]
    pin.computeJointJacobians(model,data,q)
    pin.updateFramePlacements(model,data)
    return pin.getFrameJacobian(model, data, frameId, rf);
  else:
    return pin.frameJacobian(model,data,q,frameId)
frameJacobian.__doc__ =  (
  pin.frameJacobian.__doc__
  + '\n\nframeJacobian( (Model)Model, (Data)Data, (object)Joint configuration q (size Model::nq), (Index)frameId, ReferenceFrame rf) -> object :'
  + '\n    This function signature has been deprecated and will be removed in future releases of Pinocchio.'
)

@deprecated("This function has been renamed difference and will be removed in future releases of Pinocchio. Please change for new difference function.")
def differentiate(model,q0,q1):
  return pin.difference(model,q0,q1)

@deprecated("This function has been renamed loadReferenceConfigurations and will be removed in future releases of Pinocchio. Please change for new loadReferenceConfigurations function.")
def getNeutralConfigurationFromSrdf(model, filename, verbose):
  pin.loadReferenceConfigurations(model,filename,verbose)
  return model.referenceConfigurations["half_sitting"]

@deprecated("This function has been renamed loadReferenceConfigurations and will be removed in future releases of Pinocchio. Please change for new loadReferenceConfigurations function.")
def getNeutralConfiguration(model, filename, verbose):
  pin.loadReferenceConfigurations(model,filename,verbose)
  return model.referenceConfigurations["half_sitting"]

@deprecated("This function has been renamed difference and will be removed in future releases of Pinocchio. Please change for new loadRotorParameters function.")
def loadRotorParamsFromSrdf(model, filename, verbose):
  return pin.loadRotorParams(model,filename,verbose)

@deprecated("This function has been renamed difference and will be removed in future releases of Pinocchio. Please change for new removeCollisionPairs function.")
def removeCollisionPairsFromSrdf(model, geomModel, filename, verbose):
  return pin.removeCollisionPairs(model,geomModel,filename,verbose)

@deprecated("This function will be removed in future releases of Pinocchio. You can use exp or exp6.")
def exp6FromMotion(motion):
  return pin.exp6(motion)

@deprecated("This function will be removed in future releases of Pinocchio. You can build a Motion object from a 6D vector and use the standard exp function to recover the same behavior.")
def exp6FromVector(vector6):
  v = pin.Motion(vector6)
  return pin.exp6(v)

@deprecated("This function will be removed in future releases of Pinocchio. You can use log or log6.")
def log6FromSE3(transform):
  return pin.log6(transform)

@deprecated("This function will be removed in future releases of Pinocchio. You can use log or log6.")
def log6FromMatrix(matrix4):
  return pin.log6(matrix4)

# This function is only deprecated when using a specific signature. Therefore, it needs special care
# Marked as deprecated on 16 Sept 2019
def impulseDynamics(model, data, *args):
  if len(args)==5 and type(args[4]) is bool:
    message = ("This function signature has been deprecated and will be removed in future releases of Pinocchio. "
               "Please change for the new signature of impulseDynamics(model,data[,q],v_before,J[,r_coeff[,inv_damping]]).")
    _warnings.warn(message, category=DeprecatedWarning, stacklevel=2)
    q = args[0]
    v_before = args[1]
    J = args[2]
    r_coeff = args[3]
    updateKinematics = args[4]
    inv_damping = 0.
    if updateKinematics:
      return pin.impulseDynamics(model,data,q,v_before,J,r_coeff,inv_damping)
    else:
      return pin.impulseDynamics(model,data,v_before,J,r_coeff,inv_damping)

  return pin.impulseDynamics(model, data, *args)

impulseDynamics.__doc__ =  (
  pin.impulseDynamics.__doc__
  + '\n\nimpulseDynamics( (Model)Model, (Data)Data, (object)Joint configuration q (size Model::nq), (object)Joint velocity before impact v_before (size Model::nv), (object)Contact Jacobian J (size nb_constraint * Model::nv), (float)Coefficient of restitution r_coeff (0 = rigid impact; 1 = fully elastic impact), (bool)updateKinematics) -> object :'
  + '\n    This function signature has been deprecated and will be removed in future releases of Pinocchio.'
)

# This function is only deprecated when using a specific signature. Therefore, it needs special care
# Marked as deprecated on 2 Oct 2019
def forwardDynamics(model, data, *args):
  if len(args)==7 and type(args[6]) is bool:
    message = ("This function signature has been deprecated and will be removed in future releases of Pinocchio. "
               "Please change for the new signature of forwardDynamics(model,data[,q],v,tau,J,gamma[,inv_damping]).")
    _warnings.warn(message, category=DeprecatedWarning, stacklevel=2)
    q = args[0]
    v = args[1]
    tau = args[2]
    J = args[3]
    gamma = args[4]
    inv_damping = args[5]
    updateKinematics = args[6]
    if updateKinematics:
      return pin.forwardDynamics(model,data,q,v,tau,J,gamma,inv_damping)
    else:
      return pin.forwardDynamics(model,data,tau,J,gamma,inv_damping)

  return pin.forwardDynamics(model, data, *args)

forwardDynamics.__doc__ = (
  pin.forwardDynamics.__doc__
  + '\n\nforwardDynamics( (Model)Model, (Data)Data, (object)Joint configuration q (size Model::nq), (object)Joint velocity v (size Model::nv), (object)Joint torque tau (size Model::nv), (object)Contact Jacobian J (size nb_constraint * Model::nv), (object)Contact drift gamma (size nb_constraint), (float)(double) Damping factor for cholesky decomposition of JMinvJt. Set to zero if constraints are full rank, (bool)Update kinematics) -> object :'
+ '\n    This function signature has been deprecated and will be removed in future releases of Pinocchio.'
)

def computeCentroidalDynamics(model, data, q, v, a = None):
  if a is None:
    message = ("This function signature has been renamed and will be removed in future releases of Pinocchio. "
               "Please change for the new signature of computeCentroidalMomentum(model,data,q,v).")
    _warnings.warn(message, category=DeprecatedWarning, stacklevel=2)
    return pin.computeCentroidalMomentum(model,data,q,v)
  else:
    message = ("This function signature has been renamed and will be removed in future releases of Pinocchio. "
               "Please change for the new signature of computeCentroidalMomentumTimeVariation(model,data,q,v,a).")
    _warnings.warn(message, category=DeprecatedWarning, stacklevel=2)
    return pin.computeCentroidalMomentum(model,data,q,v,a)

computeCentroidalDynamics.__doc__ = ( "This function has been renamed computeCentroidalMomentum or computeCentroidalMomentumTimeVariation to either only compute the centroidal momentum quantity or also its time derivative respectively." )

@deprecated("This function has been renamed computeJointJacobian and will be removed in future releases of Pinocchio. Please change for new computeJointJacobian.")
def jointJacobian(model, data, q, jointId):
  return computeJointJacobian(model,data,q,jointId)
  
@deprecated("This function has been renamed computeFrameJacobian and will be removed in future releases of Pinocchio. Please change for new computeFrameJacobian.")
def frameJacobian(model, data, q, jointId):
  return computeFrameJacobian(model,data,q,jointId)
  
