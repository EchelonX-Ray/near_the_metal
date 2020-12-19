#include "clib_math.h"
#include "clib_string.h"

double sin(register double x) {
	return cos(x + M_PI_2);
}

float sinf(register float x) {
	return (float)(sin((double)x));
}

double cos(register double x) {
	register double ret_val_sign;
	
	// The first step translates x into the first quadrant.
	// It makes 0 <= x < 0.5PI
	
	// Make angle positive
	// Cosine Identity: cos(x) = cos(-x)
	if (x < 0.0) {
		x = -x;
	}
	// Make angle no larger than 360 degrees (2PI) (not inclusive)
	while (x >= 2 * M_PI) {
		x -= 2 * M_PI;
	}
	// Handle quadrant 4
	if (x >= 3 * M_PI_2) {
		x = (2 * M_PI) - x;
		ret_val_sign = 1.0;
	// Handle quadrant 3
	} else if (x >= M_PI) {
		x = x - M_PI;
		ret_val_sign = -1.0;
	// Handle quadrant 2
	} else if (x >= M_PI_2) {
		x = M_PI - x;
		ret_val_sign = -1.0;
	// Handle quadrant 1
	} else {
		ret_val_sign = 1.0;
	}
	
	// Now we can actually find the approximation of sin(x)
	if (x == 0.0) {
		return 1.0 * ret_val_sign;
	}
	if (x == M_PI_2) {
		return 0.0;
	}
	//printf("[Trace A] x = %f\n", x);
	
	register double working_angle;
	register double half_angle;
	register double working_angle_value;
	register double half_angle_value;
	working_angle = M_PI_4;
	half_angle = working_angle;
	working_angle_value = sqrt(2.0) / 2.0;
	half_angle_value = working_angle_value;
	while (working_angle != x && half_angle > 0.0 && half_angle_value > 0.0) {
		//printf("[Trace B] working_angle = %f, working_angle_value = %f\n", working_angle, working_angle_value);
		half_angle /= 2.0;
		half_angle_value = sqrt((half_angle_value + 1) / 2);
		
		register double sin_working_value;
		register double sin_half_value;
		sin_working_value = sqrt(1 - (working_angle_value * working_angle_value));
		sin_half_value = sqrt(1 - (half_angle_value * half_angle_value));
		if (working_angle < x) {
			working_angle += half_angle;
			working_angle_value = (working_angle_value * half_angle_value) - (sin_working_value * sin_half_value);
		} else {
			working_angle -= half_angle;
			working_angle_value = (working_angle_value * half_angle_value) + (sin_working_value * sin_half_value);
		}
		//printf("[Trace C] working_angle = %f, working_angle_value = %f\n", working_angle, working_angle_value);
	}
	
	return working_angle_value * ret_val_sign;
	
	/*
	register double angle_val = M_PI_4;
	register double angle_cos = sqrt(2.0) / 2.0;
	register double half_angle = M_PI_4;
	register double half_cos = angle_cos;
	while (angle_val != x && half_angle > 0.0 && half_cos > 0.0) {
		register double angle_sin;
		register double half_sin;
		half_angle /= 2.0;
		//printf("[TraceA %f: %f]\n", angle_val, (1.0 + half_cos) / 2.0);
		half_cos = sqrt((1.0 + half_cos) / 2.0); // Half-angle formula
		printf("[TraceBa %f: %f]\n", angle_val, 1.0 - (half_cos * half_cos));
		return sqrt(1.0 - (half_cos * half_cos));
		printf("[TraceBb %f: %f]\n", angle_val, sqrt(1.0 - (half_cos * half_cos)));
		half_sin = sqrt(1.0 - (half_cos * half_cos)); // Pythagorean Theorem
		*/
		//unsigned long int traceval_a = (unsigned long int)(1.0 - (angle_cos * angle_cos));
		/*
		double d0 = angle_cos;
		unsigned long int traceval_a = *((unsigned long int*)(&d0));
		unsigned int traceval_b = (unsigned int)(0xFFFFFFFF & (traceval_a >> 32));
		unsigned int traceval_c = (unsigned int)(0xFFFFFFFF & (traceval_a >>  0));
		char bfr[50];
		itoa(traceval_b, bfr, 50, -16, 8);
		write(1, bfr, 8);
		itoa(traceval_c, bfr, 50, -16, 8);
		write(1, bfr, 8);
		printf("[TraceC %f: %f]\n", angle_val, angle_cos);
		*/
		/*
		angle_sin = sqrt(1.0 - (angle_cos * angle_cos)); // Pythagorean Theorem
		if (angle_val < x) {
			angle_val += half_angle;
			angle_cos = (angle_cos * half_cos) - (angle_sin * half_sin);
			printf("TraceA %f\n", angle_cos);
		} else {
			angle_val -= half_angle;
			printf("TraceB %f\n", (angle_cos * half_cos));
			printf("TraceB %f\n", angle_sin);
			printf("TraceB %f\n", half_sin);
			angle_cos = (angle_cos * half_cos) + (angle_sin * half_sin);
		}
	}
	
	if (ret_val_sign == 1) {
		return angle_cos;
	} else {
		return -angle_cos;
	}
	*/
}

float cosf(register float x) {
	return (float)(cos((double)x));
}

double sqrt(register double x) {
	if (x == 0.0) {
		return 0.0;
	}
	if (x < 0.0) {
		exit(1);
		return NAN;
	}
	if (x == NAN) {
		exit(2);
		return NAN;
	}
	if (x == +INFINITY) {
		exit(3);
		return +INFINITY;
	}
	
	/*
	TODO: Inline ASM Optimization
	*/
	
	// [How This Function Works]
	// 
	// The process of computing a square root is an iterative process.  
	// The process approches the correct answer as it is repeated.
	// We will repeat the process until one of 4 things is true:
	// --The same number has been computed twice in a row.  (We have probably reached the limit of precision of the data type)
	// --The same number has been computed twice but is alternating with another value.  (This is similar to the above case)
	// --The number multiplied by itself (squared) equals the number we are trying to find the square root of.  (We have found the square root)
	// --We have iterated 10000 times and are probably caught in some kind of infinite loop.  (Fault condition)  
	//   If this happens, print an error so I know to look for a bug.
	// 
	// For clarity in the equations and examples in this comment:
	// --The symbol ^ means exponent.
	// --The symbol == means that both sides of an equation equal each other.
	// --The symbol = means assignment.
	// --The symbol ~ means approximately.
	// 
	// The procedure to find a square root is based on the Newton–Raphson method in Calculus math.
	// The root(s) of a function are the value(s) that when passed into it, correspond to that 
	// function's intersecting of the the X-Axis.  Or, in the other words, the value(s) of x that yield f(x) == 0.
	// The Newton–Raphson method states that to find a better approximation of the root of a function f(x),
	// Use the following formula with a starting approximation of the root(Our guess): 
	// new_guess == guess - f(guess) / f'(guess)
	// 
	// Here is an example:
	// 
	// Let's say that we want to get the square root of 25.  We have the equation:
	// x^2 == 25
	// 
	// Convert to a function with an appropriate root:
	// x^2 - 25 == 0
	// f(x) == x^2 - 25
	// 
	// Find the derivative:
	// f'(x) == 2x
	// 
	// Newton–Raphson method Formula:
	// Let x be our guess for the iteration.
	// new_guess == x - f(x) / f'(x)
	// 
	// Subsitute our functions into the Newton–Raphson method formula:
	// new_guess == x - (x^2 - 25) / (2x)
	// 
	// Now just simplify:
	// new_guess == (2x^2 - (x^2 - 25)) / (2x)
	// new_guess == (2x^2 + -x^2 + 25) / (2x)
	// new_guess == (2x^2 - x^2 + 25) / (2x)
	// new_guess == (x^2 + 25) / (2x)
	// new_guess == (x + 25 / x) / 2
	// 
	// This formula can now be used to iterate to approach the square root of 25:
	// new_guess == (x + 25 / x) / 2
	// If you think about it, the above formula makes sense.  It is basically just finding an average.
	// If you divide 25 by less than it's square root, you will get an answer bigger than it's square root.
	// If you divide 25 by more than it's square root, you will get an answer smaller than it's square root.
	// The further off you are from the square root of 25 in your guess the further off from the result is 
	// from the square root of 25 in the other direction.  Therefore, we just need to find the average 
	// between the result of the division from 25 and our guess to come up with a new guess that will be 
	// closer to the square root.
	// 
	// Here we iterate with the formula:
	// As I said earlier, let x be our guess for the iteration.
	// Start with a guess of 1.
	// 
	// x = 1
	// Iteration 1) new_guess = (x + 25 / x) / 2
	// Iteration 1) new_guess == 13
	// Iteration 1) x = new_guess
	// Iteration 2) new_guess = (x + 25 / x) / 2
	// Iteration 2) new_guess == ~7.4615
	// Iteration 2) x = new_guess
	// Iteration 3) new_guess = (x + 25 / x) / 2
	// Iteration 3) new_guess == ~5.4060
	// Iteration 3) x = new_guess
	// Iteration 4) new_guess = (x + 25 / x) / 2
	// Iteration 4) new_guess == ~5.0152
	// Iteration 4) x = new_guess
	// Iteration 5) new_guess = (x + 25 / x) / 2
	// Iteration 5) new_guess == ~5.0000
	// Iteration 5) x = new_guess
	// Iteration 6) new_guess = (x + 25 / x) / 2
	// Iteration 6) new_guess == ~5.0000
	// Iteration 6) x = new_guess
	// THE SQUARE ROOT IS: 5
	// 
	// We can also use N instead of 25 to find a formula that will work to get 
	// the square root of any real and positive input number:
	// 
	// Let's say that we want to get the square root of N.  We have the equation:
	// x^2 == N
	// 
	// Convert to a function with an appropriate root:
	// x^2 - N == 0
	// f(x) == x^2 - N
	// 
	// Find the derivative:
	// f'(x) == 2x
	// 
	// Newton–Raphson method Formula:
	// Let x be our guess for the iteration.
	// new_guess == x - f(x) / f'(x)
	// 
	// Subsitute our functions into the Newton–Raphson method formula:
	// new_guess == x - (x^2 - N) / (2x)
	// 
	// Now just simplify:
	// new_guess == (2x^2 - (x^2 - N)) / (2x)
	// new_guess == (2x^2 + -x^2 + N) / (2x)
	// new_guess == (2x^2 - x^2 + N) / (2x)
	// new_guess == (x^2 + N) / (2x)
	// new_guess == (x + N / x) / 2
	// 
	// We could stop here and use this formula to get the square root: 
	// new_guess == (x + N / x) / 2
	// 
	// But there is a problem.  It is not that it doesn't work.
	// It does work, but the first step to solve it, the division of (N / x), is a computationally 
	// expensive operation.  The divide by 2 is not comparatively expensive because it is just a 
	// decimal point shift in base 2.  This formula has to be executed on every iteration.  So if 
	// many iterations are required or many calls to this function are needed by an application, 
	// this becomes a bit of a bottleneck.  What we need is a way to iterate without expensive 
	// division.
	// 
	// Let's look back at the original problem equation:
	// x^2 == N
	// 
	// What if instead we use:
	// 1/(x^2) == 1/N
	// or
	// x^(-2) == N^(-1)
	// x^2 still equals N, but we write it as reciprocals.
	// 
	// Now, let's break it up into 2 different equations:
	// I == N^(-1)
	// x^(-2) == I
	// 
	// Convert the 2nd equation to a function with an appropriate root:
	// x^(-2) - I == 0
	// f(x) == x^(-2) - I
	// 
	// Find the derivative:
	// f'(x) == -2x^(-3)
	// 
	// Newton–Raphson method Formula:
	// Let x be our guess for the iteration.
	// new_guess == x - f(x) / f'(x)
	// 
	// Subsitute our functions into the Newton–Raphson method formula:
	// new_guess == x - (x^(-2) - I) / (-2x^(-3))
	// 
	// Now just simplify:
	// new_guess == (x(-2x^(-3)) - (x^(-2) - I)) / (-2x^(-3))
	// new_guess == (-2x^(-2) - (x^(-2) - I)) / (-2x^(-3))
	// new_guess == (-2x^(-2) + -x^(-2) + I) / (-2x^(-3))
	// new_guess == (-2x^(-2) - x^(-2) + I) / (-2x^(-3))
	// new_guess == (-3x^(-2) + I) / (-2x^(-3))
	// new_guess == (-3x^(-2) + I) / (-2(1 / x^3))
	// new_guess == (-3x^(-2) + I) / (-2 / x^3)
	// new_guess == (-3(1 / x^2) + I) / (-2 / x^3)
	// new_guess == (-3 / x^2 + I) / (-2 / x^3)
	// new_guess == (-3 / x^2 + I(x^2) / x^2) / (-2 / x^3)
	// new_guess == (-3 + I(x^2) / x^2) / (-2 / x^3)
	// new_guess == (-3 + I(x^2) / x^2) * (x^3 / -2)
	// new_guess == (x^3)(-3 + I(x^2)) / -2x^2
	// new_guess == (-3x^3 + I(x^2)(x^3)) / -2x^2
	// new_guess == (-3x^3 + I(x^5)) / -2x^2
	// new_guess == (-3x + I(x^3)) / -2
	// new_guess == (3x - I(x^3)) / 2
	// new_guess == x(3 - I(x^2)) / 2
	// 
	// Now we have a formula that we can iterate with:
	// new_guess == x(3 - I(x^2)) / 2
	// 
	// It only requires multiplication, subtraction, and one division by 2 (computationally easy enough)
	// You may notice, though, that the formula requires I (I == 1/N), not N (The square that we know 
	// and are trying to find the root of).  So we do have to do one heavy division calculation up front 
	// to find it.  That is okay because it doesn't have to be repeated for each iteration of the loop.
	// 
	// Below is the implementation of this formula to find the square root.
	// In the implementation: 
	// --guess(x) is in variable guess
	// --new_guess(new_guess) is in variable new_guess
	// --value_to_find_the_square_root_of(N) is in variable x
	// --reciprocal_of_N(I) is in variable inverted
	// 
	// Additional IMPORTANT Notes/Edge Cases: 
	// --Under certain circumstances, the guessed value can alternate when at the limits of the 
	//   datatype's precision.  For this reason, the variable old_guess was added to track the guess 
	//   before the previous guess.
	// --The Newton-Raphson interation will converge on the square root if the starting guess is less 
	//   than the square root.  However, in cases where we are trying to find the square root of a 
	//   number less than 1, the square root will be both larger than that number and less than 1.  
	//   In this case, we should start our guess with the number we are trying to find the square root 
	//   of (x) instead of 1.  We can not start out with a guess of 0.  Doing so breaks the interation.
	
	register double guess;
	register double new_guess;
	register double old_guess;
	register double inverted;
	register unsigned int iteration_count;
	if (x < 1.0) {
		guess = x;
	} else {
		guess = 1.0;
	}
	old_guess = 0.0;
	inverted = 1.0 / x;
	iteration_count = 0;
	while (guess * guess != x && iteration_count < 10000) {
		new_guess = (guess * (3.0 - (inverted * guess * guess))) / 2.0;
		if (new_guess == guess || new_guess == old_guess) {
			return guess;
		}
		old_guess = guess;
		guess = new_guess;
		iteration_count++;
	}
	if (iteration_count >= 10000) {
		dprintf(2, "THIS IS A BUG!!  If you see this, please report it.  sqrt(value) iterations hit or exceeded 10000.  Probable infinite loop broken on value == %f\n", x);
	}
	return guess;
}

float sqrtf(register float x) {
	return (float)(sqrt((double)x));
}
