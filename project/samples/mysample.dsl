/* This is the sample program in the FCAL language. 
It declares a boolean variable and uses it in a while loop 
to sum the squares 1 through 5 together, resulting in 55.
*/

main() {
	// Declare the numeric variables
	int sum_of_squares;
	sum_of_squares = 0 ;
	int num;
	num = 1 ;
	int max;
	max = 5 ;
	// Declare the boolean variable 
    	boolean flag;
    	flag = True ;

	while(flag) {
	  sum_of_squares = sum_of_squares + num * num ;
	  num = num + 1 ;
	  if (num > max) {
		flag = False ;
	  }
	}
}
