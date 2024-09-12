# create-cpp-starter

Hey, welcome to `create-cpp-starter`.

It is a cli tool that scaffolds a c++ project with some batteries included like:

- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
- A nice git pre-commit hook

And there might be more added over time! For more details, check out the [batteries](#batteries) section.

Contents:

- [Installation](#installation)
- [Usage](#usage)
- [Uninstalling](#uninstalling)
- [The batteries included](#batteries)

## Installation

### 1. Install the executable

First run the command below.

`curl -sSf https://raw.githubusercontent.com/lucspt/create-cpp-starter/main/scripts/install.sh | bash`

This command runs the [install script](./scripts/install.sh), which installs the executable for you.

### 2. Add to PATH

Next, you will need to add the executable directory to your path.

This is as simple as adding the following line to your shell startup file

`source "$HOME/.create-cpp-starter/env"`

For example if you are using bash or zsh, you can add the line to your `.bash_profile` or `.zprofile`, respectively.

## Usage

To scaffold a project, invoke the following command;

`create-cpp-starter [project-name]`

So, if you wanted to create a new project named `example-app`, you would run:

`create-cpp-starter example-app`

This will create a folder in the current working directory named `example-app`.

You can specify the project name as a positional argument or with the `-n/--name` option.

### Specifying a name as a Relative path

You can also specify the name as a relative path, for example

`create-cpp-starter ../example-app`

Would create the project in a folder named `example-app` in the parent directory of the
present working directory.

## Uninstalling

To uninstall, you can simply delete the directory containing the tool:

```
rm -rf $HOME/.create-cpp-starter
```

The install script places the executable in `$HOME/.create-cpp-starter`, hence the command above.

If you followed the steps above and added any functionality to your shell startup script that will 
append the executable directory to your PATH, you should remove it. 
So, assuming you followed the steps in the [install section](#installation),
just go the file (e.g. `.bash_profile` or `.zprofile`) and remove the following line:

`source "$HOME/.create-cpp-starter/env"`

And that's it.

## Batteries

The project comes with [clang-format](https://clang.llvm.org/docs/ClangFormat.html), which will handle code formatting for you.
It is configured in such a way that it behaves like python's [`black`](https://black.readthedocs.io/en/stable/index.html#).
The configuration was taken from [this repository](https://github.com/justinchuby/clang-format-black).

It also comes with github hooks that will:

1. Run tests with cmake's ctest, aborting the commit if a test fails.
2. Format any code being committed.

For now that is about it, but new features may be added soon!
