//c++ std lib
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <random>
#include <fstream>

#define PROGRAM_NAME "Dice Simulator"
#define PROGRAM_VERSION "v0.0.0"

#ifdef DEBUG
#    define PROGRAM_VERSION_DBG "(DBG)"
#else
#    define PROGRAM_VERSION_DBG ""
#endif

struct Die {
    unsigned sides;
    int min_val;
    int increment;
};

struct Dice {
    Die* die;
    unsigned count;
};

int throw_dice(Dice* _dice);
int custom_throw();
int add_config(vector<Dice>* _configs);

int main()
{
    string program_info(PROGRAM_NAME " " PROGRAM_VERSION_DBG PROGRAM_VERSION);

    Die def_d = { .sides = 6, .min_val = 1, .increment = 1 };

    vector<Dice> throw_configs;
    throw_configs.push_back(Dice {.die = &def_d, .count = 3});

    string def_cfg_str(std::to_string(throw_configs[0].count) + "d"
            + std::to_string(def_d.sides));

    char cmd = 0;
    while (cmd != 'q') {
        cout << "\n\n"
             << "*** " << program_info << " ***\n"
             << "t - throw (" << def_cfg_str << ")\n"
             << "c - custom throw\n"
             << "a - (WIP) add config\n"
             << "s - (WIP) select config\n"
             << "m - (WIP) modify config\n"
             << "r - (WIP) remove config\n"
             << "q - quit\n";

        cout << "> ";
        cin >> cmd;

        switch(cmd) {
        case 't': throw_dice(&throw_configs[0]); break;
        case 'c': custom_throw(); break;
        case 'a': add_config(&throw_configs); break;
        case 's': 
        case 'm':
        case 'r':
            cout << "Sorry, this part is currently under construction.\n";
            break;
        case 'q': break;
        default:
            cout << "invalid selection (" << cmd << ")\n";
        }
    }

    return 0;
}

int throw_dice(Dice* _dice)
{
    if(_dice->die->sides < 1) {
        cout << "a slight problem: dice can not have less than 1 side\n";
        return -1;
    }

    int min = _dice->die->min_val;
    int max = min + (_dice->die->increment * (_dice->die->sides - 1));

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

    std::mt19937::result_type total = 0;
    for(unsigned i = 0; i < _dice->count; ++i) {
        auto rand_val = dist(rng);
        cout << "d#" << i+1 << ": " << rand_val << "\n";
        total += rand_val;
    }

    cout << "total(" << _dice->count << "d" << _dice->die->sides << ": "
         << total << "\n";

    return 0;
}

int custom_throw()
{
    Die die;
    Dice dice;

    cout << "number of sides: ";
    cin >> die.sides;
    cout << "minimum value: ";
    cin >> die.min_val;
    cout << "value increment (usually 1): ";
    cin >> die.increment;

    dice.die = &die;

    cout << "number of dice to throw: ";
    cin >> dice.count;

    throw_dice(&dice);

    return 0;
}

int add_config(vector<Dice>* _configs)
{
    cout << "Sorry, adding of configs is currently under construction.\n";

    return 0;
}
