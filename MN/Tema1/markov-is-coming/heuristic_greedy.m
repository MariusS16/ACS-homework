function [path] = heuristic_greedy (start_position, probabilities, Adj)
	% start_position -> the starting point in the labyrinth
	% probabilities -> vector of associated probabilities: including WIN and LOSE
	% Adj -> adjacency matrix
	
	% path -> the states chosen by the algorithm
  
  % TODO: heuristic_greedy implementation


	path = [start_position];
	probabilities = [probabilities; 1; 0];
   	nr = 1;
   [m n] = size(Adj);
   visited = zeros(m, 1);
   visited(start_position) = 1;
   while nr != 0
    position = path(nr);
    if position == m - 1
      return
    endif
    ok = 0;
    max = 0;
    pmax = -1;
    for i = 1 : m
      if Adj(position, i) == 1
        if visited(i) == 0
          ok = 1;
          if probabilities(i) > max
              max = probabilities(i);
              pmax = i;
          endif
        endif
      endif
    endfor
    if ok == 0
      path = path(1 : nr - 1);
      nr = nr - 1;
    else
      neigh = pmax;
      visited(neigh) = 1;
      path = [path; neigh];
      nr = nr + 1;
    endif
   endwhile
endfunction
