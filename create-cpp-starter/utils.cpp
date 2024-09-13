#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

#include <filesystem>
#include <iostream>
#include <print>
#include <string>
#include <fstream>
#include <vector>
#include <utils.hpp>

namespace fs = std::filesystem;

namespace ccs {

// clang-format off
/**
 * Get the directory of the current executable.
 */
fs::path get_executable_dir() {
  #if defined(__linux__)
    return fs::canonical("/proc/self/exe").parent_path();
  #elif defined(__APPLE__)
    unsigned int bufsize{1024};
    std::vector<char> buf(bufsize + 1);
    if (_NSGetExecutablePath(&buf[0], &bufsize) == -1) {
      buf.resize(bufsize);
      _NSGetExecutablePath(&buf[0], &bufsize);
    };
    return fs::canonical(&buf[0]).parent_path();
  #elif defined(_WIN32)
    unsigned int bufsize{1024};
    std::vector<char> buf(bufsize + 1);
    GetModuleFileName(NULL, &buf[0], bufsize);
    return fs::canonical(&buf[0]).parent_path();
  #else
  #error Unable to find required executable path on this platform
  #endif
}
// clang-format on

/**
 * Cleanup the directory if an error occurs
 */
void catch_err(const fs::path& root) {
  if (fs::exists(root)) {
    fs::remove_all(root);
  }
}

/**
 * Check if a folder is valid for template copying. 
 * A folder is considered invalid if any files or directories are present.
 * 
 * If invalid, all top-level contents a warning will be printed.
 */
bool is_folder_valid(const fs::path& root) {
  if (fs::exists(root) && !fs::is_empty(root)) {
    std::print("The directory contains files that could conflict:\n");
    for (auto const dir_entry : fs::directory_iterator(root)) {
      std::string name = dir_entry.path().string();
      std::print("{0}\n", fs::is_directory(dir_entry) ? name + "/" : name);
    };

    std::print(
      "\nEither try using a new directory name, or remove the files "
      "listed above."
    );
    return false;
  };
  return true;
}

/** Invoke a `system` command and redirect to `/dev/null` */
void run_cmd(const std::string command) {
  system(std::format("{} > /dev/null", command).c_str());
}

/** Initialize a git repo in `root`. */
void init_git_repository(const fs::path& root) {
  fs::path prev_cwd = fs::current_path();
  fs::current_path(root);
  run_cmd("git init .");
  run_cmd("git add -A");
  run_cmd(R"(git commit -am "Initial commit")");
  run_cmd("sh ./scripts/prepare");
  fs::current_path(prev_cwd);
}

/**
 * Write the given `lines` to `f`.
 */
template <class... Lines>
void write_file_lines(std::ofstream& f, Lines... lines) {
  ((f << lines << "\n"), ...);
}

/** Creates a CMakeLists.txt file in `root` */
fs::path create_cmake_file(const fs::path& root, std::string& app_name) {
  fs::path pth{root / "CMakeLists.txt"};
  std::ofstream file{pth};
  write_file_lines(
    file,
    "cmake_minimum_required(VERSION 3.10 FATAL_ERROR)",
    "",
    "if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})",
    R"( message(FATAL_ERROR "In-source build is not supported"))",
    "endif()",
    "",  // line break
    std::format("set(PROJECT_NAME {})", app_name),
    "",
    "project(${PROJECT_NAME} VERSION 0.1.0)",
    "",
    "set(CMAKE_CXX_STANDARD 23)",
    "set(CMAKE_CXX_STANDARD_REQUIRED true)",
    "",
    "include(FetchContent)",
    "FetchContent_Declare(",
    " googletest",
    " GIT_REPOSITORY https://github.com/google/googletest.git",
    " GIT_TAG main",
    " FIND_PACKAGE_ARGS NAMES googletest",
    ")\n",
    "FetchContent_MakeAvailable(googletest)",
    "",
    "# uncomment for testing",
    "# enable_testing()"
  );
  file.close();
  return pth;
};

/** Creates a build directory and generates a cmake build system in 
 * `root`.
*/
void run_cmake_build(const fs::path& root) {
  fs::path prev_path = fs::current_path();
  fs::path build_dir = root / "build";
  fs::create_directory(build_dir);
  fs::current_path(build_dir);
  run_cmd("cmake ..");
  run_cmd("cmake --build .");
  fs::current_path(prev_path);
}

}  // namespace ccs