rm -rf build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Coverage
cmake --build build -j 10
cd build
ctest --verbose
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info "/usr/*" "*/googletest/*" --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
#open ./coverage_html/index.html