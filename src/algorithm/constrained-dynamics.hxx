//
// Copyright (c) 2016-2019 CNRS INRIA
//

#ifndef __pinocchio_constrained_dynamics_hxx__
#define __pinocchio_constrained_dynamics_hxx__

#include "pinocchio/algorithm/compute-all-terms.hpp"
#include "pinocchio/algorithm/cholesky.hpp"
#include "pinocchio/algorithm/crba.hpp"
#include "pinocchio/algorithm/check.hpp"

namespace pinocchio
{
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, typename TangentVectorType,
  typename ConstraintMatrixType, typename DriftVectorType>
  inline const typename DataTpl<Scalar,Options,JointCollectionTpl>::TangentVectorType &
  forwardDynamics(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                  DataTpl<Scalar,Options,JointCollectionTpl> & data,
                  const Eigen::MatrixBase<TangentVectorType> & tau,
                  const Eigen::MatrixBase<ConstraintMatrixType> & J,
                  const Eigen::MatrixBase<DriftVectorType> & gamma,
                  const ProximalSettingsTpl<Scalar> & prox_settings)
  {
    assert(tau.size() == model.nv);
    assert(J.cols() == model.nv);
    assert(J.rows() == gamma.size());
    assert(model.check(data) && "data is not consistent with model.");
    
    typedef DataTpl<Scalar,Options,JointCollectionTpl> Data;
    
    typename Data::TangentVectorType & a = data.ddq;
    typename Data::VectorXs & lambda_c = data.lambda_c;
    typename Data::VectorXs & lambda_c_prox = data.lambda_c_prox;
    
    // Compute the UDUt decomposition of data.M
    cholesky::decompose(model, data);
    
    // Compute the dynamic drift (control - nle)
    data.torque_residual = tau - data.nle;
    cholesky::solve(model, data, data.torque_residual);
    
    data.sDUiJt = J.transpose();
    // Compute U^-1 * J.T
    cholesky::Uiv(model, data, data.sDUiJt);
    for(Eigen::DenseIndex k=0;k<model.nv;++k)
      data.sDUiJt.row(k) /= sqrt(data.D[k]);
    
    data.JMinvJt.noalias() = data.sDUiJt.transpose() * data.sDUiJt;
    
    const Scalar & mu = prox_settings.mu;
    assert(mu >= 0. && "mu must be positive");
    int max_it = prox_settings.max_it;
    assert(max_it >= 1 && "mu must greater or equal to 1");
    
    if(mu == 0.)
      max_it = 1;
    else
      data.JMinvJt.diagonal().array() += mu;
    
    data.llt_JMinvJt.compute(data.JMinvJt);
    
    lambda_c_prox = Data::VectorXs::Zero(gamma.size());
    for(int it = 0; it < max_it; ++it)
    {
      // Compute the Lagrange Multipliers
      lambda_c.noalias() = -J*data.torque_residual;
      lambda_c += -gamma + mu*lambda_c_prox;
      data.llt_JMinvJt.solveInPlace(lambda_c);
      
      data.diff_lambda_c = lambda_c - lambda_c_prox;
      lambda_c_prox = lambda_c;
      
      // Check termination
      if(max_it > 1)
        //        std::cout << "data.diff_lambda_c.template lpNorm<Eigen::Infinity>():\n" << data.diff_lambda_c.template lpNorm<Eigen::Infinity>() << std::endl;
        if(data.diff_lambda_c.template lpNorm<Eigen::Infinity>() <= prox_settings.threshold)
          break;
    }
    
    // Compute the joint acceleration
    a.noalias() = J.transpose() * lambda_c;
    cholesky::solve(model, data, a);
    a += data.torque_residual;
    
    return a;
  }
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, typename TangentVectorType,
  typename ConstraintMatrixType, typename DriftVectorType>
  inline const typename DataTpl<Scalar,Options,JointCollectionTpl>::TangentVectorType &
  forwardDynamics(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                  DataTpl<Scalar,Options,JointCollectionTpl> & data,
                  const Eigen::MatrixBase<TangentVectorType> & tau,
                  const Eigen::MatrixBase<ConstraintMatrixType> & J,
                  const Eigen::MatrixBase<DriftVectorType> & gamma,
                  const Scalar mu)
  {
    ProximalSettingsTpl<Scalar> prox_settings;
    prox_settings.mu = mu;
    return forwardDynamics(model,data,tau,J,gamma,prox_settings);
  }
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, typename ConfigVectorType, typename TangentVectorType1, typename TangentVectorType2,
  typename ConstraintMatrixType, typename DriftVectorType>
  inline const typename DataTpl<Scalar,Options,JointCollectionTpl>::TangentVectorType &
  forwardDynamics(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                  DataTpl<Scalar,Options,JointCollectionTpl> & data,
                  const Eigen::MatrixBase<ConfigVectorType> & q,
                  const Eigen::MatrixBase<TangentVectorType1> & v,
                  const Eigen::MatrixBase<TangentVectorType2> & tau,
                  const Eigen::MatrixBase<ConstraintMatrixType> & J,
                  const Eigen::MatrixBase<DriftVectorType> & gamma,
                  const ProximalSettingsTpl<Scalar> & prox_settings)
  {
    computeAllTerms(model, data, q, v);
    return forwardDynamics(model,data,tau,J,gamma,prox_settings);
  }
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, typename ConfigVectorType, typename TangentVectorType1, typename TangentVectorType2,
  typename ConstraintMatrixType, typename DriftVectorType>
  inline const typename DataTpl<Scalar,Options,JointCollectionTpl>::TangentVectorType &
  forwardDynamics(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                  DataTpl<Scalar,Options,JointCollectionTpl> & data,
                  const Eigen::MatrixBase<ConfigVectorType> & q,
                  const Eigen::MatrixBase<TangentVectorType1> & v,
                  const Eigen::MatrixBase<TangentVectorType2> & tau,
                  const Eigen::MatrixBase<ConstraintMatrixType> & J,
                  const Eigen::MatrixBase<DriftVectorType> & gamma,
                  const Scalar mu)
  {
    computeAllTerms(model, data, q, v);
    return forwardDynamics(model,data,tau,J,gamma,mu);
  }
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, typename ConfigVectorType, typename TangentVectorType1, typename TangentVectorType2,
  typename ConstraintMatrixType, typename DriftVectorType>
  PINOCCHIO_DEPRECATED
  inline const typename DataTpl<Scalar,Options,JointCollectionTpl>::TangentVectorType &
  forwardDynamics(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                  DataTpl<Scalar,Options,JointCollectionTpl> & data,
                  const Eigen::MatrixBase<ConfigVectorType> & q,
                  const Eigen::MatrixBase<TangentVectorType1> & v,
                  const Eigen::MatrixBase<TangentVectorType2> & tau,
                  const Eigen::MatrixBase<ConstraintMatrixType> & J,
                  const Eigen::MatrixBase<DriftVectorType> & gamma,
                  const Scalar mu,
                  const bool updateKinematics)
  {
    if(updateKinematics)
      return forwardDynamics(model,data,q,v,tau,J,gamma,mu);
    else
      return forwardDynamics(model,data,tau,J,gamma,mu);
  }
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl,
  typename ConstraintMatrixType, typename KKTMatrixType>
  inline void getKKTContactDynamicMatrixInverse(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                                                const DataTpl<Scalar,Options,JointCollectionTpl> & data,
                                                const Eigen::MatrixBase<ConstraintMatrixType> & J,
                                                const Eigen::MatrixBase<KKTMatrixType> & MJtJ_inv)
  {
    typedef DataTpl<Scalar,Options,JointCollectionTpl> Data;
    assert(MJtJ_inv.cols() == data.JMinvJt.cols() + model.nv);
    assert(MJtJ_inv.rows() == data.JMinvJt.rows() + model.nv);
    const typename Data::MatrixXs::Index& nc = data.JMinvJt.cols();
    
    KKTMatrixType& MJtJ_inv_ = PINOCCHIO_EIGEN_CONST_CAST(KKTMatrixType,MJtJ_inv);
    
    Eigen::Block<typename Data::MatrixXs> topLeft = MJtJ_inv_.topLeftCorner(model.nv, model.nv);
    Eigen::Block<typename Data::MatrixXs> topRight = MJtJ_inv_.topRightCorner(model.nv, nc);
    Eigen::Block<typename Data::MatrixXs> bottomLeft = MJtJ_inv_.bottomLeftCorner(nc, model.nv);
    Eigen::Block<typename Data::MatrixXs> bottomRight = MJtJ_inv_.bottomRightCorner(nc, nc);
    
    bottomRight = -Data::MatrixXs::Identity(nc,nc);    topLeft.setIdentity();
    data.llt_JMinvJt.solveInPlace(bottomRight);    cholesky::solve(model, data, topLeft);
    
    bottomLeft.noalias() = J*topLeft;
    topRight.noalias() = bottomLeft.transpose() * (-bottomRight);
    topLeft.noalias() -= topRight*bottomLeft;
    bottomLeft = topRight.transpose();
  }
  
  template<typename Scalar, int Options, template<typename,int> class JointCollectionTpl, typename ConfigVectorType, typename TangentVectorType, typename ConstraintMatrixType>
  inline const typename DataTpl<Scalar,Options,JointCollectionTpl>::TangentVectorType &
  impulseDynamics(const ModelTpl<Scalar,Options,JointCollectionTpl> & model,
                  DataTpl<Scalar,Options,JointCollectionTpl> & data,
                  const Eigen::MatrixBase<ConfigVectorType> & q,
                  const Eigen::MatrixBase<TangentVectorType> & v_before,
                  const Eigen::MatrixBase<ConstraintMatrixType> & J,
                  const Scalar r_coeff,
                  const bool updateKinematics)
  {
    assert(q.size() == model.nq);
    assert(v_before.size() == model.nv);
    assert(J.cols() == model.nv);
    assert(model.check(data) && "data is not consistent with model.");
    
    typedef DataTpl<Scalar,Options,JointCollectionTpl> Data;
    
    typename Data::VectorXs & impulse_c = data.impulse_c;
    typename Data::TangentVectorType & dq_after = data.dq_after;
    
    // Compute the mass matrix
    if (updateKinematics)
      crba(model, data, q);
    
    // Compute the UDUt decomposition of data.M
    cholesky::decompose(model, data);
    
    data.sDUiJt = J.transpose();
    // Compute U^-1 * J.T
    cholesky::Uiv(model, data, data.sDUiJt);
    for(int k=0;k<model.nv;++k) data.sDUiJt.row(k) /= sqrt(data.D[k]);
    
    data.JMinvJt.noalias() = data.sDUiJt.transpose() * data.sDUiJt;
    data.llt_JMinvJt.compute(data.JMinvJt);
    
    // Compute the Lagrange Multipliers related to the contact impulses
    impulse_c.noalias() = (-r_coeff - 1.) * (J * v_before);
    data.llt_JMinvJt.solveInPlace(impulse_c);
    
    // Compute the joint velocity after impacts
    dq_after.noalias() = J.transpose() * impulse_c;
    cholesky::solve(model, data, dq_after);
    dq_after += v_before;
    
    return dq_after;
  }
} // namespace pinocchio

#endif // ifndef __pinocchio_constrained_dynamics_hxx__