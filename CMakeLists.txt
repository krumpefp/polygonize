cmake_minimum_required(VERSION 3.13)

project(polygonize LANGUAGES CXX)

FIND_PACKAGE(CGAL REQUIRED)
FIND_PACKAGE(PkgConfig REQUIRED)

# GMP Does not support pkg-config yet ...
# pkg_check_modules(GMP REQUIRED gmp)
pkg_check_modules(MPFR REQUIRED mpfr)

add_executable(${PROJECT_NAME} src/polygonize.cpp)
target_link_libraries(${PROJECT_NAME} CGAL gmp mpfr)
install(TARGETS
        ${PROJECT_NAME}
    DESTINATION ${PROJECT_BINARY_DIR}/bin
)