//
// Created by ASUS on 27.11.2018.
//

#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;
typedef pair<int, int> cell;
typedef pair<cell, cell> s_pair;
typedef pair<int, pair<cell, cell>> e_pair;


cell& find_parent(vector<vector<cell>>& parent, cell& c){
    //cout<< "find parent" << endl;
    int x = c.first;
    int y = c.second;
    //cout << x << ", " << y << endl;
    if(parent[x][y].first == -1){
        return c;
    }
    return find_parent(parent, parent[x][y]);
}
void dfs(vector<vector<int>> &gridland, vector<vector<vector<cell>>>& adj_list,vector<vector<vector<cell>>>& mst_parents,
        vector<vector<vector<int>>>& mst_height, vector<vector<int>>& depth,  cell& c){
    int d =depth[c.first][c.second] + 1;
    //cout << d << endl;
    //cout << c.first << ", " << c.second << endl;
    for(auto& c_low : adj_list[c.first][c.second]){
        if(depth[c_low.first][c_low.second] != -1)
            continue;
        depth[c_low.first][c_low.second] = d;
        mst_parents[c_low.first][c_low.second].push_back(c);
        mst_height[c_low.first][c_low.second].push_back(abs(gridland[c_low.first][c_low.second] - gridland[c.first][c.second]));
        int i = 0;
        int k = d;
        while(k > 1){
            k /= 2;
            cell parent = mst_parents[c_low.first][c_low.second][i];
            mst_parents[c_low.first][c_low.second].push_back(mst_parents[parent.first][parent.second][i]);
            mst_height[c_low.first][c_low.second].push_back(max(mst_height[c_low.first][c_low.second][i], mst_height[parent.first][parent.second][i]));
            i++;
            //cout<< i << " - " << d << endl;
        }
        dfs(gridland, adj_list, mst_parents, mst_height, depth, c_low);
        //cout<< "dfs" << endl;

    }
}
int find_lca(const vector<vector<vector<cell>>>& mst_parents, const vector<vector<vector<int>>>& mst_height, const vector<vector<int>>& depth,  cell &c1, cell &c2){
    //cout<< "find lca" << endl;

    int d1 = depth[c1.first][c1.second];
    int d2 = depth[c2.first][c2.second];
    //cout << "depths: " << d1 << ", " << d2 << endl;
    int a = 0;
    if(d1 > d2){
        int diff = d1 - d2;
        while(diff > 0){
            int m = diff;
            int k = 0;
            while(m > 0){
                m /= 2;
                k++;
            }
            diff -= pow(2, k - 1);
            a = max(a, mst_height[c1.first][c1.second][k - 1]);
            c1 = mst_parents[c1.first][c1.second][k - 1];

        }
    }
    else if(d2 > d1){
        int diff = d2 - d1;
        //cout << diff << endl;
        while(diff > 0){
            //cout << "diff = " <<  diff << endl;
            int m = diff;
            int k = 0;
            while(m > 0){
                m /= 2;
                k++;
            }
            //cout << "k = " <<  k << endl;

            diff -= pow(2, k - 1);
            a = max(a, mst_height[c2.first][c2.second][k - 1]);
            c2 = mst_parents[c2.first][c2.second][k - 1];
            //cout << c2.first << ", " << c2.second << endl;
        }
    }
    if(c1.first == c2.first && c1.second == c2.second)
        return a;

    while(mst_parents[c2.first][c2.second][0].first != mst_parents[c1.first][c1.second][0].first ||
          mst_parents[c2.first][c2.second][0].second != mst_parents[c1.first][c1.second][0].second){
        int par = 0;
        while(mst_parents[c2.first][c2.second][par].first != mst_parents[c1.first][c1.second][par].first ||
              mst_parents[c2.first][c2.second][par].second != mst_parents[c1.first][c1.second][par].second){
            par++;
        }
        par--;
        a = max(a, mst_height[c1.first][c1.second][par]);
        a = max(a, mst_height[c2.first][c2.second][par]);
        c1 = mst_parents[c1.first][c1.second][par];
        c2 = mst_parents[c2.first][c2.second][par];
        //cout << depth[c1.first][c1.second] << ", " << depth[c2.first][c2.second] << endl;
    }
    a = max(a, mst_height[c1.first][c1.second][0]);
    a = max(a, mst_height[c2.first][c2.second][0]);
    return a;
    //return mst_parents[c2.first][c2.second][0];



}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        cout << argc << endl;
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }
    cout <<  argv[0] << " " <<argv[1] << " " << argv[2] << endl;
    // here, perform the input operation. in other words,
    // read the file named <argv[1]>
    ifstream infile(argv[1]);
    int N;
    int M;
    infile >> N >> M;
    //cout << N << M << endl;
    vector<vector<int>> gridland(N);
    vector<vector<cell>> parent(N, vector<cell>(M, cell(-1, -1)));
    vector<vector<int>> rank(N, vector<int>(M, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            //cout << parent[i][j].first<< ", " << parent[i][j].second << "\t";
        }
        //cout << endl;
    }
    vector<vector<vector<cell>>> adj_list(N);
    vector<vector<vector<cell>>> mst_parents(N);
    vector<vector<vector<int>>> mst_height(N);
    vector<vector<int>> depth(N, vector<int>(M, -1));
    vector<e_pair> edges;

    for (int i = 0; i < N; ++i) {
        gridland[i].resize(M);
        adj_list[i].resize(M);
        mst_parents[i].resize(M);
        mst_height[i].resize(M);
        //depth[i].resize(M);
        for (int j = 0; j < M; ++j) {
            //cout << i << "," << j << "; ";
            int x;
            infile >> x;
            gridland[i][j] = x;
            //cout << gridland[i][j] << "\t";
            if(i > 0){
                s_pair p(cell(i,j), cell(i-1, j));
                int y = gridland[i - 1][j];
                e_pair e(abs(y - x), p);
                edges.push_back(e);
            }
            if(j > 0){
                s_pair p(cell(i, j), cell(i, j-1));
                int y = gridland[i][j - 1];
                e_pair e(abs(y - x), p);
                edges.push_back(e);

            }
        }
        //cout << endl;
    }
    sort(edges.begin(), edges.end());
    int k;
    infile >> k;
    int j = 1;
    for(int i = 0; j < M * N; i++){
        //cout << i << "," << j << "; ";
        cell c1 = edges[i].second.first;
        cell c2 = edges[i].second.second;
        cell c1_p = find_parent(parent, c1);
        cell c2_p = find_parent(parent, c2);
        if(c1_p != c2_p){
            //cout << c1.first << ", " << c1.second << "; " << c2.first << ", " << c2.second<< endl;
            j++;
            if(rank[c1_p.first][c1_p.second] > rank[c2_p.first][c2_p.second])
                parent[c2_p.first][c2_p.second] = c1_p;
            else
                parent[c1_p.first][c1_p.second] = c2_p;
            if(rank[c1_p.first][c1_p.second] == rank[c2_p.first][c2_p.second])
                rank[c2_p.first][c2_p.second]++;
            adj_list[c1.first][c1.second].push_back(c2);
            adj_list[c2.first][c2.second].push_back(c1);
        }


    }
    //cout << j << endl;
    //cout << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            //cout << parent[i][j].first<< ", " << parent[i][j].second << "\t";
        }
        //cout << endl;
    }
    cell root(0, 0);
    //cell root = find_parent(parent, hop);
    //cout << root.first <<"Baris" << root.second << endl;
    depth[root.first][root.second] = 0;
    dfs(gridland, adj_list, mst_parents, mst_height, depth, root);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            //cout << mst_parents[i][j].size() << "\t";
        }
        //cout << endl;
    }
    //cout << endl;
    ofstream output(argv[2]);

    for (int i = 0; i < k; ++i) {
        int x;
        int y;
        int z;
        int t;
        infile >> x;
        infile >> y;
        cell sq1(x - 1, y - 1);
        infile >> z;
        infile >> t;

        cell sq2(z - 1, t - 1);

        int sayi = find_lca(mst_parents, mst_height, depth, sq1, sq2);
        sq1 = cell(x - 1, y - 1);
        sq2 = cell(z - 1, t - 1);



        /*
        int sayi = 0;
        while(!(sq1.first == lca.first && sq1.second == lca.second)){
            //cout << sq1.first << "," << sq1.second << endl;
            cell parr = mst_parents[sq1.first][sq1.second][0];
            int fark = abs(gridland[parr.first][parr.second] - gridland[sq1.first][sq1.second]);
            if(sayi < fark)
                sayi = fark;
            sq1 = parr;
            //cout << sayi << endl;
        }
        while(sq2.first != lca.first || sq2.second != lca.second){
            cell parr = mst_parents[sq2.first][sq2.second][0];
            int fark = abs(gridland[parr.first][parr.second] - gridland[sq2.first][sq2.second]);
            if(sayi < fark)
                sayi = fark;
            sq2 = parr;
            //cout << sayi << endl;
        }
         */
        output << sayi << endl;
    }
    return 0;


}