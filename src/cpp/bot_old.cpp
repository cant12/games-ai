#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

int ns = 8, ms = 8;

struct soldier {
    int x, y;
    bool colour;
};

struct cannon {
    int x1, y1, x2, y2;
    bool colour;
};

struct result {
    vector<string> plan;
    float payoff;
};

struct stagn {
    vector<vector<int>> con1, con2;
};

bool same(vector<vector<int>> config1, vector<vector<int>> config2) {
    int i, j;
    if (config1.size() != config2.size()) return false;
    for (i = 0; i < config1.size(); i++) {
        for (j = 0; j < config1[i].size(); j++)
            if (config1[i][j] != config2[i][j]) return false;
    }
    return true;
}

void print_cannon(cannon c) {
    cout << "(" << c.x1 << "," << c.y1 << ") ";
    cout << "(" << (c.x1 + c.x2) / 2 << "," << (c.y1 + c.y2) / 2 << ") ";
    cout << "(" << c.x2 << "," << c.y2 << ") " << endl;
}

vector<cannon> if_cannon(soldier sold, vector<vector<int>> &config) {
    vector<cannon> ans;
    int xc = sold.x, yc = sold.y, col;
    cannon temp;
    if (sold.colour)
        col = 1;
    else
        col = -1;
    if (xc != 0 && xc != ms - 1 && config[xc - 1][yc] == col && config[xc + 1][yc] == col) {
        temp.x1 = xc - 1;
        temp.y1 = yc;
        temp.x2 = xc + 1;
        temp.y2 = yc;
        temp.colour = sold.colour;
        ans.push_back(temp);
    }
    if (yc != 0 && yc != ns - 1 && config[xc][yc - 1] == col && config[xc][yc + 1] == col) {
        temp.x1 = xc;
        temp.y1 = yc - 1;
        temp.x2 = xc;
        temp.y2 = yc + 1;
        temp.colour = sold.colour;
        ans.push_back(temp);
    }
    if (yc != 0 && yc != ns - 1 && xc != 0 && xc != ms - 1 && config[xc - 1][yc - 1] == col &&
        config[xc + 1][yc + 1] == col) {
        temp.x1 = xc - 1;
        temp.y1 = yc - 1;
        temp.x2 = xc + 1;
        temp.y2 = yc + 1;
        temp.colour = sold.colour;
        ans.push_back(temp);
    }
    if (yc != 0 && yc != ns - 1 && xc != 0 && xc != ms - 1 && config[xc - 1][yc + 1] == col &&
        config[xc + 1][yc - 1] == col) {
        temp.x1 = xc - 1;
        temp.y1 = yc + 1;
        temp.x2 = xc + 1;
        temp.y2 = yc - 1;
        temp.colour = sold.colour;
        ans.push_back(temp);
    }
    return ans;
}

vector<cannon> update_cannons(vector<soldier> solds, vector<vector<int>> &config) {
    vector<cannon> ans, temp;
    int i;
    for (i = 0; i < solds.size(); i++) {
        temp = if_cannon(solds[i], config);
        ans.insert(ans.end(), temp.begin(), temp.end());
    }
    return ans;
}

vector<string> sold_moves(soldier s, vector<vector<int>> &config) {
    int col;
    vector<string> ans;
    string temp;
    if (s.colour == 1)
        col = 1;
    else
        col = -1;
    if (s.y + col >= 0 && s.y + col <= ns - 1 && config[s.x][s.y + col] != col) {
        temp = "S ";
        temp += s.x + 48;
        temp += " ";
        temp += s.y + 48;
        temp += " M ";
        temp += s.x + 48;
        temp += " ";
        temp += s.y + 48 + col;
        ans.push_back(temp);
    }
    if (s.y + col >= 0 && s.y + col <= ns - 1 && s.x < ms - 1 && config[s.x + 1][s.y + col] != col) {
        temp = "S ";
        temp += s.x + 48;
        temp += " ";
        temp += s.y + 48;
        temp += " M ";
        temp += s.x + 49;
        temp += " ";
        temp += s.y + 48 + col;
        ans.push_back(temp);
    }
    if (s.y + col >= 0 && s.y + col <= ns - 1 && s.x > 0 && config[s.x - 1][s.y + col] != col) {
        temp = "S ";
        temp += s.x + 48;
        temp += " ";
        temp += s.y + 48;
        temp += " M ";
        temp += s.x + 47;
        temp += " ";
        temp += s.y + 48 + col;
        ans.push_back(temp);
    }
    if (s.x > 0 && (config[s.x - 1][s.y] == -col || config[s.x - 1][s.y] == -2 * col)) {
        temp = "S ";
        temp += s.x + 48;
        temp += " ";
        temp += s.y + 48;
        temp += " M ";
        temp += s.x + 47;
        temp += " ";
        temp += s.y + 48;
        ans.push_back(temp);
    }
    if (s.x < ms - 1 && (config[s.x + 1][s.y] == -col || config[s.x + 1][s.y] == -2 * col)) {
        temp = "S ";
        temp += s.x + 48;
        temp += " ";
        temp += s.y + 48;
        temp += " M ";
        temp += s.x + 49;
        temp += " ";
        temp += s.y + 48;
        ans.push_back(temp);
    }
    if ((s.y + col >= 0 && s.y + col <= ns - 1 && config[s.x][s.y + col] == -col) ||
        (s.y + col >= 0 && s.y + col <= ns - 1 && s.x < ms - 1 && config[s.x + 1][s.y + col] == -col) ||
        (s.y + col >= 0 && s.y + col <= ns - 1 && s.x > 0 && config[s.x - 1][s.y + col] == -col) ||
        (s.x < ms - 1 && config[s.x + 1][s.y] == -col) || (s.x > 0 && config[s.x - 1][s.y] == -col)) {
        if (s.y - 2 * col <= ns - 1 && s.y - 2 * col >= 0 && config[s.x][s.y - 2 * col] != col &&
            config[s.x][s.y - 2 * col] != 2 * col) {
            temp = "S ";
            temp += s.x + 48;
            temp += " ";
            temp += s.y + 48;
            temp += " M ";
            temp += s.x + 48;
            temp += " ";
            temp += s.y + 48 - 2 * col;
            ans.push_back(temp);
        }
        if (s.y - 2 * col <= ns - 1 && s.y - 2 * col >= 0 && s.x <= ms - 3 && config[s.x + 2][s.y - 2 * col] != col &&
            config[s.x + 2][s.y - 2 * col] != 2 * col) {
            temp = "S ";
            temp += s.x + 48;
            temp += " ";
            temp += s.y + 48;
            temp += " M ";
            temp += s.x + 50;
            temp += " ";
            temp += s.y + 48 - 2 * col;
            ans.push_back(temp);
        }
        if (s.y - 2 * col <= ns - 1 && s.y - 2 * col >= 0 && s.x >= 2 && config[s.x - 2][s.y - 2 * col] != col &&
            config[s.x - 2][s.y - 2 * col] != 2 * col) {
            temp = "S ";
            temp += s.x + 48;
            temp += " ";
            temp += s.y + 48;
            temp += " M ";
            temp += s.x + 46;
            temp += " ";
            temp += s.y + 48 - 2 * col;
            ans.push_back(temp);
        }
    }
    return ans;
}

vector<string> cannon_moves(cannon c, vector<vector<int>> &config, bool mode) {
    vector<string> ans;
    string temp = "S ", temp1, temp2;
    int col;
    if (c.colour == 1)
        col = 1;
    else
        col = -1;
    temp += c.x1 + 48;
    temp += " ";
    temp += c.y1 + 48;
    int hor = c.x2 - c.x1, ver = c.y2 - c.y1;
    if (c.x2 + hor / 2 < ms && c.x2 + hor / 2 >= 0 && c.y2 + ver / 2 < ns && c.y2 + ver / 2 >= 0 &&
        config[c.x2 + hor / 2][c.y2 + ver / 2] == 0) {
        if (c.x2 + hor < ms && c.x2 + hor >= 0 && c.y2 + ver < ns && c.y2 + ver >= 0 &&
            (config[c.x2 + hor][c.y2 + ver] == -col || config[c.x2 + hor][c.y2 + ver] == -2 * col ||
             config[c.x2 + hor][c.y2 + ver] == 0)) {
            temp1 = " B ";
            temp1 += c.x2 + hor + 48;
            temp1 += " ";
            temp1 += c.y2 + ver + 48;
            temp1 = temp + temp1;
            ans.push_back(temp1);
            if (mode) {
                temp2 = "S ";
                temp2 += c.x2 + 48;
                temp2 += " ";
                temp2 += c.y2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
                temp2 = "S ";
                temp2 += (c.x2 + c.x1) / 2 + 48;
                temp2 += " ";
                temp2 += (c.y2 + c.y1) / 2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
            }
        }
        if (c.x2 + 3 * hor / 2 < ms && c.x2 + 3 * hor / 2 >= 0 && c.y2 + 3 * ver / 2 < ns && c.y2 + 3 * ver / 2 >= 0 &&
            (config[c.x2 + 3 * hor / 2][c.y2 + 3 * ver / 2] == -col ||
             config[c.x2 + 3 * hor / 2][c.y2 + 3 * ver / 2] == -2 * col ||
             config[c.x2 + 3 * hor / 2][c.y2 + 3 * ver / 2] == 0)) {
            temp1 = " B ";
            temp1 += c.x2 + 3 * hor / 2 + 48;
            temp1 += " ";
            temp1 += c.y2 + 3 * ver / 2 + 48;
            temp1 = temp + temp1;
            ans.push_back(temp1);
            if (mode) {
                temp2 = "S ";
                temp2 += c.x2 + 48;
                temp2 += " ";
                temp2 += c.y2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
                temp2 = "S ";
                temp2 += (c.x2 + c.x1) / 2 + 48;
                temp2 += " ";
                temp2 += (c.y2 + c.y1) / 2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
            }
        }
    }
    if (c.x2 + hor / 2 < ms && c.x2 + hor / 2 >= 0 && c.y2 + ver / 2 < ns && c.y2 + ver / 2 >= 0 &&
        config[c.x2 + hor / 2][c.y2 + ver / 2] == 0) {
        temp1 = " M ";
        temp1 += c.x2 + hor / 2 + 48;
        temp1 += " ";
        temp1 += c.y2 + ver / 2 + 48;
        temp1 = temp + temp1;
        ans.push_back(temp1);
    }
    if (c.x1 - hor / 2 < ms && c.x1 - hor / 2 >= 0 && c.y1 - ver / 2 < ns && c.y1 - ver / 2 >= 0 &&
        config[c.x1 - hor / 2][c.y1 - ver / 2] == 0) {
        if (c.x1 - hor < ms && c.x1 - hor >= 0 && c.y1 - ver < ns && c.y1 - ver >= 0 &&
            (config[c.x1 - hor][c.y1 - ver] == -col || config[c.x1 - hor][c.y1 - ver] == -2 * col ||
             config[c.x1 - hor][c.y1 - ver] == 0)) {
            temp1 = " B ";
            temp1 += c.x1 - hor + 48;
            temp1 += " ";
            temp1 += c.y1 - ver + 48;
            temp1 = temp + temp1;
            ans.push_back(temp1);
            if (mode) {
                temp2 = "S ";
                temp2 += c.x2 + 48;
                temp2 += " ";
                temp2 += c.y2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
                temp2 = "S ";
                temp2 += (c.x2 + c.x1) / 2 + 48;
                temp2 += " ";
                temp2 += (c.y2 + c.y1) / 2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
            }
        }
        if (c.x1 - 3 * hor / 2 < ms && c.x1 - 3 * hor / 2 >= 0 && c.y1 - 3 * ver / 2 < ns && c.y1 - 3 * ver / 2 >= 0 &&
            (config[c.x1 - 3 * hor / 2][c.y1 - 3 * ver / 2] == -col ||
             config[c.x1 - 3 * hor / 2][c.y1 - 3 * ver / 2] == -2 * col ||
             config[c.x1 - 3 * hor / 2][c.y1 - 3 * ver / 2] == 0)) {
            temp1 = " B ";
            temp1 += c.x1 - 3 * hor / 2 + 48;
            temp1 += " ";
            temp1 += c.y1 - 3 * ver / 2 + 48;
            temp1 = temp + temp1;
            ans.push_back(temp1);
            if (mode) {
                temp2 = "S ";
                temp2 += c.x2 + 48;
                temp2 += " ";
                temp2 += c.y2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
                temp2 = "S ";
                temp2 += (c.x2 + c.x1) / 2 + 48;
                temp2 += " ";
                temp2 += (c.y2 + c.y1) / 2 + 48;
                temp2 += temp1.substr(5, 6);
                ans.push_back(temp2);
            }
        }
    }
    if (c.x1 - hor / 2 < ms && c.x1 - hor / 2 >= 0 && c.y1 - ver / 2 < ns && c.y1 - ver / 2 >= 0 &&
        config[c.x1 - hor / 2][c.y1 - ver / 2] == 0) {
        temp1 = "S ";
        temp1 += c.x2 + 48;
        temp1 += " ";
        temp1 += c.y2 + 48;
        temp1 += " M ";
        temp1 += c.x1 - hor / 2 + 48;
        temp1 += " ";
        temp1 += c.y1 - ver / 2 + 48;
        ans.push_back(temp1);
    }
    return ans;
}

class board {
  public:
    vector<soldier> blacks, whites;
    vector<cannon> blackc, whitec;
    bool wtown[5], btown[5], player;
    vector<vector<int>> config;
    board() {
        int i, j;
        soldier temps;
        vector<int> t;
        player = 0;
        for (i = 0; i < ms; i++) {
            for (j = 0, t.clear(); j < ns; j++) {
                t.push_back(0);
            }
            config.push_back(t);
        }
        for (i = 1; i < ms; i += 2) {
            for (j = 0; j < 3; j++) {
                temps.x = i;
                temps.y = j;
                temps.colour = 1;
                whites.push_back(temps);
                config[i][j] = 1;
            }
        }
        for (i = 0; i < ms; i += 2) {
            for (j = ns - 1; j > ns - 4; j--) {
                temps.x = i;
                temps.y = j;
                temps.colour = 0;
                blacks.push_back(temps);
                config[i][j] = -1;
            }
        }
        for (i = 0; i < ms / 2; i++) {
            wtown[i] = 1;
            btown[i] = 1;
            config[2 * i][0] = 2;
            config[2 * i + 1][ns - 1] = -2;
        }
        blackc = update_cannons(blacks, config);
        whitec = update_cannons(whites, config);
    }

    board(vector<vector<int>> config, bool player) {
        this->config = config;
        this->player = player;

        soldier temps;
        for (int i = 0; i < config.size(); i++) {
            for (int j = 0; j < config[0].size(); j++) {
                if (config[i][j] == 1) {
                    temps.x = i;
                    temps.y = j;
                    temps.colour = 1;
                    whites.push_back(temps);
                } else if (config[i][j] == -1) {
                    temps.x = i;
                    temps.y = j;
                    temps.colour = 0;
                    blacks.push_back(temps);
                }
            }
        }

        for (int i = 0; i < ms / 2; i++) {
            if (config[2 * i][0] == 2) {
                wtown[i] = 1;
            } else {
                wtown[i] = 0;
            }
            if (config[2 * i + 1][ns - 1] == -2) {
                btown[i] = 1;
            } else {
                btown[i] = 0;
            }
        }

        blackc = update_cannons(blacks, config);
        whitec = update_cannons(whites, config);
    }

    vector<string> valid_moves(bool mode) {
        int i;
        vector<string> ans, temp;
        if (player) {
            for (i = 0; i < whites.size(); i++) {
                temp = sold_moves(whites[i], config);
                ans.insert(ans.end(), temp.begin(), temp.end());
            }
            for (i = 0; i < whitec.size(); i++) {
                temp = cannon_moves(whitec[i], config, mode);
                ans.insert(ans.end(), temp.begin(), temp.end());
            }
        }
        if (!player) {
            for (i = 0; i < blacks.size(); i++) {
                temp = sold_moves(blacks[i], config);
                ans.insert(ans.end(), temp.begin(), temp.end());
            }
            for (i = 0; i < blackc.size(); i++) {
                temp = cannon_moves(blackc[i], config, mode);
                ans.insert(ans.end(), temp.begin(), temp.end());
            }
        }
        return ans;
    }

    bool make_move(string move) {
        int i;
        // vector<string> moves = this->valid_moves(1);
        // for(i=0;i<moves.size();i++)
        // {
        //   if(moves[i]==move)
        //     break;
        // }
        // if(i==moves.size())
        //   return false;
        if (move[6] == 'M') {
            config[move[2] - 48][move[4] - 48] = 0;
            if (player) {
                for (i = 0; i < whites.size(); i++) {
                    if (whites[i].x == move[2] - 48 && whites[i].y == move[4] - 48) {
                        whites[i].x = move[8] - 48;
                        whites[i].y = move[10] - 48;
                        break;
                    }
                }
                {
                    for (i = 0; i < blacks.size() && config[move[8] - 48][move[10] - 48] == -1; i++)
                        if (blacks[i].x == move[8] - 48 && blacks[i].y == move[10] - 48) {
                            blacks.erase(blacks.begin() + i);
                            break;
                        }
                }
                config[move[8] - 48][move[10] - 48] = 1;
                whitec = update_cannons(whites, config);
                blackc = update_cannons(blacks, config);
            } else {
                for (i = 0; i < blacks.size(); i++) {
                    if (blacks[i].x == move[2] - 48 && blacks[i].y == move[4] - 48) {
                        blacks[i].x = move[8] - 48;
                        blacks[i].y = move[10] - 48;
                        break;
                    }
                }
                for (i = 0; i < whites.size() && config[move[8] - 48][move[10] - 48] == 1; i++) {
                    if (whites[i].x == move[8] - 48 && whites[i].y == move[10] - 48) {
                        whites.erase(whites.begin() + i);
                        break;
                    }
                }
                config[move[8] - 48][move[10] - 48] = -1;
                blackc = update_cannons(blacks, config);
                whitec = update_cannons(whites, config);
            }
        }

        if (move[6] == 'B') {
            if (player) {
                for (i = 0; i < blacks.size() && config[move[8] - 48][move[10] - 48] == -1; i++) {
                    if (blacks[i].x == move[8] - 48 && blacks[i].y == move[10] - 48) {
                        blacks.erase(blacks.begin() + i);
                        break;
                    }
                }
                config[move[8] - 48][move[10] - 48] = 0;
                blackc = update_cannons(blacks, config);
            } else {
                for (i = 0; i < whites.size() && config[move[8] - 48][move[10] - 48] == 1; i++) {
                    if (whites[i].x == move[8] - 48 && whites[i].y == move[10] - 48) {
                        whites.erase(whites.begin() + i);
                        break;
                    }
                }
                config[move[8] - 48][move[10] - 48] = 0;
                whitec = update_cannons(whites, config);
            }
        }

        for (i = 0; i < (ms / 2); i++) {
            if (move[8] - 48 == 2 * i && move[10] - 48 == 0) wtown[i] = 0;
            if (move[8] - 48 == 2 * i + 1 && move[10] - 48 == ns - 1) btown[i] = 0;
        }
        player = !player;
        return true;
    }

    bool game_over() {
        int i, w, b;
        for (i = 0, w = 0, b = 0; i < ms / 2; i++) {
            if (wtown[i] == 1) w++;
            if (btown[i] == 1) b++;
        }
        // cerr << b << " " << w << endl;
        if (b == (ms / 2) - 2 || w == (ms / 2) - 2) return true;
        return false;
    }

    void print_board() {
        int i, j;
        for (i = 0; i < ms; i++) {
            for (j = 0; j < ns; j++) {
                if (config[j][i] == 2) cerr << "T ";
                if (config[j][i] == 1) cerr << "W ";
                if (config[j][i] == 0) cerr << "0 ";
                if (config[j][i] == -1) cerr << "B ";
                if (config[j][i] == -2) cerr << "U ";
            }
            cerr << endl;
        }
    }

    float utility(bool stale) {
        int i, w, b;
        float ans;
        int xs = whites.size(), ys = blacks.size();

        for (i = 0, w = 0, b = 0; i < ms / 2; i++) {
            if (wtown[i] == 1) w++;
            if (btown[i] == 1) b++;
        }

        if (b == (ms / 2) && w == (ms / 2)) ans = 5;
        if (b == (ms / 2) - 1 && w == (ms / 2) - 1) ans = 5;
        if (b == (ms / 2) - 1 && w == (ms / 2)) ans = 7;
        if (b == (ms / 2) && w == (ms / 2) - 1) ans = 3;
        if (b == (ms / 2) - 2 && w == (ms / 2) - 1) ans = 8;
        if (b == (ms / 2) - 1 && w == (ms / 2) - 2) ans = 2;
        if (b == (ms / 2) && w == (ms / 2) - 2) ans = 0;
        if (b == (ms / 2) - 2 && w == (ms / 2)) ans = 10;

        ans += ((float)(xs - ys)) / 100;

        int a = whitec.size();
        int bc = blackc.size();
        int dc;
        for (i = 0, dc = 0; i < a; i++) {
            if (whitec[i].x1 != whitec[i].x2 && whitec[i].y1 != whitec[i].y2) {
                if ((whitec[i].x1 == 0 && whitec[i].y2 == 0) || (whitec[i].x2 == 0 && whitec[i].y1 == 0) ||
                    (whitec[i].x1 == ms - 1 && whitec[i].y2 == 0) || (whitec[i].x2 == ms - 1 && whitec[i].y1 == 0))
                    dc--;
                else
                    dc++;
            }
        }
        for (i = 0; i < bc; i++) {
            if (blackc[i].x1 != blackc[i].x2 && blackc[i].y1 != blackc[i].y2) {
                if ((blackc[i].x1 == 0 && blackc[i].y2 == ns - 1) || (blackc[i].x2 == 0 && blackc[i].y1 == ns - 1) ||
                    (blackc[i].x1 == ms - 1 && blackc[i].y2 == ns - 1) ||
                    (blackc[i].x2 == ms - 1 && blackc[i].y1 == ns - 1))
                    dc++;
                else
                    dc--;
            }
        }
        ans += 0.001 * dc + 0.001 * (a - bc);

        int sum = 0;

        for (i = 0; i < xs; i++) sum += whites[i].y - ns / 2;

        for (i = 0; i < ys; i++) sum += blacks[i].y - ns / 2;

        float temp = ((float)(sum)) / ((float)(xs + ys));

        // if(ms!=10 || xs<=5 || ys<=5)
        ans += 0.01 * temp;

        return ans;
    }

    result best_move(int look, float alpha, float beta, float t, bool f, vector<string> killer, bool kill,
                     vector<vector<int>> tcon, bool stag) {
        // //cerr << ns << " " << ms << endl;
        clock_t starttime = clock();
        board state = *this;
        result ans, temp;
        int i, n;
        string temps = "init";
        if (look == 0 || state.game_over()) {
            // int a = state.whites.size(), b = state.blacks.size();
            //  if(a+b<=12)
            //  {
            //    ans.payoff = state.utility();
            //    return ans;
            //  }
            ans.payoff = state.utility(0);
            ////cerr <<"hello"<< endl;
            return ans;
        }
        vector<string> actions = state.valid_moves(0), y;
        n = actions.size();
        if (n == 0) {
            // int a = state.whites.size(), b = state.blacks.size();
            //  if(a+b<=12)
            //  {
            //    ans.payoff = state.utility();
            //    return ans;
            //  }
            ans.payoff = state.utility(1);
            // cerr <<"yo"  << endl;
            return ans;
        }
        if (player) {
            if (kill && killer.size() > 0) {
                actions.insert(actions.begin(), killer[0]);
                killer.erase(killer.begin());
                n++;
            }
            for (i = 0, temp.payoff = alpha; i < n; i++) {
                if (f == 1 && ((float)(clock() - starttime) / CLOCKS_PER_SEC >= t)) {
                    y = temp.plan;
                    y.insert(y.begin(), temps);
                    ans.payoff = temp.payoff;
                    ans.plan = y;
                    // cerr <<"pls" <<y.size() << endl;
                    return ans;
                }
                if ((i == 0 || actions[i] != actions[0]) && state.make_move(actions[i])) {
                    if (!stag || !same(state.config, tcon)) {
                        result x = state.best_move(look - 1, temp.payoff, beta,
                                                   t - (float)(clock() - starttime) / CLOCKS_PER_SEC, 0, killer,
                                                   i == 0 && kill, tcon, 0);
                        if (temp.payoff < x.payoff || (temp.payoff == x.payoff && actions[i][6] == 'B' &&
                                                       config[actions[i][8] - 48][actions[i][10] - 48] > 0)) {
                            temp = x;
                            temps = actions[i];
                            if (temp.payoff >= beta) break;
                        }
                    }
                }
                state = *this;
            }
            y = temp.plan;
            y.insert(y.begin(), temps);
            ans.payoff = temp.payoff;
            if (temps == "init") ans.payoff -= 1;
            ans.plan = y;
        } else {
            if (kill && killer.size() > 0) {
                actions.insert(actions.begin(), killer[0]);
                killer.erase(killer.begin());
                n++;
            }
            for (i = 0, temp.payoff = beta; i < n; i++) {
                if (f == 1 && ((float)(clock() - starttime) / CLOCKS_PER_SEC >= t)) {
                    y = temp.plan;
                    y.insert(y.begin(), temps);
                    ans.payoff = temp.payoff;
                    ans.plan = y;
                    // cerr <<"wtf" <<y.size() << endl;
                    return ans;
                }
                if ((i == 0 || actions[i] != actions[0]) && state.make_move(actions[i])) {
                    if (!stag || !same(state.config, tcon)) {
                        result x = state.best_move(look - 1, alpha, temp.payoff,
                                                   t - (float)(clock() - starttime) / CLOCKS_PER_SEC, 0, killer,
                                                   i == 0 && kill, tcon, 0);
                        if (temp.payoff > x.payoff || (temp.payoff == x.payoff && actions[i][6] == 'B' &&
                                                       config[actions[i][8] - 48][actions[i][10] - 48] < 0)) {
                            temp = x;
                            temps = actions[i];
                            if (temp.payoff <= alpha) break;
                        }
                    }
                }
                state = *this;
            }
            y = temp.plan;
            y.insert(y.begin(), temps);
            ans.payoff = temp.payoff;
            if (temps == "init") ans.payoff += 1;
            ans.plan = y;
        }
        // cerr <<"last" <<ans.plan.size() << endl;
        return ans;
    }

    // time logic need to be tested
    result idd(vector<vector<int>> tcon, bool stag) {
        // cerr << "idd start" << "\n";
        clock_t starttime = clock();
        result ans, x, temp_ans;
        int i, s1 = whites.size(), s2 = blacks.size(), n = s1 + s2, j = 4;
        float t = 2, temp;
        vector<string> killer;
        //  if(time_left<=20)
        //  t = time_left/n ;
        if (n <= 16) {
            j = 5;
            if (ms == 10 && ns == 10) j = 6;
        }
        if (player)
            ans.payoff = -0.12;
        else
            ans.payoff = 10.12;
        for (i = j, killer.clear();; i++) {
            ////cerr << ns << " " << ms << " " << i << endl;
            temp = (float)(clock() - starttime) / CLOCKS_PER_SEC;
            // need to be changed to use the bigger one after strict time cut
            // can be commented
            if (temp >= t) {
                // if ((player && temp_ans.payoff >= ans.payoff) ||
                //     (!player && temp_ans.payoff <= ans.payoff)) {
                //     i--;
                //     ans = temp_ans;
                // }
                // cerr << i-1 << endl;
                // for(int k =0; k<ans.plan.size(); k++)
                //   cerr << ans.plan[k] << endl;
                // cerr << endl;
                break;
            }
            if (i == j) {
                x = best_move(i, -0.12, 10.12, t - temp, 0, killer, 0, tcon, stag);
                killer = x.plan;
                // cerr << i << " " << x.payoff << endl;
            } else {
                x = best_move(i, -0.12, 10.12, t - temp, 1, killer, 1, tcon, stag);
                // cerr << i << " " << x.payoff << endl;
            }
            if (player) {
                if (x.payoff != ans.payoff) {
                    temp_ans = ans;
                    ans = x;
                    killer = x.plan;
                    // cerr << i << endl;
                }
            } else {
                if (x.payoff != ans.payoff) {
                    temp_ans = ans;
                    ans = x;
                    killer = x.plan;
                    // cerr << i << endl;
                }
            }
        }
        return ans;
    }
};

class Bot {
  public:
    string find_best_move(vector<vector<int>> config, bool player) {
        board *game = new board(config, player);
        vector<vector<int>> tcon;

        result x = game->idd(tcon, 0);
        vector<string> strategy = x.plan;
        cout << strategy.size() << endl;

        string move = strategy[0];
        cout << move << endl;

        game->make_move(move);
        return move;
    }
};

string invertMove(string move) {
    char temp;

    temp = move[2];
    move[2] = move[4];
    move[4] = temp;

    temp = move[8];
    move[8] = move[10];
    move[10] = temp;

    return move;
}

// interface with c to be used as a python library
extern "C" {
    void *new_bot_cannon() { return new Bot(); }

    void find_best_move_cannon(Bot * bot, int **gameStateArr, int numRows, int numColumns, int ***forbiddenStatesArr,
                               int numForbiddenStates, bool isBlackTurn, char *responseMoveBuffer) {
        // create c++ vector from array
        vector<vector<int>> gameState;
        for (int i = 0; i < numColumns; i++) {
            vector<int> column;
            for (int j = 0; j < numRows; j++) {
                column.push_back(gameStateArr[j][i]);
            }
            gameState.push_back(column);
        }

        ns = numRows;
        ms = numColumns;

        string responseMove = invertMove(bot->find_best_move(gameState, !isBlackTurn));
        strcpy(responseMoveBuffer, responseMove.c_str());
    }
}
