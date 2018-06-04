### Project AGATHA
AGATHA (acronymous of AGAtha Tunes Hybrid Applicatons) is a wrapper which uses mARGOt to tune hybrid OpenMP/MPI applications. It comes with a tool that can be helpful to get the values needed for the tuning. You can also link the execution with an ARGO instance to get these values using a remote server.
##### Installation
- First thing to do is compiling the code you need to autotune: AGATHA doesn't need you to modify the code you have already written. However, if your code needs arguments passed via command line, you will have to modify a configuration file. AGATHA needs a copy of your executable code in the folders src/build and bin (create these folders if they are not there).
- The next step is to get the data mARGOt needs to run the application. You can do it in two ways: either you run agorizer.sh and gather them locally, or using the AGORA plugin and getting them remotely. At the end you should have a file called oplist.conf into your config folder. The repository provides an example oplist.conf file you can use to test the next steps, however to autotune correctly you have to generate a new one. [AGORA support will be added in the near future]
- Now you need to compile the wrapper: just run builder.sh. It will create the executable in the folder bin.
- Launch the wrapper and watch the result.
##### Configuration
- There is a file into the target folder called target_data.txt: here you can modify the settings of the agorizer application. You can change the number of threads/process AGATHA can choose from. You can also modify the number of times each configuration is tested.
- [There will be a file which allow you to choose the command line arguments you want to give to the program]
##### See also
https://gitlab.com/margot_project/
