/* This program is just a copy of sample_1.dsl. 
You should write your second interesting FCAL
program and save it in a file with this name. */

main () { 
	boolean flag;
	flag = True;
	
	int num;
	num = 1;
	int limit;
	limit = 10;
	int print_num;

	while (flag) {
		print_num = 3 * num;
		print(print_num);
		print("\n");		
		num = num + 1;	
		if (num > limit) {
			flag = False;
		}
	}
}



