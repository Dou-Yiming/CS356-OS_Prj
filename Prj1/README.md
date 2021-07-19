# CS-356 Operating System Project1

#### 519021910366 Dou Yi-Ming

## 1. Description of files

1. For problem1, the implementation of the system call 356 is in /Project1/problem1/hello.c
2. For problem2, the code to test system call 356 is in /Project1/problem2/jni/syscall_prtee.c
3. For problem3, the code executing ptree which uses execl is in /Project1/problem3/jni/syscall_prtee.c
4. For problem4, there is only one file called BurgerBuddies.c.

## 2. Environment

1. VMware + Ubuntu18.04
2. Android SDK & Android NDK
3. goldfish kernel

## 3. Outputs

1. Problem1: There is no outputs, since only implementing the syscall is required.

2. Problem2:

    ```
    swapper,0,0,0,0,0,0
    	init,1,1,0,1,1,0
    		ueventd,43,1,1,0,43,0
    		logd,61,1,1,0,61,1036
    		vold,62,1,1,0,62,0
    		healthd,69,1,1,0,69,0
    		lmkd,70,1,1,0,70,0
    		servicemanager,71,1,1,0,71,1000
    		surfaceflinger,72,1,1,0,72,1000
    		qemud,74,1,1,0,74,0
    		sh,77,1,1,0,77,2000
    		adbd,78,1,1,78,78,0
    			sh,1224,1,78,0,1224,0
    			sh,28166,1,78,0,28166,0
    			sh,9812,1,78,9812,9812,0
    				helloARM,6131,0,9812,0,6131,0
    			sh,20741,1,78,0,20741,0
    		debuggerd,80,1,1,0,80,0
    		rild,81,1,1,0,81,1001
    		drmserver,82,1,1,0,82,1019
    		installd,85,1,1,0,85,0
    		keystore,86,1,1,0,86,1017
    		gatekeeperd,88,1,1,0,88,1000
    		perfprofd,89,1,1,0,89,0
    		fingerprintd,90,1,1,0,90,1000
    		bootanimation,1277,1,1,0,1277,1003
    		netd,5846,1,1,0,5846,0
    		mediaserver,5847,1,1,0,5847,1013
    		main,5848,1,1,5848,5848,0
    			system_server,5944,64,5848,0,5944,1000
    	kthreadd,2,1,0,2,2,0
    		ksoftirqd/0,3,1,2,0,3,0
    		kworker/u:0,5,1,2,0,5,0
    		khelper,6,1,2,0,6,0
    		sync_supers,7,1,2,0,7,0
    		bdi-default,8,1,2,0,8,0
    		kblockd,9,1,2,0,9,0
    		rpciod,10,1,2,0,10,0
    		kswapd0,12,1,2,0,12,0
    		fsnotify_mark,13,1,2,0,13,0
    		crypto,14,1,2,0,14,0
    		kworker/u:1,25,1,2,0,25,0
    		mtdblock0,30,1,2,0,30,0
    		mtdblock1,35,1,2,0,35,0
    		mtdblock2,40,1,2,0,40,0
    		binder,41,1,2,0,41,0
    		deferwq,42,1,2,0,42,0
    		jbd2/mtdblock0-,45,1,2,0,45,0
    		ext4-dio-unwrit,46,1,2,0,46,0
    		jbd2/mtdblock1-,52,1,2,0,52,0
    		ext4-dio-unwrit,53,1,2,0,53,0
    		jbd2/mtdblock2-,59,1,2,0,59,0
    		ext4-dio-unwrit,60,1,2,0,60,0
    		kauditd,118,1,2,0,118,0
    		kworker/0:1,16932,1,2,0,16932,0
    		flush-31:1,30614,1,2,0,30614,0
    		kworker/0:2,9571,1,2,0,9571,0
    ```
    
3. Problem3:

    ```
    This is parent process, pid = 6137
    root@generic:/data/code/problem3 # This is child process, pid = 6138
    
    syscall return 0
    nr = 57
    swapper,0,0,0,0,0,0
    	init,1,1,0,1,1,0
    		ueventd,43,1,1,0,43,0
    		logd,61,1,1,0,61,1036
    		vold,62,1,1,0,62,0
    		healthd,69,1,1,0,69,0
    		lmkd,70,1,1,0,70,0
    		servicemanager,71,1,1,0,71,1000
    		surfaceflinger,72,1,1,0,72,1000
    		qemud,74,1,1,0,74,0
    		sh,77,1,1,0,77,2000
    		adbd,78,1,1,78,78,0
    			sh,1224,1,78,0,1224,0
    			sh,28166,1,78,0,28166,0
    			sh,9812,1,78,0,9812,0
    			sh,20741,1,78,0,20741,0
    		debuggerd,80,1,1,0,80,0
    		rild,81,1,1,0,81,1001
    		drmserver,82,1,1,0,82,1019
    		installd,85,1,1,0,85,0
    		keystore,86,1,1,0,86,1017
    		gatekeeperd,88,1,1,0,88,1000
    		perfprofd,89,1,1,0,89,0
    		fingerprintd,90,1,1,0,90,1000
    		bootanimation,1277,1,1,0,1277,1003
    		netd,5846,1,1,0,5846,0
    		mediaserver,5847,1,1,0,5847,1013
    		main,5848,1,1,5848,5848,0
    			system_server,5944,64,5848,0,5944,1000
    		helloARM,6138,0,1,0,6138,0
    	kthreadd,2,1,0,2,2,0
    		ksoftirqd/0,3,1,2,0,3,0
    		kworker/u:0,5,1,2,0,5,0
    		khelper,6,1,2,0,6,0
    		sync_supers,7,1,2,0,7,0
    		bdi-default,8,1,2,0,8,0
    		kblockd,9,1,2,0,9,0
    		rpciod,10,1,2,0,10,0
    		kswapd0,12,1,2,0,12,0
    		fsnotify_mark,13,1,2,0,13,0
    		crypto,14,1,2,0,14,0
    		kworker/u:1,25,1,2,0,25,0
    		mtdblock0,30,1,2,0,30,0
    		mtdblock1,35,1,2,0,35,0
    		mtdblock2,40,1,2,0,40,0
    		binder,41,1,2,0,41,0
    		deferwq,42,1,2,0,42,0
    		jbd2/mtdblock0-,45,1,2,0,45,0
    		ext4-dio-unwrit,46,1,2,0,46,0
    		jbd2/mtdblock1-,52,1,2,0,52,0
    		ext4-dio-unwrit,53,1,2,0,53,0
    		jbd2/mtdblock2-,59,1,2,0,59,0
    		ext4-dio-unwrit,60,1,2,0,60,0
    		kauditd,118,1,2,0,118,0
    		kworker/0:1,16932,1,2,0,16932,0
    		flush-31:1,30614,1,2,0,30614,0
    		kworker/0:2,9571,1,2,0,9571,0
    ```
    
4. Problem4:

    ```
    Cooks [2], Cashiers [4], Customers [41]
    Begin run
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Customer [1] come.
    Customer [2] come.
    Customer [3] come.
    Customer [4] come.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Cook [2] make a burger.
    Casher [1] accepts an order.
    Casher [1] take a burger to customor.
    Cook [2] make a burger.
    Customer [6] come.
    Customer [7] come.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Customer [8] come.
    Cook [1] make a burger.
    Cook [1] make a burger.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Cook [2] make a burger.
    Customer [9] come.
    Casher [4] accepts an order.
    Casher [4] take a burger to customor.
    Cook [2] make a burger.
    Customer [5] come.
    Customer [11] come.
    Customer [12] come.
    Customer [13] come.
    Casher [1] accepts an order.
    Customer [14] come.
    Casher [1] take a burger to customor.
    Casher [1] accepts an order.
    Casher [1] take a burger to customor.
    Casher [1] accepts an order.
    Casher [1] take a burger to customor.
    Casher [1] accepts an order.
    Casher [1] take a burger to customor.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Cook [2] make a burger.
    Customer [15] come.
    Casher [1] accepts an order.
    Casher [1] take a burger to customor.
    Cook [2] make a burger.
    Customer [22] come.
    Casher [4] accepts an order.
    Casher [4] take a burger to customor.
    Customer [10] come.
    Cook [1] make a burger.
    Customer [23] come.
    Customer [24] come.
    Customer [16] come.
    Customer [18] come.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Customer [19] come.
    Casher [3] accepts an order.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Casher [3] take a burger to customor.
    Cook [2] make a burger.
    Customer [28] come.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Cook [2] make a burger.
    Customer [29] come.
    Casher [1] accepts an order.
    Cook [1] make a burger.
    Casher [1] take a burger to customor.
    Cook [2] make a burger.
    Customer [32] come.
    Customer [27] come.
    Customer [17] come.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Customer [20] come.
    Cook [1] make a burger.
    Cook [1] make a burger.
    Customer [21] come.
    Casher [4] accepts an order.
    Customer [25] come.
    Casher [3] accepts an order.
    Casher [1] accepts an order.
    Customer [30] come.
    Customer [31] come.
    Customer [34] come.
    Casher [4] take a burger to customor.
    Casher [4] accepts an order.
    Casher [4] take a burger to customor.
    Casher [4] accepts an order.
    Casher [4] take a burger to customor.
    Casher [4] accepts an order.
    Casher [4] take a burger to customor.
    Customer [33] come.
    Cook [1] make a burger.
    Cook [1] make a burger.
    Cook [1] make a burger.
    Cook [2] make a burger.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Casher [1] take a burger to customor.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Casher [3] take a burger to customor.
    Cook [2] make a burger.
    Customer [36] come.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Cook [2] make a burger.
    Customer [37] come.
    Casher [1] accepts an order.
    Casher [1] take a burger to customor.
    Cook [2] make a burger.
    Customer [38] come.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Cook [2] make a burger.
    Customer [41] come.
    Casher [4] accepts an order.
    Casher [4] take a burger to customor.
    Cook [2] make a burger.
    Customer [26] come.
    Casher [2] accepts an order.
    Casher [2] take a burger to customor.
    Customer [39] come.
    Customer [40] come.
    Casher [1] accepts an order.
    Cook [2] make a burger.
    Casher [3] accepts an order.
    Casher [3] take a burger to customor.
    Customer [35] come.
    Casher [1] take a burger to customor.
    Casher [1] accepts an order.
    Casher [1] take a burger to customor.
    Cook [2] make a burger.
    Cook [2] make a burger.
    Cook [1] make a burger.
    ```



If there is any mistake or question, please do not hesitate to contact:

Author: Dou Yi-Ming

Tel: 18017112986

E-mail: douyiming@sjtu.edu.cn