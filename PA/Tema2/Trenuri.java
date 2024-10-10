import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class Trenuri {
	public static void main(String[] args) throws FileNotFoundException {
		var scanner = new Scanner(new FileReader("trenuri.in"));
		String[] cts = scanner.nextLine().split(" ");
		cts[0] = cts[0].trim();
		int M = Integer.parseInt(scanner.nextLine());


		Map<String, List<String>> adj = new HashMap<>();
		for (int i = 0; i < M; i++) {
			String[] line = scanner.nextLine().split(" ");
			String a = line[0];
			String b = line[1];
			if (!adj.containsKey(a)) {
				adj.put(a, new ArrayList<>());
			}
			adj.get(a).add(b);
		}


		List<String> topo = new ArrayList<>();
		Set<String> visited = new HashSet<>();
		for (String city : adj.keySet()) {
			if (!visited.contains(city)) {
				dfs(city, adj, visited, topo);
			}
		}
		Collections.reverse(topo);


		Map<String, Integer> dist = new HashMap<>();
		for (String city : topo) {
			dist.put(city, Integer.MIN_VALUE);
		}

		String cityA = cts[0];
		dist.put(cityA, 0);

		for (String city : topo) {
			if (dist.get(city) == Integer.MIN_VALUE) {
				continue;
			}
			if (adj.containsKey(city)) {
				for (String neigh : adj.get(city)) {
					if (dist.get(neigh) < dist.get(city) + 1) {
						dist.put(neigh, dist.get(city) + 1);
					}
				}
			}
		}

		String cityB = cts[1];
		int result = dist.get(cityB) + 1;

		try (BufferedWriter writer = new BufferedWriter(new FileWriter("trenuri.out"))) {
			writer.write(String.valueOf(result));
		} catch (IOException e) {
			System.err.println("Eroare scriere");
		}
	}

	private static void dfs(String city, Map<String,
			List<String>> adj, Set<String> visited, List<String> topo) {
		visited.add(city);
		if (adj.containsKey(city)) {
			for (String neigh : adj.get(city)) {
				if (!visited.contains(neigh)) {
					dfs(neigh, adj, visited, topo);
				}
			}
		}
		topo.add(city);
	}
}
