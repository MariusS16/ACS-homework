import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Compresie {

	public static void main(String[] args) {

		int N = 0, M = 0;
		int[] vector1 = new int[100000000];
		int[] vector2 = new int[100000000];

		try (BufferedReader reader = new BufferedReader(new FileReader("compresie.in"))) {
			N = Integer.parseInt(reader.readLine());
			String[] vector1String = reader.readLine().split(" ");
			for (int i = 0; i < N; i++) {
				vector1[i] = Integer.parseInt(vector1String[i]);
			}

			M = Integer.parseInt(reader.readLine());
			String[] vector2String = reader.readLine().split(" ");
			for (int i = 0; i < M; i++) {
				vector2[i] = Integer.parseInt(vector2String[i]);
			}
		} catch (IOException e) {
			System.err.println("Eroare citire");
		}

		int i = 0, j = 0, nr = 0;
		//parcurg vectorii simultan si compar elementele lor
		while (i < N && j < M) {
			//daca elementul din vector1 este mai mare, incrementez j si adun elementul anterior
			if (vector1[i] > vector2[j]) {
				j++;
				vector2[j] = vector2[j] + vector2[j - 1];
			}
			//daca sunt egale, incrementez i si j si numarul de elemente comune
			if (vector1[i] == vector2[j]) {
				i++;
				j++;
				nr++;
			}
			//daca elementul din vector1 este mai mic, incrementez i si adun elementul anterior
			if (vector1[i] < vector2[j]) {
				i++;
				vector1[i] = vector1[i] + vector1[i - 1];
			}
		}

		//daca nu am parcurs toate elementele din vectori, inseamna ca nu sunt egali
		if (i != N || j != M) {
			nr = -1;
		}

		try (BufferedWriter writer = new BufferedWriter(new FileWriter("compresie.out"))) {
			writer.write(nr + "");
		} catch (IOException e) {
			System.err.println("Eroare la scrire");
		}
	}
}
