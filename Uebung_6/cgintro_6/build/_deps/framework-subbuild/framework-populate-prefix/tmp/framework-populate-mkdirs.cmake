# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-src")
  file(MAKE_DIRECTORY "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-src")
endif()
file(MAKE_DIRECTORY
  "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-build"
  "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-subbuild/framework-populate-prefix"
  "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-subbuild/framework-populate-prefix/tmp"
  "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-subbuild/framework-populate-prefix/src/framework-populate-stamp"
  "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-subbuild/framework-populate-prefix/src"
  "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-subbuild/framework-populate-prefix/src/framework-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-subbuild/framework-populate-prefix/src/framework-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/Jestry/Documents/Dokumente – Jestry Mac/6. Semester/CGV/CGV REPO/CGV_Abgaben_Repo/Uebung_6/cgintro_6/build/_deps/framework-subbuild/framework-populate-prefix/src/framework-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
