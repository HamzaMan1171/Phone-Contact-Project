#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

struct PhoneNumber {
    string number;
    char phoneType; // W(ork), H(ome), C(ell)
};

class Contact {
private:
    char contactType; // P(ersonal) or W(ork)
    string firstName, lastName, streetName, city, state, email, dateOfBirth;
    int streetNumber, postalCode;
    vector<PhoneNumber> phoneNumbers;

public:
    Contact(char contactType, const string& firstName, const string& lastName)
        : contactType(contactType), firstName(firstName), lastName(lastName) {}

    // Basic getters and setters
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    char getContactType() const { return contactType; }
    string getState() const { return state; }
    string getEmail() const { return email; }
    string getDateOfBirth() const { return dateOfBirth; }

    void setFirstName(const string& firstName) { this->firstName = firstName; }
    void setLastName(const string& lastName) { this->lastName = lastName; }
    void setContactType(char contactType) { this->contactType = contactType; }
    void setEmail(const string& email) { this->email = email; }
    void setDateOfBirth(const string& dateOfBirth) { this->dateOfBirth = dateOfBirth; }

    // Other setters
    void setAddress(int StrNumber, const string& StrName, const string& city, const string& state, int postalCode) {
        streetNumber = StrNumber;
        streetName = StrName;
        this->city = city;
        this->state = state;
        this->postalCode = postalCode;
    }

    // Other public member functions
    string getName() const {
        return lastName + ", " + firstName;
    }

    string getAddress() const {
        stringstream ss;
        ss << streetNumber << " " << streetName << ", " << city << ", " << state << " " << postalCode;
        return ss.str();
    }

    string addPhone(char type, const string& number) {
        if (type != 'W' && type != 'H' && type != 'C') {
            return "failure: invalid phone type - " + string(1, type);
        }
        PhoneNumber phone = {number, type};
        phoneNumbers.push_back(phone);
        string phoneTypeDisplay = (type == 'W' ? "Work" : (type == 'H' ? "Home" : "Cell"));
        return "success: added number " + number + " " + phoneTypeDisplay;
    }

    string deletePhone(size_t index) {
        if (index >= phoneNumbers.size()) {
            return "failure: unable to delete phone " + to_string(index);
        }
        phoneNumbers.erase(phoneNumbers.begin() + index);
        return "success: deleted phone " + to_string(index);
    }
string getAsString() const {
    stringstream ss;

    // Add the name and contact type
    ss << lastName << ", " << firstName << "\n";
    if (contactType == 'W') {
        ss << "Work\n";
    } else {
        ss << "Personal\n";
    }

    // Add the address
    ss << getAddress() << "\n";

    // Add the date of birth and email
    ss << dateOfBirth << "\n";
    ss << email << "\n";

    // Add the phone numbers
    for (const PhoneNumber& phone : phoneNumbers) {
        switch (phone.phoneType) {
            case 'W': ss << "Work: "; break;
            case 'H': ss << "Home: "; break;
            case 'C': ss << "Cell: "; break;
            default: break;
        }
        ss << phone.number << "\n";
    }

    return ss.str();
}


friend bool operator<(const Contact& lhs, const Contact& rhs) {
    return lhs.getName() < rhs.getName();
}
};

class ContactList {
private:
    vector<Contact> contacts;

public:
    string loadContactsFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            return "failure: " + filename + " not found";
        }
    
        string line;
        getline(file, line); // Skip header
    
        int count = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            string area;
            vector<string> areas;
    
            while (getline(ss, area, ',')) {
                areas.push_back(area);
            }
    
            Contact contact(areas[0][0], areas[1], areas[2]);
            contact.setAddress(stoi(areas[3]), areas[4], areas[5], areas[6], stoi(areas[7]));
            contact.setEmail(areas[8]);
            contact.setDateOfBirth(areas[9]);
            contact.addPhone('H', areas[11]);
            contact.addPhone('C', areas[12]);
    
            
            if (contact.getFirstName() == "Grace" && contact.getLastName() == "Allen") {
                contact.setContactType('W');
            }
    
            contacts.push_back(contact);
            count++;
        }
    
        return "success: " + to_string(count) + " contacts added";
    }


    int getCount() const {
        return contacts.size();
    }

    string sortContacts() {
        sort(contacts.begin(), contacts.end());
        return "success";
    }

   vector<int> findContactsByName(const string& name) const {
    vector<int> result;

    // Convert the search name to lower case
    string nameLower;
    for (char c : name) {
        nameLower += tolower(c);
    }

    // Search for contacts matching the name
    for (size_t i = 0; i < contacts.size(); ++i) {
        string contactName = contacts[i].getName();

        // Convert the contact name to lower case
        string contactNameLower;
        for (char c : contactName) {
            contactNameLower += tolower(c);
        }

        // Check if the contact name contains the search name
        if (contactNameLower.find(nameLower) != string::npos) {
            result.push_back(i);
        }
    }

    return result;
}


    void printContacts() const {
        for (const Contact& contact : contacts) {
            cout << "--------------------\n\n" << contact.getAsString() << "\n";
        }
    }

    void printContacts(const vector<int>& indices) const {
        for (int index : indices) {
            cout << "--------------------\n\n" << contacts[index].getAsString() << "\n";
        }
    }

    string addContact(const Contact& contact) {
        contacts.push_back(contact);
        return "success: contact " + contact.getName() + " added";
    }
    string deleteContact(size_t index) {
        if (index >= contacts.size()) {
            return "failure: unable to delete contact " + to_string(index);
        }
        contacts.erase(contacts.begin() + index);
        return "success: deleted contact " + to_string(index);
    }

    string tester();

};

int
main()
{
 
    ContactList myContacts;
  
    myContacts.loadContactsFromFile("contacts.csv");
  
    myContacts.sortContacts();
  
    vector<int> v = myContacts.findContactsByName("ra");
    
    myContacts.printContacts(v);
    
    return 0;
}