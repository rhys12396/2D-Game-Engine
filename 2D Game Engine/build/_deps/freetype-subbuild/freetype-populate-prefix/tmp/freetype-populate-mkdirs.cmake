# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/CPP/2D Game Engine/build/_deps/freetype-src")
  file(MAKE_DIRECTORY "D:/CPP/2D Game Engine/build/_deps/freetype-src")
endif()
file(MAKE_DIRECTORY
  "D:/CPP/2D Game Engine/build/_deps/freetype-build"
  "D:/CPP/2D Game Engine/build/_deps/freetype-subbuild/freetype-populate-prefix"
  "D:/CPP/2D Game Engine/build/_deps/freetype-subbuild/freetype-populate-prefix/tmp"
  "D:/CPP/2D Game Engine/build/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp"
  "D:/CPP/2D Game Engine/build/_deps/freetype-subbuild/freetype-populate-prefix/src"
  "D:/CPP/2D Game Engine/build/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/CPP/2D Game Engine/build/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/CPP/2D Game Engine/build/_deps/freetype-subbuild/freetype-populate-prefix/src/freetype-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
