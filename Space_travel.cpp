#include<bits/stdc++.h>
using namespace std;

// Returns the current date as a string in the "dd-mm-yyyy" format
string getCurrentDate()
{

    time_t tt;
    time(&tt);

    // Convert the time to a struct tm object representing a calendar date and time
    struct tm *timeinfo;
    timeinfo = localtime(&tt);

     // Create a stringstream to format the date
    ostringstream oss;
    oss << put_time(timeinfo, "%d-%m-%Y");

    // Convert stringstream to string and return
    return oss.str();
}

// Converts a date string in "dd-mm-yyyy" format to a chrono::system_clock::time_point object
chrono::system_clock::time_point convertToDate(string dateStr)
{
    struct tm time = {0};
    
    // Extract the day, month, and year from the input date string
    stringstream ss(dateStr);
    ss >> time.tm_mday;
    ss.ignore();
    ss >> time.tm_mon;
    ss.ignore();
    ss >> time.tm_year;

    // Adjust the month and year values to conform to the struct tm format
    time.tm_mon -= 1;
    time.tm_year -= 1900;

    // Convert the struct tm object to a chrono::system_clock::time_point object using mktime() function
    return chrono::system_clock::from_time_t(mktime(&time));
}

// Calculates the difference between two dates in days
int differenceOfDates(string s1, string s2)
{
    // Convert both date strings to chrono::system_clock::time_point objects using the convertToDate() function
    auto startDate = convertToDate(s1);
    auto endDate = convertToDate(s2);

    // Calculate the difference between the two dates in hours
    auto diff = chrono::duration_cast<chrono::hours>(endDate - startDate);

    // Convert the difference from hours to days and return it as an integer
    return diff.count() / 24;
}

bool isvalidreturn(string going, string returning)
{
    return differenceOfDates(going, returning) > 0;
}

int is_leap_year(int year)
{
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

bool is_valid_date(int day, int month, int year)
{
    if (year < 0 || month < 1 || month > 12 || day < 1)
    {
        return false;
    }

    int max_day = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        max_day--;
    }
    else if (month == 2)
    {
        max_day = is_leap_year(year) + 28;
    }

    return day <= max_day;
}

bool checkvalifdityofdate(string input_date)
{
    int day, month, year;
    char delimiter;
    stringstream ss(input_date);
    ss >> day >> delimiter >> month >> delimiter >> year;

    if (ss.fail() || !ss.eof())
    {
        cout << "Invalid date format\n";
        return false;
    }
    if (!is_valid_date(day, month, year) )
    {
        cout << "Please enter a valid date.\n";
    }
    return is_valid_date(day, month, year);
}

class Planet
{
public:
    static map<string, Planet> globalPlanet; // used to map planet objects to their name
    string name;
    double x;
    double y;
    double z;
    Planet()
    {
        ;
    }
    Planet(string, double, double, double);
    void getCoordinates(void);
    virtual void getwaterinfo() { ; }
};

Planet::Planet(string s, double x1, double y1, double z1)
{
    name = s;
    x = x1;
    y = y1;
    z = z1;
    globalPlanet.insert({s, *this});
}

map<string, Planet> Planet::globalPlanet = {};

class Moon : public Planet // optional part : child class 'Moon' of 'Planet' class
{
public:
    int atmosphere;
    int water;
    void getatmosphereinfo(void)
    {
        cout << "Atmosphere: " << atmosphere << endl;
    };
    void getwaterinfo(void)
    {
        cout << "Water: " << water << endl;
    }
};

class Mars : public Planet // optional part : child class 'Mars' of 'Planet' class
{
public:
    int habitants;
    int water;
    void getwaterinfo(void)
    {
        cout << "Water: " << water << endl;
    }
    void updatewaterinfo(int wa)
    {
        water = wa;
    }
};

void Planet::getCoordinates(void) // prints the coordinates of a Planet
{
    cout << "The coordinates of planet " << name << " are:\nx = " << x << "\ny = " << y << "\nz = " << z << endl;
}

double distance(Planet p1, Planet p2) // return the Euclidean distance between two planets
{
    double dis = pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2) + pow((p1.z - p2.z), 2);
    return sqrt(dis);
}

bool equalPlanets(Planet p1, Planet p2) // check if two planet objects are equal by checking equality of their coordinates and name
{
    return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z && p1.name == p2.name);
}

class Traveller; // forward declaration of 'Traveller' class

class Ticket
{
private:
    double Price;
    double K0 = 100000000.0; // Constant independent of date of travel or distance between planets
    double K;                // Constant for a particular pair of planets , equal to distance times K0
    static int cnt;
    bool booked;

public:
    static vector<Ticket> pendingTicket; // list of tickets which are not yet included in any space travel
    int ticketId;                        // unique id of every ticket
    Planet source;
    Planet destination;
    Traveller *person;
    string dateOfTravel;
    Ticket(Planet, Planet, Traveller *, string);
    Ticket(Planet, Planet, Traveller *, string, int);
    void bookTicket();
    void cancelTicket(void);
    void updateTicket(Planet, string);
    double price(void);
    void getprice(void);
    void printTicket();
};

vector<Ticket> Ticket::pendingTicket = {};
int Ticket::cnt = 0;

Ticket ::Ticket(Planet p1, Planet p2, Traveller *pers, string date)
{
    source = p1;
    destination = p2;
    person = pers;
    dateOfTravel = date;
    booked = false;
    K = K0 * distance(source, destination);
    Price = price();
    ticketId = ++cnt;
}

Ticket ::Ticket(Planet p1, Planet p2, Traveller *pers, string date, int i)
{
    source = p1;
    destination = p2;
    person = pers;
    dateOfTravel = date;
    booked = false;
    K = K0 * distance(source, destination);
    Price = price();
    ticketId = i;
}

double Ticket ::price(void)
{
    string currdate = getCurrentDate();
    double time_to_travel = double(differenceOfDates(currdate, dateOfTravel));
    return K / time_to_travel; // price is inversely proportional to the time to the travel date
}

void Ticket ::getprice(void)
{
    cout << "The price of this ticket is: $" << Price << endl;
}

bool equalTickets(Ticket t1, Ticket t2) // checks the equaliy of tickets by comparing source, destination and date of travel
{
    return (equalPlanets(t1.destination, t2.destination) && equalPlanets(t1.source, t2.source) && t1.dateOfTravel == t2.dateOfTravel);
}

class Traveller
{
private:
    static int cnt;

protected:
    int traveller_id; // this id is private to the system
public:
    static vector<Traveller> all_travellers; // list of all travellers of all categories
    string name;
    int id;
    vector<Ticket> list_of_tickets; // list of tickets of traveller
    Traveller() { ; }
    Traveller(string s, int i)
    {
        name = s;
        id = i;
        traveller_id = ++cnt;
        list_of_tickets = {};
        all_travellers.push_back(*this);
    }
    void updateName(string);
    void getName(void);
    void getId(void);
    void getTicket(void);
    void updateId(int);
    friend class Ticket;
};

int Traveller ::cnt = 0;
vector<Traveller> Traveller ::all_travellers = {};

void Traveller::getName(void)
{
    cout << "Name of the traveller: " << name << endl;
    return;
}

void Traveller::getId(void)
{
    cout << "Id of the traveller: " << id << endl;
    return;
}

void Traveller::getTicket(void)
{
    for (int i = 0; i < list_of_tickets.size(); i++)
    {
        list_of_tickets[i].printTicket();
    }
    return;
}

void Traveller::updateName(string s)
{
    name = s;
}

void Traveller::updateId(int newId)
{
    for (auto it : all_travellers)
    {
        if (it.id == newId)
        {
            cout << "This id has been already taken. Try with a new id.\n";
            return;
        }
    }
    id = newId;
    cout << "Id updated successfully.\n";
    return;
}

class Astronaut : public Traveller
{
private:
    int experience;

public:
    static vector<Astronaut> list_of_astronauts;
    void see_own_travel(void);
    Astronaut() { ; }
    Astronaut(string s, int i, int exp) : Traveller(s, i)
    {
        experience = exp;
        list_of_astronauts.push_back(*this);
    }
    friend class Ticket;
};

vector<Astronaut> Astronaut ::list_of_astronauts = {};

int findastroID(string date)
{
    for (int i = 0; i < Astronaut::list_of_astronauts.size(); i++)
    {
        bool flag = true;
        for (auto iter : Astronaut::list_of_astronauts[i].list_of_tickets)
        {
            if (iter.dateOfTravel == date)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            return i;
        }
    }
    return -1;
}

class Passenger : public Traveller
{
public:
    Passenger(string s, int i) : Traveller(s, i)
    {
        ;
    }
};

class Commander : public Traveller
{
private:
    int authority;

public:
    int experience;
    int license_id;
    static vector<Commander> list_of_commanders;
    void see_all_travels(void);
    void see_own_travels(void);
    Commander()
    {
        ;
    }
    Commander(string s, int i, int auth, int exp, int lId) : Traveller(s, i)
    {
        authority = auth;
        experience = exp;
        license_id = lId;
        list_of_commanders.push_back(*this);
    }
    friend class Ticket;
};

vector<Commander> Commander ::list_of_commanders = {};

int findCommandoID(string date)
{
    for (int i = 0; i < Commander::list_of_commanders.size(); i++)
    {
        bool flag = true;
        for (auto iter : Commander::list_of_commanders[i].list_of_tickets)
        {
            if (iter.dateOfTravel == date)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            return i;
        }
    }
    return -1;
}

void Ticket ::printTicket(void)
{
    string tmp = (booked) ? "Booked" : "Not Booked";
    cout << "\n";

    cout << "*        Details of the Journey        \n";
    cout << "*        Unique idenification number of Ticket: "
         << ticketId << "        \n"
         << "*        Source Planet: "
         << source.name << "        \n"
         << "*        Destination Planet: " << destination.name << "        \n"
         << "*        Date of travel: " << dateOfTravel << "        \n"
         << "*        ID of the traveller: " << person->id << "        \n"
         << "*        Name of the traveller: " << person->name << "        \n"
         << "*        Amount: $" << Price << "        \n"
         << "*        Status: " << tmp << "        \n";
    cout << "\n";
}

class SpaceTravel
{
private:
    static vector<SpaceTravel> all_travels;
    vector<int> ticketIds;                  // stores the IDs of all tickets involved in space travel
    vector<Traveller *> list_of_travellers; // list of all passengers included in a travel
    Planet source;
    Planet destination;
    string dateofTravel;
    int astroID;
    int commandoID;

public:
    SpaceTravel(int ai, int ci, vector<Traveller *> v, vector<int> ti, Planet p1, Planet p2, string da)
    {
        astroID = Astronaut::list_of_astronauts[ai].id;
        commandoID = Commander::list_of_commanders[ci].id;
        list_of_travellers = v;
        ticketIds = ti;
        source = p1;
        destination = p2;
        dateofTravel = da;
        all_travels.push_back(*this);
    }
    void add_traveller(Traveller *);
    void printDetails(void);
    void updateAstroID(int);
    void updateCommandoID(int);
    friend class Ticket;
    friend class Commander;
    friend class Astronaut;
};

vector<SpaceTravel> SpaceTravel ::all_travels = {};

void SpaceTravel ::add_traveller(Traveller *pers)
{
    list_of_travellers.push_back(pers);
}

void SpaceTravel ::updateAstroID(int id)
{
    bool flag = false;
    for (int i = 0; i < Astronaut::list_of_astronauts.size(); i++)
    {
        if (Astronaut::list_of_astronauts[i].id == id)
        {
            astroID = id;
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        cout << "No astronaut with this ID found. Try with some other valid ID.\n";
    }
}

void SpaceTravel ::updateCommandoID(int id)
{
    bool flag = false;
    for (int i = 0; i < Commander::list_of_commanders.size(); i++)
    {
        if (Commander::list_of_commanders[i].id == id)
        {
            commandoID = id;
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        cout << "No commander with this ID found. Try with some other valid ID.\n";
    }
}

void SpaceTravel ::printDetails(void)
{
    cout << "\n";

    cout << "*        Details of the Journey        \n";
    cout << "*        Source Planet: " << source.name << "        \n"
         << "*        Destination Planet: " << destination.name << "        \n"
         << "*        Date of travel: " << dateofTravel << "        \n"
         << "*        Astronaut ID: " << astroID << "        \n"
         << "*        Commander ID: " << commandoID << "        \n"
         << "*        Passenger List:        \n";
    for (auto it : list_of_travellers)
    {
        cout << "*        " << it->name << "        \n";
    }
    cout << "\n";
}

void Commander ::see_all_travels()
{
    if (SpaceTravel::all_travels.size() == 0)
    {
        cout << "No space travels exist\n";
    }
    for (auto it : SpaceTravel::all_travels)
    {
        it.printDetails();
    }
}

void Commander ::see_own_travels()
{
    bool flag = false;
    for (auto it : SpaceTravel::all_travels)
    {
        if (it.commandoID == this->id)
        {
            flag = true;
            it.printDetails();
            cout << "\n\n";
        }
    }
    if (!flag)
    {
        cout << "No space travels exist\n";
    }
}

void Astronaut ::see_own_travel()
{
    bool flag = false;
    for (auto it : SpaceTravel::all_travels)
    {
        if (it.astroID == this->id)
        {
            flag = true;
            it.printDetails();
            cout << "\n\n";
        }
    }
    if (!flag)
    {
        cout << "No space travels exist\n";
    }
}

void Ticket ::bookTicket()
{

    if (differenceOfDates(getCurrentDate(), dateOfTravel) > 3650)
    {
        cout << "Tickets for dates beyond 10 years cannot be booked. Please enter a valid date.\n";
        return;
    }
    if(differenceOfDates(getCurrentDate(), dateOfTravel)<0){
        cout << "This date is passed. Please enter a valid date.\n";
        return;
    }
    if (!booked)
    {
        booked = true;
        person->list_of_tickets.push_back(*this);
        cout << "Ticket booked for " << person->name << "\n";
    }
    bool flag = false;
    for (auto &it : SpaceTravel::all_travels)
    {
        if (equalPlanets(it.source, this->source) && equalPlanets(it.destination, this->destination) && it.dateofTravel == this->dateOfTravel && it.list_of_travellers.size() <= 8)
        {
            // this if block is executed when there exists a space travel from same source to same destination on same date and has a vacancy.
            it.add_traveller(this->person);
            it.ticketIds.push_back(this->ticketId);
            flag = true;
            break;
        }
    }
    int count = 0;
    if (!flag)
    {
        vector<int> tickets = {this->ticketId};
        vector<Traveller *> v = {this->person};
        for (auto &it : pendingTicket)
        {
            if (equalTickets(it, *this))
            {
                count++;
                v.push_back(it.person);
                tickets.push_back(it.ticketId);
            }
        }
        if (count >= 2)
        {
            // this block is executed when there are more than 2 tickets and a new space travel is to be created
            flag = true;
            int aID = findastroID(this->dateOfTravel);    // search for a astro free on that date
            int cID = findCommandoID(this->dateOfTravel); // search for a commando free on that date
            if (aID == -1 || cID == -1)
            {
                cout << "Sorry! Astronaut or Commando not available for Space Travel. You will be notified when we have them ready.\n";
                pendingTicket.push_back(*this);
                return;
            }
            Ticket tastro(this->source, this->destination, &Astronaut::list_of_astronauts[aID], dateOfTravel);
            tastro.booked = true;
            Astronaut::list_of_astronauts[aID].list_of_tickets.push_back(tastro); // insert the ticket in list of tickets of astro
            Ticket tcommando(this->source, this->destination, &Commander::list_of_commanders[cID], dateOfTravel);
            tcommando.booked = true;
            Commander::list_of_commanders[cID].list_of_tickets.push_back(tcommando); // insert the ticket in list of tickets of commando
            tickets.push_back(tastro.ticketId);
            tickets.push_back(tcommando.ticketId);
            SpaceTravel newtravel(aID, cID, v, tickets, this->source, this->destination, this->dateOfTravel); // create a new space travel
            auto it = pendingTicket.begin();
            while (it != pendingTicket.end())
            {
                // this loop is used to remove all the tickets which get confirmed for a space travel from pending list
                if (std::find(tickets.begin(), tickets.end(), it->ticketId) != tickets.end())
                {
                    it = pendingTicket.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }
    if (!flag)
    {
        // this is executed when there is no possibility of a space travel and the ticket is pushed into the pending list
        cout << "Space travel will be assigned when more than 2 persons book ticket on the same date from same source to same destination. Currently available passengers (except you): " << count << endl;
        pendingTicket.push_back(*this);
    }
}

void Ticket ::cancelTicket()
{
    auto it = SpaceTravel::all_travels.begin();
    auto iter3 = (this->person)->list_of_tickets.begin();
    while (iter3 != this->person->list_of_tickets.end())
    {
        // this loop is used to remove the ticket from the list of tickets of the traveller
        if (iter3->ticketId == this->ticketId)
        {
            // ticket found in the list so erase it
            iter3 = this->person->list_of_tickets.erase(iter3);
            break;
        }

        ++iter3;
    }
    while (it != SpaceTravel::all_travels.end())
    {
        // search the space travel in which the ticket were going to be used
        if (find(it->ticketIds.begin(), it->ticketIds.end(), this->ticketId) != it->ticketIds.end())
        {
            // space travel found
            auto iter2 = find(it->list_of_travellers.begin(), it->list_of_travellers.end(), this->person);
            if (iter2 != it->list_of_travellers.end())
            {
                // remove the owner of the ticket from the list of travellers of space travel
                it->list_of_travellers.erase(iter2);
            }
            if (it->list_of_travellers.size() >= 3)
            {
                // if the space travel has sufficient members after removing the ticket owner just remove the concerned ticket ID from the list of ticket IDs
                auto iter1 = find(it->ticketIds.begin(), it->ticketIds.end(), this->ticketId);
                if (iter1 != it->ticketIds.end())
                {
                    it->ticketIds.erase(iter1);
                }
            }
            else
            {
                // if the space travel falls short of members after removing the ticket owner, push the tickets of all other passengers into pending list
                for (auto iter4 : Traveller::all_travellers)
                {
                    for (auto iter5 : iter4.list_of_tickets)
                    {
                        // search the ticket list of all travellers
                        bool found = false;
                        for (auto iter6 : it->ticketIds)
                        {
                            if (iter5.ticketId == iter6)
                            {
                                // the traveller has a ticket in the space travel
                                found = true;
                                break;
                            }
                        }
                        if (found)
                        {
                            pendingTicket.push_back(iter5);
                        }
                    }
                }
                it = SpaceTravel::all_travels.erase(it); // remove the travel from all travels list
            }
        }
        else
        {
            ++it;
        }
    }
}

void Ticket ::updateTicket(Planet p2, string date)
{
    int temp = this->ticketId;
    Planet p1 = this->source;
    Traveller *pers = this->person;
    cout << "Cancelling your old ticket...\n";
    this->cancelTicket();
    Ticket tmp(p1, p2, pers, date, temp);
    cout << "Booking new ticket with updated details...\n";
    tmp.bookTicket();
}

void printmenu(void)
{
    cout << "Press one of the following commands\n";
    cout << "1 to book ticket\n";
    cout << "2 to update ticket\n";
    cout << "3 to cancel ticket\n";
    cout << "4 to view your tickets\n";
    cout << "5 to access the system as a commander\n";
    cout << "6 to access general traveller functions\n";
    cout << "0 to exit\n";
}

int verify_id(int id)
{
    for (auto it : Traveller::all_travellers)
    {
        if (it.id == id)
        {
            return 1;
        }
    }
    return 0;
}

void command1(void)
{
    int id;
    cout << "Enter your UID\n";
    cin >> id;
    if (!verify_id(id))
    {
        cout << "No Passenger found! Enter a valid ID\n";
        return;
    }
    bool flag = false;
    for (int i = 0; i < Traveller::all_travellers.size(); i++)
    {
        if (Traveller::all_travellers[i].id == id)
        {
            string src;
            string dest;
            flag = true;
            cout << "Enter Source Planet (Earth, Mars or Jupiter)\n";
            cin >> src;
            if (Planet::globalPlanet.find(src) == Planet::globalPlanet.end())
            {
                cout << "Planet not found. Please checkk your input.\n";
                return;
            }
            cout << "Enter Destination Planet (Earth, Mars or Jupiter)\n";
            cin >> dest;
            if (src == dest)
            {
                cout << "Source and Destination cannot be same.\n";
                return;
            }
            if (Planet::globalPlanet.find(dest) == Planet::globalPlanet.end())
            {
                cout << "Planet not found. Please checkk your input.\n";
                return;
            }
            string dat;
            cout << "Enter the date of travel in dd-mm-yyyy format\n";
            cin >> dat;
            if (!(checkvalifdityofdate(dat)) || differenceOfDates(getCurrentDate(), dat)<0)
            {   cout << "This is not a valid date \n";
                return;
            }
            int returntick;
            Ticket tick(Planet::globalPlanet[src], Planet::globalPlanet[dest], &Traveller::all_travellers[i], dat);
            tick.bookTicket();
            cout << "Press 1 if you want to book return ticket also.. else press 0\n";
            cin >> returntick;
            if (returntick)
            {
                string datreturn;
                cout << "Enter the date of return travel\n";
                cin >> datreturn;
                if (!(checkvalifdityofdate(datreturn)))
                {
                    return;
                }
                if (!isvalidreturn(dat, datreturn))
                {
                    cout << "Invalid return date\n";
                    return;
                }
                Ticket tick1(Planet::globalPlanet[dest], Planet::globalPlanet[src], &Traveller::all_travellers[i], datreturn);
                tick1.bookTicket();
            }

            break;
        }
    }
    if (!flag)
    {
        cout << "No passenger found! Enter a valid id.\n";
        return;
    }
}
void command2(void)
{
    int id;
    cout << "Enter your UID\n";
    cin >> id;
    if (!verify_id(id))
    {
        cout << "No passenger found! Enter a valid ID\n";
        return;
    }
    bool flag = false;
    for (int i = 0; i < Traveller::all_travellers.size(); i++)
    {
        if (Traveller::all_travellers[i].id == id)
        {
            flag = true;
            int tid;
            cout << "Enter a ticket ID\n";
            cin >> tid;
            bool flag2 = false;
            for (auto &it : Traveller::all_travellers[i].list_of_tickets)
            {
                if (it.ticketId == tid)
                {
                    flag2 = true;
                    string newdest;
                    string dat;
                    cout << "Enter the updated destination\n";
                    cin >> newdest;
                    if (Planet::globalPlanet.find(newdest) == Planet::globalPlanet.end())
                    {
                        cout << "Planet not found. Please checkk your input.\n";
                        return;
                    }
                    cout << "Enter the update date of travel\n";
                    cin >> dat;
                    if (!(checkvalifdityofdate(dat)))
                    {
                        return;
                    }
                    if (dat == it.dateOfTravel && equalPlanets(it.destination, Planet::globalPlanet[newdest]))
                    {
                        cout << "Ticket with hese details has already been booked. No need to book again.\n";
                        return;
                    }
                    it.updateTicket(Planet::globalPlanet[newdest], dat);
                    break;
                }
            }
            if (!flag2)
            {
                "No ticket with this ID exists\n";
                return;
            }
        }
    }
    if (!flag)
    {
        cout << "No passenger found! Enter a valid id.\n";
        return;
    }
}
void command3(void)
{
    int id;
    cout << "Enter your UID\n";
    cin >> id;
    if (!verify_id(id))
    {
        cout << "No passenger found! Enter a valid ID\n";
        return;
    }
    bool flag = false;
    for (int i = 0; i < Traveller::all_travellers.size(); i++)
    {
        if (Traveller::all_travellers[i].id == id)
        {
            int tid;
            flag = true;
            cout << "Enter a ticket ID\n";
            cin >> tid;
            bool flag2 = false;
            for (auto &it : Traveller::all_travellers[i].list_of_tickets)
            {
                if (it.ticketId == tid)
                {
                    flag2 = true;
                    it.cancelTicket();
                    cout << "Ticket cancelled successfully\n";
                    break;
                }
            }
            if (!flag2)
            {
                "No ticket with this ID exists\n";
                return;
            }
        }
    }
    if (!flag)
    {
        cout << "No passenger found! Enter a valid id.\n";
        return;
    }
}
void command4(void)
{
    int id;
    cout << "Enter your UID\n";
    cin >> id;
    if (!verify_id(id))
    {
        cout << "No passenger found! Enter a valid ID\n";
        return;
    }
    bool flag = false;
    for (int i = 0; i < Traveller::all_travellers.size(); i++)
    {
        if (Traveller::all_travellers[i].id == id)
        {
            flag = true;
            Traveller::all_travellers[i].getTicket();
            break;
        }
    }
    if (!flag)
    {
        cout << "No passenger found! Enter a valid id.\n";
    }
}

void command5(void)
{
    int id;
    cout << "Enter your UID\n";
    cin >> id;
    if (!verify_id(id))
    {
        cout << "Enter a valid ID\n";
        return;
    }
    bool flag = false;
    for (auto it : Commander::list_of_commanders)
    {
        if (it.id == id)
        {
            flag = true;
            int type;
            cout << "Press 0 to see your upcoming travels and 1 to see all upcoming travels\n";
            cin >> type;
            if (type == 0)
            {
                it.see_own_travels();
            }
            else if (type == 1)
            {
                it.see_all_travels();
            }
            else
            {
                cout << "Invalid input\n";
            }
            break;
        }
    }
    if (!flag)
    {
        cout << "No commander found with the id. You are not authorised.\n";
    }
}

void command6(void)
{
    int id;
    cout << "Enter your UID\n";
    cin >> id;
    if (!verify_id(id))
    {
        cout << "Enter a valid ID\n";
        return;
    }
    for (auto &it : Traveller::all_travellers)
    {
        if (it.id == id)
        {
            int proc;
            cout << "Press 1 to update name\n";
            cout << "Press 0 to update ID\n";
            cin >> proc;
            if (proc == 0)
            {
                int newid;
                cout << "Enter new ID\n";
                cin >> newid;
                it.updateId(newid);
            }
            else if (proc == 1)
            {
                string newname;
                cout << "Enter new Name\n";
                cin >> newname;
                it.updateName(newname);
            }
            else
            {
                cout << "Enter a valid input\n";
            }
            break;
        }
    }
    return;
}

int main()
{
    Planet earth("Earth", 1.1, 1.0, 1.0);
    Planet mars("Mars", 4.1, 5.0, 2.0);
    Planet jupiter("Jupiter", 8.2, 6.5, 4.1);
    Astronaut a1("Astro1", 65, 4);
    Astronaut a2("Astro2", 66, 9);
    Commander c1("Commando1", 88, 10, 6, 1256);
    Commander c2("Commando2", 89, 10, 6, 1257);
    Passenger p1("Pass1", 23);
    Passenger p2("Pass2", 24);
    Passenger p3("Pass3", 25);
    Passenger p4("Pass4", 26);
    Passenger p5("Pass5", 27);
    Passenger p6("Pass6", 28);
    Passenger p7("Pass7", 29);
    Passenger p8("Pass8", 30);
    Passenger p9("Pass9", 31);
    Passenger p10("Pass10", 32);
    Passenger p11("Pass11", 33);
    Passenger p12("Pass12", 34);
    Passenger p13("Pass13", 35);
    cout << "Welcome to Space Travel System!!\n";
    int command = 1;
    while (1)
    {   
        printmenu();
        cin >> command;
        if (command == 1)
        {
            command1();
        }
        else if (command == 2)
        {
            command2();
        }
        else if (command == 3)
        {
            command3();
        }
        else if (command == 4)
        {
            command4();
        }
        else if (command == 5)
        {
            command5();
        }
        else if (command == 6)
        {
            command6();
        }
        else if (command == 0)
        {
            cout << "Thank you for using Space Travel System!!\n";
            break;
        }
    }
}

// Currently there are 13 passengers whose ids are from 23 to 35, 2 astronauts with ids 65, 66 and two commanders with ids 88 and 89.