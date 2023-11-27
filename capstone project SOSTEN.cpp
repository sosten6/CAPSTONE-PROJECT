#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_ROUTES 9
#define MAX_BOOKINGS 120
#define MAX_SEAT_NAME_LENGTH 50  

// Structure to represent a bus route
struct BusRoute {
    int routeID;
    char origin[50];
    char destination[50];
    char departureTime[10];
    int totalSeats;
    char seatNames[40][MAX_SEAT_NAME_LENGTH];
};

// Structure to represent a user reservation
struct Reservation {
    int routeID;
    int numSeats;
    char userName[50];
    char ticketID[10];
    float totalCost; 
    char paymentSecret[10];
};

// Structure to represent an employee
struct Employee {
    char username[50];
    char password[50];
};

// Arrays to store bus routes, reservations, and employees
struct BusRoute routes[MAX_ROUTES];
struct Reservation bookings[MAX_BOOKINGS];
struct Employee employees[MAX_ROUTES];

int routeCount = 0;
int bookingCount = 0;
int employeeCount = 0;

// Function to add a new bus route
void addRoute(int routeID, char origin[], char destination[], char departureTime[], int totalSeats) {
    routes[routeCount].routeID = routeID;
    strcpy(routes[routeCount].origin, origin);
    strcpy(routes[routeCount].destination, destination);
    strcpy(routes[routeCount].departureTime, departureTime);
    routes[routeCount].totalSeats = totalSeats;
    routeCount++;
}

// Function to display available routes
void displayRoutes() {
    printf("Available Bus Routes:\n");
    for (int i = 0; i < routeCount; i++) {
        printf("Route %d: %s to %s, Departure Time: %s, Total Seats: %d\n",
               routes[i].routeID, routes[i].origin, routes[i].destination,
               routes[i].departureTime, routes[i].totalSeats);
    }
}

// Function to handle employee login
int employeeLogin(char username[], char password[]) {
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].username, username) == 0 && strcmp(employees[i].password, password) == 0) {
            return 1; // Login successful
        }
    }
    return 0; // Login failed
}

// Function to create an employee account
void createEmployeeAccount(char username[], char password[]) {
    strcpy(employees[employeeCount].username, username);
    strcpy(employees[employeeCount].password, password);
    employeeCount++;
}
// Function to handle payment
float calculatePayment(int numSeats) {
    float pricePerSeat = 150.0;
    printf(" \n            PAY TO KCB \n");
    printf("     PAY BILL 522522 : A/C 1318974674:\n\n" );
    return numSeats * pricePerSeat;
}

// Function to validate the M-Pesa payment secret
int validatePaymentSecret(char paymentSecret[]) {
 
    return (strlen(paymentSecret) == 6);
}

// Function to securely input a password
void secureInputPassword(char password[], int maxLength) {
    int charCount = 0;
    while (charCount < maxLength - 1) {
        char c = _getch(); 

        if (c == '\r' || c == '\n') { 
            password[charCount] = '\0';
            break;
        } else if (c == 8 && charCount > 0) { 
            charCount--;
            printf("\b \b"); 
        } else {
            password[charCount++] = c;
            printf("*"); 
        }
    }
}
// Function to display available seats for a route
void displayAvailableSeats(int routeID) {
    for (int i = 0; i < routeCount; i++) {
        if (routes[i].routeID == routeID) {
            printf("Available Seats for Route %d: %s to %s\n", routeID, routes[i].origin, routes[i].destination);

            // Display the seat layout
            for (int j = 0; j < routes[i].totalSeats; j++) {
                if (j % 4 == 0) {
                    printf("\n");
                }
                if (routes[i].seatNames[j][0] == '\0') {
                    printf("%d: O  ", j + 1); 
                } else {
                    printf("%d: X  ", j + 1);
                }
            }
            printf("\n");

            return;
        }
    }
    printf("Route not found.\n");
}

// Function to reserve seats
void reserveSeats(int routeID, int numSeats, char userName[]) {
    for (int i = 0; i < routeCount; i++) {
        if (routes[i].routeID == routeID) {
            printf("Enter the seat reference (e.g., seat13(space)seat7): ");
            char seatReference[50];
            scanf("%s", seatReference);

            // Parse the seat reference to extract the seat number
            int seatNumber;
            if (sscanf(seatReference, "seat %d", &seatNumber) == 1) {
                int seatIndex = seatNumber - 1;

                if (seatIndex >= 0 && seatIndex < routes[i].totalSeats && routes[i].seatNames[seatIndex][0] == '\0') {
                    float totalCost = calculatePayment(numSeats);

                    printf("Total Cost: $%.2f\n", totalCost);

                    char paymentSecret[10];
                    printf("Enter M-Pesa Payment Secret: ");

                int charCount = 0;
                while (charCount < 10) {
                    char c = _getch(); // Read a character without echoing it
                    if (c == '\r' || c == '\n') { // Enter key pressed
                        paymentSecret[charCount] = '\0';
                        break;
                    } else if (c == 8 && charCount > 0) { // Backspace key
                        charCount--;
                        printf("\b \b"); // Move the cursor back and erase a character
                    } else {
                        paymentSecret[charCount++] = c;
                        printf("*"); // Display an asterisk for each character
                    }
                }

               if (validatePaymentSecret(paymentSecret)) {
                        strcpy(routes[i].seatNames[seatIndex], userName);

                        routes[i].totalSeats--;
                        strcpy(bookings[bookingCount].userName, userName);
                        bookings[bookingCount].routeID = routeID;
                        bookings[bookingCount].numSeats = numSeats;
                        sprintf(bookings[bookingCount].ticketID, "T%d", bookingCount + 1);
                        strcpy(bookings[bookingCount].paymentSecret, paymentSecret);
                        bookings[bookingCount].totalCost = totalCost;  

                        bookingCount++;

                        printf("\nReservation successful. Your Ticket ID: %s\n", bookings[bookingCount - 1].ticketID);
                    } else {
                        printf("\nInvalid M-Pesa payment secret. Reservation failed.\n");
                    }
                } else {
                    printf("Seat is not available or does not exist. Reservation failed.\n");
                }
            } else {
                printf("Invalid seat reference format. Use 'seat <seat number>' (e.g., 'seat13'). Reservation failed.\n");
            }
            return;
        }
    }
    printf("Route not found.\n");
}

// Function to cancel a reservation
void cancelReservation(char ticketID[]) {
    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].ticketID, ticketID) == 0) {
            for (int j = 0; j < routeCount; j++) {
                if (routes[j].routeID == bookings[i].routeID) {
                    routes[j].totalSeats += bookings[i].numSeats;
                    break;
                }
            }
            printf("Reservation with Ticket ID %s canceled.\n", ticketID);
            for (int j = i; j < bookingCount - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            bookingCount--;
            return;
        }
    }
    printf("Reservation with Ticket ID %s not found.\n", ticketID);
}

// Function to display booking history
void displayBookingHistory() {
    printf("Booking History:\n");
    for (int i = 0; i < bookingCount; i++) {
        printf("Ticket ID: %s, Route ID: %d, Seats: %d, User: %s\n",
               bookings[i].ticketID, bookings[i].routeID, bookings[i].numSeats, bookings[i].userName);
    }
}
// Function to delete a bus route
void deleteRoute(int routeID) {
    for (int i = 0; i < routeCount; i++) {
        if (routes[i].routeID == routeID) {
            // Remove the route from the list
            for (int j = i; j < routeCount - 1; j++) {
                routes[j] = routes[j + 1];
            }
            routeCount--;

            printf("Route %d has been deleted.\n", routeID);
            return;
        }
    }
    printf("Route not found.\n");
}

// Function to save data to files
void saveDataToFile() {
    // Save bus routes data to a file
    FILE *routesFile = fopen("routes.txt", "w");
    if (routesFile) {
        for (int i = 0; i < routeCount; i++) {
            fprintf(routesFile, "%d %s %s %s %d\n", routes[i].routeID, routes[i].origin, routes[i].destination, routes[i].departureTime, routes[i].totalSeats);
        }
        fclose(routesFile);
    } else {
        printf("Error: Could not save bus routes data to file.\n");
    }

    // Save reservations data to a file
    FILE *bookingsFile = fopen("reservations.txt", "w");
    if (bookingsFile) {
        for (int i = 0; i < bookingCount; i++) {
            fprintf(bookingsFile, "%d %d %s %s\n", bookings[i].routeID, bookings[i].numSeats, bookings[i].userName, bookings[i].ticketID);
        }
        fclose(bookingsFile);
    } else {
        printf("Error: Could not save reservations data to file.\n");
    }
}
// Function to load data from files
void loadDataFromFile() {
    // Load bus routes data from a file
    FILE *routesFile = fopen("routes.txt", "r");
    if (routesFile) {
        while (fscanf(routesFile, "%d %s %s %s %d", &routes[routeCount].routeID, routes[routeCount].origin, routes[routeCount].destination, routes[routeCount].departureTime, &routes[routeCount].totalSeats) == 5) {
            routeCount++;
        }
        fclose(routesFile);
    } else {
        printf("Warning: Could not load bus routes data from file.\n");
    }

    // Load reservations data from a file
    FILE *bookingsFile = fopen("reservations.txt", "r");
    if (bookingsFile) {
        while (fscanf(bookingsFile, "%d %d %s %s", &bookings[bookingCount].routeID, &bookings[bookingCount].numSeats, bookings[bookingCount].userName, bookings[bookingCount].ticketID) == 4) {
            bookingCount++;
        }
        fclose(bookingsFile);
    } else {
        printf("Warning: Could not load reservations data from file.\n");
    }
}
    // Function to add an employee account
void addEmployeeAccount(char username[], char password[]) {
    strcpy(employees[employeeCount].username, username);
    strcpy(employees[employeeCount].password, password);
    employeeCount++;
    printf("\n Employee account added successfully.\n");
}

// Function to delete an employee account
void deleteEmployeeAccount(char username[]) {
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].username, username) == 0) {
            // Remove the employee account from the list
            for (int j = i; j < employeeCount - 1; j++) {
                employees[j] = employees[j + 1];
            }
            employeeCount--;

            printf("Employee account '%s' has been deleted.\n", username);
            return;
        }
    }
    printf("Employee account '%s' not found.\n", username);
}



void deleteAllData() {
    routeCount = 0;
    bookingCount = 0;
    employeeCount = 0;

    // Clear the routes array
    for (int i = 0; i < MAX_ROUTES; i++) {
        routes[i].routeID = 0;
        routes[i].origin[0] = '\0';
        routes[i].destination[0] = '\0';
        routes[i].departureTime[0] = '\0';
        routes[i].totalSeats = 0;
        for (int j = 0; j < 40; j++) {
            routes[i].seatNames[j][0] = '\0';
        }
    }

    // Clear the bookings array
    for (int i = 0; i < MAX_BOOKINGS; i++) {
        bookings[i].routeID = 0;
        bookings[i].numSeats = 0;
        bookings[i].userName[0] = '\0';
        bookings[i].ticketID[0] = '\0';
        bookings[i].totalCost = 0.0;
        bookings[i].paymentSecret[0] = '\0';
    }

    // Clear the employees array
    for (int i = 0; i < MAX_ROUTES; i++) {
        employees[i].username[0] = '\0';
        employees[i].password[0] = '\0';
    }

    printf("All data has been deleted.\n");
}

int main() {
    addRoute(1, "NAIROBI", "NAROK", "10:00 AM", 40);
    addRoute(2, "NAIROBI", "THIKA", "02:00 PM", 40);
    addRoute(3, "NAIROBI", "NAKURU", "04:00 PM", 40);

    createEmployeeAccount("admin", "1234");
    createEmployeeAccount("sosten", "62sos35ten");

    int choice;
    char username[50], password[50];
    int loggedIn = 0;

    printf("Welcome to the Bus Reservation System\n");

   do {
        if (!loggedIn) {
            printf("Please log in to continue:\n");
            printf("Username: ");
            scanf("%s", username);
            printf("Password: ");
            secureInputPassword(password, 50); 

            loggedIn = employeeLogin(username, password);

            if (!loggedIn) {
                printf("\nLogin failed. Please try again.\n");
                continue;
            }
        }
        printf("\nBus Reservation System Menu\n");
printf("1. Display Available Routes\n");
printf("2. Add a Route\n");
printf("3. Delete a Route\n");
printf("4. View Available Seats for a Route\n");
printf("5. Reserve Seats\n");
printf("6. Cancel Reservation\n");
printf("7. Display Booking History\n");
printf("8. Save Data to Files\n");
printf("9. Load Data from Files\n");
printf("10. Logout\n");
printf("11. Exit\n");
printf("12. Delete all data\n");
printf("13. Add employee account\n");
printf("14. Delete an employee account\n");
printf("Enter your choice: ");
scanf("%d", &choice);


        switch (choice) {
            case 1:
                displayRoutes();
                break;
            case 2:
                if (loggedIn) {
                    int routeID, totalSeats;
                    char origin[50], destination[50], departureTime[10];
                    printf("Enter Route ID, Origin, Destination, Departure Time, Total Seats: ");
                    scanf("%d %s %s %s %d", &routeID, origin, destination, departureTime, &totalSeats);
                    addRoute(routeID, origin, destination, departureTime, totalSeats);
                    printf("Route added successfully.\n");
                } else {
                    printf("Permission denied. Please login as an employee.\n");
                }
                break;
             case 3:
               if (loggedIn) {
                  int routeID;
                   printf("Enter Route ID to delete: ");
                   scanf("%d", &routeID);
                  deleteRoute(routeID);
                  } else {
                   printf("Permission denied. Please login as an employee.\n");
               }
               break;
             case 4:
                if (loggedIn) {
                    int routeID;
                    printf("Enter Route ID to view available seats: ");
                    scanf("%d", &routeID);
                    displayAvailableSeats(routeID);
                } else {
                    printf("Permission denied. Please login as an employee.\n");
                }
            case 5:
                if (loggedIn) {
                    int routeID, numSeats;
                    char userName[50];
                    printf("Enter Route ID, Number of Seats, and Your Name: ");
                    scanf("%d %d %s", &routeID, &numSeats, userName);
                    reserveSeats(routeID, numSeats, userName);
                   } else {
                    printf("Permission denied. Please login as an employee.\n");
                }
               
                break;
            case 6:
                if (loggedIn) {
                    char ticketID[10];
                    printf("Enter Ticket ID to cancel: ");
                    scanf("%s", ticketID);
                    cancelReservation(ticketID);
                } else {
                    printf("Permission denied. Please login as an employee.\n");
                }
                break;
            case 7:
                displayBookingHistory();
                break;
             case 8:
        if (loggedIn) {
            saveDataToFile();
            printf("Data saved to files.\n");
        } else {
            printf("Permission denied. Please log in as an employee.\n");
        }
        break;
    case 9:
  
        if (loggedIn) {
            loadDataFromFile();
            printf("Data loaded from files.\n");
        } else {
            printf("Permission denied. Please log in as an employee.\n");
        }
        break;
         case 10:
                printf("Logged out!Thankyou For Using The bus Reservation System.\n");
                loggedIn = 0;
                break;
    case 11:

        printf("Exiting the Bus Reservation System. Goodbye!\n");
        exit(0);
                break;
            case 12:
    if (loggedIn) {
        deleteAllData();
    } else {
        printf("Permission denied. Please log in as an employee.\n");
    }
    break;
      case 13:
        if (loggedIn) {
            char newUsername[50], newPassword[50];
            printf("Enter new employee username: ");
            scanf("%s", newUsername);
            printf("Enter new employee password: ");
            secureInputPassword(newPassword, 50);
            addEmployeeAccount(newUsername, newPassword);
        } else {
            printf("Permission denied. Please log in as an employee.\n");
        }
        break;

    case 14:
        if (loggedIn) {
            char deleteUsername[50];
            printf("Enter the username of the employee account to delete: ");
            scanf("%s", deleteUsername);
            deleteEmployeeAccount(deleteUsername);
        } else {
            printf("Permission denied. Please log in as an employee.\n");
        }
        break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);

    return 0;
}
