#
# Copyright (c) 2016-2019 CNRS INRIA
#

add_subdirectory(python)
if(BUILD_PYTHON_INTERFACE)
  set(PYWRAP
      ${PYWRAP}
      PARENT_SCOPE)
endif(BUILD_PYTHON_INTERFACE)
