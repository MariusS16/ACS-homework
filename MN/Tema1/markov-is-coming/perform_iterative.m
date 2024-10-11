function [x, err, steps] = perform_iterative (G, c, x0, tol, max_steps)
	% G -> iteration matrix
	% c -> iteration vector
	% x0 -> initial solution
	% tol -> accepted tolerance (norm of the difference between two solutions)
	% max_steps -> the maximum number of iterations
	
	% x -> final solution
	% err -> last calculated difference (as an euclidean norm)
	% steps -> actual number of steps performed

   % TODO: perform_iterative implementation
   

   	G = full(G);
  	c = full(c);
  	x = x0;
  	steps = 0;
 	err = tol;
  	while (steps <= max_steps) && (err >= tol)
    	prev = x;
    	x = G * x + c;
    	steps = steps + 1;
    	err = norm(x - prev);
  endwhile
endfunction
