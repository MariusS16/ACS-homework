#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"


int main(int argc, char const *argv[])
{
	FILE *in;
	in  = fopen(argv[1], "rb");

	int nr = 0;
	fread(&nr, sizeof(int), 1, in); //citire numar de sensori
	sensor *s = calloc(nr, sizeof(sensor));

	for (int i = 0; i < nr; i++) {//citirea datelor despre
		int tip;                   //fiecare senzor din fisierul binar
		enum sensor_type sensortype;
		fread(&tip, sizeof(int), 1, in); // citire tip

		if (tip == 0) { //citire tire
			sensortype = TIRE;
			s[i].sensor_type = sensortype;
			s[i].sensor_data = (tire_sensor *) malloc(sizeof(tire_sensor));
			fread(s[i].sensor_data, sizeof(tire_sensor), 1, in);
		}
		if (tip == 1) { //citire power
			sensortype = PMU;
			s[i].sensor_type = sensortype;
			s[i].sensor_data = malloc(sizeof(power_management_unit));
			fread(s[i].sensor_data, sizeof(power_management_unit), 1, in);
		}

		fread(&(s[i].nr_operations), sizeof(int), 1, in);//citire numar operatii

		s[i].operations_idxs = calloc(s[i].nr_operations, sizeof(int));
		for (int j = 0; j < s[i].nr_operations; j++) {
			fread(&(s[i].operations_idxs[j]), sizeof(int), 1, in);
		}
	}

	for (int i = 0; i < nr - 1; i++)   //bubble sort
		for (int j = 0; j < nr - i - 1; j++) {
			if (s[j].sensor_type == TIRE && s[j+1].sensor_type == PMU) {
				sensor aux = s[j];
				s[j] = s[j+1];
				s[j+1] = aux;
			}
		}

	char Comand[8];
	int NrComanda;
	scanf("%s", Comand);
	while (strcmp("exit", Comand)) {//citirea comenzilor de la tastatura
		if (strcmp("clear", Comand)) {//citire numarului de comanda->
			scanf("%d", &NrComanda);//pentru print si analyze
		}
		if (strstr("print", Comand)) {//afisare print
			if (NrComanda >= 0 && NrComanda < nr) {
				if (s[NrComanda].sensor_type == TIRE) {
					tire_sensor *rf = s[NrComanda].sensor_data;
					printf("Tire Sensor\n");
					printf("Pressure: %.2f\n", rf->pressure);
					printf("Temperature: %.2f\n", rf->temperature);
					printf("Wear Level: %d%%\n", rf->wear_level);
					if (rf->performace_score == 0)
						printf("Performance Score: Not Calculated\n");
					else
						printf("Performance Score: %d\n", rf->performace_score);
				}
				if (s[NrComanda].sensor_type == PMU) {
					printf("Power Management Unit\n");
					power_management_unit *rf = s[NrComanda].sensor_data;
					printf("Voltage: %.2f\n", rf->voltage);
					printf("Current: %.2f\n", rf->current);
					printf("Power Consumption: %.2f\n", rf->power_consumption);
					printf("Energy Regen: %d%%\n", rf->energy_regen);
					printf("Energy Storage: %d%%\n", rf->energy_storage);
				}
			} else {
				printf("Index not in range!\n");
			}
		}
		if (strstr("analyze", Comand)) {//analyze
			void (*op[8])(void *);
			get_operations((void **)op);
			if (NrComanda >= 0 && NrComanda < nr) {
				int number = NrComanda;
				for (int k = 0; k < s[number].nr_operations; k++) {
					op[s[number].operations_idxs[k]](s[number].sensor_data);
				}
			} else {
				printf("Index not in range!\n");
			}
		}
		if (strstr("clear", Comand)) {//clear
			for (int i = 0; i < nr; i++) {//presupun ca senzorul este bun,
				int ok = 1;//daca incalca una dintre contidii atunci ok=0
				if (s[i].sensor_type == TIRE) {
					if (((tire_sensor *)(s[i].sensor_data))->pressure < 19)
					ok = 0;
					if (((tire_sensor *)(s[i].sensor_data))->pressure > 28)
						ok = 0;
					if (((tire_sensor *)(s[i].sensor_data))->temperature < 0)
						ok = 0;
					if (((tire_sensor *)(s[i].sensor_data))->temperature > 120)
					ok = 0;
					if (((tire_sensor *)(s[i].sensor_data))->wear_level < 0)
						ok = 0;
					if (((tire_sensor *)(s[i].sensor_data))->wear_level > 100)
					ok = 0;
				}
				if (s[i].sensor_type == PMU) {
					power_management_unit *rf = s[i].sensor_data;
					if ((rf->voltage < 10) || (rf->voltage > 20))
						ok = 0;
					if ((rf->current < -100) || (rf->current > 100))
						ok = 0;
					if (rf->power_consumption < 0)
						ok = 0;
					if (rf->power_consumption > 1000)
					ok = 0;
					if ((rf->energy_regen < 0) || (rf->energy_regen > 100))
						ok = 0;
					if ((rf->energy_storage < 0) || (rf->energy_storage > 100))
						ok = 0;
				}
				if (ok == 0) {//daca senzorul nu este bun(ok == 0) atunci
					free(s[i].sensor_data);//il dezaloc si mut toate elementele
					free(s[i].operations_idxs);//din vector la stanga
					for (int l = i; l < nr - 1; l++) {
						s[l] = s[l + 1];
					}
					i--;
					nr--;
				}
			}
		}
		scanf("%s", Comand);
	}


	for (int i = 0; i < nr; i++) {
		free(s[i].sensor_data);
		free(s[i].operations_idxs);
	}
	free(s);


	fclose(in);
	return 0;
}
