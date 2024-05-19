#include "HomesRecord.h"

struct CreditCardRecord {
    int id; // Unique identifier for each transaction
    std::array<float, 28> V; // Anonymized features V1 to V28
    float Amount; // The transaction amount
    int Class; // Binary label indicating whether the transaction is fraudulent (1) or not (0)

    void set_data_from_string(const std::string& _params) {
        std::stringstream ss(_params);
        std::string token;

        // Parse id
        getline(ss, token, ',');
        id = std::stoi(token);

        // Parse V1 to V28
        for (int i = 0; i < 28; ++i) {
            getline(ss, token, ',');
            V[i] = std::stof(token);
        }

        // Parse Amount
        getline(ss, token, ',');
        Amount = std::stof(token);

        // Parse Class
        getline(ss, token, ',');
        Class = std::stoi(token);
    }

    void set_data(int _id, const std::array<float, 28>& _V, float _Amount, int _Class) {
        id = _id;
        V = _V;
        Amount = _Amount;
        Class = _Class;
    }

    void print_data() const {
        std::cout << id << " | ";
        for (int i = 0; i < 28; ++i) {
            std::cout << V[i] << " | ";
        }
        std::cout << Amount << " | " << Class << std::endl;
    }

    void load(std::fstream& file) {
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        file.read(reinterpret_cast<char*>(V.data()), V.size() * sizeof(float));
        file.read(reinterpret_cast<char*>(&Amount), sizeof(Amount));
        file.read(reinterpret_cast<char*>(&Class), sizeof(Class));
    }

    void save(std::fstream& file) const {
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
        file.write(reinterpret_cast<const char*>(V.data()), V.size() * sizeof(float));
        file.write(reinterpret_cast<const char*>(&Amount), sizeof(Amount));
        file.write(reinterpret_cast<const char*>(&Class), sizeof(Class));
    }

    void save(std::ofstream& file) const {
        file.write(reinterpret_cast<const char*>(&id), sizeof(id));
        file.write(reinterpret_cast<const char*>(V.data()), V.size() * sizeof(float));
        file.write(reinterpret_cast<const char*>(&Amount), sizeof(Amount));
        file.write(reinterpret_cast<const char*>(&Class), sizeof(Class));
    }

    int get_key() const {
        return id;
    }

    std::string print_csv() const {
        std::stringstream ss;
        ss << id << ",";
        for (int i = 0; i < 28; ++i) {
            ss << V[i] << ",";
        }
        ss << Amount << "," << Class << "\n";
        return ss.str();
    }
};
