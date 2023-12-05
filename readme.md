## How to make the executable file? ###

#### Step 0: Prerequisites

1) Your system should have `git` installed.
2) Your system should have `cmake` installed.
3) Your system should have `make` installed.

#### Step 1: Git clone

```bash
git clone "https://github.com/singhmeharjeet/dct.git" . --recurse-submodules -j8 
```

- This command will clone the project into current directory and will also clone the required submodules.
- The `-j8` flag is optional and is used to speed up the cloning process.
- This command will download `SDL`, `imgui` and `tinyfiledialogs` submodules, so it will take some time

#### Step 2: Build the project

```bash
cmake -S . -B build
cmake --build build
```

- I have added a command to run the executable automatically after building the project, so you can see the GUI.
- So the next step is optional.

#### Step 3: Run the executable

```bash
./build/src/dct
``` 

Now, the executable will be found in the `./build/src/` folder by the name of ``
