#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstring>
#include <algorithm>

using namespace std;

struct Record
{
    int cod;
    char nombre[12];
    int ciclo;

    long left = -1;
    long right = -1;
    long height = 1;

    void setData()
    {
        cout << "Codigo: ";
        cin >> cod;
        cout << "Nombre: ";
        cin >> nombre;
        cout << "Ciclo: ";
        cin >> ciclo;
    }

    void showData()
    {
        cout << "\nCodigo: " << cod;
        cout << "\nNombre: " << nombre;
        cout << "\nLeft: " << left;
        cout << "\nRight: " << right;
        cout << "\nCiclo : " << ciclo << endl;
    }
};

class AVLFile
{
private:
    string fileName;
    long posRoot;

    bool fileExists()
    {
        ifstream dataFile(this->fileName, ios::binary);
        bool exists = dataFile.good();
        dataFile.close();
        return exists;
    }

    void createFile()
    {
        ofstream dataFile(this->fileName, ios::app | ios::binary);
        dataFile.close();
    }

public:
    AVLFile(string fileName)
    {
        this->posRoot = -1;
        this->fileName = fileName;
        if (!this->fileExists())
            this->createFile();
    }

    ~AVLFile() 
    {
        fstream file(this->fileName, ios::binary | ios::in | ios::out);
        file.write(reinterpret_cast<char *>(&posRoot), sizeof(long));
        file.close();
    }

    Record find(int key)
    {
        return find(this->posRoot, key);
    }

    void insert(Record record)
    {
        insert(posRoot, record);
    }

    void remove(int key) {
        remove(posRoot, key);
    }

    vector<Record> inorder()
    {
        return inorder(this->posRoot);
    }

    vector<Record> range_search(int start, int end) {
        vector<Record> result;
        range_search(posRoot, start, end, result);
        return result;
    }

private:

    void range_search(int pos_node, int start, int end, vector<Record> result)
    {
        if (pos_node == -1) return;
        fstream dataFile(fileName, ios::binary);
        Record record = getRecord(pos_node,dataFile);

        if (record.cod >= start && record.cod <= end) {
            result.push_back(record);
        }
        if (record.cod > start) {
            range_search(record.left, start, end, result);
        }
        if (record.cod < end) {
            range_search(record.right, start, end, result);
        }
    }

    void remove(int pos_node, int key){
        if (pos_node == -1) return; 
        fstream dataFile(fileName, ios::binary);
        Record record = getRecord(pos_node,dataFile);

        if (record.cod == key)
        {
            if (record.left == -1 || record.right == -1) 
            {
                long temp = (record.left == -1) ? record.right : record.left;
                if (temp == -1) {
                    temp = pos_node;
                    pos_node = -1; 
                    } 
                else {
                    record = getRecord(temp,dataFile); 
                    pos_node = temp;
                    }
            }
            else {
            long new_pos = findMin(record.right,dataFile);
            Record &successor = getRecord(new_pos,dataFile);
            //Copia los atributos del sucesor al record
            strcpy(record.nombre, successor.nombre);
            record.cod = successor.cod;
            record.ciclo = successor.ciclo;

             remove(record.right, successor.cod);
        }
        }

        else if (key < record.cod) {remove(record.left, key);} 
        else {remove(record.right, key);}
        if (pos_node == -1) return;
    
        record.height = 1 + max(getHeight(record.left, dataFile), getHeight(record.right, dataFile));

        balance(pos_node); //Aplica balanceo general

    }

    long findMin(long pos_node, fstream &dataFile) {
        while (getRecord(pos_node,dataFile).left != -1) {
            pos_node = getRecord(pos_node,dataFile).left;
        }
        return pos_node;
    }

    Record find(long pos_node, int key)
    {
        ifstream dataFile(this->fileName, ios::binary);
        Record record{};
        if (pos_node == -1) return record;

        dataFile.seekg(pos_node * sizeof(Record), ios::beg);
        dataFile.read(reinterpret_cast<char *>(&record), sizeof(Record));
        dataFile.close();

        if (record.cod == key)
            return record;

        else if (key < record.cod & record.left!=-1){
            return find(record.left, key);
        }
        else if (key > record.cod & record.right!=-1){
            return find(record.right, key);
        }
        return record;
    }

    void insert(long &pos_node, Record record)
    {
        if (pos_node == -1)
        {
            fstream dataFile(fileName, ios::binary | ios::in | ios::out);
            //Si el nodo es nulo, inserta el nuevo nodo aquí.
            dataFile.seekp(0, ios::end);
            pos_node = dataFile.tellp() / sizeof(Record);
            dataFile.write(reinterpret_cast<char *>(&record), sizeof(Record));
            dataFile.close();
            return;
        }
        else{
            fstream dataFile(fileName, ios::binary | ios::in | ios::out);
            dataFile.seekg(pos_node * sizeof(Record));
            Record record;
            dataFile.read(reinterpret_cast<char *>(&record), sizeof(Record));

            if (record.cod < record.cod)
            {
                if (record.left == -1) {
                    dataFile.seekp(0, ios::end);
                    record.left = dataFile.tellp() / sizeof(Record);
                    dataFile.write(reinterpret_cast<char *>(&record), sizeof(Record));
                    dataFile.seekp(pos_node*sizeof(Record));
                    dataFile.write(reinterpret_cast<char *>(&record), sizeof(Record));
                    dataFile.close();
                }
                else {
                    insert(record.left, record);
                }
            }

            else
            {
                if (record.right == -1) {
                    dataFile.seekp(0, ios::end);
                    record.right = dataFile.tellp() / sizeof(Record);
                    dataFile.write(reinterpret_cast<char *>(&record), sizeof(Record));
                    dataFile.seekp(pos_node*sizeof(Record));
                    dataFile.write(reinterpret_cast<char *>(&record), sizeof(Record));
                    dataFile.close();
                }
                else {
                    insert(record.right, record);
                }
            }

            record.height = 1 + max(getHeight(record.left, dataFile), getHeight(record.right, dataFile));

            balance(pos_node); //Aplica balanceo general
        }
    }

    int getHeight(long pos_node, fstream &dataFile)
    {
        if (pos_node == -1)
            return 0;
        Record record = getRecord(pos_node, dataFile);
        return record.height;
    }

    Record &getRecord(long pos_node, fstream &dataFile)
    {
        static Record record;
        dataFile.seekg(pos_node * sizeof(Record), ios::beg);
        dataFile.read(reinterpret_cast<char *>(&record), sizeof(Record));
        return record;
    }

    int getBalanceFactor(Record record, fstream &dataFile)
    {
        if (record.left == -1 && record.right == -1)
            return 0;

        int leftHeight = getHeight(record.left, dataFile);
        int rightHeight = getHeight(record.right, dataFile);

        return rightHeight - leftHeight;
    }

    void rotateRight(long &pos_node, fstream &dataFile)
    {
        long newRoot = getRecord(pos_node, dataFile).left;
        long newRootRight = getRecord(newRoot, dataFile).right;

        getRecord(pos_node, dataFile).left = newRootRight;
        getRecord(newRoot, dataFile).right = pos_node;

        // Actualiza alturas
        getRecord(pos_node, dataFile).height = 1 + max(getHeight(getRecord(pos_node, dataFile).left, dataFile), getHeight(getRecord(pos_node, dataFile).right, dataFile));
        getRecord(newRoot, dataFile).height = 1 + max(getHeight(getRecord(newRoot, dataFile).left, dataFile), getHeight(getRecord(newRoot, dataFile).right, dataFile));

        // Actualiza la posición de la raíz
        pos_node = newRoot;
    }

    void rotateLeft(long &pos_node, fstream &dataFile)
    {
        long newRoot = getRecord(pos_node, dataFile).right;
        long newRootLeft = getRecord(newRoot, dataFile).left;

        getRecord(pos_node, dataFile).right = newRootLeft;
        getRecord(newRoot, dataFile).left = pos_node;

        // Actualiza alturas
        getRecord(pos_node, dataFile).height = 1 + max(getHeight(getRecord(pos_node, dataFile).left, dataFile), getHeight(getRecord(pos_node, dataFile).right, dataFile));
        getRecord(newRoot, dataFile).height = 1 + max(getHeight(getRecord(newRoot, dataFile).left, dataFile), getHeight(getRecord(newRoot, dataFile).right, dataFile));

        // Actualiza la posición de la raíz
        pos_node = newRoot;
    }

    vector<Record> inorder(long pos_node)
    {
        if (pos_node == -1) return vector<Record>();;

        fstream dataFile(fileName, ios::binary);
        Record record = getRecord(pos_node,dataFile);
        vector<Record> result;
        vector<Record> left_tree = inorder(record.left);
        result.insert(result.end(), left_tree.begin(), left_tree.end());
        result.push_back(record);
        vector<Record> right_tree = inorder(record.right);
        result.insert(result.end(), right_tree.begin(), right_tree.end());
        return result;
    }

    int size()
    {
        ifstream dataFile(this->fileName, ios::binary);
        if (!dataFile.is_open())
            throw runtime_error("Error opening file " + this->fileName);
        dataFile.seekg(0, ios::end);
        long total_bytes = dataFile.tellg();
        dataFile.close();
        return total_bytes / sizeof(Record);
    }

    void balance(long pos_node){

        fstream dataFile(fileName, ios::binary);  
        int balance;
        if (pos_node==-1){balance = 0;}   
        else{
            balance = getHeight(getRecord(pos_node,dataFile).left,dataFile) - getHeight(getRecord(pos_node,dataFile).right,dataFile);
        }

        if (balance > 1) {
            long leftChild = getRecord(pos_node,dataFile).left;
            //Left-Right
            if (getHeight(getRecord(leftChild,dataFile).left,dataFile) - getHeight(getRecord(leftChild,dataFile).right,dataFile) < 0) {
                rotateLeft(leftChild,dataFile);
            }
            rotateRight(pos_node,dataFile);
        }
        else if (balance < -1) {
            long rightChild = getRecord(pos_node,dataFile).right;
            //Right-Left
            if (getHeight(getRecord(rightChild,dataFile).left,dataFile) - getHeight(getRecord(rightChild,dataFile).right,dataFile) < 0) {
                rotateRight(rightChild,dataFile);
            }
            rotateLeft(pos_node,dataFile);
        }
    }
};
