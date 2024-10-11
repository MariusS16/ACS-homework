function [decoded_path] = decode_path (path, lines, cols)
	% path -> vector containing the order of the states that arrived
	% 		 to a winning position
	% lines -> numeber of lines
	% cols -> number of columns
	
	% decoded_path -> vector of pairs (line_index, column_index)
  %                 corresponding to path states
  % hint: decoded_path does not contain indices for the WIN state

  % TODO: decode_path implementation

	nr = size(path) - 1;
  decoded_path = zeros(nr, 2);
  for i = 1 : nr
    if path(i) <= lines * cols
      decoded_path(i, 1) = floor((path(i) - 1) / cols) + 1;
      col = mod(path(i), cols);
      if col == 0
        decoded_path(i, 2) = cols;
      else
        decoded_path(i, 2) = col;
      endif
    endif
  endfor
  
endfunction