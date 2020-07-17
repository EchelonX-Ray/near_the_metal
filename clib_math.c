#include "clib_math.h"

float c_sqrtf(register float x) {
	if (x == 0.0) {
		return 0.0;
	}
	if (x < 0.0) {
		// TODO: return NaN;
	}
	/*
	TODO: Handle NaN
	if (x < 0.0) {
		// TODO: return NaN;
	}
	*/
	/*
	TODO: Handle +Inf
	if (x < 0.0) {
		// TODO: return +Inf;
	}
	*/
	
	/*
	TODO: Inline ASM Optimization
	*/
	
	// [How This Function Works]
	// 
	// The process of computing a square root is an iterative process.  
	// The process approches the correct answer as it is repeated.
	// We will repeat the process until one of 3 things is true:
	// --The same number has been computed twice in a row.  (We have probably reached the limit of precision of the data type)
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
	// The root(s) of a function are the value(s) that when passed into it correspond to that 
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
	
	register float guess = 1.0;
	register float inverted = 1.0 / x;
	register unsigned int iteration_count = 0;
	while (guess * guess != x && iteration_count < 10000) {
		register float new_guess;
		new_guess = (guess * (3.0 - (inverted * guess * guess))) / 2.0;
		if (new_guess == guess) {
			return guess;
		}
		guess = new_guess;
		iteration_count++;
	}
	if (iteration_count >= 10000) {
		dprintf(2, "THIS IS A BUG!!  If you see this, please report it.  sqrtf(value) iterations hit or exceeded 10000.  Probable infinite loop broken on value == %f\n", x);
	}
	return guess;
}
