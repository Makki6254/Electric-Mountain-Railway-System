#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

const int NUM_TRAINS = 8;
const int NUM_COACHES = 6;
const int NUM_SEATS = 80;
const int TICKET_PRICE = 25;

struct Train {
    string id;
    int time;
    int available;
    int passengers;
    int revenue;
};

struct Ticket {
    string customerName;
    string phoneNumber;
    int numPassengers;
    int totalPrice;
    int discount;
    string trainUp;
    string trainDown;
};

vector<Train> trains;

void init_trains() {
    trains.push_back({"U1", 900, NUM_COACHES * NUM_SEATS, 0, 0});
    trains.push_back({"U2", 1100, NUM_COACHES * NUM_SEATS, 0, 0});
    trains.push_back({"U3", 1300, NUM_COACHES * NUM_SEATS, 0, 0});
    trains.push_back({"U4", 1500, NUM_COACHES * NUM_SEATS, 0, 0});
    trains.push_back({"D1", 1000, NUM_COACHES * NUM_SEATS, 0, 0});
    trains.push_back({"D2", 1200, NUM_COACHES * NUM_SEATS, 0, 0});
    trains.push_back({"D3", 1400, NUM_COACHES * NUM_SEATS, 0, 0});
    trains.push_back({"D4", 1600, (NUM_COACHES + 2) * NUM_SEATS, 0, 0});
}

void display_screen() {
    cout << "\t\t\t\tWelcome to the Electric Mountain Railway System\n";
    cout << "\t\t\tThe train times and the number of tickets available are shown below\n";
    cout << "---------------------------------------------------------------\n";
    cout << "Train\tTime\tAvailable Tickets\n";
    cout << "---------------------------------------------------------------\n";
    for (int i = 0; i < NUM_TRAINS; i++) {
        cout << trains[i].id << "\t";
        cout << setw(2) << setfill('0') << trains[i].time / 100 << ":";
        cout << setw(2) << setfill('0') << trains[i].time % 100 << "\t";
        if (trains[i].available > 0) {
            cout << trains[i].available << "\n";
        }
        else {
            cout << "Closed\n";
        }
    }
    cout << "---------------------------------------------------------------\n";
}

int input_int(string prompt, int min, int max) {
    int value;
    bool valid;
    do {
        cout << prompt;
        cin >> value;
        if (cin.good() && value >= min && value <= max) {
            valid = true;
        }
        else {
            valid = false;
            cout << "Invalid input. Please enter an integer between " << min << " and " << max << ".\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (!valid);
    cout << "---------------------------------------------------------------\n";
    return value;
}

string input_string(string prompt) {
    string value;
    cout << prompt;
    cin.ignore();  // Clear the newline left in the buffer
    getline(cin, value);
    return value;
}

void purchase_tickets() {
    int up_index, down_index;
    int num_passengers;
    string customerName;
    string phoneNumber;
    int total_price, discount;
    char confirm;
    bool valid;

    customerName = input_string("Enter your name: ");
    phoneNumber = input_string("Enter your phone number: ");

    cout << "Please choose a train for the journey up the mountain.\n";
    cout << "Enter the train number (1-4) or 0 to cancel: ";
    up_index = input_int("", 0, 4) - 1;

    if (up_index == -1) {
        cout << "You have cancelled the purchase.\n";
        cout << "---------------------------------------------------------------\n";
        return;
    }

    cout << "Please choose a train for the journey down the mountain.\n";
    cout << "Enter the train number (1-4) or 0 to cancel: ";
    down_index = input_int("", 0, 4) + 3;

    if (down_index == 3) {
        cout << "You have cancelled the purchase.\n";
        return;
    }

    if (trains[down_index].available == 0) {
        cout << "Sorry, the train " << trains[down_index].id << " is full. Please choose another train.\n";
        return;
    }

    if (trains[down_index].time < trains[up_index].time) {
        cout << "Sorry, you cannot choose a train for the journey down that is earlier than the train for the journey up.\n";
        return;
    }

    cout << "Please enter the number of passengers (1-80) or 0 to cancel: ";
    num_passengers = input_int("", 0, 80);

    if (num_passengers == 0) {
        cout << "You have cancelled the purchase.\n";
        return;
    }

    if (num_passengers > trains[up_index].available || num_passengers > trains[down_index].available) {
        cout << "Sorry, there are not enough seats available for the number of passengers you entered.\n";
        cout << "---------------------------------------------------------------\n";
        return;
    }

    total_price = num_passengers * TICKET_PRICE * 2;
    discount = 0;

    if (num_passengers >= 10 && num_passengers <= 80) {
        discount = (num_passengers / 10) * TICKET_PRICE * 2;
    }

    total_price -= discount;

    cout << "You have chosen the following trains:\n";
    cout << "Train " << trains[up_index].id << " for the journey up at ";
    cout << setw(2) << setfill('0') << trains[up_index].time / 100 << ":";
    cout << setw(2) << setfill('0') << trains[up_index].time % 100 << "\n";
    cout << "Train " << trains[down_index].id << " for the journey down at ";
    cout << setw(2) << setfill('0') << trains[down_index].time / 100 << ":";
    cout << setw(2) << setfill('0') << trains[down_index].time % 100 << "\n";
    cout << "The number of passengers is " << num_passengers << "\n";
    cout << "The total price is $" << total_price << "\n";
    if (discount > 0) {
        cout << "You have received a discount of $" << discount << "\n";
    }
    cout << "---------------------------------------------------------------\n";
    cout << "Do you want to confirm the purchase? (Y/N): ";
    do {
        cin >> confirm;
        if (cin.good() && (confirm == 'Y' || confirm == 'N')) {
            valid = true;
        }
        else {
            valid = false;
            cout << "Invalid input. Please enter Y or N.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (!valid);

    if (confirm == 'Y') {
        trains[up_index].available -= num_passengers;
        trains[up_index].passengers += num_passengers;
        trains[up_index].revenue += num_passengers * TICKET_PRICE;
        trains[down_index].available -= num_passengers;
        trains[down_index].passengers += num_passengers;
        trains[down_index].revenue += num_passengers * TICKET_PRICE;

        // Create a ticket slip
        Ticket ticket = {customerName, phoneNumber, num_passengers, total_price, discount, trains[up_index].id, trains[down_index].id};
        
        // Display the ticket slip
        cout << "\n--------------------- Ticket Slip ---------------------\n";
        cout << "Customer Name: " << ticket.customerName << "\n";
        cout << "Phone Number: " << ticket.phoneNumber << "\n";
        cout << "Train for the journey up: " << ticket.trainUp << " at " << setw(2) << setfill('0') << trains[up_index].time / 100 << ":";
        cout << setw(2) << setfill('0') << trains[up_index].time % 100 << "\n";
        cout << "Train for the journey down: " << ticket.trainDown << " at " << setw(2) << setfill('0') << trains[down_index].time / 100 << ":";
        cout << setw(2) << setfill('0') << trains[down_index].time % 100 << "\n";
        cout << "Number of Passengers: " << ticket.numPassengers << "\n";
        cout << "Total Price: $" << ticket.totalPrice << "\n";
        if (ticket.discount > 0) {
            cout << "Discount Received: $" << ticket.discount << "\n";
        }
        cout << "-------------------------------------------------------\n";

        cout << "You have successfully purchased the tickets. Thank you for choosing the Electric Mountain Railway System.\n";
    }
    else {
        cout << "You have cancelled the purchase.\n";
    }
    cout << "---------------------------------------------------------------\n";
}

void display_report() {
    int total_passengers = 0;
    int total_revenue = 0;
    int max_passengers = 0;
    int max_index = 0;

    cout << "End of the day report\n";
    cout << "---------------------------------------------------------------\n";
    cout << "Train\tPassengers\tRevenue\n";
    cout << "---------------------------------------------------------------\n";

    for (int i = 0; i < NUM_TRAINS; i++) {
        cout << trains[i].id << "\t";
        cout << trains[i].passengers << "\t\t";
        cout << "$" << trains[i].revenue << "\n";

        total_passengers += trains[i].passengers;
        total_revenue += trains[i].revenue;

        if (trains[i].passengers > max_passengers) {
            max_passengers = trains[i].passengers;
            max_index = i;
        }
    }

    cout << "---------------------------------------------------------------\n";
    cout << "Total\t" << total_passengers << "\t\t" << "$" << total_revenue << "\n";
    cout << "---------------------------------------------------------------\n";
    cout << "The train with the most passengers is " << trains[max_index].id << " with " << trains[max_index].passengers << " passengers.\n";
}

int main() {
    init_trains();
    display_screen();

    int choice;
    bool exit;

    do {
        cout << "Please choose an option from the menu:\n";
        cout << "1. Purchase tickets\n";
        cout << "2. Display end of the day report\n";
        cout << "3. Exit\n";
        choice = input_int("Enter your choice (1-3): ", 1, 3);

        switch (choice) {
            case 1:
                cout << "---------------------------------------------------------------\n";
                purchase_tickets();
                cout << "---------------------------------------------------------------\n";
                break;
            case 2:
                display_report();
                break;
            case 3:
                exit = true;
                cout << "---------------------------------------------------------------\n";
                cout << "\nThank you for using the Electric Mountain Railway System.\n";
                cout << "---------------------------------------------------------------\n";
                break;
            default:
                exit = false;
                cout << "---------------------------------------------------------------\n";
                cout << "Invalid choice. Please try again.\n";
                cout << "---------------------------------------------------------------\n";
                break;
        }
    } while (!exit);
    cout << "---------------------------------------------------------------\n";
    return 0;
}
