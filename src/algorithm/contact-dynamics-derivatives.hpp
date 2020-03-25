//
// Copyright (c) 2020 LAAS, INRIA
//

#ifndef __pinocchio_algorithm_contact_dynamics_derivatives_hpp__
#define __pinocchio_algorithm_contact_dynamics_derivatives_hpp__

#include "pinocchio/algorithm/contact-info.hpp"

namespace pinocchio
{

  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, class Allocator, class AllocatorData, typename MatrixType1, typename MatrixType2, typename MatrixType3, typename MatrixType4, typename MatrixType5, typename MatrixType6>  
  inline void computeContactDynamicsDerivatives(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                                                DataTpl<Scalar,Options,JointCollectionTpl> & data,
                                                const std::vector<RigidContactModelTpl<Scalar,Options>,Allocator> & contact_models,
                                                const std::vector<RigidContactDataTpl<Scalar,Options>,AllocatorData> & contact_datas,
                                                const Scalar mu,
                                                const Eigen::MatrixBase<MatrixType1> & ddq_partial_dq,
                                                const Eigen::MatrixBase<MatrixType2> & ddq_partial_dv,
                                                const Eigen::MatrixBase<MatrixType3> & ddq_partial_dtau,
                                                const Eigen::MatrixBase<MatrixType4> & lambda_partial_dq,
                                                const Eigen::MatrixBase<MatrixType5> & lambda_partial_dv,
                                                const Eigen::MatrixBase<MatrixType6> & lambda_partial_dtau);
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, class Allocator, class AllocatorData>
  inline void computeContactDynamicsDerivatives(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                                                DataTpl<Scalar,Options,JointCollectionTpl> & data,
                                                const std::vector<RigidContactModelTpl<Scalar,Options>,Allocator> & contact_models,
                                                const std::vector<RigidContactDataTpl<Scalar,Options>,AllocatorData> & contact_datas,
                                                const Scalar mu = Scalar(0.))
  {
    computeContactDynamicsDerivatives(model, data, contact_models, contact_datas, mu, data.ddq_dq, data.ddq_dv, data.ddq_dtau,
                                      data.dlambda_dq, data.dlambda_dv, data.dlambda_dtau);
  };
  
} // namespace pinocchio

#include "pinocchio/algorithm/contact-dynamics-derivatives.hxx"

#endif // ifndef __pinocchio_algorithm_contact_dynamics_hpp__
