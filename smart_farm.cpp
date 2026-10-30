#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <limits>
using namespace std;

// Abstract class for farming advice
class Advisory {
public:
    virtual void giveAdvice() const = 0;
    virtual ~Advisory() = default;
};

// Base class
class Crop {
protected:
    string name, season;
    double area, yieldPerAcre;
    static int cropCount;

public:
    // Default constructor
    Crop() : name("Unknown"), season("Unknown"),
             area(1), yieldPerAcre(1) {
        cropCount++;
    }

    // Parameterized constructor
    Crop(string n, double a, string s, double y)
        : name(n), season(s), area(a), yieldPerAcre(y) {
        if (a <= 0 || y < 0)
            throw invalid_argument(
                "Area must be positive and yield cannot be negative."
            );
        cropCount++;
    }

    virtual ~Crop() {
        cropCount--;
    }

    double totalYield() const {
        return area * yieldPerAcre;
    }

    string getName() const {
        return name;
    }

    static int getCropCount() {
        return cropCount;
    }

    // Operator overloading: compare total yields
    bool operator>(const Crop& other) const {
        return totalYield() > other.totalYield();
    }

    // Runtime polymorphism
    virtual void display() const {
        cout << fixed << setprecision(2);
        cout << "\nCrop: " << name
             << "\nSeason: " << season
             << "\nArea: " << area << " acres"
             << "\nYield per acre: " << yieldPerAcre
             << "\nTotal yield: " << totalYield() << " units\n";
    }

    virtual void save(ofstream& file) const = 0;
};

int Crop::cropCount = 0;

// Derived class 1
class CerealCrop : public Crop, public Advisory {
    string fertilizerType;

public:
    // Overloaded constructors
    CerealCrop() : Crop(), fertilizerType("Organic") {}

    CerealCrop(string n, double a, string s, double y,
               string fertilizer)
        : Crop(n, a, s, y), fertilizerType(fertilizer) {}

    void giveAdvice() const override {
        cout << name
             << ": Use balanced fertilizer and monitor soil nutrients.\n";
    }

    void display() const override {
        Crop::display();
        cout << "Type: Cereal"
             << "\nFertilizer: " << fertilizerType << '\n';
    }

    void save(ofstream& file) const override {
        file << "Cereal," << name << ',' << area << ','
             << season << ',' << yieldPerAcre << ','
             << fertilizerType << '\n';
    }
};

// Derived class 2
class VegetableCrop : public Crop, public Advisory {
    int irrigationFrequency;

public:
    // Overloaded constructors
    VegetableCrop() : Crop(), irrigationFrequency(3) {}

    VegetableCrop(string n, double a, string s, double y,
                  int frequency)
        : Crop(n, a, s, y),
          irrigationFrequency(frequency) {
        if (frequency <= 0)
            throw invalid_argument(
                "Irrigation frequency must be positive."
            );
    }

    void giveAdvice() const override {
        cout << name
             << ": Maintain suitable soil moisture and "
                "check plants regularly for pests.\n";
    }

    void display() const override {
        Crop::display();
        cout << "Type: Vegetable"
             << "\nIrrigation: " << irrigationFrequency
             << " times per week\n";
    }

    void save(ofstream& file) const override {
        file << "Vegetable," << name << ',' << area << ','
             << season << ',' << yieldPerAcre << ','
             << irrigationFrequency << '\n';
    }
};

int main() {
    try {
        // Creating objects using parameterized constructors
        CerealCrop wheat("Wheat", 10, "Winter", 25, "NPK");
        VegetableCrop potato("Potato", 5, "Winter", 30, 4);

        // Runtime polymorphism
        Crop* crops[2] = {&wheat, &potato};

        cout << "===== SMART FARM MANAGEMENT SYSTEM =====\n";

        for (int i = 0; i < 2; i++) {
            crops[i]->display();
        }

        // Operator overloading
        cout << "\n===== YIELD COMPARISON =====\n";
        if (wheat > potato)
            cout << wheat.getName() << " has a higher total yield.\n";
        else if (potato > wheat)
            cout << potato.getName() << " has a higher total yield.\n";
        else
            cout << "Both crops have equal total yields.\n";

        // Abstract class and farming advice
        cout << "\n===== FARMING ADVICE =====\n";
        Advisory* advisors[2] = {&wheat, &potato};

        for (int i = 0; i < 2; i++)
            advisors[i]->giveAdvice();

        // Static member
        cout << "\nNumber of crop objects: "
             << Crop::getCropCount() << '\n';

        // File handling: save crop data
        ofstream outFile("crops.txt");
        if (!outFile)
            throw runtime_error("Unable to open file for writing.");

        for (int i = 0; i < 2; i++)
            crops[i]->save(outFile);

        outFile.close();
        cout << "\nCrop data saved successfully.\n";

        // File handling: retrieve crop data
        ifstream inFile("crops.txt");
        if (!inFile)
            throw runtime_error("Unable to open file for reading.");

        cout << "\n===== SAVED CROP DATA =====\n";
        string line;

        while (getline(inFile, line))
            cout << line << '\n';

        inFile.close();

    } catch (const exception& e) {
        cerr << "\nError: " << e.what() << '\n';
        return 1;
    }

    return 0;
}