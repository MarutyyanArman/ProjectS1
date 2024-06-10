#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define PARKING_PLACES 20
#define CAR_NUMBER 10
#define CAR_NAME 40

// Parking Space Properties
typedef struct {
    int id;                     // parking spot unique identifier
    bool isOccupied;            // indicates if the parking spot is occupied or not
    char carName[CAR_NAME];     // car name and model
    char carNumber[CAR_NUMBER]; // car number
    time_t start_time;          // entry time
    time_t end_time;            // exit time
} ParkingSpace;


void initializeParkingSpots(ParkingSpace parkingPlace[], int size) {
    for (int i = 0; i < size; i++) {
        parkingPlace[i].id = i + 1;
        parkingPlace[i].isOccupied = false;
        strcpy(parkingPlace[i].carName, "");
        strcpy(parkingPlace[i].carNumber, "");
        parkingPlace[i].start_time = 0;
        parkingPlace[i].end_time = 0;
    }
}

// Show available and occupied parking spots
void showParkingInfo(ParkingSpace parkingPlace[], int size) {
    printf("\nParking Status:\n");
    for (int i = 0; i < size; i++) {
        if (parkingPlace[i].isOccupied) {
            printf("Spot %d: Occupied\n", parkingPlace[i].id);
            printf("  Car Name: %s\n", parkingPlace[i].carName);
            printf("  Car Number: %s\n", parkingPlace[i].carNumber);
            printf("  Entry Time: %s", ctime(&parkingPlace[i].start_time));
            printf("  Exit Time: %s\n", parkingPlace[i].end_time == 0 ? "-" : ctime(&parkingPlace[i].end_time));
        } else {
            printf("Spot %d: Available\n", parkingPlace[i].id);
        }
    }
}

// Add a new car  
void enterParking(ParkingSpace parkingPlace[], int size) {
    int entered_ID;
    printf("Enter the ID of the parking spot you want to park (1-%d): ", size);
    scanf("%d", &entered_ID);
    entered_ID--;

    if (entered_ID >= 0 && entered_ID < size && !parkingPlace[entered_ID].isOccupied) {
        char carName[CAR_NAME], carNumber[CAR_NUMBER];
        printf("Enter the car model: ");
        // so that it will be able to enter several words as a car name
        getchar(); 
        fgets(carName, CAR_NAME, stdin); // Use fgets to read the input

        if (carName[strlen(carName) - 1] == '\n'){
            carName[strlen(carName) - 1] = '\0';
        }

        printf("Enter the car number (00XX000): ");
        scanf("%s", carNumber);

        // check if the entered number corresponds to the format
        if(isdigit(carNumber[0]) && isdigit(carNumber[1]) && 
        isupper(carNumber[2]) && isupper(carNumber[3]) && 
        isdigit(carNumber[4]) && isdigit(carNumber[5]) && 
        isdigit(carNumber[6]) && carNumber[7] == '\0'){
            parkingPlace[entered_ID].isOccupied = true;
            strcpy(parkingPlace[entered_ID].carNumber, carNumber);
            strcpy(parkingPlace[entered_ID].carName, carName);

            // The time when the car parks
            time(&parkingPlace[entered_ID].start_time);

            printf("Thank you! The car parked at spot %d.\n", parkingPlace[entered_ID].id);
        } else{
            printf("Invalid format! The car number should be in '00XX000' format.\n");
        }
    } else {
        printf("Invalid ID or the spot is occupied!\n");
    }
}

// Deleting the data of the car leaving the parking
void leaveParking(ParkingSpace parkingPlace[], int size) {
    int spotId;
    printf("Enter the spot ID to remove the car (1-%d): ", size);
    scanf("%d", &spotId);
    spotId--; 

    if (spotId >= 0 && spotId < size && parkingPlace[spotId].isOccupied) {
        // Exit time
        time(&parkingPlace[spotId].end_time);

        printf("\nYou left the spot %d\n", spotId + 1);
        printf("Entry Time: %s", ctime(&parkingPlace[spotId].start_time));
        printf("Exit Time: %s", ctime(&parkingPlace[spotId].end_time));

        double duration = difftime(parkingPlace[spotId].end_time, parkingPlace[spotId].start_time) / 3600; // Duration in hours

        // if less than 15 minutes, then the parking is FREE, otherwise 300 AMD per every next hour
        if (duration <= 0.25) {
            printf("The parking bill is 0 AMD\n");
        } else {
            double fee = (duration-0.25) * 300; 
            printf("The parking bill is %.2f AMD\n", fee);
        }

        parkingPlace[spotId].isOccupied = false;
        strcpy(parkingPlace[spotId].carName, "");
        strcpy(parkingPlace[spotId].carNumber, "");
        parkingPlace[spotId].start_time = 0;
        parkingPlace[spotId].end_time = 0;
    } else {
        printf("\nInvalid ID or the spot is already empty!\n");
    }
}

// Save all the data in the file
void saveParkingInfo(ParkingSpace parkingPlace[], int size) {
    FILE *parking_data = fopen("C:\\Users\\MSI\\Desktop\\PMS\\Parking_Info.txt", "w");
    fprintf(parking_data, "--Parking Space Information--\n\n");

    for (int i = 0; i < size; i++) {
        if (parkingPlace[i].isOccupied) {
            fprintf(parking_data, "Spot %d: Occupied\n", parkingPlace[i].id);
            fprintf(parking_data, "  Car Name: %s\n", parkingPlace[i].carName);
            fprintf(parking_data, "  Car Number: %s\n", parkingPlace[i].carNumber);
            fprintf(parking_data, "  Entry Time: %s", ctime(&parkingPlace[i].start_time));
            fprintf(parking_data, "  Exit Time: %s\n", parkingPlace[i].end_time == 0 ? "-" : ctime(&parkingPlace[i].end_time));
        } else {
            fprintf(parking_data, "Spot %d: Available\n", parkingPlace[i].id);
        }
    }
    fclose(parking_data);
}

int main() {
    ParkingSpace parkingPlace[PARKING_PLACES];
    initializeParkingSpots(parkingPlace, PARKING_PLACES);
    saveParkingInfo(parkingPlace, PARKING_PLACES);

    int continueOrNot;
    printf("\n--PARKING MANAGEMENT SYSTEM--\n\n");
    printf("0-15 minutes is FREE. Every next 1 hour 300 AMD\n");
    printf("Do you want to continue?: \n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Your answer: ");
    scanf("%d", &continueOrNot);

    if (continueOrNot == 1) {
        int choice;
        do {
            printf("\n-MENU-\n");
            printf("1. Show the parking spots\n");
            printf("2. Park the car\n");
            printf("3. Leave the parking\n");
            printf("0. Exit the system\n");
            printf("Enter the number (1, 2, 3 or 0): ");
            scanf("%d", &choice);
            printf("\n");

            switch (choice) {
                case 1:
                    showParkingInfo(parkingPlace, PARKING_PLACES);
                    saveParkingInfo(parkingPlace, PARKING_PLACES);
                    break;
                case 2:
                    enterParking(parkingPlace, PARKING_PLACES);
                    saveParkingInfo(parkingPlace, PARKING_PLACES);
                    break;
                case 3:
                    leaveParking(parkingPlace, PARKING_PLACES);
                    saveParkingInfo(parkingPlace, PARKING_PLACES);
                    break;
                case 0:
                    printf("Exiting the system.\n");
                    break;
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        } while (choice != 0);
    } else if (continueOrNot == 2) {
        printf("OK, bye!\n");
    } else {
        printf("Invalid answer! Try again.\n");
    }

    return 0;
}