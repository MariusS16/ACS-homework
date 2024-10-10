import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Servere {
	public static void main(String[] args) {

		int N = 0;
		int[] putere = new int[100000000];
		int[] alimentare = new int[100000000];

		try (BufferedReader reader = new BufferedReader(new FileReader("servere.in"))) {
			N = Integer.parseInt(reader.readLine());
			String[] putereString = reader.readLine().split(" ");
			String[] alimentareString = reader.readLine().split(" ");
			for (int i = 0; i < N; i++) {
				putere[i] = Integer.parseInt(putereString[i]);
			}
			for (int i = 0; i < N; i++) {
				alimentare[i] = Integer.parseInt(alimentareString[i]);
			}
		} catch (IOException e) {
			System.err.println("Eroare citire");
		}

		//aflu minimimul si maximul din vectorul alimentare
		double min = 1000000000, max = -1000000000;
		for (int i = 0; i < N; i++) {
			if (alimentare[i] < min) {
				min = alimentare[i];
			}
			if (alimentare[i] > max) {
				max = alimentare[i];
			}
		}

		double marja = 0.1;
		double max_alm = -1000000000;

		while (marja < max - min) {
			//calculez mijlocul intervalului
			double mij_alm = (min + max) / 2;

			double min_alm_local = 1000000000;
			double min_alm_local_plus = 1000000000;
			double min_alm_local_minus = 1000000000;

			for (int i = 0; i < N; i++) {
				//calculez dupa formula putere - |mij_alm - alimentare|
				double form = putere[i] - Math.abs(mij_alm - alimentare[i]);
				if (form < min_alm_local) {
					min_alm_local = form;
				}

				form = putere[i] - Math.abs(mij_alm + marja - alimentare[i]);
				if (form < min_alm_local_plus) {
					min_alm_local_plus = form;
				}

				form = putere[i] - Math.abs(mij_alm - marja - alimentare[i]);
				if (form < min_alm_local_minus) {
					min_alm_local_minus = form;
				}
			}

			//daca minimul local este mai mare decat maximul, il inlocuiesc
			if (min_alm_local > max_alm) {
				max_alm = min_alm_local;
			}

			//daca minimul local plus este mai mare decat minimul, schimb intervalul
			if (min_alm_local_plus > min_alm_local) {
				min = mij_alm;
			} else {
				max = mij_alm;
			}
		}

		try (BufferedWriter writer = new BufferedWriter(new FileWriter("servere.out"))) {
			writer.write(String.format("%.1f", max_alm));
		} catch (IOException e) {
			System.err.println("Eroare la scrire");
		}
	}
}
