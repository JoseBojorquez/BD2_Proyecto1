#include "include/ExtendibleHash.h"

void print_usage() {
    std::cout << "===============================================" << std::endl;
    std::cout << "           ERROR: INCORRECT ARGUMENTS FORMAT   " << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "Usage: ./extHash {RecordType} {function} {parameter(s)}" << std::endl << std::endl;
    std::cout << "Available RecordTypes and Functions:" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "TestRecord:" << std::endl;
    std::cout << "  - add          codigo,nombre,telefono" << std::endl;
    std::cout << "  - remove       key" << std::endl;
    std::cout << "  - search       key" << std::endl;
    std::cout << "  - rangeSearch  startkey,endkey" << std::endl;
    std::cout << "  - print" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "HomesRecord:" << std::endl;
    std::cout << "  - add          id,address,city,state,zip,price,property_type,property_status" << std::endl;
    std::cout << "  - remove       key" << std::endl;
    std::cout << "  - search       key" << std::endl;
    std::cout << "  - rangeSearch  startkey,endkey" << std::endl;
    std::cout << "  - print" << std::endl;
    std::cout << "===============================================" << std::endl;
}

template<typename RecordType>
void handle_add(ExtendibleHash<int, RecordType> &ehash, const std::string &input_parameter) {
    RecordType record;
    record.set_data_from_string(input_parameter);
    ehash.insert(record);
    ehash.write(ehash.load());
}


template<typename RecordType>
void handle_remove(ExtendibleHash<int, RecordType> &ehash, const std::string &input_parameter) {
    ehash.remove(stringToKey(input_parameter));
    ehash.write(ehash.load());
}

template<typename RecordType>
void handle_search(ExtendibleHash<int, RecordType> &ehash, const std::string &input_parameter) {
    auto readdata = ehash.search(stringToKey(input_parameter));
    ehash.write(readdata);
}

template<typename RecordType>
void handle_rangeSearch(ExtendibleHash<int, RecordType> &ehash, const std::string &input_parameter) {
    std::stringstream ss(input_parameter);
    std::string start_key, end_key;
    std::getline(ss, start_key, ',');
    std::getline(ss, end_key, ',');
    auto readdatarange = ehash.rangeSearch(stringToKey(start_key), stringToKey(end_key));
    ehash.write(readdatarange);
}

template<typename RecordType>
void handle_print(ExtendibleHash<int, RecordType> &ehash) {
    auto data = ehash.load();
    std::cout << "===============================================" << std::endl;
    std::cout << "                 LOADED RECORDS                " << std::endl;
    std::cout << "===============================================" << std::endl;
    for (const auto &record : data) {
        const_cast<RecordType&>(record).print_data();
    }
    std::cout << "===============================================" << std::endl;
    std::cout << "Total records: " << data.size() << std::endl;
}

template<typename RecordType>
void execute_function(ExtendibleHash<int, RecordType> &ehash, const std::string &input_function, const std::string &input_parameter) {
    if (input_function == "add") {
        handle_add(ehash, input_parameter);
    } else if (input_function == "remove") {
        handle_remove(ehash, input_parameter);
    } else if (input_function == "search") {
        handle_search(ehash, input_parameter);
    } else if (input_function == "rangeSearch") {
        handle_rangeSearch(ehash, input_parameter);
    } else if (input_function == "print") {
        handle_print(ehash);
    } else {
        std::cout << "UNKNOWN FUNCTION" << std::endl;
    }
}

template<typename RecordType>
void run_ehash(int argc, char *argv[], const filename_t &filename) {
    ExtendibleHash<int, RecordType> ehash(filename, 1024, 32);

    if (argc == 3 && std::string(argv[2]) == "print") {
        handle_print(ehash);
        return;
    }
    if ((argc == 2 && std::string(argv[1]) != "print") || argc != 4) {
        print_usage();
        return;
    }

    std::string input_function = argv[2];
    std::string input_parameter = argv[3];

    execute_function(ehash, input_function, input_parameter);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::string record_type = argv[1];
    filename_t filename = "ehash";

    if (record_type == "TestRecord") {
        filename += "-"+record_type;
        run_ehash<TestRecord>(argc, argv, filename);
    } else if (record_type == "HomesRecord") {
        filename += "-"+record_type;
        run_ehash<HomesRecord>(argc, argv, filename);
    } else {
        std::cout << "ERROR: UNKNOWN RECORD TYPE" << std::endl;
        return 1;
    }

    return 0;
}
