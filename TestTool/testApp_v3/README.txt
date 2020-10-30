
File list:

	testApp:  The test tool is used for verifying and testing the calucation dynamic library.
	
	matrixA.txt:
	matrixB.txt: The matrix data used in testApp.

 	libCalculation.so: The calculation dynamic library file example used for testing the testApp tool.


Usage of TestTool:

	testApp  -load  <dynamic_library_absolute_path> [-loop N1] [-matrixnum N2] [-printstep N3] [-seed N4]

	example: testApp -load ./libCalculation.so -loop 1000 -matrixnum 1024


command line parameters:

	-load: Specify the calculation library path.

	-loop: Specify the task number, equal to simultor iteration or step number. The default is 1024.

	-matrixnum: Specify the matrix number for taskA and taskB. The default is 1024.

	-printstep: Print the iteration number after specified steps. Iteration is not printed by default.

	-seed: Specify the seed of random generator for the rowArray values. The default is 0.

	-tol: Specify the tolerance value. The default is 0.



