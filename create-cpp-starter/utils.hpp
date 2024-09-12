#include <filesystem>

bool is_folder_valid(std::filesystem::path& root);
void catch_err(std::filesystem::path& root);
void init_git_repository(std::filesystem::path& root);
std::string create_cmake_file(std::filesystem::path& root, std::string& app_name);
void run_cmake_build(std::filesystem::path& root);
std::filesystem::path get_executable_dir();