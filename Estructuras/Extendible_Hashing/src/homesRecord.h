#ifndef BD2_PROYECTO1_HOMESRECORD_H
#define BD2_PROYECTO1_HOMESRECORD_H

#include <iostream>
#include <sstream>

struct HomesRecord {
    char property_url[130]{'\0'};
    char property_id[10]{'\0'};
    char address[114]{'\0'};
    char street_name[105]{'\0'};
    char city[26]{'\0'};
    char state[3]{'\0'};
    char postcode[7]{'\0'};
    float price{};
    float land_space{};
    char land_space_unit[5]{'\0'};
    char property_type[13]{'\0'};
    char property_status[8]{'\0'};
    char RunDate[19]{'\0'};
    char agency_name[81]{'\0'};

    std::string to_string() {
        std::stringstream ss;
        ss << property_url << "," << property_id << "," << address << "," << street_name << "," << city << "," << state
           << "," << postcode << "," << price << "," << land_space << "," << land_space_unit << "," << property_type
           << "," << property_status << "," << RunDate << "," << agency_name;
        return ss.str();
    }
};

#endif //BD2_PROYECTO1_HOMESRECORD_H
