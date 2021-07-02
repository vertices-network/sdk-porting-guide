cmake_minimum_required (VERSION 3.20)
project (porting_guide)

# adding the Vertices SDK by adding the directory
add_subdirectory(external/c-vertices-sdk)

# adding the User code
add_subdirectory(myproject)
