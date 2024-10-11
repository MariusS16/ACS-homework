function [Adj] = get_adjacency_matrix (Labyrinth)
	% Labyrinth -> the matrix of all encodings
  
  % Adj -> the adjacency matrix associated to the given labyrinth
  
  % TODO: get_adjacency_matrix implementation

	[m n] = size(Labyrinth);
  AdjFull = zeros(m * n + 2);
  AdjFull(m * n + 1, m * n + 1) = 1;
  AdjFull(m * n + 2, m * n + 2) = 1;
  for i = 1 : m
    for j = 1 : n
      I = n * (i - 1) + j;
      val = Labyrinth(i, j);
      binaryVal = dec2bin(val, 4);
      if binaryVal(4) == '0'
        if j == 1
          AdjFull(I, m * n + 2) = 1;
        else
          J = I - 1;
          AdjFull(I, J) = 1;
        endif
      endif

      if binaryVal(3) == '0'
        if j == n
          AdjFull(I, m * n + 2) = 1;
        else
          J = I + 1;
          AdjFull(I, J) = 1;
        endif
      endif

      if binaryVal(2) == '0'
        if i == m
          AdjFull(I, m * n + 1) = 1;
        else
          J = I + n;
          AdjFull(I, J) = 1;
        endif
      endif

      if binaryVal(1) == '0'
        if i == 1
          AdjFull(I, m * n + 1) = 1;
        else
          J = I - n;
          AdjFull(I, J) = 1;
        endif
      endif
    endfor
  endfor
  Adj = sparse(AdjFull);
endfunction
