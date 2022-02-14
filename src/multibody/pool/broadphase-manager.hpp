//
// Copyright (c) 2022 INRIA
//

#ifndef __pinocchio_multibody_pool_broadphase_manager_hpp__
#define __pinocchio_multibody_pool_broadphase_manager_hpp__

#include <omp.h>

#include "pinocchio/multibody/pool/geometry.hpp"
#include "pinocchio/multibody/broadphase-manager.hpp"
  
namespace pinocchio
{

  template<typename _BroadPhaseManagerDerived, typename _Scalar, int _Options, template<typename,int> class JointCollectionTpl>
  class BroadPhaseManagerPoolTpl
  : public GeometryPoolTpl<_Scalar,_Options,JointCollectionTpl>
  {
  public:
    
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    
    typedef _BroadPhaseManagerDerived BroadPhaseManagerDerived;
    typedef BroadPhaseManagerTpl<BroadPhaseManagerDerived> BroadPhaseManager;
    typedef GeometryPoolTpl<_Scalar,_Options,JointCollectionTpl> Base;
    typedef _Scalar Scalar;
    enum { Options = _Options };
    
    typedef typename Base::Model Model;
    typedef typename Base::Data Data;
    typedef typename Base::DataVector DataVector;
    typedef typename Base::GeometryModel GeometryModel;
    typedef typename Base::GeometryData GeometryData;
    
//    typedef std::vector<BroadPhaseManager,Eigen::aligned_allocator<BroadPhaseManager> > BroadPhaseManagerVector;
    
    typedef std::vector<BroadPhaseManager> BroadPhaseManagerVector;

    /// \brief Default constructor from a model and a pool size.
    ///
    /// \param[in] model input model used for parallel computations.
    /// \param[in] geometry_model input geometry model used for parallel computations.
    /// \param[in] pool_size total size of the pool.
    ///
    BroadPhaseManagerPoolTpl(const Model * model_ptr,
                             const GeometryModel * geometry_model_ptr,
                             const size_t pool_size = (size_t)omp_get_max_threads())
    : Base(model_ptr,geometry_model_ptr,pool_size)
    {
      init();
    }
    
    /// \brief Copy constructor from an other BroadPhaseManagerPoolTpl.
    ///
    /// \param[in] other BroadPhaseManagerPoolTpl to copy.
    ///
    BroadPhaseManagerPoolTpl(const BroadPhaseManagerPoolTpl & other)
    : Base(other)
    , m_managers(other.m_managers)
    {
    }
    
    /// \brief Returns the geometry_data at index
    const BroadPhaseManager & getBroadPhaseManager(const size_t index) const
    {
      PINOCCHIO_CHECK_INPUT_ARGUMENT(index < m_managers.size(),
                                     "Index greater than the size of the manager vector.");
      return m_managers[index];
    }
    
    /// \brief Returns the geometry_data at index
    BroadPhaseManager & getBroadPhaseManager(const size_t index)
    {
      PINOCCHIO_CHECK_INPUT_ARGUMENT(index < m_managers.size(),
                                     "Index greater than the size of the manager vector.");
      return m_managers[index];
    }
    
    /// \brief Access to the vector of broad phase managers
    const BroadPhaseManagerVector & getBroadPhaseManagers() const { return m_managers; }
    
    /// \brief Access to the vector of broad phase managers
    BroadPhaseManagerVector & getBroadPhaseManagers() { return m_managers; }
    
    using Base::update;
    using Base::size;
    using Base::getModel;
    using Base::getDatas;
    using Base::getGeometryData;
    using Base::getGeometryModel;
    
    ///
    /// \brief Update the geometry datas with the new value
    ///
    /// \param[in] geometry_data new geometry data value
    ///
    virtual void update(const GeometryData & geometry_data)
    {
      Base::update(geometry_data);
      
      for(size_t i = 0; i < size(); ++i)
      {
        m_managers[i].update(&getGeometryData(i));
      }
    }
    
    /// \brief Check the validity of the current broadphase
    bool check() const
    {
      for(size_t i = 0; i < size(); ++i)
      {
        const BroadPhaseManager & manager = m_managers[i];
        bool res = true;
        res &= (&manager.getGeometryData() == &getGeometryData(i));

        res &= (&manager.getGeometryModel() == &getGeometryModel());
        res &= manager.check();
        
        if(!res)
          return false;
      }
    
      return true;
    }
    
    /// \brief Destructor
    virtual ~BroadPhaseManagerPoolTpl() {};
    
  protected:
    
    void init()
    {
      m_managers.reserve(size());
      for(size_t i = 0; i < size(); ++i)
      {
        m_managers.push_back(BroadPhaseManager(&getGeometryModel(),&getGeometryData(i)));
      }
    }
    
    /// \brief Broad phase managers associated to the pool.
    BroadPhaseManagerVector m_managers;
    
    /// \brief Method to implement in the derived classes.
    virtual void doResize(const size_t new_size)
    {
      m_managers.resize(new_size);
      if(size() < new_size)
      {
        typename BroadPhaseManagerVector::iterator it = m_managers.begin();
        std::advance(it, (long)(new_size - size()));
        std::fill(it,m_managers.end(),m_managers[0]);
      }
    }
    
  };
}

#endif // ifndef __pinocchio_multibody_pool_broadphase_manager_hpp__