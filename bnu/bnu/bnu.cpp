//BÜNYAMİN BİLGEHAN KILIÇER   1306181425




#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

class Matrix {
private:
    int** size;
    int n;

public:
    void createMatrix(int);
    void showMatrix(int);
    void addMatrix(int, int, int);
    void cleanMatrix();

};



void Matrix::createMatrix(int n) {
    this->size = new int* [n];
    for (int i = 0; i < n; ++i)
        this->size[i] = new int[n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            this->size[i][j] = 0;
        }
    }
    this->n = n;
}



void Matrix::cleanMatrix() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            this->size[i][j] = 0;
        }
    }
}



void Matrix::addMatrix(int x, int y, int enf) {
    this->size[x - 1][y - 1] = enf;
}



void Matrix::showMatrix(int turn) {

    ofstream file_;
    file_.open("TURNS.txt", std::ios_base::app);

    file_ << "Turn: " << turn + 1 << endl;
    int i;
    for (i = 0; i < n + 2; i++) {

        if (i == 0 || i == n + 1) {
            for (int j = 0; j < n + 2; j++) {
                file_ << "-";
            }

            file_ << endl;
        }

        else {
            file_ << "-";
            for (int j = 0; j < n; j++) {
                if (size[i - 1][j] == 1) {
                    file_ << "X";
                }
                else if (size[i - 1][j] == 2) {
                    file_ << "O";
                }
                else if (size[i - 1][j] == 3) {
                    file_ << "D";
                }
                else {
                    file_ << " ";
                }

            }
            file_ << "-";
            file_ << endl;
        }
    }

    file_ << endl;
    file_.close();
}


class Entity {
public:
    string gates;
    int placement_x;
    int placement_y;
    //1-enfected , 2-healthy, 3-dead
    int securation;
    void create_entity(string gate, int x, int y, int sec, int round) {
        this->gates = gate;
        this->securation = sec;
        this->placement_x = x;
        this->placement_y = y;
        this->enfected_round = round;
    }

    int will_enfected = -1;
    int will_dead = 15;
    int isWeak = 0;
    int can_enfected = 0;
    int selected = 0;
    int enfected_round;
    int recover_counter = 30;
    int is_recovered = 0;
    int will_destroyed = 5;

};





map<int, Entity> CreateEntity(map<int, vector<string>> lines) {

    map<int, Entity> creates;
    string gates;
    int pos_x;
    int pos_y;

    Entity ent;

    for (int i = 0; i < lines.size(); i++) {

        if (lines[i].size() == 5) {
            pos_x = stoi(lines[i][3].substr(0, lines[i][3].find('x')));
            pos_y = stoi(lines[i][3].substr(lines[i][3].find('x') + 1, lines[i][3].size()));
            ent.create_entity(lines[i][2], pos_x, pos_y, 1, 0);
            creates[i] = ent;
        }

        else {
            pos_x = stoi(lines[i][3].substr(0, lines[i][3].find('x')));
            pos_y = stoi(lines[i][3].substr(lines[i][3].find('x') + 1, lines[i][3].size()));
            ent.create_entity(lines[i][2], pos_x, pos_y, 2, -1);
            creates[i] = ent;
        }
    }

    return creates;
}




void addFirstValuesForMatrix(Matrix m, map<int, Entity> ents) {

    for (int i = 0; i < ents.size(); i++) {
        m.addMatrix(ents[i].placement_x, ents[i].placement_y, ents[i].securation);
    }

}



void weakControl(map<int, Entity>& ents) {
    string gate;
    int Acount = 0, Bcount = 0, Ccount = 0, Dcount = 0;

    for (int i = 0; i < ents.size(); i++) {
        gate = ents[i].gates;
        Acount = 0, Bcount = 0, Ccount = 0, Dcount = 0;
        for (int j = 0; j < 5; j++) {

            if (gate[j] == 'A') {
                Acount++;
            }

            if (gate[j] == 'B') {
                Bcount++;
            }

            if (gate[j] == 'C') {
                Ccount++;
            }

            if (gate[j] == 'D') {
                Dcount++;
            }
        }

        if (Acount + Bcount + Ccount + Dcount >= 3) {
            ents[i].isWeak = 1;
        }

        if (Acount != 0 || Bcount != 0) {
            ents[i].can_enfected = 1;
        }
    }
}


void output_txt(int normal_count, int infected_count, int dead_count, int recovered_count, map<int, Entity> entities) {
    ofstream file_;
    file_.open("OUTPUT.txt");

    file_ << "Normal: " << normal_count << endl;
    file_ << "Infected: " << infected_count << endl;
    file_ << "Dead: " << dead_count << endl;
    file_ << "Recovered: " << recovered_count << endl;

    for (int i = 0; i < entities.size(); i++) {
        file_ << "entity " << i + 1 << " " << entities[i].placement_x << "x" << entities[i].placement_y << " ";

        if (entities[i].securation == 1) {
            file_ << "infected";
        }

        if (entities[i].securation == 2) {
            if (entities[i].is_recovered == 1) {
                file_ << "Recovered";
            }
            else {
                file_ << "Normal";
            }
        }

        if (entities[i].securation == 3) {
            file_ << "Dead";
        }

        file_ << endl;
    }

    file_.close();
}


void clear_turns_txt() {
    ofstream ofs;
    ofs.open("TURNS.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

int main()
{
    ifstream infile;
    infile.open("INPUT.txt");

    clear_turns_txt();

    int entity_count = 0;

    string data;
    vector<string>all_lines;
    if (infile.is_open()) {
        while (getline(infile, data)) {
            all_lines.push_back(data + '\n');
        }
    }
    infile.close();

    int size = stoi(all_lines[0].substr(all_lines[0].find(' ') + 1, all_lines[0].size() - 1));
    int turn_count = stoi(all_lines[1].substr(all_lines[1].find(' ') + 1, all_lines[1].size() - 1));

    vector<string> entities_lines;
    vector<string> turns_lines;


    for (int i = 0; i < all_lines.size(); i++) {
        string sub = all_lines[i].substr(0, all_lines[i].find(' '));
        if (sub == "entity") {
            entities_lines.push_back(all_lines[i]);
        }

        if (sub == "turn") {
            turns_lines.push_back(all_lines[i]);
        }
    }

    string word = "";
    map<int, vector<string>> entities_map;
    for (int i = 0; i < entities_lines.size(); i++) {
        word = "";
        for (int j = 0; j < entities_lines[i].size(); j++) {
            if (entities_lines[i][j] == ' ') {
                entities_map[i].push_back(word);
                word = "";
            }
            else {
                word = word + entities_lines[i][j];
            }
        }
        entities_map[i].push_back(word);
    }

    map<int, Entity> entities = CreateEntity(entities_map);
    weakControl(entities);

    map<int, vector<string>> turns_map;


    for (int i = 0; i < turns_lines.size(); i++) {
        word = "";
        for (int j = 0; j < turns_lines[i].size(); j++) {
            if (turns_lines[i][j] == ' ') {
                turns_map[i].push_back(word);
                word = "";
            }
            else {
                word = word + turns_lines[i][j];
            }
        }
        turns_map[i].push_back(word);
    }

    Matrix field;
    field.createMatrix(size);
    addFirstValuesForMatrix(field, entities);
    field.cleanMatrix();


    int mesafe_x = 0;
    int mesafe_y = 0;
    int enfected_count = 0;

    for (int i = 0; i < entities.size(); i++) {
        if (entities[i].securation == 1) {
            enfected_count++;
        }
    }



    int tempX;
    int tempY;

    int recovered_counter = 0, normal_counter = 0, infected_counter = 0, dead_counter = 0;


    for (int i = 0; i < turn_count; i++) {

        for (int j = 0; j < entities.size(); j++) {
            if (entities[j].isWeak && entities[j].securation == 1) {
                entities[j].will_dead--;
            }

            if (entities[j].will_dead == 1) {
                entities[j].securation = 3;
                entities[j].will_destroyed--;
                //5 raund hareketsiz kalacak ve 5.raund sonunda infected olacak.

            }

            //5.raund'a kadar enfekte gibi davrancak onu yaz
            if (entities[j].securation == 2) {

                for (int t = 0; t < entities.size(); t++) {

                    if (entities[t].securation == 1) {
                        mesafe_x = abs(entities[j].placement_x - entities[t].placement_x);
                        mesafe_y = abs(entities[j].placement_y - entities[t].placement_y);

                        if (mesafe_x <= 3 || mesafe_y <= 3) {

                            if (entities[j].selected == 0) {
                                entities[j].selected = 1;
                                entities[j].will_enfected = 3;
                            }

                            if (entities[j].selected != 0) {
                                entities[j].will_enfected--;
                            }

                            if (entities[j].will_enfected < 0 && entities[j].can_enfected == 1) {
                                entities[j].securation = 1;
                                if (entities[j].will_enfected == -1) {
                                    entities[j].enfected_round = i;
                                }
                            }
                        }
                    }

                    if (entities[t].securation == 3 && entities[t].will_destroyed >= 0) {
                        if (mesafe_x <= 3 || mesafe_y <= 3) {

                            if (entities[j].selected == 0) {
                                entities[j].selected = 1;
                                entities[j].will_enfected = 3;
                            }

                            if (entities[j].selected != 0) {
                                entities[j].will_enfected--;
                            }

                            if (entities[j].will_enfected < 0 && entities[j].can_enfected == 1) {
                                entities[j].securation = 1;
                                if (entities[j].will_enfected == -1) {
                                    entities[j].enfected_round = i;
                                }
                            }
                        }

                    }

                }

            }

            if (entities[j].securation == 1 && entities[j].isWeak == 0) {
                entities[j].recover_counter--;
                //recovered_counter++;
            }

            if (entities[j].securation == 1 && entities[j].recover_counter == 0) {
                entities[j].securation = 2;
                entities[j].can_enfected = 0;
                entities[j].is_recovered = 1;
                recovered_counter++;

            }

            if (entities[j].securation != 3) {
                tempX = stoi(turns_map[i][j + 2].substr(0, turns_map[i][j + 2].find('x')));
                tempY = stoi(turns_map[i][j + 2].substr(turns_map[i][j + 2].find('x') + 1, turns_map[i][j + 2].size()));

                entities[j].placement_x = tempX;
                entities[j].placement_y = tempY;

                field.addMatrix(tempX, tempY, entities[j].securation);

            }

            if (entities[j].securation == 3 && entities[j].will_destroyed > 0) {
                field.addMatrix(entities[j].placement_x, entities[j].placement_y, 3);
            }

        }
        field.showMatrix(i);
        field.cleanMatrix();
    }


    for (int i = 0; i < entities.size(); i++) {
        if (entities[i].securation == 1) {
            infected_counter++;
        }

        else if (entities[i].securation == 2) {
            if (entities[i].is_recovered != 1) {
                normal_counter++;
            }
        }

        else if (entities[i].securation == 3) {
            dead_counter++;
        }
    }


    output_txt(normal_counter, infected_counter, dead_counter, recovered_counter, entities);
}




