#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>      // For cross-platform sleep
#include <chrono>      // For cross-platform sleep
using namespace std;

void menu(); // main menu function prototype

class ManageMenu {
protected:
    string userName;

public:
    ManageMenu() {
        cout << "\033[2J\033[1;1H"; // Clear the screen
        cout << "\n\n\n\n\n\n\n\n\n\t  Enter Your Name to Continue as an Admin: ";
        cin >> userName;
        cout << "\033[2J\033[1;1H"; // Clear the screen
        menu(); // Call to main function after executing the constructor
    }

    ~ManageMenu() {}
};

class Customer {
public:
    string name, gender, address;
    int age, mobileNo, menuBack;
    static int cusID;
    char all[999];

    void getDetails() {
        ofstream out("old-customers.txt", ios::app); // Open file in append mode to write customer details
        cout << "\nEnter Customer ID: ";
        cin >> cusID;
        cout << "Enter Name: ";
        cin >> name;
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Mobile Number: ";
        cin >> mobileNo;
        cout << "Address: ";
        cin >> address;
        cout << "Gender: ";
        cin >> gender;

        out << "\nCustomer ID: " << cusID << "\nName: " << name << "\nAge: " << age << "\nMobile Number: " << mobileNo
            << "\nAddress: " << address << "\nGender: " << gender << endl;
        out.close();
        cout << "\nDetails Saved.\nNOTE: We save your record for future purposes.\n" << endl;
    }

    void showDetails() {
        ifstream in("old-customers.txt");
        if (!in) {
            cout << "File Error!" << endl;
        }
        while (!(in.eof())) {
            in.getline(all, 999);
            cout << all << endl;
        }
        in.close();
    }
};

int Customer::cusID;

class Transport {
public:
    int transportChoice;
    int distance;
    float transportCost;
    static float lastTransportCost;

    struct TransportOption {
        string name;
        float costPerKM;
        float speed; // in KM per hour
        int convenienceScore; // Higher is better
    };

    // Transport options with predefined values (cost per KM, speed, convenience)
    TransportOption transportOptions[3] = {
        {"Bike", 5.0, 30.0, 1},
        {"Bus", 10.0, 50.0, 2},
        {"Cab", 25.0, 80.0, 3}
    };

    // Ranking algorithm to choose the best transport based on money, time, and distance
    void chooseTransport() {
        float money, time;

        cout << "Enter the distance to your destination (in KM): ";
        cin >> distance;
        cout << "Enter your available budget (in Rs): ";
        cin >> money;
        cout << "Enter your available time for the journey (in hours): ";
        cin >> time;

        float scores[3] = {0}; // Scores for each transport option
        float weights[3] = {0.5, 0.3, 0.2}; // Weights for cost, time, and convenience (these can be adjusted)

        for (int i = 0; i < 3; i++) {
            // Calculate cost and time for each option
            float cost = transportOptions[i].costPerKM * distance;
            float travelTime = distance / transportOptions[i].speed;

            // Scoring based on the user's constraints
            float costScore = (money >= cost) ? 1 : money / cost; // Higher if cost is within budget
            float timeScore = (time >= travelTime) ? 1 : time / travelTime; // Higher if time is sufficient
            float convenienceScore = transportOptions[i].convenienceScore / 3.0; // Normalize convenience score

            // Calculate total score based on weighted sum of cost, time, and convenience
            scores[i] = (costScore * weights[0]) + (timeScore * weights[1]) + (convenienceScore * weights[2]);
        }

        // Find the transport option with the highest score
        int bestOption = 0;
        for (int i = 1; i < 3; i++) {
            if (scores[i] > scores[bestOption]) {
                bestOption = i;
            }
        }

        transportChoice = bestOption + 1;
        transportCost = transportOptions[bestOption].costPerKM * distance;

        // Display the best transport option
        cout << "\n" << transportOptions[bestOption].name << " chosen. Cost for your journey: " << transportCost << " Rs." << endl;
        lastTransportCost = transportCost;

        cout << "\nPress 1 to go back to the main menu: ";
        int gotoMenu;
        cin >> gotoMenu;
        cout << "\033[2J\033[1;1H"; // Clear the screen
        menu();
    }
};

float Transport::lastTransportCost;

class Hotel {
public:
    int choice;
    int nights;
    float hotelCost;
    static float lastHotelCost;

    struct HotelOption {
        string name;
        float pricePerNight;
        int stars;
    };

    HotelOption hotels[6] = {
        {"Budget Inn", 1500.0, 2},
        {"City Lodge", 2000.0, 3},
        {"Comfort Stay", 2500.0, 3},
        {"Midtown Suites", 4000.0, 4},
        {"Luxury Palace", 7000.0, 5},
        {"Royal Grand Hotel", 10000.0, 5}
    };

    void chooseHotel() {
        cout << "We have the following hotel options for you:\n" << endl;
        for (int i = 0; i < 6; i++) {
            cout << i + 1 << ". " << hotels[i].name
                 << " (" << hotels[i].stars << " stars) - "
                 << hotels[i].pricePerNight << " Rs per night" << endl;
        }

        cout << "\nChoose the hotel you would like to stay at (1-6): ";
        cin >> choice;

        if (choice < 1 || choice > 6) {
            cout << "Invalid choice! Please select a valid hotel option." << endl;
            chooseHotel();
            return;
        }

        cout << "How many nights will you be staying?: ";
        cin >> nights;

        hotelCost = hotels[choice - 1].pricePerNight * nights;
        lastHotelCost = hotelCost;

        cout << "\nYou chose " << hotels[choice - 1].name << " for " << nights << " night(s)." << endl;
        cout << "Total cost: " << hotelCost << " Rs." << endl;

        cout << "\nPress 1 to go back to the main menu: ";
        int gotoMenu;
        cin >> gotoMenu;
        cout << "\033[2J\033[1;1H"; // Clear the screen
        menu();
    }
};

float Hotel::lastHotelCost;

class Chargers : public Hotel, public Transport, public Customer {
public:
    void printBill() {
        ofstream outf("receipt.txt"); // Receipt for bought items
        outf << "--------ABC Travel Agency--------" << endl;
        outf << "-------------Receipt-------------" << endl;
        outf << "_________________________________" << endl;

        outf << "Customer ID: " << Customer::cusID << endl << endl;
        outf << "Description\t\t Total" << endl;
        outf << "Hotel cost:\t\t " << fixed << setprecision(2) << Hotel::lastHotelCost << endl;
        outf << "Transport cost:\t\t " << fixed << setprecision(2) << Transport::lastTransportCost << endl;

        outf << "_________________________________" << endl;
        outf << "Total Charge:\t\t " << fixed << setprecision(2) << Hotel::lastHotelCost + Transport::lastTransportCost << endl;
        outf << "_________________________________" << endl;
        outf << "------------THANK YOU------------" << endl;

        outf.close();
    }

    void showBill() {
        ifstream inf("receipt.txt");
        if (!inf) {
            cout << "File Error!" << endl;
        }
        while (!(inf.eof())) {
            inf.getline(all, 999);
            cout << all << endl;
        }
        inf.close();
    }
};

void menu() {
    int mainChoice;
    int gotoMenu;
    cout << "\t\t      * ABC Travels *\n" << endl;
    cout << "-------------------------Main Menu--------------------------" << endl;
    cout << "\t|\tCustomer Management -> 1\t|" << endl;
    cout << "\t|\tTransport Options    -> 2\t|" << endl;
    cout << "\t|\tHotel Booking        -> 3\t|" << endl;
    cout << "\t|\tCharges & Bill       -> 4\t|" << endl;
    cout << "\t|\tExit                 -> 5\t|" << endl;
    cout << "\nEnter Choice: ";
    cin >> mainChoice;

    cout << "\033[2J\033[1;1H"; // Clear the screen

    Customer a2;
    Transport a3;
    Hotel a4;
    Chargers a5;

    if (mainChoice == 1) {
        cout << "1. Enter New Customer" << endl;
        cout << "2. See Old Customers" << endl;
        int inChoice;
        cin >> inChoice;
        cout << "\033[2J\033[1;1H"; // Clear the screen
        if (inChoice == 1) {
            a2.getDetails();
        } else if (inChoice == 2) {
            a2.showDetails();
        } else {
            cout << "Invalid Input! Redirecting to Main Menu..." << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            cout << "\033[2J\033[1;1H"; // Clear the screen
            menu();
        }
        cout << "\nPress 1 to go back to the main menu: ";
        cin >> gotoMenu;
        cout << "\033[2J\033[1;1H"; // Clear the screen
        menu();
    } else if (mainChoice == 2) {
        a3.chooseTransport();
    } else if (mainChoice == 3) {
        a4.chooseHotel();
    } else if (mainChoice == 4) {
        a5.printBill();
        cout << "Your receipt is already printed. You can get it from the file.\n" << endl;
        cout << "To display your receipt on the screen, press 1: ";
        int displayBill;
        cin >> displayBill;
        if (displayBill == 1) {
            cout << "\033[2J\033[1;1H"; // Clear the screen
            a5.showBill();
            cout << "\nPress 1 to go back to the main menu: ";
            cin >> gotoMenu;
            cout << "\033[2J\033[1;1H"; // Clear the screen
            menu();
        } else {
            cout << "\033[2J\033[1;1H"; // Clear the screen
            menu();
        }
    } else if (mainChoice == 5) {
        cout << "\n\n\t--GOODBYE!--" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        exit(0);
    } else {
        cout << "Invalid Input! Redirecting to Main Menu..." << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "\033[2J\033[1;1H"; // Clear the screen
        menu();
    }
}

int main() {
    ManageMenu startObj; // The main program starts with this constructor
    return 0;
}