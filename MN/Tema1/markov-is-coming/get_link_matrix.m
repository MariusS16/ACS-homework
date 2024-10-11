function [Link] = get_link_matrix (Labyrinth)
	% Labyrinth -> the matrix of all encodings
  
  % Link -> the link matrix associated to the given labyrinth
  
  % TODO: get_link_matrix implementation

  AdjFull = full(get_adjacency_matrix(Labyrinth));
  [m n] = size(AdjFull);

  for i = 1 : m
    no_links = sum(AdjFull(i, 1 : n));
    for j = 1 : n
      AdjFull(i, j) = AdjFull(i, j) / no_links;
    endfor
  endfor

  Link = sparse(AdjFull);
endfunction
