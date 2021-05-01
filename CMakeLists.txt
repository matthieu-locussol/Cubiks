CMAKE_MINIMUM_REQUIRED( VERSION 3.0.0 )

# Configure project
PROJECT( Cubiks VERSION 1.0.0 )

# Configuration CMake Module Path
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
SET_PROPERTY( GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1 )

# Choose default build type (if none specified)
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE Debug )
ENDIF()

# Disable in-source builds
IF( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    MESSAGE( FATAL_ERROR "In-source builds are not allowed. Please create a new directory (i.e. a build/ directory)." )
ENDIF()

# Compilation flags
INCLUDE( flags )

# Set prefix path if none specified
IF( NOT CMAKE_PREFIX_PATH )
    SET( CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
ENDIF()

# Configure Cubiks sources
INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/includes" )

FILE( GLOB_RECURSE CUBIKS_APPLICATION "application/main.cpp" )
FILE( GLOB_RECURSE CUBIKS_SOURCES "sources/*.cpp" "sources/**/*.cpp" )
FILE( GLOB_RECURSE CUBIKS_HEADERS "includes/*.hpp" "includes/**/*.hpp" )
FILE( GLOB_RECURSE CUBIKS_INLINES "includes/*.inl" "includes/**/*.inl" )

FILE(
    GLOB_RECURSE
    CUBIKS_FILES
    ${CUBIKS_SOURCES}
    ${CUBIKS_HEADERS}
    ${CUBIKS_INLINES}
)

# Configuring SFML
SET( SFML_BUILD_AUDIO OFF )
SET( SFML_BUILD_GRAPHICS OFF )
SET( SFML_BUILD_NETWORK OFF )

ADD_SUBDIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/SFML" )

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/SFML/include" )

# Configuring glm
# ADD_SUBDIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/glm" )

# INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/glm" )

find_package(GLM REQUIRED)
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIRS} )

# Configuring glbinding
SET( OPTION_BUILD_EXAMPLES OFF )
SET( OPTION_BUILD_OWN_KHR_HEADERS ON )
SET( CMAKE_POLICY_DEFAULT_CMP0120 OLD )

ADD_SUBDIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/glbinding" )

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/glbinding/source/glbinding/include" )

set( glbinding_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules/glbinding" )
find_package(glbinding REQUIRED)

# Configuring globjects
ADD_SUBDIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/globjects" )

set( globjects_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules/globjects" )
find_package(globjects REQUIRED)

INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/globjects/source/globjects/include" )

# Configuring Doxygen target
FIND_PACKAGE( Doxygen )

IF( DOXYGEN_FOUND )
	SET( DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in )
	SET( DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxyfile )

	CONFIGURE_FILE( ${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY )

	ADD_CUSTOM_TARGET(
		Documentation ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs
		COMMENT "Generating documentation using Doxygen."
		VERBATIM
	)
ELSE()
	MESSAGE( STATUS "${PROJECT_NAME}: Doxygen needs to be installed to generate documentation." )
ENDIF()

# Adding application
ADD_EXECUTABLE( ${PROJECT_NAME} ${CUBIKS_FILES} ${CUBIKS_APPLICATION} )

# add_library(glm::glm ALIAS_GLOBAL glm)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC sfml-system )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC sfml-window )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC glbinding::glbinding )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC globjects::globjects )

# Configuring Catch2
ADD_SUBDIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/submodules/Catch2" )

# Adding tests
FILE( GLOB_RECURSE CUBIKS_TESTS_FILES "tests/**/*.cpp" )

ADD_EXECUTABLE( "${PROJECT_NAME}Tests" ${CUBIKS_FILES} ${CUBIKS_TESTS_FILES} )
TARGET_LINK_LIBRARIES( "${PROJECT_NAME}Tests" PRIVATE Catch2::Catch2WithMain )