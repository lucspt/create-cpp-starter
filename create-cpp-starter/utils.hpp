#include <filesystem>

namespace ccs {
bool is_folder_valid(const std::filesystem::path& root);

void catch_err(const std::filesystem::path& root);

void init_git_repository(const std::filesystem::path& root);

std::filesystem::path create_cmake_file(
  const std::filesystem::path& root, std::string& app_name
);

void run_cmake_build(const std::filesystem::path& root);
std::filesystem::path get_executable_dir();

template <class... Lines>
void write_file_lines(std::ofstream& f, Lines... lines);
}  // namespace ccs