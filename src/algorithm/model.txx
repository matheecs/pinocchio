//
// Copyright (c) 2022 INRIA
//

#ifndef __pinocchio_algorithm_model_txx__
#define __pinocchio_algorithm_model_txx__

namespace pinocchio {

  extern template PINOCCHIO_DLLAPI void appendModel
    <context::Scalar, context::Options, JointCollectionDefaultTpl>
    (const context::Model &, const context::Model &, const FrameIndex, const SE3Tpl<context::Scalar,context::Options> &, context::Model &);

  extern template PINOCCHIO_DLLAPI context::Model appendModel
    <context::Scalar, context::Options, JointCollectionDefaultTpl>
    (const context::Model &, const context::Model &, const FrameIndex, const SE3Tpl<context::Scalar,context::Options> &);

  extern template PINOCCHIO_DLLAPI void appendModel
    <context::Scalar, context::Options, JointCollectionDefaultTpl>
    (const context::Model &, const context::Model &, const GeometryModel &, const GeometryModel &, const FrameIndex, const SE3Tpl<context::Scalar,context::Options> &, context::Model &, GeometryModel &);

  extern template PINOCCHIO_DLLAPI void buildReducedModel
    <context::Scalar, context::Options, JointCollectionDefaultTpl, context::VectorXs>
    (const context::Model &, const std::vector<JointIndex>, const Eigen::MatrixBase<context::VectorXs> &, context::Model &);

  extern template PINOCCHIO_DLLAPI context::Model buildReducedModel
    <context::Scalar, context::Options, JointCollectionDefaultTpl, context::VectorXs>
    (const context::Model &, const std::vector<JointIndex> &, const Eigen::MatrixBase<context::VectorXs> &);

  extern template PINOCCHIO_DLLAPI void buildReducedModel
    <context::Scalar, context::Options, JointCollectionDefaultTpl, context::VectorXs>
    (const context::Model &, const GeometryModel &, const std::vector<JointIndex> &, const Eigen::MatrixBase<context::VectorXs> &, context::Model &, GeometryModel &);

  extern template PINOCCHIO_DLLAPI void buildReducedModel
    <context::Scalar, context::Options, JointCollectionDefaultTpl, Eigen::aligned_allocator<GeometryModel>, context::VectorXs>
    (const context::Model &, const std::vector<GeometryModel,Eigen::aligned_allocator<GeometryModel> > &, const std::vector<JointIndex> &, const Eigen::MatrixBase<context::VectorXs> &, context::Model &, std::vector<GeometryModel,Eigen::aligned_allocator<GeometryModel> > &);

  extern template PINOCCHIO_DLLAPI JointIndex findCommonAncestor
    <context::Scalar, context::Options, JointCollectionDefaultTpl>
    (const context::Model &, JointIndex, JointIndex, size_t &, size_t &);

} // namespace pinocchio

#endif // ifndef __pinocchio_algorithm_model_txx__