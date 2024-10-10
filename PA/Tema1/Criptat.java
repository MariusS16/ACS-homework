import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
public class Criptat {
	public static void main(String[] args) {
		int N = 0;
		String[] cuvinte = new String[100000000];
		try (BufferedReader reader = new BufferedReader(new FileReader("criptat.in"))) {
			N = Integer.parseInt(reader.readLine());
			for (int i = 0; i < N; i++) {
				cuvinte[i] = reader.readLine();
			}
		} catch (IOException e) {
			System.err.println("Eroare citire");
		}

		char[] litere = new char[26];
		for (int i = 0; i < 26; i++) {
			litere[i] = (char) ('a' + i);
		}

		//creez un vector de N vectori de 27 elemente
		double[][] litereCuvinte = new double[N][];
		for (int i = 0; i < N; i++) {
			//creez un vector de 26 elemente care contine raportul fiecarei litere din cuvant
			litereCuvinte[i] = numarareLitere(cuvinte[i]);
			//adaug la finalul fiecarui vector de litere numarul de litere din cuvant
			litereCuvinte[i][26] = cuvinte[i].length();
		}

		int numar = 1;

		//sortrez  litereCuvinte de 26 ori(pentru fiecare litera) descrescator
		for (int i = 0; i < 26; i++) {
			int numar_local = 0;
			String[] cuvinte_local = new String[N];
			for (int j = 0; j < N; j++) {
				cuvinte_local[j] = cuvinte[j];
			}

			for (int j = 0; j < N - 1; j++) {
				for (int k = j + 1; k < N; k++) {
					if (litereCuvinte[j][i] < litereCuvinte[k][i]) {
						double[] aux = litereCuvinte[j];
						litereCuvinte[j] = litereCuvinte[k];
						litereCuvinte[k] = aux;
						String aux2 = cuvinte_local[j];
						cuvinte_local[j] = cuvinte_local[k];
						cuvinte_local[k] = aux2;
					}
				}
			}


			//verific daca litera apare in mai mult de jumatate din cuvinte
			//daca da, adaug la numarul local numarul de litere din cuvant
			int aparitii_totale = 0;
			int aparitii_curente = 0;
			for (int j = 0; j < N; j++) {
				aparitii_curente = (int) Math.ceil(litereCuvinte[j][i] * litereCuvinte[j][26]);
				System.out.print(litereCuvinte[j][i] + " " + litereCuvinte[j][26] + '\n');
				if (aparitii_curente + aparitii_totale > (N + cuvinte_local[j].length()) / 2) {
					numar_local += cuvinte_local[j].length();
				}
			}
			//daca numarul local este mai mare decat numarul, il inlocuiesc
			if (numar_local > numar) {
				numar = numar_local;
			}
		}


		try (BufferedWriter writer = new BufferedWriter(new FileWriter("criptat.out"))) {
			System.out.println(numar);
			writer.write(numar + "");
		} catch (IOException e) {
			System.err.println("Eroare la scrire");
		}
	}

	private static double[] numarareLitere(String cuvant) {
		double[] litere = new double[27];
		for (int i = 0; i < cuvant.length(); i++) {
			litere[cuvant.charAt(i) - 'a'] += (double) 1 / cuvant.length();
		}
		return litere;
	}
}
