//
// Copyright (c) 2022 INRIA
//

#include "pinocchio/algorithm/constrained-dynamics-derivatives.hpp"

namespace pinocchio {

    template void computeConstraintDynamicsDerivatives
      <context::Scalar, context::Options, JointCollectionDefaultTpl, typename context::RigidConstraintModelVector::allocator_type, typename context::RigidConstraintDataVector::allocator_type,
       context::MatrixXs, context::MatrixXs, context::MatrixXs, context::MatrixXs, context::MatrixXs, context::MatrixXs>
    (const context::Model &, context::Data &, const context::RigidConstraintModelVector &, context::RigidConstraintDataVector &,
     const ProximalSettingsTpl<context::Scalar> &, const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &,
     const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &);

    template void computeConstraintDynamicsDerivatives
      <context::Scalar, context::Options, JointCollectionDefaultTpl, typename context::RigidConstraintModelVector::allocator_type, typename context::RigidConstraintDataVector::allocator_type,
       context::MatrixXs, context::MatrixXs, context::MatrixXs, context::MatrixXs, context::MatrixXs, context::MatrixXs>
    (const context::Model &, context::Data &, const context::RigidConstraintModelVector &, context::RigidConstraintDataVector &,
     const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &,
     const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &, const Eigen::MatrixBase<context::MatrixXs> &);

    template void computeConstraintDynamicsDerivatives
      <context::Scalar, context::Options, JointCollectionDefaultTpl, typename context::RigidConstraintModelVector::allocator_type, typename context::RigidConstraintDataVector::allocator_type>
    (const context::Model &, context::Data &, const context::RigidConstraintModelVector &, context::RigidConstraintDataVector &, const ProximalSettingsTpl<context::Scalar> &);

    template void computeConstraintDynamicsDerivatives
      <context::Scalar, context::Options, JointCollectionDefaultTpl, typename context::RigidConstraintModelVector::allocator_type, typename context::RigidConstraintDataVector::allocator_type>
    (const context::Model &, context::Data &, const context::RigidConstraintModelVector &, context::RigidConstraintDataVector &);
} // namespace pinocchio