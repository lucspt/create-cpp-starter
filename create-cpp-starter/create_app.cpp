#include <CLI11.hpp>
#include <filesystem>
#include <print>
#include <string>
#include <utils.hpp>

int main(int argc, char** argv) {
  namespace fs = std::filesystem;
  CLI::App app{"Create a C++ app"};
  std::string app_name;
  app.add_option("-n,--name,name", app_name, "The name of the app")->required();
  CLI11_PARSE(app, argc, argv);

  fs::path project_path{app_name};

  project_path = fs::weakly_canonical(fs::current_path() / project_path);
  app_name = project_path.stem().string();

  try {
    std::print("\nCreating project directory...\n");
    if (fs::exists(project_path) && !is_folder_valid(project_path)) {
      return 1;
    };
    // copy the templates
    fs::path template_dir{get_executable_dir().parent_path() / "templates"};
    fs::copy_options copy_opts{fs::copy_options::recursive};
    fs::copy(template_dir / "common", project_path, copy_opts);
    fs::path src_dir{project_path / app_name};
    // we could have just had a single template
    // but using "common" will allow us to factor in new templates in future.

    // src, tests dir
    fs::create_directory(project_path / "tests");
    fs::create_directory(project_path / app_name);

    std::print("Generating build system...\n");
    create_cmake_file(project_path, app_name);
    run_cmake_build(project_path);

    std::print("Initializing git repository...\n");
    init_git_repository(project_path);

    std::print("Successfully created project at {0}!\n\n", project_path.string());
  } catch (std::exception& e) {
    std::print("{}\n", e.what());
    catch_err(project_path);
  } catch (...) {
    catch_err(project_path);
  }
};
