#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class Record {
public:
    Record(
        int applicantId, const std::string& lastName, const std::string& firstName, const std::string& middleName, const std::string& dateOfBirth, 
        const std::string& resumeLink, int hrManagerId, int contestId, 
        const std::string& programmingLanguage, int totalTasks, int solvedTasks, bool cheatingDetected
    ) :
        applicantId(applicantId),
        lastName(lastName),
        firstName(firstName),
        middleName(middleName),
        dateOfBirth(dateOfBirth),
        resumeLink(resumeLink),
        hrManagerId(hrManagerId),
        contestId(contestId),
        programmingLanguage(programmingLanguage),
        totalTasks(totalTasks),
        solvedTasks(solvedTasks),
        cheatingDetected(cheatingDetected) {}
    
    Record() :
        applicantId(0),
        lastName(""),
        firstName(""),
        middleName(""),
        dateOfBirth(""),
        resumeLink(""),
        hrManagerId(0),
        contestId(0),
        programmingLanguage(""),
        totalTasks(0),
        solvedTasks(0),
        cheatingDetected(false) {}

    // Геттеры для получения информации о записи
    int getApplicantId() const {
        return applicantId;
    }

    std::string getLastName() const {
        return lastName;
    }
    
    std::string getFirstName() const {
        return firstName;
    }

    std::string getMiddleName() const {
        return middleName;
    }

    std::string getDateOfBirth() const {
        return dateOfBirth;
    }

    std::string getResumeLink() const {
        return resumeLink;
    }

    int getHrManagerId() const {
        return hrManagerId;
    }

    int getContestId() const {
        return contestId;
    }

    std::string getProgrammingLanguage() const {
        return programmingLanguage;
    }

    int getTotalTasks() const {
        return totalTasks;
    }

    int getSolvedTasks() const {
        return solvedTasks;
    }

    bool isCheatingDetected() const {
        return cheatingDetected;
    }

private:
    int applicantId;
    std::string lastName;
    std::string firstName;
    std::string middleName;
    std::string dateOfBirth;
    std::string resumeLink;
    int hrManagerId;
    int contestId;
    std::string programmingLanguage;
    int totalTasks;
    int solvedTasks;
    bool cheatingDetected;
};

template <typename k, typename v>
class AbstractCollection {
public:
    virtual void addRecord(const k& key, const v& value) = 0; // Добавление записи в коллекцию (ключ, значение
    virtual v * getRecord(const k& key) = 0; // Получение записи по ключу
    virtual std::vector<v> getRecordsInRange(const k& minKey, const k& maxKey) = 0; // Получение записей в диапазоне ключей
    virtual void updateRecord(const k& key, const v& value) = 0; // Обновление записи по ключу (значение)
    virtual void deleteRecord(const k& key) = 0; // Удаление записи по ключу
};

template <typename k, typename v>
struct SplayNode {
    k key;
    v value;
    SplayNode<k, v>* parent;
    SplayNode<k, v>* left;
    SplayNode<k, v>* right;

    SplayNode() : parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename k, typename v>
class SplayTree : public AbstractCollection<k, v> {
private:
    SplayNode<k, v>* root;

    SplayNode<k, v>* rotateLeft(SplayNode<k, v>* node) {
        SplayNode<k, v>* right = node->right;
        node->right = right->left;
        if (right->left != nullptr) {
            right->left->parent = node;
        }
        right->parent = node->parent;
        if (node->parent == nullptr) {
            root = right;
        } else if (node == node->parent->left) {
            node->parent->left = right;
        } else {
            node->parent->right = right;
        }
        right->left = node;
        node->parent = right;
        return right;
    }

    SplayNode<k, v>* rotateRight(SplayNode<k, v>* node) {
        SplayNode<k, v>* left = node->left;
        node->left = left->right;
        if (left->right != nullptr) {
            left->right->parent = node;
        }
        left->parent = node->parent;
        if (node->parent == nullptr) {
            root = left;
        } else if (node == node->parent->right) {
            node->parent->right = left;
        } else {
            node->parent->left = left;
        }
        left->right = node;
        node->parent = left;
        return left;
    }

    void splay(SplayNode<k, v>* curr){
        if (curr == nullptr) {
            return;
        }
        while (curr->parent != nullptr) {
            if (curr->parent->parent == nullptr) {
                if (curr->parent->left == curr) {
                    rotateRight(curr->parent);
                } else {
                    rotateLeft(curr->parent);
                }
            } else if (curr->parent->left == curr && curr->parent->parent->left == curr->parent) {
                rotateRight(curr->parent->parent);
                rotateRight(curr->parent);
            } else if (curr->parent->right == curr && curr->parent->parent->right == curr->parent) {
                rotateLeft(curr->parent->parent);
                rotateLeft(curr->parent);
            } else if (curr->parent->left == curr && curr->parent->parent->right == curr->parent) {
                rotateRight(curr->parent);
                rotateLeft(curr->parent);
            } else {
                rotateLeft(curr->parent);
                rotateRight(curr->parent);
            }
        }
    }

    v * find(SplayNode<k, v>* curr, const k& key) {
        if (curr == nullptr) {
            return nullptr;
        }
        if (key < curr->key) {
            return find(curr->left, key);
        } else if (key > curr->key) {
            return find(curr->right, key);
        } else {
            splay(curr);
            return &(curr->value);
        }
    }

    void insert(SplayNode<k, v>* curr, const k& key, const v& value) {
        if (key < curr->key) {
            if (curr->left == nullptr) {
                curr->left = new SplayNode<k, v>();
                curr->left->key = key;
                curr->left->value = value;
                curr->left->parent = curr;
                splay(curr->left);
            } else {
                insert(curr->left, key, value);
            }
        } else if (key > curr->key) {
            if (curr->right == nullptr) {
                curr->right = new SplayNode<k, v>();
                curr->right->key = key;
                curr->right->value = value;
                curr->right->parent = curr;
                splay(curr->right);
            } else {
                insert(curr->right, key, value);
            }
        } else {
            splay(curr);
        }
    }

    void deleteNode(SplayNode<k, v>*& curr, const k& key) {
        if (curr == nullptr) {
            return;
        }
        if ((key < curr->key && curr->left == nullptr) || (key > curr->key && curr->right == nullptr)) {
            splay(curr);
            return;
        }
        else if (key < curr->key){
            deleteNode(curr->left, key);
            return;
        }
        else if (key > curr->key){
            deleteNode(curr->right, key);
            return;
        }
        else {
            if (curr->left == nullptr && curr->right == nullptr) {
                SplayNode<k, v>* parent = curr->parent;
                delete curr;
                curr = nullptr;
                splay(parent);
                return;
            }
            else if (curr->left == nullptr) {
                SplayNode<k, v>* temp = curr;
                curr = curr->right;
                curr->parent = temp->parent;
                splay(curr->parent);
                delete temp;
                return;
            }
            else if (curr->right == nullptr) {
                SplayNode<k, v>* temp = curr;
                curr = curr->left;
                curr->parent = temp->parent;
                splay(curr->parent);
                delete temp;
                return;
            }
            else {
                SplayNode<k, v>* temp = curr->right;
                while (temp->left != nullptr) {
                    temp = temp->left;
                }
                curr->key = temp->key;
                curr->value = temp->value;
                deleteNode(curr->right, temp->key);
                return;
            }
        }
    }

    void getRecordsInRange(SplayNode<k, v>* node, const k& minKey, const k& maxKey, std::vector<v>& result) {
        if (node == nullptr) {
            return;
        }
        if (node->key > minKey) {
            getRecordsInRange(node->left, minKey, maxKey, result);
        }
        if (node->key >= minKey && node->key <= maxKey) {
            result.push_back(node->value);
        }
        if (node->key < maxKey) {
            getRecordsInRange(node->right, minKey, maxKey, result);
        }
    }

    void inOrderPrintKeys(SplayNode<k, v>* node) {
        if (node == nullptr) {
            return;
        }
        inOrderPrintKeys(node->left);
        std::cout << node->key << " ";
        inOrderPrintKeys(node->right);
    }

    void clearTree(SplayNode<k, v>* node) {
        if (node == nullptr) {
            return;
        }
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

public:

    SplayTree() : root(nullptr) {}

    void addRecord(const k& key, const v& value) override {
        if (root == nullptr) {
            root = new SplayNode<k, v>();
            root->key = key;
            root->value = value;
            return;
        }
        insert(root, key, value);
    }

    v * getRecord(const k& key) override {
        return find(root, key);
    }

    std::vector<v> getRecordsInRange(const k& minKey, const k& maxKey) override {
        std::vector<v> result;
        getRecordsInRange(root, minKey, maxKey, result);
        return result;
    }

    void updateRecord(const k& key, const v& value) override {
        v* record = find(root, key);
        if (record != nullptr) {
            *record = value;
        }
    }

    void deleteRecord(const k& key) override {
        deleteNode(root, key);
    }

    void printKeys() {
        inOrderPrintKeys(root);
        std::cout << std::endl;
    }

    SplayNode<k, v>* getLeft() {
        SplayNode<k, v>* node = root;
        while(node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    SplayNode<k, v>* getNext(const k& key) {
        SplayNode<k, v>* curr = root;
        SplayNode<k, v>* next = nullptr;

        while (curr != nullptr) {
            if (key < curr->key) {
                next = curr;
                curr = curr->left;
            } else if (key > curr->key) {
                curr = curr->right;
            } else {
                // Найден ключ, переходим к правому поддереву, если оно есть
                if (curr->right != nullptr) {
                    next = curr->right;
                    while (next->left != nullptr) {
                        next = next->left;
                    }
                }
                break;
            }
        }

        return next;
    }

    void clear() {
        clearTree(root);
        root = nullptr;
    }

};


class DataManager {
private:
    SplayTree<std::string, SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>>> pools;

public:
    void addDataPool(const std::string& poolName) {
        pools.addRecord(poolName, SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>>());
    }

    void removeDataPool(const std::string& poolName) {
        pools.deleteRecord(poolName);
    }

    void addDataScheme(const std::string& poolName, const std::string& schemeName) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return;
        } else {
            pool->addRecord(schemeName, SplayTree<std::string, SplayTree<int, Record>>());
        }
    }

    void removeDataScheme(const std::string& poolName, const std::string& schemeName) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return;
        } else {
            pools.getRecord(poolName)->deleteRecord(schemeName);
        }
    }

    void addDataCollection(const std::string& poolName, const std::string& schemeName, const std::string& collectionName) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return;
        } else {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme == nullptr) {
                std::cout << "Scheme with key " << schemeName << " not found" << std::endl;
                return;
            } else {
                scheme->addRecord(collectionName, SplayTree<int, Record>());
            }
        }
    }

    void removeDataCollection(const std::string& poolName, const std::string& schemeName, const std::string& collectionName) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return;
        } else {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme == nullptr) {
                std::cout << "Scheme with key " << schemeName << " not found" << std::endl;
                return;
            } else {
                scheme->deleteRecord(collectionName);
            }
        }
    }

    void addRecord(const std::string& poolName, const std::string& schemeName, const std::string& collectionName, const Record& record) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return;
        } else {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme == nullptr) {
                std::cout << "Scheme with key " << schemeName << " not found" << std::endl;
                return;
            } else {
                SplayTree<int, Record> * collection = scheme->getRecord(collectionName);
                if (collection == nullptr) {
                    std::cout << "Collection with key " << collectionName << " not found" << std::endl;
                    return;
                } else {
                    collection->addRecord(record.getApplicantId(), record);
                }
            }
        }
    }

    void removeRecord(const std::string& poolName, const std::string& schemeName, const std::string& collectionName, int applicantId) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return;
        } else {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme == nullptr) {
                std::cout << "Scheme with key " << schemeName << " not found" << std::endl;
                return;
            } else {
                SplayTree<int, Record> * collection = scheme->getRecord(collectionName);
                if (collection == nullptr) {
                    std::cout << "Collection with key " << collectionName << " not found" << std::endl;
                    return;
                } else {
                    collection->deleteRecord(applicantId);
                }
            }
        }
    }

    Record* getRecord(const std::string& poolName, const std::string& schemeName, const std::string& collectionName, int applicantId) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return nullptr;
        } else {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme == nullptr) {
                std::cout << "Scheme with key " << schemeName << " not found" << std::endl;
                return nullptr;
            } else {
                SplayTree<int, Record> * collection = scheme->getRecord(collectionName);
                if (collection == nullptr) {
                    std::cout << "Collection with key " << collectionName << " not found" << std::endl;
                    return nullptr;
                } else {
                    return collection->getRecord(applicantId);
                }
            }
        }
    }

    std::vector<Record> getRecordsInRange(const std::string& poolName, const std::string& schemeName, const std::string& collectionName, int minApplicantId, int maxApplicantId) {
        std::vector<Record> result;
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return result;
        } else {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme == nullptr) {
                std::cout << "Scheme with key " << schemeName << " not found" << std::endl;
                return result;
            } else {
                SplayTree<int, Record> * collection = scheme->getRecord(collectionName);
                if (collection == nullptr) {
                    std::cout << "Collection with key " << collectionName << " not found" << std::endl;
                    return result;
                } else {
                    return collection->getRecordsInRange(minApplicantId, maxApplicantId);
                }
            }
        }
    }

    void updateRecord(const std::string& poolName, const std::string& schemeName, const std::string& collectionName, const Record& record) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool == nullptr) {
            std::cout << "Pool with key " << poolName << " not found" << std::endl;
            return;
        } else {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme == nullptr) {
                std::cout << "Scheme with key " << schemeName << " not found" << std::endl;
                return;
            } else {
                SplayTree<int, Record> * collection = scheme->getRecord(collectionName);
                if (collection == nullptr) {
                    std::cout << "Collection with key " << collectionName << " not found" << std::endl;
                    return;
                } else {
                    collection->updateRecord(record.getApplicantId(), record);
                }
            }
        }
    }

    void printPools() {
        pools.printKeys();
    }

    void printSchemes(const std::string& poolName) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool != nullptr) {
            pool->printKeys();
        }
    }

    void printCollections(const std::string& poolName, const std::string& schemeName) {
        SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>> * pool = pools.getRecord(poolName);
        if (pool != nullptr) {
            SplayTree<std::string, SplayTree<int, Record>> * scheme = pool->getRecord(schemeName);
            if (scheme != nullptr) {
                scheme->printKeys();
            }
        }
    }


    void clear() {
        pools.clear();
    }

    void saveState(const std::string& filename) {
        std::ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }

        SplayNode<std::string, SplayTree<std::string, SplayTree<std::string, SplayTree<int, Record>>>>* poolNode = pools.getLeft();
        while (poolNode != nullptr) {
            outputFile << "ADD_POOL " << poolNode->key << std::endl;
            SplayNode<std::string, SplayTree<std::string, SplayTree<int, Record>>>* schemeNode = poolNode->value.getLeft();
            while (schemeNode != nullptr) {
                outputFile << "ADD_SCHEME " << poolNode->key << " " << schemeNode->key << std::endl;
                SplayNode<std::string, SplayTree<int, Record>>* collectionNode = schemeNode->value.getLeft();
                while (collectionNode != nullptr) {
                    outputFile << "ADD_COLLECTION " << poolNode->key << " " << schemeNode->key << " " << collectionNode->key << std::endl;
                    SplayNode<int, Record>* recordNode = collectionNode->value.getLeft();
                    while (recordNode != nullptr) {
                        Record record = recordNode->value;
                        outputFile << "ADD_RECORD " << poolNode->key << " " << schemeNode->key << " " << collectionNode->key << " "
                            << record.getApplicantId() << " " << record.getLastName() << " " << record.getFirstName() << " " << record.getMiddleName() << " "
                            << record.getDateOfBirth() << " " << record.getResumeLink() << " " << record.getHrManagerId() << " " << record.getContestId() << " "
                            << record.getProgrammingLanguage() << " " << record.getTotalTasks() << " " << record.getSolvedTasks() << " " << record.isCheatingDetected() << std::endl;
                        recordNode = collectionNode->value.getNext(recordNode->key);
                    }
                    collectionNode = schemeNode->value.getNext(collectionNode->key);
                }
                schemeNode = poolNode->value.getNext(schemeNode->key);
            }
            poolNode = pools.getNext(poolNode->key);
        }

        outputFile.close();
    }

    void loadState(const std::string& filename) {
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file for reading: " << filename << std::endl;
            return;
        }

        clear();

        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string command;
            iss >> command;

            if (command == "ADD_POOL") {
                std::string poolName;
                iss >> poolName;
                addDataPool(poolName);
            } else if (command == "ADD_SCHEME") {
                std::string poolName, schemeName;
                iss >> poolName >> schemeName;
                addDataScheme(poolName, schemeName);
            } else if (command == "ADD_COLLECTION") {
                std::string poolName, schemeName, collectionName;
                iss >> poolName >> schemeName >> collectionName;
                addDataCollection(poolName, schemeName, collectionName);
            } else if (command == "ADD_RECORD") {
                std::string poolName, schemeName, collectionName, lastName, firstName, middleName, dateOfBirth, resumeLink, programmingLanguage;
                int applicantId, hrManagerId, contestId, totalTasks, solvedTasks;
                bool cheatingDetected;
                iss >> poolName >> schemeName >> collectionName >> applicantId >> lastName >> firstName >> middleName >> dateOfBirth
                    >> resumeLink >> hrManagerId >> contestId >> programmingLanguage >> totalTasks >> solvedTasks >> cheatingDetected;
                Record record(applicantId, lastName, firstName, middleName, dateOfBirth, resumeLink, hrManagerId, contestId,
                            programmingLanguage, totalTasks, solvedTasks, cheatingDetected);
                addRecord(poolName, schemeName, collectionName, record);
            }
        }

        inputFile.close();
    }


};

void processCommands(const std::string& filename, DataManager& dataManager) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "ADD_POOL") {
            std::string poolName;
            iss >> poolName;
            dataManager.addDataPool(poolName);
        }
        else if (command == "REMOVE_POOL") {
            std::string poolName;
            iss >> poolName;
            dataManager.removeDataPool(poolName);
            std::cout << "Pool with key " << poolName << " deleted" << std::endl;
        }
        else if (command == "ADD_SCHEME") {
            std::string poolName, schemeName;
            iss >> poolName >> schemeName;
            dataManager.addDataScheme(poolName, schemeName);
        }
        else if (command == "REMOVE_SCHEME") {
            std::string poolName, schemeName;
            iss >> poolName >> schemeName;
            dataManager.removeDataScheme(poolName, schemeName);
            std::cout << "Scheme with key " << schemeName << " deleted" << std::endl;
        }
        else if (command == "ADD_COLLECTION") {
            std::string poolName, schemeName, collectionName;
            iss >> poolName >> schemeName >> collectionName;
            dataManager.addDataCollection(poolName, schemeName, collectionName);
        }
        else if (command == "REMOVE_COLLECTION") {
            std::string poolName, schemeName, collectionName;
            iss >> poolName >> schemeName >> collectionName;
            dataManager.removeDataCollection(poolName, schemeName, collectionName);
            std::cout << "Collection with key " << collectionName << " deleted" << std::endl;
        }
        else if (command == "ADD_RECORD") {
            std::string poolName, schemeName, collectionName;
            iss >> poolName >> schemeName >> collectionName;
            int applicantId;
            std::string lastName, firstName, middleName, dateOfBirth, resumeLink, programmingLanguage;
            int hrManagerId, contestId, totalTasks, solvedTasks;
            bool cheatingDetected;
            iss >> applicantId >> lastName >> firstName >> middleName >> dateOfBirth >> resumeLink >> hrManagerId >> contestId >> programmingLanguage >> totalTasks >> solvedTasks >> cheatingDetected;
            Record record(applicantId, lastName, firstName, middleName, dateOfBirth, resumeLink, hrManagerId, contestId, programmingLanguage, totalTasks, solvedTasks, cheatingDetected);
            dataManager.addRecord(poolName, schemeName, collectionName, record);
        }
        else if (command == "REMOVE_RECORD") {
            std::string poolName, schemeName, collectionName;
            iss >> poolName >> schemeName >> collectionName;
            int applicantId;
            iss >> applicantId;
            dataManager.removeRecord(poolName, schemeName, collectionName, applicantId);
            std::cout << "Record with key " << applicantId << " deleted" << std::endl;
        }
        else if (command == "GET_RECORD") {
            std::string poolName, schemeName, collectionName;
            iss >> poolName >> schemeName >> collectionName;
            int applicantId;
            iss >> applicantId;
            Record* record = dataManager.getRecord(poolName, schemeName, collectionName, applicantId);
            if (record != nullptr) {
                std::cout << "\nApplicant ID: " << record->getApplicantId() << std::endl;
                std::cout << "Last name: " << record->getLastName() << std::endl;
                std::cout << "First name: " << record->getFirstName() << std::endl;
                std::cout << "Middle name: " << record->getMiddleName() << std::endl;
                std::cout << "Date of birth: " << record->getDateOfBirth() << std::endl;
                std::cout << "Resume link: " << record->getResumeLink() << std::endl;
                std::cout << "HR manager ID: " << record->getHrManagerId() << std::endl;
                std::cout << "Contest ID: " << record->getContestId() << std::endl;
                std::cout << "Programming language: " << record->getProgrammingLanguage() << std::endl;
                std::cout << "Total tasks: " << record->getTotalTasks() << std::endl;
                std::cout << "Solved tasks: " << record->getSolvedTasks() << std::endl;
                std::cout << "Cheating detected: " << (record->isCheatingDetected() ? "true" : "false") << std::endl << std::endl;
            } else {
                std::cout << "Record not found" << std::endl;
            }
        }
        else if (command == "GET_RECORDS_RANGE") {
            std::string poolName, schemeName, collectionName;
            iss >> poolName >> schemeName >> collectionName;
            int minApplicantId, maxApplicantId;
            iss >> minApplicantId >> maxApplicantId;
            std::vector<Record> records = dataManager.getRecordsInRange(poolName, schemeName, collectionName, minApplicantId, maxApplicantId);
            std::cout << "\nRecords in range: " << std::endl;
            for (Record record : records) {
                std::cout << "Applicant ID: " << record.getApplicantId() << std::endl;
                std::cout << "Last name: " << record.getLastName() << std::endl;
                std::cout << "First name: " << record.getFirstName() << std::endl;
                std::cout << "Middle name: " << record.getMiddleName() << std::endl;
                std::cout << "Date of birth: " << record.getDateOfBirth() << std::endl;
                std::cout << "Resume link: " << record.getResumeLink() << std::endl;
                std::cout << "HR manager ID: " << record.getHrManagerId() << std::endl;
                std::cout << "Contest ID: " << record.getContestId() << std::endl;
                std::cout << "Programming language: " << record.getProgrammingLanguage() << std::endl;
                std::cout << "Total tasks: " << record.getTotalTasks() << std::endl;
                std::cout << "Solved tasks: " << record.getSolvedTasks() << std::endl;
                std::cout << "Cheating detected: " << (record.isCheatingDetected() ? "true" : "false") << std::endl << std::endl;
            }
        }
        else if (command == "UPDATE_RECORD") {
            std::string poolName, schemeName, collectionName;
            iss >> poolName >> schemeName >> collectionName;
            int applicantId;
            std::string lastName, firstName, middleName, dateOfBirth, resumeLink, programmingLanguage;
            int hrManagerId, contestId, totalTasks, solvedTasks;
            bool cheatingDetected;
            iss >> applicantId >> lastName >> firstName >> middleName >> dateOfBirth >> resumeLink >> hrManagerId >> contestId >> programmingLanguage >> totalTasks >> solvedTasks >> cheatingDetected;
            Record record(applicantId, lastName, firstName, middleName, dateOfBirth, resumeLink, hrManagerId, contestId, programmingLanguage, totalTasks, solvedTasks, cheatingDetected);
            dataManager.updateRecord(poolName, schemeName, collectionName, record);
        }
        else if (command == "PRINT_POOLS") {
            dataManager.printPools();
        }
        else if (command == "PRINT_SCHEMES") {
            std::string poolName;
            iss >> poolName;
            dataManager.printSchemes(poolName);
        }
        else if (command == "PRINT_COLLECTIONS") {
            std::string poolName, schemeName;
            iss >> poolName >> schemeName;
            dataManager.printCollections(poolName, schemeName);
        }
        else if (command == "CLEAR") {
            dataManager.clear();
        }
        else if (command == "SAVE_STATE") {
            std::string filename;
            iss >> filename;
            dataManager.saveState(filename);
        }
        else if (command == "LOAD_STATE") {
            std::string filename;
            iss >> filename;
            dataManager.loadState(filename);
        }
        else if (command == ""){
            continue;
        }
        else {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
    inputFile.close();
}

void processDialog(DataManager& dataManager) {
    std::cout << "Welcome to the Data Management System!" << std::endl;

    while (true) {
        std::string command;
        std::cout << "\nEnter a command (or 'exit' to quit): ";
        std::cin >> command;

        if (command == "exit") {
            std::cout << "Exiting the program." << std::endl;
            break;
        }

        if (command == "file") {
            std::string filename;
            std::cout << "Enter the filename: ";
            std::cin >> filename;
            processCommands(filename, dataManager);
        }
        else if (command == "ADD_POOL") {
            std::string poolName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            dataManager.addDataPool(poolName);
        }
        else if (command == "REMOVE_POOL") {
            std::string poolName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            dataManager.removeDataPool(poolName);
        }
        else if (command == "ADD_SCHEME") {
            std::string poolName, schemeName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            dataManager.addDataScheme(poolName, schemeName);
        }
        else if (command == "REMOVE_SCHEME") {
            std::string poolName, schemeName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            dataManager.removeDataScheme(poolName, schemeName);
        }
        else if (command == "ADD_COLLECTION") {
            std::string poolName, schemeName, collectionName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            std::cout << "Enter the collection name: ";
            std::cin >> collectionName;
            dataManager.addDataCollection(poolName, schemeName, collectionName);
        }
        else if (command == "REMOVE_COLLECTION") {
            std::string poolName, schemeName, collectionName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            std::cout << "Enter the collection name: ";
            std::cin >> collectionName;
            dataManager.removeDataCollection(poolName, schemeName, collectionName);
        }
        else if (command == "ADD_RECORD") {
            std::string poolName, schemeName, collectionName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            std::cout << "Enter the collection name: ";
            std::cin >> collectionName;
            int applicantId;
            std::string lastName, firstName, middleName, dateOfBirth, resumeLink, programmingLanguage;
            int hrManagerId, contestId, totalTasks, solvedTasks;
            bool cheatingDetected;
            std::cout << "Enter the applicant ID: ";
            std::cin >> applicantId;
            std::cout << "Enter the last name: ";
            std::cin >> lastName;
            std::cout << "Enter the first name: ";
            std::cin >> firstName;
            std::cout << "Enter the middle name: ";
            std::cin >> middleName;
            std::cout << "Enter the date of birth: ";
            std::cin >> dateOfBirth;
            std::cout << "Enter the resume link: ";
            std::cin >> resumeLink;
            std::cout << "Enter the HR manager ID: ";
            std::cin >> hrManagerId;
            std::cout << "Enter the contest ID: ";
            std::cin >> contestId;
            std::cout << "Enter the programming language: ";
            std::cin >> programmingLanguage;
            std::cout << "Enter the total tasks: ";
            std::cin >> totalTasks;
            std::cout << "Enter the solved tasks: ";
            std::cin >> solvedTasks;
            std::cout << "Enter the cheating detected (true/false): ";
            std::cin >> cheatingDetected;
            Record record(applicantId, lastName, firstName, middleName, dateOfBirth, resumeLink, hrManagerId, contestId, programmingLanguage, totalTasks, solvedTasks, cheatingDetected);
            dataManager.addRecord(poolName, schemeName, collectionName, record);
        }
        else if (command == "REMOVE_RECORD") {
            std::string poolName, schemeName, collectionName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            std::cout << "Enter the collection name: ";
            std::cin >> collectionName;
            int applicantId;
            std::cout << "Enter the applicant ID: ";
            std::cin >> applicantId;
            dataManager.removeRecord(poolName, schemeName, collectionName, applicantId);
        }
        else if (command == "GET_RECORD") {
            std::string poolName, schemeName, collectionName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            std::cout << "Enter the collection name: ";
            std::cin >> collectionName;
            int applicantId;
            std::cout << "Enter the applicant ID: ";
            std::cin >> applicantId;
            Record* record = dataManager.getRecord(poolName, schemeName, collectionName, applicantId);
            if (record != nullptr) {
                std::cout << "\nApplicant ID: " << record->getApplicantId() << std::endl;
                std::cout << "Last name: " << record->getLastName() << std::endl;
                std::cout << "First name: " << record->getFirstName() << std::endl;
                std::cout << "Middle name: " << record->getMiddleName() << std::endl;
                std::cout << "Date of birth: " << record->getDateOfBirth() << std::endl;
                std::cout << "Resume link: " << record->getResumeLink() << std::endl;
                std::cout << "HR manager ID: " << record->getHrManagerId() << std::endl;
                std::cout << "Contest ID: " << record->getContestId() << std::endl;
                std::cout << "Programming language: " << record->getProgrammingLanguage() << std::endl;
                std::cout << "Total tasks: " << record->getTotalTasks() << std::endl;
                std::cout << "Solved tasks: " << record->getSolvedTasks() << std::endl;
                std::cout << "Cheating detected: " << (record->isCheatingDetected() ? "true" : "false") << std::endl << std::endl;
            } else {
                std::cout << "Record not found" << std::endl;
            }
        }
        else if (command == "GET_RECORDS_RANGE") {
            std::string poolName, schemeName, collectionName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            std::cout << "Enter the collection name: ";
            std::cin >> collectionName;
            int minApplicantId, maxApplicantId;
            std::cout << "Enter the min applicant ID: ";
            std::cin >> minApplicantId;
            std::cout << "Enter the max applicant ID: ";
            std::cin >> maxApplicantId;
            std::vector<Record> records = dataManager.getRecordsInRange(poolName, schemeName, collectionName, minApplicantId, maxApplicantId);
            std::cout << "\nRecords in range: " << std::endl;
            for (Record record : records) {
                std::cout << "Applicant ID: " << record.getApplicantId() << std::endl;
                std::cout << "Last name: " << record.getLastName() << std::endl;
                std::cout << "First name: " << record.getFirstName() << std::endl;
                std::cout << "Middle name: " << record.getMiddleName() << std::endl;
                std::cout << "Date of birth: " << record.getDateOfBirth() << std::endl;
                std::cout << "Resume link: " << record.getResumeLink() << std::endl;
                std::cout << "HR manager ID: " << record.getHrManagerId() << std::endl;
                std::cout << "Contest ID: " << record.getContestId() << std::endl;
                std::cout << "Programming language: " << record.getProgrammingLanguage() << std::endl;
                std::cout << "Total tasks: " << record.getTotalTasks() << std::endl;
                std::cout << "Solved tasks: " << record.getSolvedTasks() << std::endl;
                std::cout << "Cheating detected: " << (record.isCheatingDetected() ? "true" : "false") << std::endl << std::endl;
            }
        }
        else if (command == "UPDATE_RECORD") {
            std::string poolName, schemeName, collectionName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            std::cout << "Enter the collection name: ";
            std::cin >> collectionName;
            int applicantId;
            std::string lastName, firstName, middleName, dateOfBirth, resumeLink, programmingLanguage;
            int hrManagerId, contestId, totalTasks, solvedTasks;
            bool cheatingDetected;
            std::cout << "Enter the applicant ID: ";
            std::cin >> applicantId;
            std::cout << "Enter the last name: ";
            std::cin >> lastName;
            std::cout << "Enter the first name: ";
            std::cin >> firstName;
            std::cout << "Enter the middle name: ";
            std::cin >> middleName;
            std::cout << "Enter the date of birth: ";
            std::cin >> dateOfBirth;
            std::cout << "Enter the resume link: ";
            std::cin >> resumeLink;
            std::cout << "Enter the HR manager ID: ";
            std::cin >> hrManagerId;
            std::cout << "Enter the contest ID: ";
            std::cin >> contestId;
            std::cout << "Enter the programming language: ";
            std::cin >> programmingLanguage;
            std::cout << "Enter the total tasks: ";
            std::cin >> totalTasks;
            std::cout << "Enter the solved tasks: ";
            std::cin >> solvedTasks;
            std::cout << "Enter the cheating detected (true/false): ";
            std::cin >> cheatingDetected;
            Record record(applicantId, lastName, firstName, middleName, dateOfBirth, resumeLink, hrManagerId, contestId, programmingLanguage, totalTasks, solvedTasks, cheatingDetected);
            dataManager.updateRecord(poolName, schemeName, collectionName, record);
        }
        else if (command == "PRINT_POOLS") {
            dataManager.printPools();
        }
        else if (command == "PRINT_SCHEMES") {
            std::string poolName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            dataManager.printSchemes(poolName);
        }
        else if (command == "PRINT_COLLECTIONS") {
            std::string poolName, schemeName;
            std::cout << "Enter the pool name: ";
            std::cin >> poolName;
            std::cout << "Enter the scheme name: ";
            std::cin >> schemeName;
            dataManager.printCollections(poolName, schemeName);
        }
        else if (command == "CLEAR") {
            dataManager.clear();
        }
        else if (command == "SAVE_STATE") {
            std::string filename;
            std::cout << "Enter the filename: ";
            std::cin >> filename;
            dataManager.saveState(filename);
        }
        else if (command == "LOAD_STATE") {
            std::string filename;
            std::cout << "Enter the filename: ";
            std::cin >> filename;
            dataManager.loadState(filename);
        }
        else if (command == ""){
            continue;
        }

        else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }
}



int main() {
    DataManager dataManager;
    processDialog(dataManager);
    return 0;
}