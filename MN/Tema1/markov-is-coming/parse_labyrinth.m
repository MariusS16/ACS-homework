function [Labyrinth] = parse_labyrinth(file_path)
	% file_path -> the relative path to a file that needs to
  %              be loaded_graphics_toolkits
  
  fileID = fopen(file_path);
  m = fscanf(fileID, "%d", 1);
  n = fscanf(fileID, "%d", 1);
  A = fscanf(fileID, "%d", [n, m]);
  Labyrinth = A';

endfunction
