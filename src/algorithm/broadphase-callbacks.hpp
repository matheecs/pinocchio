//
// Copyright (c) 2022 INRIA
//

#ifndef __pinocchio_algo_broadphase_callback_hpp__
#define __pinocchio_algo_broadphase_callback_hpp__

#ifdef PINOCCHIO_WITH_HPP_FCL

#include <hpp/fcl/broadphase/broadphase_callbacks.h>
#include <iostream>

#include "pinocchio/multibody/fcl.hpp"
#include "pinocchio/multibody/geometry.hpp"

#include "pinocchio/algorithm/collision.hpp"

namespace pinocchio
{

/// @brief Interface for Pinocchio collision callback functors
struct CollisionCallBackBase
: hpp::fcl::CollisionCallBackBase
{
  CollisionCallBackBase(const GeometryModel & geometry_model,
                        GeometryData & geometry_data)
  : geometry_model_ptr(&geometry_model)
  , geometry_data_ptr(&geometry_data)
  , collision(false)
  , accumulate(false)
  {}
  
  const GeometryModel & getGeometryModel() const { return *geometry_model_ptr; }
  const GeometryData & getGeometryData() const { return *geometry_data_ptr; }
  GeometryData & getGeometryData() { return *geometry_data_ptr; }
  
  /// \brief If true, the stopping criteria related to the collision callback has been met and one can stop.
  virtual bool stop() const = 0;
  
  /// \brief Callback method called after the termination of a collisition detection algorithm.
  ///        The default implementation does nothing.
  virtual void done() {};

protected:
  /// @brief Geometry model associated to the callback
  const GeometryModel * geometry_model_ptr;
  
  /// @brief Geometry data associated to the callback
  GeometryData * geometry_data_ptr;
  
public:
  
  /// @brief Whether there is a collision or not
  bool collision;
  
  /// @brief Whether the callback is used in an accumulate mode where several collide methods are called successively.
  bool accumulate;
  
};

struct CollisionCallBackDefault : CollisionCallBackBase
{
  CollisionCallBackDefault(const GeometryModel & geometry_model,
                           GeometryData & geometry_data,
                           bool stopAtFirstCollision = false)
  : CollisionCallBackBase(geometry_model, geometry_data)
  , stopAtFirstCollision(stopAtFirstCollision)
  , count(0)
//  , visited(Eigen::MatrixXd::Zero(geometry_model.ngeoms,geometry_model.ngeoms))
  {}
  
  void init()
  {
    if(accumulate) // skip reseting of the parameters
      return;
    
    count = 0;
    collision = false;
    collisionPairIndex = std::numeric_limits<PairIndex>::max();
//    visited.setZero();
  }
  
  bool collide(hpp::fcl::CollisionObject* o1, hpp::fcl::CollisionObject* o2)
  {
    assert(stopAtFirstCollision && collision && "must never happened");
    
    CollisionObject & co1 = reinterpret_cast<CollisionObject&>(*o1);
    CollisionObject & co2 = reinterpret_cast<CollisionObject&>(*o2);
    
    const Eigen::DenseIndex go1_index = (Eigen::DenseIndex)co1.geometryObjectIndex;
    const Eigen::DenseIndex go2_index = (Eigen::DenseIndex)co2.geometryObjectIndex;
    
    const GeometryModel & geometry_model = *geometry_model_ptr;
    
    PINOCCHIO_CHECK_INPUT_ARGUMENT(go1_index < (Eigen::DenseIndex)geometry_model.ngeoms && go1_index >= 0);
    PINOCCHIO_CHECK_INPUT_ARGUMENT(go2_index < (Eigen::DenseIndex)geometry_model.ngeoms && go2_index >= 0);

    const int pair_index = geometry_model.collisionPairMapping(go1_index,go2_index);
    if(pair_index == -1)
      return false;
    
    const GeometryData & geometry_data = *geometry_data_ptr;
    const CollisionPair & cp = geometry_model.collisionPairs[(PairIndex)pair_index];
    const bool do_collision_check
    =  geometry_data.activeCollisionPairs[(PairIndex)pair_index]
    && !(geometry_model.geometryObjects[cp.first].disableCollision || geometry_model.geometryObjects[cp.second].disableCollision);
    if(!do_collision_check)
      return false;
    
    count++;
    
    const bool res = computeCollision(*geometry_model_ptr, *geometry_data_ptr, (PairIndex)pair_index);
    
    if(res && !collision)
    {
      collision = true;
      collisionPairIndex = (PairIndex)pair_index;
    }
    
    if(!stopAtFirstCollision)
      return false;
    else
      return res;
  }
  
  bool stop() const final
  {
    if(stopAtFirstCollision && collision)
      return true;
    
    return false;
  }
  
  void done() final
  {
    if(collision)
      geometry_data_ptr->collisionPairIndex = collisionPairIndex;
  }

  /// @brief Whether to stop or not when localizing a first collision
  bool stopAtFirstCollision;
  
  /// @brief The collision index of the first pair in collision
  PairIndex collisionPairIndex;
  
  /// @brief Number of visits of the collide method
  size_t count;
  
//  Eigen::MatrixXd visited;
};

} // namespace pinocchio

/* --- Details -------------------------------------------------------------------- */
#include "pinocchio/algorithm/broadphase-callbacks.hxx"

#endif // ifdef PINOCCHIO_WITH_HPP_FCL

#endif // ifndef __pinocchio_algo_broadphase_callback_hpp__