#ifndef CCS_UTILS
#define CCS_UTILS
#include <filesystem>
#include <string>

namespace ccs {
bool is_folder_valid(const std::filesystem::path& root);

void catch_err(const std::filesystem::path& root);

void init_git_repository(const std::filesystem::path& root);

std::filesystem::path create_cmake_file(
  const std::filesystem::path& root, std::string& app_name
);

void run_cmake_build(const std::filesystem::path& root);
std::filesystem::path get_executable_dir();

void add_coverage_command_to_test_script(
  const std::filesystem::path& test_script, std::string_view src_dirname
);

template <class... Lines>
void write_file_lines(std::ofstream& f, Lines... lines);
}  // namespace ccs
#endif