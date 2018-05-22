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
#include <memory>
using std::shared_ptr;

#define PROGRAM_NAME "Dice Simulator"
#define PROGRAM_VERSION "v0.0.1"

#ifdef DEBUG
#    define PROGRAM_VERSION_DBG "(DBG)"
#else
#    define PROGRAM_VERSION_DBG ""
#endif

struct Dice {
    unsigned sides;
    int min_val;
    int increment;
    unsigned count;
};

int throw_dice(Dice* _dice);
int custom_throw();
int add_config(vector<Dice*>* _configs);
Dice* dice_from_input();
int select_config(vector<Dice*>* _configs, Dice** cur_config_);
int remove_config(vector<Dice*>* _configs);
string dice_to_str(Dice* _dice);
void print_configs(vector<Dice*>* _configs);
void free_configs(vector<Dice*>* _configs);
int get_int(int _default, int _min, int _max);

int main()
{
    string program_info(PROGRAM_NAME " " PROGRAM_VERSION_DBG PROGRAM_VERSION);

    vector<Dice*> throw_configs;
    //creating the default config at position 0
    throw_configs.push_back(new Dice {.sides = 6, .min_val = 1, .increment = 1,
                                  .count = 3});

    string def_cfg_str(std::to_string(throw_configs[0]->count) + "d"
            + std::to_string(throw_configs[0]->sides));

    Dice* cur_config = throw_configs[0]; //currently selected config

    char cmd = 0;
    while (cmd != 'q') {
        cout << "\n\n"
             << "*** " << program_info << " ***\n"
             << "t - throw " << dice_to_str(cur_config) << "\n"
             << "c - custom throw\n"
             << "s - select config\n"
             << "a - add config\n"
             << "r - remove config\n"
             << "w - (WIP) write config database\n"
             << "q - quit\n";

        cout << "> ";
        cin >> cmd;

        switch(cmd) {
        case 't': throw_dice(cur_config); break;
        case 'c': custom_throw(); break;
        case 's': select_config(&throw_configs, &cur_config); break;
        case 'a': add_config(&throw_configs); break;
        case 'r': remove_config(&throw_configs); break;
            cout << "Sorry, this part is currently under construction.\n";
            break;
        case 'w': break;
        case 'q': break;
        default:
            cout << "invalid selection (" << cmd << ")\n";
        }
    }

    free_configs(&throw_configs);

    return 0;
}

int throw_dice(Dice* _dice)
{
    if(_dice->sides < 1) {
        cout << "a slight problem: dice can not have less than 1 side\n";
        return -1;
    }

    int min = _dice->min_val;
    int max = min + (_dice->increment * (_dice->sides - 1));

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

    std::mt19937::result_type total = 0;
    for(unsigned i = 0; i < _dice->count; ++i) {
        auto rand_val = dist(rng);
        cout << "d#" << i+1 << ": " << rand_val << "\n";
        total += rand_val;
    }

    cout << "total(" << _dice->count << "d" << _dice->sides << ": "
         << total << "\n";

    return 0;
}

int custom_throw()
{
    Dice* dice = dice_from_input();

    throw_dice(dice);

    delete dice;

    return 0;
}

Dice* dice_from_input()
{
    Dice* dice = new Dice;

    cout << "number of sides: ";
    cin >> dice->sides;
    cout << "minimum value: ";
    cin >> dice->min_val;
    cout << "value increment (usually 1): ";
    cin >> dice->increment;
    cout << "number of dice to throw: ";
    cin >> dice->count;

    return dice;
}

int add_config(vector<Dice*>* _configs)
{
    _configs->push_back(dice_from_input());

    return 0;
}

int select_config(vector<Dice*>* _configs, Dice** cur_config_)
{
    //just printing for now to test adding of a config
    cout << "*** DICE CONFIGS ***\n";
    print_configs(_configs);

    unsigned def_sel = 0;
    unsigned sel = get_int(def_sel, 0, _configs->size() - 1);

    *cur_config_ = (*_configs)[sel];
    cout << "selected " << dice_to_str(*cur_config_) << endl;

    return 0;
}

int remove_config(vector<Dice*>* _configs)
{
    cout << "*** REMOVE CONFIGS ***\n";
    print_configs(_configs);

    if(_configs->size() <= 1) {
        cout << "too few configurations left, refusing to remove\n";
        return -1;
    }

    unsigned def_sel = 0;
    unsigned sel = get_int(def_sel, 0, _configs->size() - 1);

    cout << "deleting... ";
    delete (*_configs)[sel];
    (*_configs)[sel] = _configs->back();
    _configs->pop_back();
    cout << " deleted\n";

    return 0;
}

string dice_to_str(Dice* _dice)
{
    int die_max = _dice->min_val
        + (_dice->increment * (_dice->sides - 1));

    string notation_str(std::to_string(_dice->count) + "d"
        + std::to_string(_dice->sides));
    string die_min_str(std::to_string(_dice->min_val));
    string die_max_str(std::to_string(die_max));

    string result(notation_str
        + "(" + die_min_str + "-" + die_max_str
        + ":" + std::to_string(_dice->min_val * _dice->count)
        + "-" + std::to_string(die_max * _dice->count) + ")"
    );

    return result;
}

void print_configs(vector<Dice*>* _configs) {
    for(unsigned i = 0; i < _configs->size(); ++i) {
        cout << i << ": " << dice_to_str((*_configs)[i]) << "\n";
    }
}

void free_configs(vector<Dice*>* _configs) {
    for(unsigned i = 0; i < _configs->size(); ++i) {
        delete (*_configs)[i];
        (*_configs)[i] = nullptr;
    }
}

int get_int(int _default, int _min, int _max)
{
    string in_buf;
    int ret_val = _default;

    cout << "> ";
    cin.ignore(); //ignoring whatever is left over in the input stream
    std::getline(cin, in_buf);

    try {
        ret_val = std::stoul(in_buf);
    } catch(const std::invalid_argument& _e) {
        cout << "bad input format, selecting default value\n";
    } catch(const std::exception& _e) {
        cout << "conversion exception, what: " << _e.what() << endl;
    }

    if(ret_val < _min || ret_val > _max) {
        cout << "selection out of range, selecting default value\n";
        ret_val = _default;
    }

    return ret_val;
}
