import java.io.BufferedWriter;
import java.io.FileWriter;

public class Colorare {
	public static void main(String[] args) {
		final int mod = 1_000_000_007;
		int N = 0;
		int[] nr = new int[100000000];
		char[] tip = new char[100000000];
		try {
			java.io.File file = new java.io.File("colorare.in");
			java.util.Scanner scanner = new java.util.Scanner(file);
			N = scanner.nextInt();
			for (int i = 0; i < N; i++) {
				nr[i] = scanner.nextInt();
				tip[i] = scanner.next().charAt(0);
			}
			scanner.close();
		} catch (java.io.FileNotFoundException e) {
			System.out.println("Eroare citire");
		}


		long numar = 0;
		//daca primul tip este V, numarul de posibilitati este 3, altfel 6
		char tipSt = tip[0];
		if ('V' == tipSt) {
			numar = 3;
		} else {
			numar = 6;
		}

		//calculez numarul de posibilitati pentru primul element
		long nrr = nr[0] - 1;
		if (nrr > 0) {
			int m = ('V' == tipSt) ? 2 : 3;
			//calculez m la puterea nrr
			long putere = pow(m, nrr);
			//inmultesc numarul de posibilitati cu m la puterea nrr
			numar = (numar * putere) % mod;
		}

		//calculez numarul de posibilitati pentru restul elementelor
		for (int i = 1; i < N; i++) {
			numar *= (tip[i] == 'V') ? (tipSt == 'H' ? 1 : 2) : (tipSt == 'V' ? 2 : 3);
			numar %= mod;
			//daca numarul de elemente este mai mare decat 1,
			//inmultesc cu 2 sau 3 la puterea numarului de elemente
			if (nr[i] - 1 > 0) {
				numar *= pow((tip[i] == 'V') ? 2 : 3, nr[i] - 1);
				numar %= mod;
			}
			tipSt = tip[i];
		}


		try (BufferedWriter writer = new BufferedWriter(new FileWriter("colorare.out"))) {
			writer.write(String.valueOf(numar));
		} catch (java.io.IOException e) {
			System.out.println("Eroare scrire");
		}
	}

	private static long pow(long a, long b) {
		//calculez a la puterea b
		final int mod = 1_000_000_007;
		long result = 1;
		a %= mod;
		while (b > 0) {
			if ((b & 1) != 0) {
				result = (result * a) % mod;
			}
			a = (a * a) % mod;
			b >>= 1;
		}
		return result;
	}
}