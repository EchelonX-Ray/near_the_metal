#include "clib_math.h"
#include "clib_string.h"

double sin(register double x) {
	// A 90 degree phase shift to the right of a cosine function 
	// is equivalent to a sine function
	return cos(x - M_PI_2);
}

float sinf(register float x) {
	return (float)(sin((double)x));
}

double cos(register double x) {
	register double ret_val_sign;
	
	// For simpler computation, translate the angle x, into the first quadrant. 
	// Set the variable ret_val_sign to be ether positive or negative depending 
	// on what the cosine is should be.
	
	// Make the angle positive
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
	
	// Handle the easy cases
	if (x <= 0.0) {
		return ret_val_sign;
	}
	if (x == M_PI_2) {
		return 0.0;
	}
	
	// Now we have an angle in quadrant 1.
	
	// [How This Function Works]
	// 
	// The examples provided here are in degrees for ease of comprehension.
	// However, the actual function takes radians.
	// 
	// Start at a 45 degree angle.
	// The cosine of 45 degrees is (sqrt(2) / 2).
	// 
	// With these known values the following formulas are used:
	// Half Angle Formula:             cos(x / 2) = sqrt((1 + cos(x)) / 2)
	// Relationship of sine to cosine: sin(x) = sqrt(1 - (cos(x) * cos(x)))
	// Add cosines:                    cos(a + b) = (cos(a) * cos(b)) - (sin(a) * sin(b))
	// Subtract cosines:               cos(a - b) = (cos(a) * cos(b)) + (sin(a) * sin(b))
	// 
	// The general process is this:
	// --The idea is to start with an estimation of 45 degrees and the known cosine value of that 
	//   (sqrt(2) / 2).
	// --Next, use the half angle formula to iteratively divide an angle, starting at 45 degrees, 
	//   with the value of (sqrt(2) / 2), in half.
	// --Then add or subtract the result to the previous estimation.
	// 
	// Here is an example: 
	// To approximate the value of cos(56.25 degrees) to following process is preformed.
	// 1) Start with a 45 degree angle and the known cosine value of it, (sqrt(2) / 2)
	//      A = 45 deg
	//      B = sqrt(2) / 2 = ~0.707
	// 2) Next, starting again with 45 degrees and it's known cosine, divide the angle in half.
	//      a = 45 deg
	//      b = sqrt(2) / 2 = ~0.707
	//      new_a = a / 2 = 22.5 deg
	//      new_b = sqrt((1 + b) / 2) = ~0.924
	// 3) new_a and new_b become the new values of a and b respectively.
	//      a = new_a = 22.5 deg
	//      b = new_b = ~0.924
	// 4) Now, determine whether 45 degrees is more or less than 56.25 degrees.
	//    Since it is less, we should add this new half angle to it.
	//    To do this, we will need to find the sine of each of both the new half angle 
	//    and the current working angle.  These sine values will be used in the cosine addition 
	//    formula.
	//      new_A = A + a = 45 deg + 22.5 deg = 67.5 deg
	//      sin_B = sqrt(1 - (B * B)) = sqrt(1 - (~0.707 * ~0.707)) = ~0.707
	//      sin_b = sqrt(1 - (b * b)) = sqrt(1 - (~0.924 * ~0.924)) = ~0.383
	//      new_B = (B * b) - (sin_B * sin_b) = (~0.707 * ~0.924) - (~0.707 * ~0.383) = ~0.383
	// 5) new_A and new_B become the new values of A and B respectively.
	//      A = new_A = 67.5 deg
	//      B = new_B = ~0.383
	// 6) Now, divide the angle in half again.
	//      new_a = a / 2 = 22.5 deg / 2 = 11.25 deg
	//      new_b = sqrt((1 + b) / 2) = sqrt((1 + ~0.924) / 2) = ~0.981
	// 7) new_a and new_b become the new values of a and b respectively.
	//      a = new_a = 11.25 deg
	//      b = new_b = ~0.981
	// 8) Now, determine whether 67.5 degrees is more or less than 56.25 degrees.
	//    Since it is more, we should subtract this new half angle from it.
	//    To do this, we will need to find the sine of each of both the new half angle 
	//    and the current working angle.  These sine values will be used in the cosine 
	//    subtraction formula.
	//      new_A = A - a = 67.5 deg - 11.25 deg = 56.25 deg
	//      sin_B = sqrt(1 - (B * B)) = sqrt(1 - (~0.383 * ~0.383)) = ~0.924
	//      sin_b = sqrt(1 - (b * b)) = sqrt(1 - (~0.981 * ~0.981)) = ~0.195
	//      new_B = (B * b) + (sin_B * sin_b) = (~0.383 * ~0.981) + (~0.924 * ~0.195) = ~0.556
	// 9) new_A and new_B become the new values of A and B respectively.
	//      A = new_A = 56.25 deg
	//      B = new_B = ~0.556
	// And there you have it.  The angle of 56.25 degrees with a cosine value of approximately 0.556.
	//
	// Below is the implementation of this:
	
	register double working_angle;
	register double half_angle;
	register double working_angle_value;
	register double half_angle_value;
	
	// Initialize to 45 degrees (M_PI_4) and sqrt(2) / 2
	working_angle = M_PI_4;
	half_angle = working_angle;
	working_angle_value = sqrt(2.0) / 2.0;
	half_angle_value = working_angle_value;
	
	// Begin iterating
	while (working_angle != x && half_angle > 0.0 && half_angle_value > 0.0) {
		// Half the angle
		half_angle /= 2.0;
		half_angle_value = sqrt((half_angle_value + 1) / 2);
		
		register double sin_working_value;
		register double sin_half_value;
		
		// Use the Pythagorean Theorem / Unit Circle formula to find the sin values 
		// of the half angle and the current working angle.
		sin_working_value = sqrt(1 - (working_angle_value * working_angle_value));
		sin_half_value = sqrt(1 - (half_angle_value * half_angle_value));
		
		// Do we need to add or subtract the half angle from the current approximation 
		// to get closer to the angle we are trying to approximate (x)?
		if (working_angle < x) {
			// Add the half angle
			working_angle += half_angle;
			working_angle_value = (working_angle_value * half_angle_value) - (sin_working_value * sin_half_value);
		} else {
			// Subtract the half angle
			working_angle -= half_angle;
			working_angle_value = (working_angle_value * half_angle_value) + (sin_working_value * sin_half_value);
		}
	}
	
	// Take the resultant working angle approximated cosine value.
	// Multiply it by the return sign to get the correct negation for the quadrant.
	// Return the final value.
	return working_angle_value * ret_val_sign;
}

float cosf(register float x) {
	return (float)(cos((double)x));
}

double sqrt(register double x) {
	if (x == 0.0) {
		return 0.0;
	}
	if (x < 0.0) {
		return NAN;
	}
	if (x == NAN) {
		return NAN;
	}
	if (x == +INFINITY) {
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
