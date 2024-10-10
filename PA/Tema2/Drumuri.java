import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Scanner;


public class Drumuri {
	static class Muchie implements Comparable<Muchie> {
		int dist;
		long cost;
		Muchie(int dist, long cost) {
			this.dist = dist;
			this.cost = cost;
		}
		@Override
		public int compareTo(Muchie other) {
			return Long.compare(this.cost, other.cost);
		}
	}

	public static void main(String[] args) throws FileNotFoundException {
		var scanner = new Scanner(new FileReader("drumuri.in"));

		String[] number = scanner.nextLine().split(" ");
		int N = Integer.parseInt(number[0]);
		int M = Integer.parseInt(number[1]);

		ArrayList<Muchie>[] adj = new ArrayList[N + 1];
		ArrayList<Muchie>[] adjTr = new ArrayList[N + 1];
		for (int i = 1; i <= N; i++) {
			adj[i] = new ArrayList<>();
			adjTr[i] = new ArrayList<>();
		}

		for (int i = 0; i < M; i++) {
			String[] line = scanner.nextLine().split(" ");
			int a = Integer.parseInt(line[0]);
			int b = Integer.parseInt(line[1]);
			int c = Integer.parseInt(line[2]);
			adj[a].add(new Muchie(b, c));
			adjTr[b].add(new Muchie(a, c));
		}

		String[] xyz = scanner.nextLine().split(" ");
		int x = Integer.parseInt(xyz[0]);
		int y = Integer.parseInt(xyz[1]);
		int z = Integer.parseInt(xyz[2]);

		long[] distX = dijkstra(x, N, adj, z);
		long[] distY = dijkstra(y, N, adj, z);
		long[] distZ = dijkstra(z, N, adjTr, z);

		long min = distX[z] + distY[z];
		for (int i = 1; i <= N; i++) {
			if (distX[i] != Long.MAX_VALUE && distY[i] != Long.MAX_VALUE
					&& distZ[i] != Long.MAX_VALUE) {
				if (distX[i] + distY[i] + distZ[i] < min) {
					min = distX[i] + distY[i] + distZ[i];
				}
			}
		}

		try (BufferedWriter writer = new BufferedWriter(new FileWriter("drumuri.out"))) {
			writer.write(String.valueOf(min));
		} catch (IOException e) {
			System.err.println("Eroare scriere");
		}
	}

	private static long[] dijkstra(int source, int n, ArrayList<Muchie>[] adj, int dest) {
		long[] d = new long[n + 1];
		PriorityQueue<Muchie> pr = new PriorityQueue<>();

		for (int i = 1; i <= n; i++) {
			d[i] = Long.MAX_VALUE;
		}
		d[source] = 0;
		pr.add(new Muchie(source, 0));

		while (!pr.isEmpty()) {
			int node = pr.peek().dist;
			long cost = pr.poll().cost;

			if (cost > d[node]) {
				continue;
			}

			for (Muchie e : adj[node]) {
				int neigh = e.dist;
				long w = e.cost + d[node];
				if (w < d[neigh]) {
					d[neigh] = w;
					pr.add(new Muchie(neigh, w));
				}
			}
		}
		return d;
	}
}
