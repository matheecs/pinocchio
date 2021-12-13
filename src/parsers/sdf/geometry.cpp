//
// Copyright (c) 2021 CNRS
//

#include "pinocchio/parsers/sdf.hpp"
#include "pinocchio/parsers/utils.hpp"

#include <sstream>
#include <iomanip>
#include <boost/shared_ptr.hpp>

#include <hpp/fcl/mesh_loader/loader.h>
#include <hpp/fcl/mesh_loader/assimp.h>


namespace pinocchio
{
  namespace sdf
  {
    namespace details
    {

      void recursiveParseGraphForGeom(const SdfGraph& graph,
                                      ::hpp::fcl::MeshLoaderPtr& meshLoader,
                                      const ::sdf::ElementPtr link,
                                      GeometryModel & geomModel,
                                      const std::vector<std::string> & package_dirs,
                                      const GeometryType type)
      {
        const std::string& linkName = link->Get<std::string>("name");
        switch(type)
        {
          case COLLISION:
            addLinkGeometryToGeomModel< ::pinocchio::COLLISION >(graph, meshLoader, link,
                                                            geomModel, package_dirs);
            break;
          case VISUAL:
            addLinkGeometryToGeomModel< ::pinocchio::VISUAL >(graph, meshLoader, link,
                                                         geomModel, package_dirs);
            break;
          default:
            break;
        }
        const std::vector<std::string>& childrenOfLink =
          graph.childrenOfLinks.find(linkName)->second;

        for(std::vector<std::string>::const_iterator childOfChild =
              std::begin(childrenOfLink);
            childOfChild != std::end(childrenOfLink); ++childOfChild)
        {
          const ::sdf::ElementPtr childJointElement =
            graph.mapOfJoints.find(*childOfChild)->second;

          const std::string childLinkName =
            childJointElement->GetElement("child")->template Get<std::string>();
          const ::sdf::ElementPtr childLinkElement =
            graph.mapOfLinks.find(childLinkName)->second;
          recursiveParseGraphForGeom(graph, meshLoader, childLinkElement,
                                     geomModel, package_dirs,type);
          
        }
      }
      
      void parseTreeForGeom(const SdfGraph& graph,
                            GeometryModel & geomModel,
                            const std::string& rootLinkName,
                            const GeometryType type,
                            const std::vector<std::string> & package_dirs,
                            ::hpp::fcl::MeshLoaderPtr meshLoader)
      {
        std::vector<std::string> hint_directories(package_dirs);
        std::vector<std::string> ros_pkg_paths = rosPaths();
        hint_directories.insert(hint_directories.end(), ros_pkg_paths.begin(),
                                ros_pkg_paths.end());
        
        if (!meshLoader) meshLoader = fcl::MeshLoaderPtr(new fcl::MeshLoader);

        const ::sdf::ElementPtr rootElement = graph.mapOfLinks.find(rootLinkName)->second;
        recursiveParseGraphForGeom(graph, meshLoader, rootElement,
                                   geomModel, hint_directories, type);
        
      }
    } // namespace details    
  } // namespace sdf
} // namespace pinocchio
