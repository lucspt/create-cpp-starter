#include <filesystem>
#include <iostream>
#include <print>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

void catch_err(fs::path& root) {
  if (fs::exists(root)) {
    fs::remove_all(root);
  }
}

bool is_folder_valid(fs::path& root) {
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

void run_cmd(std::string command) {
  system(std::format("{} > /dev/null", command).c_str());
}

void init_git_repository(fs::path& root) {
  fs::path prev_cwd = fs::current_path();
  fs::current_path(root);
  run_cmd("git init .");
  run_cmd("git add -A");
  run_cmd(R"(git commit -am "Initial commit")");
  run_cmd("sh ./scripts/prepare");
  fs::current_path(prev_cwd);
}

template <class... Lines>
void write_file_lines(std::ofstream& f, Lines... lines) {
  ((f << lines << "\n"), ...);
}

fs::path create_cmake_file(fs::path& root, std::string& app_name) {
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

void run_cmake_build(fs::path& root) {
  fs::path prev_path = fs::current_path();
  fs::path build_dir = root / "build";
  fs::create_directory(build_dir);
  fs::current_path(build_dir);
  run_cmd("cmake ..");
  run_cmd("cmake --build .");
  fs::current_path(prev_path);
}