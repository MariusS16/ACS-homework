import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Numarare {
	public static void main(String[] args) throws FileNotFoundException {
		var scanner = new Scanner(new FileReader("numarare.in"));

		String[] number = scanner.nextLine().split(" ");
		int N = Integer.parseInt(number[0]);
		int M = Integer.parseInt(number[1]);

		List<Integer>[] adj1 = new ArrayList[N + 1];
		List<Integer>[] adj2 = new ArrayList[N + 1];
		List<Integer>[] adj3 = new ArrayList[N + 1];
		for (int i = 1; i <= N; i++) {
			adj1[i] = new ArrayList<>();
			adj2[i] = new ArrayList<>();
			adj3[i] = new ArrayList<>();
		}

		int indexE = 0;
		while (indexE < M) {
			indexE++;
			String[] ed = scanner.nextLine().split(" ");
			adj1[Integer.parseInt(ed[0])].add(Integer.parseInt(ed[1]));
		}

		indexE = 0;
		while (indexE < M) {
			indexE++;
			String[] ed = scanner.nextLine().split(" ");
			adj2[Integer.parseInt(ed[0])].add(Integer.parseInt(ed[1]));
		}

		for (int i = 1; i <= N; i++) {
			for (int neigh1 : adj1[i]) {
				for (int neigh2 : adj2[i]) {
					if (neigh1 == neigh2) {
						adj3[i].add(neigh1);
					}
				}
			}
		}

		int[] dp = new int[N + 1];
		dp[1] = 1;

		for (int i = 1; i <= N; i++) {
			if (dp[i] != 0) {
				for (int neigh : adj3[i]) {
					dp[neigh] = (dp[neigh] + dp[i]) % 1_000_000_007;
				}
			}
		}

		int result = dp[N];

		try (BufferedWriter writer = new BufferedWriter(new FileWriter("numarare.out"))) {
			writer.write(String.valueOf(result));
		} catch (IOException e) {
			System.err.println("Eroare scriere");
		}
	}
}