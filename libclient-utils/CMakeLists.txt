###########################################################################
# Copyright 2015, 2016, 2017 IoT.bzh
#
# author: Fulup Ar Foll <fulup@iot.bzh>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################

# Add target to project dependency list
PROJECT_TARGET_ADD(libclient-utils)

    # Define targets 
    ADD_LIBRARY(${TARGET_NAME} STATIC charset.c search.c misc.c output.c status.c control.c list.c)

    # Library properties
    SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME ${TARGET_NAME}
    )

    # Library dependencies
    TARGET_LINK_LIBRARIES(${TARGET_NAME}
      common-utils
    )
   
    # Define target includes
    TARGET_INCLUDE_DIRECTORIES(${TARGET_NAME} 
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}
)