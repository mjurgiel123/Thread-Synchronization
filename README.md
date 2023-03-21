# Description

When the program is run, we initialize two binary semaphores for int connected and int next_id, and we initialize a counting semaphore for int NUM_OPERATORS.
We also declare a pthread_t array of length numOfCalls (argv[1]). We run a for loop, and for each element of this array, create a thread, which runs 
the code in the phonecall() function. In phonecall(), first we set a variable callerId as the increment of the global variable next_id, utilizing sem_t next_id_lock. 
Utilizing sem_t connected_lock, we then continue into a while loop, which continues looping until the caller is able to connect to a line. If the line is busy, 
the caller waits 1 second until trying to connect again. Once connected to a line, the caller waits for an operator to be available by utilizing 
sem_t operators. Once the caller is connected to an operator, the ticket purchase is simulated using sleep(3). Once the purchase has been completed, 
both an operator and line open up for other calls. This continues until all of the callers have purchased tickets, 
as the main method waits for the threads to terminate using pthread_join. Finally, the semaphores are destroyed, and the program terminates.
