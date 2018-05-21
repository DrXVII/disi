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

#define PROGRAM_NAME "Dice Simulator"
#define PROGRAM_VERSION "v0.0.0"

struct Die {
    unsigned sides;
    int min_val;
    int increment;
};

struct Dice {
    struct Die* die;
    unsigned count;
};

int throw_dice(struct Dice* _dice);

int main()
{
    struct Die def_d = { .sides = 6, .min_val = 1, .increment = 1 };
    struct Dice def_cfg = { .die = &def_d, .count = 3 };

    string def_cfg_str(std::to_string(def_cfg.count) + "d"
            + std::to_string(def_d.sides));

    char cmd = 0;
    while (cmd != 'q') {
        cout << "\n\n"
             << "*** " PROGRAM_NAME " " PROGRAM_VERSION " ***\n"
             << "t - throw (" << def_cfg_str << ")\n"
             << "c - (WIP) custom throw\n"
             << "s - (WIP) select config\n"
             << "a - (WIP) add config\n"
             << "m - (WIP) modify config\n"
             << "r - (WIP) remove config\n"
             << "q - quit\n";

        cout << "> ";
        cin >> cmd;

        switch(cmd) {
        case 't':
            throw_dice(&def_cfg);
            break;
        case 'c':
        case 's':
        case 'a':
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

int throw_dice(struct Dice* _dice)
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
        cout << "d#1: " << rand_val << "\n";
        total += rand_val;
    }

    cout << "total: " << total << "\n";

    return 0;
}
