### Project AGATHA
AGATHA (acronyms of AGAtha Tunes Hybrid Applications) is a wrapper which uses mARGOt to tune hybrid OpenMP/MPI applications. It comes with a tool that can be helpful to get the values needed for the tuning. You can also link the execution with an AGORA instance to get these values using a remote server.
##### Installation
- First thing to do is compiling the code you need to autotune: AGATHA doesn't need you to modify the code you have already written. However, if your code needs arguments passed via command line, you will have to modify a configuration file. AGATHA needs a copy of your executable code in the folders src/build and bin (create these folders if they are not there). The name of the executable must be target.out. There are some examples of hybrid OMP-MPI programs in the examples folder.
- The next step is to get the data mARGOt needs to run the application. You can do it in two ways:
  - Locally, you have to go to the folder config and delete the file named autotuningagora.conf, then run agorizer.sh.
  - Remotely using AGORA, you have to go in the same folder as before and delete autotuning.conf. Then rename autotuningagora.conf into autotuning.conf. Make sure to configure the parameters inside autoconfig.conf (if you have any troubles with the setup visit the link in the "See also" section).
- Now you need to compile the tuner/wrapper: just run builder.sh. It will create the executable in the folder bin.
- Launch Wrapper and watch the result.
- If you want to start over, run clean.sh. Store a copy of the application you want to autotune first, because clean.sh may delete them.
##### Configuration
- There is a file into the target folder called target_data.txt: here you can modify the settings of the agorizer application. You can change the number of threads/process AGATHA can choose from. You can also modify the number of times each configuration is tested.
- You can also create a file called command_line_parameters.txt into the target folders: the content of the file will be used as command line parameters during the execution of the application.
##### See also
https://gitlab.com/margot_project/
